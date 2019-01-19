#include <stdlib.h> //free
#include <string.h> //str*
#include <netdb.h> //hstrerror
#include <arpa/nameser.h> //NS_PACKETSZ, ns_*
#include <resolv.h> //res_*
#include <idn2.h> //idn2_to_ascii_8z
#include "utils.h"

#define BUFFER_SIZE NS_PACKETSZ
#define DOMAIN_PREFIX "_redirect."
#define DOMAIN_WWW "www."
#define VALUE_PREFIX "uri="
#define T_URI 256

unsigned char *redirector_query_remove_www(const unsigned char *domain) {
    unsigned char *new_domain;

    //TODO Case sensitivity
    if(strstr(domain, DOMAIN_WWW) == (char *)domain) {
        size_t www_length = strlen(DOMAIN_WWW) * sizeof(unsigned char);
        size_t length = (strlen(domain) * sizeof(unsigned char)) - www_length;
        new_domain = calloc(length + 1, sizeof(unsigned char));
        strcpy(new_domain, domain + www_length);
    }
    else {
        new_domain = calloc(strlen(domain) + 1, sizeof(unsigned char));
        strcpy(new_domain, domain);
    }

    return new_domain;
}

unsigned char *redirector_query_create_subdomain(const unsigned char *domain) {
    unsigned char *subdomain;
    unsigned char *_subdomain = redirector_query_remove_www(domain);
    size_t prefix_length = strlen(DOMAIN_PREFIX) * sizeof(unsigned char);
    size_t subdomain_length = strlen(_subdomain) * sizeof(unsigned char);
    unsigned char *encoded_domain;

    subdomain = calloc(prefix_length + subdomain_length, sizeof(unsigned char));
    strcpy(subdomain, DOMAIN_PREFIX);
    strcat(subdomain, _subdomain);
    free(_subdomain);

    int result = idn2_to_ascii_8z(subdomain, (char **)&encoded_domain, IDN2_TRANSITIONAL);
    if(result == IDN2_OK) {
        free(subdomain);
        subdomain = encoded_domain;
    }

    return subdomain;
}

int _redirector_query(const unsigned char *domain, int type, unsigned char **dest) {
    int result;
    unsigned char buffer[BUFFER_SIZE];
    unsigned char *subdomain;
    ns_msg msg;
    ns_rr rr;
    struct __res_state state;
    res_state p_state = &state;

    *dest = NULL;
    if(domain == NULL) {
        return -1;
    }

    //Keep connection open to name server
    state.options |= RES_STAYOPEN;

    res_ninit(p_state);

    //Construct subdomain
    subdomain = redirector_query_create_subdomain(domain);

    //Query domain for text
    result = res_nsearch(p_state, subdomain, ns_c_in, type, buffer, sizeof(buffer));

    if(result == -1) {
        debug_print("Error %s under %s", hstrerror(p_state->res_h_errno), subdomain);
        free(subdomain);
        return -1;
    }
    else if(result < 1) {
        debug_print("Nothing under %s", subdomain);
        free(subdomain);
        return -1;
    }

    //Parse response into messages
    result = ns_initparse(buffer, result, &msg);
    if(result == -1) {
        debug_print("Failed to parse message buffer under %s", subdomain);
        free(subdomain);
        return -1;
    }

    //Count the number of messages
    result = ns_msg_count(msg, ns_s_an);
    if(result < 1) {
        debug_print("No records found under %s", subdomain);
        free(subdomain);
        return -1;
    }

    result = ns_parserr(&msg, ns_s_an, 0, &rr);
    if(result == -1) {
        debug_print("Failed to parse the response buffer under %s", subdomain);
        free(subdomain);
        return -1;
    }

    const unsigned char *data = ns_rr_rdata(rr);
    size_t record_length = ns_rr_rdlen(rr);
    *dest = calloc(record_length + 1, sizeof(unsigned char));
    strncpy(*dest, data, record_length);

    return record_length;
}

int redirector_query_txt(const unsigned char *domain, unsigned char **dest) {
    int result = _redirector_query(domain, ns_t_txt, dest);

    if(result == -1) {
        return -1;
    }

    if(strstr(*dest, VALUE_PREFIX) == (char *)*dest) {

        /*debug_print("%s", *dest);
        *dest = *dest + (strlen(VALUE_PREFIX) * sizeof(unsigned char));
        debug_print("%s", *dest);
        return strlen(*dest);*/
        size_t www_length = strlen(VALUE_PREFIX) * sizeof(unsigned char);
        size_t length = (strlen(domain) * sizeof(unsigned char)) - www_length;
        unsigned char *new_domain = calloc(length + 1, sizeof(unsigned char));
        strcpy(new_domain, *dest + www_length);
        strcpy(*dest, new_domain);
        free(new_domain);
        return strlen(*dest);
    }
    else {
        free(*dest);
        *dest = NULL;
        return -1;
    }
}

int redirector_query_uri(const unsigned char *domain, unsigned char **dest) {
    return _redirector_query(domain, T_URI, dest);
}