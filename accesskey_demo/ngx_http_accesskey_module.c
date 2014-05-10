#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

typedef struct {
        ngx_flag_t enable;
}ngx_http_accesskey_loc_conf_t;

static ngx_command_t ngx_http_accesskey_commands[]={
    {
      ngx_string("accesskey"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_accesskey_loc_conf_t,enable),
      NULL
    },
    ngx_null_command
};

static  void*  ngx_http_accesskey_create_loc_conf(ngx_conf_t *cf);
static  char*  ngx_http_accesskey_merge_loc_conf(ngx_conf_t* cf, void* parent,void *child);
static ngx_int_t ngx_http_accesskey_handler(ngx_http_request_t* r);
static ngx_int_t ngx_http_accesskey_init(ngx_conf_t *cf);

static ngx_http_module_t ngx_http_accesskey_module_ctx = {
    NULL,
    ngx_http_accesskey_init,
    NULL,
    NULL,
    NULL,
    NULL,
    ngx_http_accesskey_create_loc_conf,
    ngx_http_accesskey_merge_loc_conf
};

ngx_module_t ngx_http_accesskey_module = {
    NGX_MODULE_V1,
    &ngx_http_accesskey_module_ctx,
    ngx_http_accesskey_commands,
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


static  void*  ngx_http_accesskey_create_loc_conf(ngx_conf_t *cf)
{
      ngx_http_accesskey_loc_conf_t *conf;
      conf = ngx_pcalloc(cf->pool,sizeof(ngx_http_accesskey_loc_conf_t));  
      if( conf==NULL ){
         return NGX_CONF_ERROR;
      } 
      conf->enable = NGX_CONF_UNSET ;
      return conf; 
}

static  char*  ngx_http_accesskey_merge_loc_conf(ngx_conf_t* cf, void* parent,void *child)
{
    ngx_http_accesskey_loc_conf_t *prev=parent;
    ngx_http_accesskey_loc_conf_t *conf=child;
    ngx_conf_merge_off_value(conf->enable,prev->enable,0); 
    return NGX_CONF_OK;
}

static ngx_int_t ngx_http_accesskey_handler(ngx_http_request_t* r) {
    ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "\"ngx_http_accesskey_handler, args=%V\"", &r->args);
    ngx_http_accesskey_loc_conf_t  *alcf;
    alcf = ngx_http_get_module_loc_conf(r,ngx_http_accesskey_module);
    if( !alcf->enable ){
        ngx_log_error(NGX_LOG_INFO, r->connection->log, 0,  "\n off \n");
        return NGX_OK;
    }
   return NGX_HTTP_FORBIDDEN;
}

static ngx_int_t ngx_http_accesskey_init(ngx_conf_t *cf){
   ngx_log_error(NGX_LOG_INFO, cf->log, 0, "\n ngx_http_accesskey_init \n");
   ngx_http_handler_pt  *h;
   ngx_http_core_main_conf_t *cmcf;
   cmcf = ngx_http_conf_get_module_main_conf(cf,ngx_http_core_module);
   h = ngx_array_push(&cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers);
   if( h==NULL ){
       ngx_log_error(NGX_LOG_INFO,cf->log,0,"\n error ngx_http_accesskey_init \n"); 
       return NGX_ERROR;
   }
   *h = ngx_http_accesskey_handler;
   return NGX_OK;
}

