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

    //Check to ensure we have a host name
    if(uri->hostText.first == NULL || uri->hostText.afterLast == NULL) {
        return false;
    }
    if(uri->hostText.afterLast - uri->hostText.first <= 0) {
        return false;
    }

    //Check if there is a scheme
    if(uri->scheme.first == NULL || uri->scheme.afterLast == NULL) {
        return false;
    }
    if(uri->scheme.afterLast - uri->scheme.first <= 0) {
        return false;
    }

    //Check if scheme is even allowed
    scheme = calloc(uri->scheme.afterLast - uri->scheme.first + 1, sizeof(unsigned char));
    if(scheme == NULL) {
        redirector_debug_print("%s", "calloc failed");
        return false;
    }
    redirector_strncpy(scheme, uri->scheme.first, uri->scheme.afterLast-uri->scheme.first);
    bool schemeNotAllowed = redirector_strstr(scheme, SCHEME_HTTP) == NULL 
        && redirector_strstr(scheme, SCHEME_HTTPS) == NULL;
    free(scheme);
    if(schemeNotAllowed) {
        return false;
    }

    return true;
}

static UriUriA *redirector_uri_parse(const unsigned char* source) {
    UriUriA *uri;
    int result;

    if(source == NULL) {
        return NULL;
    }
    
    uri = malloc(sizeof(UriUriA));
    if(uri == NULL) {
        redirector_debug_print("%s", "malloc failed");
        return NULL;
    }

    if( (result = (uriParseSingleUriA(uri, (const char *)source, NULL))) != URI_SUCCESS) {
        redirector_debug_print("Failed to parse %s because %d", source, result);
        free(uri);
        return NULL;
    }

    return uri;
}

extern unsigned char *redirector_uri_normalize(const unsigned char *source, unsigned char *sourcePath) {
    UriUriA *uri;
    UriUriA *path;
    UriUriA *uriDest;
    int size;
    unsigned char *dest;
    int result;

    if(source == NULL) {
        return NULL;
    }

    //Nothing provided, default to root
    if(sourcePath == NULL) {
        sourcePath = (unsigned char *)"./";
    }

    uri = redirector_uri_parse(source);
    path = redirector_uri_parse(sourcePath);

    if(!redirector_uri_valid(uri)) {
        redirector_debug_print("URI is not valid %s", source);
        uriFreeUriMembersA(uri);
        uriFreeUriMembersA(path);
        free(uri);
        free(path);
        return NULL;
    }

    if((uriDest = malloc(sizeof(UriUriA))) == NULL) {
        redirector_debug_print("%s", "malloc failed");
        uriFreeUriMembersA(uri);
        uriFreeUriMembersA(path);
        free(uri);
        free(path);
        return NULL;
    }

    result = uriAddBaseUriA(uriDest, path, uri);

    //If the source URI has a query string, but not the path, just append the query string as-is
    if(result == URI_SUCCESS && path->query.first == NULL && uri->query.first != NULL) {
        uriDest->query = uri->query;
    }

    uriFreeUriMembersA(uri);
    uriFreeUriMembersA(path);
    free(uri);
    free(path);
    uri = NULL;
    path = NULL;
    if(result != URI_SUCCESS) {
        redirector_debug_print("Unable to combine %s and %s", source, sourcePath);
        return NULL;
    }
    if(uriNormalizeSyntaxA(uriDest) != URI_SUCCESS) {
        redirector_debug_print("Unable to normalize %s%s", source, sourcePath);
        uriFreeUriMembersA(uriDest);
        free(uriDest);
        return NULL;
    }

    if((result = uriToStringCharsRequiredA(uriDest, &size)) != URI_SUCCESS) {
        redirector_debug_print("URI %s unable to get size because %d", source, result);
        uriFreeUriMembersA(uriDest);
        free(uriDest);
        return NULL;
    }
    size++;

    dest = calloc(size, sizeof(unsigned char));
    if(dest == NULL) {
        redirector_debug_print("%s", "calloc failure");
        uriFreeUriMembersA(uriDest);
        free(uriDest);
        return NULL;
    }

    if((result = uriToStringA((char *)dest, uriDest, size, NULL)) != URI_SUCCESS) {
        redirector_debug_print("URI %s cannot be converted to string because %d", source, result);
        uriFreeUriMembersA(uriDest);
        free(uriDest);
        free(dest);
        return NULL;
    }

    uriFreeUriMembersA(uriDest);
    free(uriDest);
    return dest;
}