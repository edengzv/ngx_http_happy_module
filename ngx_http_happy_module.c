/************************************************
Author        :kantian
Created Time  :Mon 01 Dec 2014 02:27:50 PM CST
File Name     :ngx_http_happy_module.c
************************************************ */
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

//the function used for dealing with the command;
static char *
ngx_http_happy(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

//the main function to do something
static ngx_int_t ngx_http_happy_handler(ngx_http_request_t *r);

//define the command struct
static ngx_command_t ngx_http_happy_commands[] = {
    {
        //name
        ngx_string("happy"),
        //type
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LMT_CONF | NGX_CONF_NOARGS,
        //set func point
        ngx_http_happy,
        //offset
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL
    },
};
//the module context
static ngx_http_module_t ngx_http_happy_module_ctx = {
    //pre configuration
    //post configuration
    NULL,
    NULL,
    //main block
    NULL,
    NULL,
    //server block
    NULL,
    NULL,
   //loc block 
    NULL,
    NULL,
};

//define the ngx_module_t 
ngx_module_t ngx_http_happy_module = {
    NGX_MODULE_V1,
    &ngx_http_happy_module_ctx,
    ngx_http_happy_commands,
    NGX_HTTP_MODULE,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NGX_MODULE_V1_PADDING
};

static char *
ngx_http_happy(ngx_conf_t *cf, ngx_command_t *cmd, void *conf){
    ngx_http_core_loc_conf_t *clcf;

    clcf = ngx_http_conf_get_module_loc_conf(cf,ngx_http_core_module);

    clcf->handler = ngx_http_happy_handler;

    return NGX_CONF_OK;
}

static ngx_int_t ngx_http_happy_handler(ngx_http_request_t *r){

    if(!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD))){
        return NGX_HTTP_NOT_ALLOWED;
    }

    ngx_int_t rc = ngx_http_discard_request_body(r);
    if(rc != NGX_OK){
        return rc;
    }

    ngx_str_t type = ngx_string("text/plain");
    ngx_str_t response = ngx_string("Happy Coding");
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = response.len;
    r->headers_out.content_type = type;

    rc = ngx_http_send_header(r);
    if(rc == NGX_ERROR || rc > NGX_OK || r->header_only){
        return rc;
    }
    ngx_buf_t  *b;
    b = ngx_create_temp_buf(r->pool,response.len);
    if(b == NULL){
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    ngx_memcpy(b->pos,response.data,response.len);
    b->last = b->pos + response.len;
    b->last_buf = 1;
    
    ngx_chain_t out;
    out.buf = b;
    out.next = NULL;

    return ngx_http_output_filter(r,&out);
}
