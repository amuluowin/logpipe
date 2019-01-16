#include "logpipe_api.h"

int	__LOGPIPE_FILTER_PACK_METADATA_0_1_0 = 1 ;

/*
[[system=...][server=...][filename=...][offset=...][line=...]]log\n
*/

struct FilterPluginContext
{
	char			*system ;
	char			*server ;
	char			metadata[ 1024 ] ;
	uint16_t		system_and_server_len ;
	uint16_t		system_and_server_and_filename_len ;
	uint16_t		all_metadata_len ;
	
	uint16_t		*filename_len ;
	char			*filename ;
} ;

/* ��������ṹ */
funcLoadFilterPluginConfig LoadFilterPluginConfig ;
int LoadFilterPluginConfig( struct LogpipeEnv *p_env , struct LogpipeFilterPlugin *p_logpipe_filter_plugin , struct LogpipePluginConfigItem *p_plugin_config_items , void **pp_context )
{
	struct FilterPluginContext	*p_plugin_ctx = NULL ;
	
	// uint16_t			len ;
	
	/* �����ڴ��Դ�Ų�������� */
	p_plugin_ctx = (struct FilterPluginContext *)malloc( sizeof(struct FilterPluginContext) ) ;
	if( p_plugin_ctx == NULL )
	{
		ERRORLOGC( "malloc failed , errno[%d]" , errno )
		return -1;
	}
	memset( p_plugin_ctx , 0x00 , sizeof(struct FilterPluginContext) );
	
	p_plugin_ctx->system = QueryPluginConfigItem( p_plugin_config_items , "system" ) ;
	INFOLOGC( "system[%s]" , p_plugin_ctx->system )
	
	p_plugin_ctx->server = QueryPluginConfigItem( p_plugin_config_items , "server" ) ;
	INFOLOGC( "server[%s]" , p_plugin_ctx->server )
	
	/* ���ò������������ */
	(*pp_context) = p_plugin_ctx ;
	
	return 0;
}

funcInitFilterPluginContext InitFilterPluginContext ;
int InitFilterPluginContext( struct LogpipeEnv *p_env , struct LogpipeFilterPlugin *p_logpipe_filter_plugin , void *p_context )
{
	struct FilterPluginContext	*p_plugin_ctx = (struct FilterPluginContext *)p_context ;
	
	uint16_t			len ;
	
	memset( p_plugin_ctx->metadata , 0x00 , sizeof(p_plugin_ctx->metadata) );
	
	len = snprintf( p_plugin_ctx->metadata+p_plugin_ctx->system_and_server_len , sizeof(p_plugin_ctx->metadata)-1-p_plugin_ctx->system_and_server_len , "[" ) ;
	if( len > 0 )
	{
		p_plugin_ctx->system_and_server_len += len ;
	}
	else
	{
		ERRORLOGC( "system[%s] too long" , p_plugin_ctx->system )
		return -1;
	}
	
	if( p_plugin_ctx->system )
	{
		len = snprintf( p_plugin_ctx->metadata+p_plugin_ctx->system_and_server_len , sizeof(p_plugin_ctx->metadata)-1-p_plugin_ctx->system_and_server_len , "[system=%s]" , p_plugin_ctx->system ) ;
		if( len > 0 )
		{
			p_plugin_ctx->system_and_server_len += len ;
		}
		else
		{
			ERRORLOGC( "system[%s] too long" , p_plugin_ctx->system )
			return -1;
		}
	}
	
	if( p_plugin_ctx->server )
	{
		len = snprintf( p_plugin_ctx->metadata+p_plugin_ctx->system_and_server_len , sizeof(p_plugin_ctx->metadata)-1-p_plugin_ctx->system_and_server_len , "[server=%s]" , p_plugin_ctx->server ) ;
		if( len > 0 )
		{
			p_plugin_ctx->system_and_server_len += len ;
		}
		else
		{
			ERRORLOGC( "server[%s] too long" , p_plugin_ctx->server )
			return -1;
		}
	}
	
	return 0;
}

