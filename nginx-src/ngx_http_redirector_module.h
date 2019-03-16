#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#ifndef NGX_HTTP_REDIRECTOR_MODULE_H
#define NGX_HTTP_REDIRECTOR_MODULE_H
    typedef struct {
        ngx_http_complex_value_t *domain;
    } ngx_http_redirector_loc_conf_t;

    static ngx_int_t ngx_http_redirector_handler(ngx_http_request_t *r);
    static void *ngx_http_redirector_create_loc_conf(ngx_conf_t *cf);
    static char *ngx_http_redirector_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
    //static char *ngx_http_redirector(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

    static ngx_command_t ngx_http_redirector_commands[] = {
        { 
            ngx_string("redirector"),
            NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
            ngx_http_set_complex_value_slot,
            NGX_HTTP_LOC_CONF_OFFSET,
            offsetof(ngx_http_redirector_loc_conf_t, domain),
            NULL
        },

        ngx_null_command
    };


    static ngx_http_module_t ngx_http_redirector_module_ctx = {
        NULL,                                  /* preconfiguration */
        NULL,                                  /* postconfiguration */

        NULL,                                  /* create main configuration */
        NULL,                                  /* init main configuration */

        NULL,                                  /* create server configuration */
        NULL,                                  /* merge server configuration */

        ngx_http_redirector_create_loc_conf,   /* create location configuration */
        ngx_http_redirector_merge_loc_conf     /* merge location configuration */
    };


    ngx_module_t ngx_http_redirector_module = {
        NGX_MODULE_V1,
        &ngx_http_redirector_module_ctx,      /* module context */
        ngx_http_redirector_commands,         /* module directives */
        NGX_HTTP_MODULE,                       /* module type */
        NULL,                                  /* init master */
        NULL,                                  /* init module */
        NULL,                                  /* init process */
        NULL,                                  /* init thread */
        NULL,                                  /* exit thread */
        NULL,                                  /* exit process */
        NULL,                                  /* exit master */
        NGX_MODULE_V1_PADDING
    };

#endif