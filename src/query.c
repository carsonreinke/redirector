#include <string.h>
#include <resolv.h>

#define BUFFER_SIZE 1024
#define DOMAIN_PREFIX "_redirect"

const unsigned char* redirector_query(const unsigned char* domain) {
    int length;
    char buffer[BUFFER_SIZE];
    unsigned char subdomain[strlen(DOMAIN_PREFIX) + strlen(".") + strlen(domain)];
    ns_msg msg;
    ns_rr rr;
    unsigned char *data;


    strcpy(subdomain, DOMAIN_PREFIX);
    strcat(subdomain, ".");
    strcat(subdomain, domain);

    //Query domain for text
    length = res_query(domain, C_IN, T_TXT, buffer, sizeof(buffer));

    //
    if(length < 1) {
        return NULL;
    }

    ns_initparse(buffer, length, &msg);
    length = ns_msg_count(msg, ns_s_an);

    if(length < 1) {
        return NULL;
    }

    ns_parserr(&msg, ns_s_an, 0, &rr);
    data = ns_rr_rdata(rr);
    size_t record_length = data[0];
    unsigned char dest[record_length];
    strncpy(dest, data + 1, record_length);

    return dest;
}