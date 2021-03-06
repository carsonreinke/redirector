#include <stdlib.h> //NULL, free
#include "utils.h"
#include "query.h"
#include "uri.h"
#include "redirector.h"

extern unsigned char *redirector(const unsigned char *domain, unsigned char *request_uri) {
    int result;
    unsigned char *raw;
    unsigned char *formatted;

    if(request_uri == NULL) {
        request_uri = (unsigned char *)"/";
    }

    result = redirector_query_txt(domain, &raw);
    if(REDIRECTOR_UNSUCCESSFUL(result)) {
        result = redirector_query_uri(domain, &raw);
    }
    if(REDIRECTOR_UNSUCCESSFUL(result)) {
        return NULL;
    }

    formatted = redirector_uri_normalize(raw, request_uri);
    free(raw);
    return formatted;
}