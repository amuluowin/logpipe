#include "logpipe_api.h"

int	__LOGPIPE_SPLIT_LINE_VERSION_0_1_0 = 1 ;

/* ��������ṹ */
#define PARSE_BUFFER_MAX_LOG_LENGTH	256

struct FilterPluginContext
{
	struct SplitLineBuffer		*split_line_buf;
} ;

funcLoadFilterPluginConfig LoadFilterPluginConfig ;
int LoadFilterPluginConfig( struct LogpipeEnv *p_env , struct LogpipeFilterPlugin *p_logpipe_filter_plugin , struct LogpipePluginConfigItem *p_plugin_config_items , void **pp_context )
{
	struct FilterPluginContext	*p_plugin_ctx = NULL ;
	
	/* �����ڴ��Դ�Ų�������� */
	p_plugin_ctx = (struct FilterPluginContext *)malloc( sizeof(struct FilterPluginContext) ) ;
	if( p_plugin_ctx == NULL )
	{
		ERRORLOGC( "malloc failed , errno[%d]" , errno )
		return -1;
	}
	memset( p_plugin_ctx , 0x00 , sizeof(struct FilterPluginContext) );
	
	/* ���ò������������ */
	(*pp_context) = p_plugin_ctx ;
	
	return 0;
}

funcInitFilterPluginContext InitFilterPluginContext ;
int InitFilterPluginContext( struct LogpipeEnv *p_env , struct LogpipeFilterPlugin *p_logpipe_filter_plugin , void *p_context )
{
	struct FilterPluginContext	*p_plugin_ctx = (struct FilterPluginContext *)p_context ;
	
	p_plugin_ctx->split_line_buf = AllocSplitLineCache() ;
	if( p_plugin_ctx->split_line_buf == NULL )
	{
		ERRORLOGC( "malloc failed , errno[%d]" , errno )
		return -1;
	}
	
	return 0;
}

funcBeforeProcessFilterPlugin BeforeProcessFilterPlugin ;
int BeforeProcessFilterPlugin( struct LogpipeEnv *p_env , struct LogpipeFilterPlugin *p_logpipe_filter_plugin , void *p_context , uint16_t filename_len , char *filename )
{
	return 0;
}

funcProcessFilterPlugin ProcessFilterPlugin ;
int ProcessFilterPlugin( struct LogpipeEnv *p_env , struct LogpipeFilterPlugin *p_logpipe_filter_plugin , void *p_context , uint64_t file_offset , uint64_t file_line , uint64_t *p_block_len , char *block_buf )
{
	struct FilterPluginContext	*p_plugin_ctx = (struct FilterPluginContext *)p_context ;
	
	int				nret = 0 ;
	
	INFOLOGC( "block_buf [%d][%.*s]" , (*p_block_len) , MIN((*p_block_len),PARSE_BUFFER_MAX_LOG_LENGTH) , block_buf )
	INFOLOGC( "before combine [%d][%.*s]" , GetSplitLineBufferLength(p_plugin_ctx->split_line_buf) , MIN(GetSplitLineBufferLength(p_plugin_ctx->split_line_buf),PARSE_BUFFER_MAX_LOG_LENGTH) , GetSplitLineBufferPtr(p_plugin_ctx->split_line_buf,NULL) )
	
	nret = FetchSplitLineBuffer( p_plugin_ctx->split_line_buf , p_block_len , block_buf ) ;
	if( nret && nret != LOGPIPE_CONTINUE_TO_FILTER )
	{
		ERRORLOGC( "FetchSplitLineBuffer failed[%d]" , nret )
		return -1;
	}
	else
	{
		INFOLOGC( "FetchSplitLineBuffer return[%d]" , nret )
	}
	
	INFOLOGC( "after combine , [%d][%.*s]" , GetSplitLineBufferLength(p_plugin_ctx->split_line_buf) , MIN(GetSplitLineBufferLength(p_plugin_ctx->split_line_buf),PARSE_BUFFER_MAX_LOG_LENGTH) , GetSplitLineBufferPtr(p_plugin_ctx->split_line_buf,NULL) )
	
	return nret;
}

funcAfterProcessFilterPlugin AfterProcessFilterPlugin ;
int AfterProcessFilterPlugin( struct LogpipeEnv *p_env , struct LogpipeFilterPlugin *p_logpipe_filter_plugin , void *p_context , uint16_t filename_len , char *filename )
{
	return 0;
}

funcCleanFilterPluginContext CleanFilterPluginContext ;
int CleanFilterPluginContext( struct LogpipeEnv *p_env , struct LogpipeFilterPlugin *p_logpipe_filter_plugin , void *p_context )
{
	struct FilterPluginContext	*p_plugin_ctx = (struct FilterPluginContext *)p_context ;
	
	FreeSplitLineBuffer( p_plugin_ctx->split_line_buf ); p_plugin_ctx->split_line_buf = NULL ;
	
	return 0;
}

funcUnloadFilterPluginConfig UnloadFilterPluginConfig ;
int UnloadFilterPluginConfig( struct LogpipeEnv *p_env , struct LogpipeFilterPlugin *p_logpipe_filter_plugin , void **pp_context )
{
	struct FilterPluginContext	**pp_plugin_ctx = (struct FilterPluginContext **)pp_context ;
	
	/* �ͷ��ڴ��Դ�Ų�������� */
	free( (*pp_plugin_ctx) ); (*pp_plugin_ctx) = NULL ;
	
	return 0;
}