funcBeforeProcessFilterPlugin BeforeProcessFilterPlugin ;
int BeforeProcessFilterPlugin( struct LogpipeEnv *p_env , struct LogpipeFilterPlugin *p_logpipe_filter_plugin , void *p_context , uint16_t filename_len , char *filename )
{
	struct FilterPluginContext	*p_plugin_ctx = (struct FilterPluginContext *)p_context ;
	
	uint16_t			len ;
	
	len = snprintf( p_plugin_ctx->metadata+p_plugin_ctx->system_and_server_len , sizeof(p_plugin_ctx->metadata)-1-p_plugin_ctx->system_and_server_len , "[filename=%.*s]" , filename_len,filename ) ;
	if( len > 0 )
		p_plugin_ctx->system_and_server_and_filename_len = p_plugin_ctx->system_and_server_len + len ;
	else
	{
		ERRORLOGC( "filename[%.*s] too long" , filename_len,filename )
		return 1;
	}
	
	return 0;
}

funcProcessFilterPlugin ProcessFilterPlugin ;
int ProcessFilterPlugin( struct LogpipeEnv *p_env , struct LogpipeFilterPlugin *p_logpipe_filter_plugin , void *p_context , uint64_t file_offset , uint64_t file_line , uint64_t *p_block_len , char *block_buf , uint64_t block_buf_size )
{
	struct FilterPluginContext	*p_plugin_ctx = (struct FilterPluginContext *)p_context ;
	
	uint16_t			len ;
	
	len = snprintf( p_plugin_ctx->metadata+p_plugin_ctx->system_and_server_and_filename_len , sizeof(p_plugin_ctx->metadata)-1-p_plugin_ctx->system_and_server_and_filename_len , "[offset=%d][line=%d]]" , (int)file_offset , (int)file_line ) ;
	if( len > 0 )
		p_plugin_ctx->all_metadata_len = p_plugin_ctx->system_and_server_and_filename_len + len ;
	else
	{
		ERRORLOGC( "offset[%d] or line[%d] too long" , (int)file_offset , (int)file_line )
		return 1;
	}
	
	if( (*p_block_len) + p_plugin_ctx->all_metadata_len > block_buf_size-1 )
	{
		ERRORLOGC( "output buffer overflow" )
		return 1;
	}
	
	if( p_plugin_ctx->all_metadata_len + (*p_block_len) > block_buf_size-1 )
		len = block_buf_size-1 - p_plugin_ctx->all_metadata_len ;
	else
		len = (*p_block_len) ;
	memmove( block_buf+p_plugin_ctx->all_metadata_len , block_buf , len );
	memcpy( block_buf , p_plugin_ctx->metadata , p_plugin_ctx->all_metadata_len );
	(*p_block_len) += len ;
	
	return 0;
}

funcAfterProcessFilterPlugin AfterProcessFilterPlugin ;
int AfterProcessFilterPlugin( struct LogpipeEnv *p_env , struct LogpipeFilterPlugin *p_logpipe_filter_plugin , void *p_context , uint16_t filename_len , char *filename )
{
	return 0;
}

funcCleanFilterPluginContext CleanFilterPluginContext ;
int CleanFilterPluginContext( struct LogpipeEnv *p_env , struct LogpipeFilterPlugin *p_logpipe_filter_plugin , void *p_context )
{
	return 0;
}

funcUnloadFilterPluginConfig UnloadFilterPluginConfig ;
int UnloadFilterPluginConfig( struct LogpipeEnv *p_env , struct LogpipeFilterPlugin *p_logpipe_filter_plugin , void **pp_context )
{
	struct OutputPluginContext	**pp_plugin_ctx = (struct OutputPluginContext **)pp_context ;
	
	/* �ͷ��ڴ��Դ�Ų�������� */
	free( (*pp_plugin_ctx) ); (*pp_plugin_ctx) = NULL ;
	
	return 0;
}

