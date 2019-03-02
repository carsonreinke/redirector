#include <stdlib.h> //free, calloc, NULL
#include <string.h> //str*
#include <netdb.h> //hstrerror
#include <arpa/nameser.h> //NS_PACKETSZ, ns_*
#include <resolv.h> //res_*
#include <idn2.h> //idn2_to_ascii_8z
#include "utils.h"
#include "query.h"

#define BUFFER_SIZE NS_PACKETSZ
#define DOMAIN_PREFIX "_redirect."
#define DOMAIN_WWW "www."
#define VALUE_PREFIX "uri="
#ifndef T_URI
    #define T_URI 256
#endif

static unsigned char *redirector_query_remove_www(const unsigned char *domain) {
    unsigned char *new_domain = calloc(redirector_strlen(domain) + 1, sizeof(unsigned char));
    unsigned char *lower_domain = redirector_strtolower(domain);

    if(redirector_strstr(lower_domain, DOMAIN_WWW) == (char *)lower_domain) {
        redirector_strcpy(new_domain, domain + redirector_strlen(DOMAIN_WWW));
    }
    else {
        redirector_strcpy(new_domain, domain);
    }

    free(lower_domain);
    return new_domain;
}

static unsigned char *redirector_query_create_subdomain(const unsigned char *domain) {
    unsigned char *new_domain = redirector_query_remove_www(domain);
    unsigned char *subdomain = calloc(redirector_strlen(new_domain) + redirector_strlen(DOMAIN_PREFIX) + 1, sizeof(unsigned char));
    unsigned char *encoded_subdomain;

    redirector_strcpy(subdomain, DOMAIN_PREFIX);
    redirector_strcat(subdomain, new_domain);

    free(new_domain);

    int result = idn2_to_ascii_8z((const char *)subdomain, (char **)&encoded_subdomain, IDN2_TRANSITIONAL);
    if(result == IDN2_OK) {
        free(subdomain);
        subdomain = encoded_subdomain;
    }

    return subdomain;
}

static int _redirector_query(const unsigned char *subdomain, int type, unsigned char **dest) {
    int result;
    unsigned char buffer[BUFFER_SIZE];
    ns_msg msg;
    ns_rr rr;
    struct __res_state state;
    res_state p_state = &state;

    if(dest == NULL) {
        return REDIRECTOR_ERROR;
    }

    *dest = NULL;
    if(subdomain == NULL) {
        return REDIRECTOR_ERROR;
    }

    //Keep connection open to name server
    state.options = RES_STAYOPEN;

    if(res_ninit(p_state) == -1) {
        return REDIRECTOR_ERROR;
    }

    //Query domain for text
    result = res_nsearch(p_state, (const char *)subdomain, ns_c_in, type, buffer, sizeof(buffer));

    int res_h_errno = p_state->res_h_errno;

    res_nclose(p_state);

    if(result == -1) {
        debug_print("Error %s under %s", hstrerror(res_h_errno), subdomain);
        return REDIRECTOR_ERROR;
    }
    else if(result < 1) {
        debug_print("Nothing under %s", subdomain);
        return REDIRECTOR_OK;
    }

    //Parse response into messages
    result = ns_initparse(buffer, result, &msg);
    if(result == -1) {
        debug_print("Failed to parse message buffer under %s", subdomain);
        return REDIRECTOR_ERROR;
    }

    //Count the number of messages
    result = ns_msg_count(msg, ns_s_an);
    if(result < 1) {
        debug_print("No records found under %s", subdomain);
        return REDIRECTOR_OK;
    }

    result = ns_parserr(&msg, ns_s_an, 0, &rr);
    if(result == -1) {
        debug_print("Failed to parse the response buffer under %s", subdomain);
        return REDIRECTOR_ERROR;
    }

    const unsigned char *data = ns_rr_rdata(rr);
    size_t record_length = ns_rr_rdlen(rr);
    *dest = calloc(record_length + 1, sizeof(unsigned char));
    memcpy(*dest, data, record_length);

    return record_length;
}

extern int redirector_query_txt(const unsigned char *domain, unsigned char **dest) {
    unsigned char *subdomain;

    if(domain == NULL) {
        return REDIRECTOR_ERROR;
    }

    //Create subdomain without "www"
    subdomain = redirector_query_create_subdomain(domain);
    debug_print("Created subdomain %s", subdomain);

    int result = _redirector_query(subdomain, T_TXT, dest);
    free(subdomain);

    if(REDIRECTOR_UNSUCCESSFUL(result)) {
        return result;
    }

    //Super simple support RFC1464
    //TODO Double check this
    if(redirector_strstr(*dest, VALUE_PREFIX) == (char *)*dest) {
        unsigned char *new_dest = calloc(redirector_strlen(*dest) - redirector_strlen(VALUE_PREFIX) + 1, sizeof(unsigned char));
        redirector_strcpy(new_dest, *dest + redirector_strlen(VALUE_PREFIX));
        free(*dest);
        *dest = new_dest;
        return redirector_strlen(*dest);
    }
    else {
        free(*dest);
        *dest = NULL;
        return REDIRECTOR_ERROR;
    }
}

extern int redirector_query_uri(const unsigned char *domain, unsigned char **dest) {
    unsigned char *subdomain;

    if(domain == NULL) {
        return REDIRECTOR_ERROR;
    }

    //Just remove "www", don't add subdomain
    subdomain = redirector_query_remove_www(domain);
    debug_print("Created subdomain %s", subdomain);

    int result = _redirector_query(subdomain, T_URI, dest);
    free(subdomain);

    if(REDIRECTOR_SUCCESSFUL(result)) {
        unsigned char *new_dest = calloc(result + 1, sizeof(unsigned char));
        //Advanced pointer to skip priority and weight
        redirector_strcpy(new_dest, *dest + ((NS_INT16SZ * 2) / sizeof(unsigned char)));
        free(*dest);
        *dest = new_dest;
        result = redirector_strlen(*dest);
    }

    return result;
}