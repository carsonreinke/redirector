#include <stdlib.h> //NULL, free
#include "utils.h"
#include "query.h"
#include "uri.h"
#include "redirector.h"

unsigned char *redirector(const unsigned char *domain) {
    int result;
    unsigned char *raw;
    unsigned char *formatted;

    result = redirector_query_txt(domain, &raw);
    if(REDIRECTOR_UNSUCCESSFUL(result)) {
        result = redirector_query_uri(domain, &raw);
    }
    if(REDIRECTOR_UNSUCCESSFUL(result)) {
        return NULL;
    }

    formatted = redirect_uri_normalize(raw);
    free(raw);
    return formatted;
}