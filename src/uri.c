#include <stdbool.h> //bool
#include <uriparser/Uri.h> //uri*, Uri*
#include <string.h> //str*
#include "uri.h"
#include "utils.h"

#define SCHEME_HTTP "http"
#define SCHEME_HTTPS "https"

static bool redirector_uri_valid(UriUriA *uri) {
    unsigned char *scheme;

    if(uri == NULL) {
        return false;
    }

    //Absolute URIs are only allowed
    if(uri->absolutePath != URI_FALSE) {
        return false;
    }

    //Check if there is a scheme
    if(uri->scheme.first == NULL || uri->scheme.afterLast == NULL) {
        return false;
    }

    //Check if scheme is even allowed
    scheme = calloc(uri->scheme.afterLast - uri->scheme.first + 1, sizeof(unsigned char));
    strncpy(scheme, uri->scheme.first, uri->scheme.afterLast-uri->scheme.first);
    bool schemeNotAllowed = strstr(scheme, SCHEME_HTTP) == NULL 
        && strstr(scheme, SCHEME_HTTPS) == NULL;
    free(scheme);
    if(schemeNotAllowed) {
        return false;
    }

    return true;
}

static UriUriA *redirect_uri_parse(const unsigned char* source) {
    UriUriA *uri;
    int result;

    if(source == NULL) {
        return NULL;
    }
    
    uri = malloc(sizeof(UriUriA));
    if(uri == NULL) {
        debug_print("malloc failed", NULL);
        return NULL;
    }

    if(result = (uriParseSingleUriA(uri, source, NULL)) != URI_SUCCESS) {
        debug_print("Failed to parse %s because %d", source, result);
        free(uri);
        return NULL;
    }

    return uri;
}

extern unsigned char *redirect_uri_normalize(const unsigned char* source) {
    UriUriA *uri;
    int size;
    unsigned char *dest;
    int result;

    if(source == NULL) {
        return NULL;
    }

    uri = redirect_uri_parse(source);

    if(!redirector_uri_valid(uri)) {
        debug_print("URI is not valid %s", source);
        uriFreeUriMembersA(uri);
        free(uri);
        return NULL;
    }

    if((result = uriToStringCharsRequiredA(uri, &size)) != URI_SUCCESS) {
        debug_print("URI %s unable to get size because %d", source, result);
        uriFreeUriMembersA(uri);
        free(uri);
        return NULL;
    }
    size++;

    dest = calloc(size, sizeof(unsigned char));
    if(dest == NULL) {
        debug_print("calloc failure", NULL);
        uriFreeUriMembersA(uri);
        free(uri);
        return NULL;
    }

    if((result = uriToStringA(dest, uri, size, NULL)) != URI_SUCCESS) {
        debug_print("URI %s cannot be converted to string because", source, result);
        uriFreeUriMembersA(uri);
        free(uri);
        free(dest);
        return NULL;
    }

    uriFreeUriMembersA(uri);
    free(uri);
    return dest;
}