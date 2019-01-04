#include <string.h>
#include <netdb.h> //hstrerror
#include <arpa/nameser.h> //NS_PACKETSZ, ns_*
#include <resolv.h> //res_*
#include "utils.h"

#define BUFFER_SIZE NS_PACKETSZ
#define DOMAIN_PREFIX "_redirect."
#define T_URI 256

size_t redirector_query(const unsigned char *domain, unsigned char *dest) {
    int result;
    unsigned char buffer[BUFFER_SIZE];
    unsigned char subdomain[strlen(DOMAIN_PREFIX) + strlen(domain)];
    ns_msg msg;
    ns_rr rr;
    struct __res_state state;
    res_state p_state = &state;

    //Keep connection open to name server
    state.options |= RES_STAYOPEN;

    res_ninit(p_state);

    //Construct subdomain
    strcpy(subdomain, DOMAIN_PREFIX);
    strcat(subdomain, domain);

    //Query domain for text
    result = res_nsearch(p_state, subdomain, ns_c_in, ns_t_txt, buffer, sizeof(buffer));

    if(result == -1) {
        debug_print("Error %s under %s", hstrerror(p_state->res_h_errno), subdomain);
        return -1;
    }
    else if(result < 1) {
        debug_print("Nothing under %s", subdomain);
        return -1;
    }

    //Parse response into messages
    result = ns_initparse(buffer, result, &msg);
    if(result == -1) {
        debug_print("Failed to parse message buffer under %s", subdomain);
        return -1;
    }

    //Count the number of messages
    result = ns_msg_count(msg, ns_s_an);
    if(result < 1) {
        debug_print("No records found under %s", subdomain);
        return -1;
    }

    result = ns_parserr(&msg, ns_s_an, 0, &rr);
    if(result == -1) {
        debug_print("Failed to parse the response buffer under %s", subdomain);
        return -1;
    }

    const unsigned char *data = ns_rr_rdata(rr);
    size_t record_length = ns_rr_rdlen(rr);
    strncpy(dest, data, record_length);

    return record_length;
}