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
    unsigned char *dest;
    ngx_str_t dest_domain;

    cf = ngx_http_get_module_loc_conf(r, ngx_http_redirector_module);

    if(cf == NULL || !cf->domain) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    if(ngx_http_complex_value(r, cf->domain, &domain) != NGX_OK) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    source = ngx_calloc(domain.len + 1, r->connection->log);
    memcpy(source, domain.data, domain.len);

    dest = redirector(source);
    ngx_free(source);

    if(dest == NULL) {
        return NGX_HTTP_NOT_FOUND;
    }

    //free(dest); //TODO
    dest_domain.data = dest;
    dest_domain.len = redirector_strlen(dest);

    r->headers_out.location = ngx_list_push(&r->headers_out.headers);
    if(r->headers_out.location == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    r->headers_out.location->hash = 1;
    ngx_str_set(&r->headers_out.location->key, "Location");
    r->headers_out.location->value = dest_domain;

    //TODO Free dest_domain?

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

    if(prev->domain) {
        conf->domain = prev->domain;
    }
    //ngx_conf_merge_ptr_value(conf->domain, prev->domain, NULL);

    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_redirector_handler;

    return NGX_CONF_OK;
}