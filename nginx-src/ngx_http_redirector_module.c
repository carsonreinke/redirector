#include <stdlib.h> //calloc
#include <string.h> //memcpy
#include "ngx_http_redirector_module.h"
#include "../src/redirector.h"
#include "../src/utils.h"

static ngx_int_t ngx_http_redirector_handler(ngx_http_request_t *r)
{
    ngx_http_redirector_loc_conf_t *cf;
    ngx_str_t domain;
    unsigned char *source;
    unsigned char *path;
    unsigned char *dest;
    ngx_str_t dest_domain;

    cf = ngx_http_get_module_loc_conf(r, ngx_http_redirector_module);
    if(cf == NULL || !cf->domain) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    if(ngx_http_complex_value(r, cf->domain, &domain) != NGX_OK) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    //Copy out a Nginx string for domain
    source = calloc(domain.len + 1, sizeof(unsigned char));
    if(source == NULL) {
        redirector_debug_print("%s", "calloc failed");
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    memcpy(source, domain.data, domain.len);

    //Copy out request URI and prefix with "."
    path = calloc(r->unparsed_uri.len + 2, sizeof(unsigned char));
    if(path == NULL) {
        redirector_debug_print("%s", "calloc failed");
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    memcpy(path + 1, r->unparsed_uri.data, r->unparsed_uri.len);
    path[0] = '.';

    //Find out where we should go
    dest = redirector(source, path);
    free(source);
    free(path);

    //No results, just produce a 404
    if(dest == NULL) {
        return NGX_HTTP_NOT_FOUND;
    }

    //Copy into a Nginx string
    dest_domain.data = ngx_alloc(redirector_strlen(dest), r->connection->log);
    dest_domain.len = redirector_strlen(dest);
    ngx_memcpy(dest_domain.data, dest, dest_domain.len);
    free(dest);

    r->headers_out.location = ngx_list_push(&r->headers_out.headers);
    if(r->headers_out.location == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    r->headers_out.location->hash = 1;
    ngx_str_set(&r->headers_out.location->key, "Location");
    r->headers_out.location->value = dest_domain;

    //TODO Does `dest_domain` need to be freed?

    return NGX_HTTP_MOVED_PERMANENTLY;
}

static void *ngx_http_redirector_create_loc_conf(ngx_conf_t *cf) {
    ngx_http_redirector_loc_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_redirector_loc_conf_t));
    if(conf == NULL) {
        return NULL;
    }

    return conf;
}

static char *ngx_http_redirector_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child) {
    ngx_http_redirector_loc_conf_t *prev = parent;
    ngx_http_redirector_loc_conf_t *conf = child;
    ngx_http_core_loc_conf_t *clcf;

    //Supply the selected domain from the parents to the children
    if(prev->domain) {
        conf->domain = prev->domain;
    }

    //Supply our handler if none is supplied
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    if(clcf->handler == NULL) {
        clcf->handler = ngx_http_redirector_handler;
    }

    return NGX_CONF_OK;
}