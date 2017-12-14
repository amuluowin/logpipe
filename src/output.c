#include "logpipe_in.h"

int WriteAllOutputPlugins( struct LogpipeEnv *p_env , struct LogpipeInputPlugin *p_logpipe_input_plugin , uint16_t filename_len , char *filename )
{
	struct LogpipeOutputPlugin	*p_logpipe_output_plugin = NULL ;
	
	char				block_buf[ LOGPIPE_BLOCK_BUFSIZE + 1 ] ;
	uint32_t			block_len ;
	
	int				nret = 0 ;
	
	/* 执行所有输出端写前函数 */
	list_for_each_entry( p_logpipe_output_plugin , & (p_env->logpipe_output_plugins_list.this_node) , struct LogpipeOutputPlugin , this_node )
	{
		nret = p_logpipe_output_plugin->pfuncBeforeWriteOutputPlugin( p_env , p_logpipe_output_plugin , p_logpipe_output_plugin->context , filename_len , filename ) ;
		if( nret < 0 )
		{
			ERRORLOG( "[%s]->pfuncBeforeWriteOutputPlugin failed , errno[%d]" , p_logpipe_output_plugin->so_filename , errno );
			return -1;
		}
		else if( nret > 0 )
		{
			ERRORLOG( "[%s]->pfuncBeforeWriteOutputPlugin failed , errno[%d]" , p_logpipe_output_plugin->so_filename , errno );
			list_for_each_entry( p_logpipe_output_plugin , & (p_env->logpipe_output_plugins_list.this_node) , struct LogpipeOutputPlugin , this_node )
			{
				nret = p_logpipe_output_plugin->pfuncAfterWriteOutputPlugin( p_env , p_logpipe_output_plugin , p_logpipe_output_plugin->context ) ;
				INFOLOG( "[%s]->pfuncAfterWriteOutputPlugin return[%d]" , p_logpipe_output_plugin->so_filename , nret );
			}
			return 0;
		}
		else
		{
			INFOLOG( "[%s]->pfuncBeforeWriteOutputPlugin ok" , p_logpipe_output_plugin->so_filename );
		}
	}
	
	while(1)
	{
		/* 执行输入端读函数 */
		nret = p_logpipe_input_plugin->pfuncReadInputPlugin( p_env , p_logpipe_input_plugin , p_logpipe_input_plugin->context , & block_len , block_buf , sizeof(block_buf) ) ;
		if( nret == LOGPIPE_READ_END_OF_INPUT )
		{
			INFOLOG( "[%s]->pfuncReadInputPlugin done" , p_logpipe_input_plugin->so_filename );
			break;
		}
		else if( nret < 0 )
		{
			ERRORLOG( "[%s]->pfuncReadInputPlugin failed[%d]" , p_logpipe_input_plugin->so_filename , nret );
			return -1;
		}
		else if( nret > 0 )
		{
			INFOLOG( "[%s]->pfuncReadInputPlugin return[%d]" , p_logpipe_input_plugin->so_filename , nret );
			list_for_each_entry( p_logpipe_output_plugin , & (p_env->logpipe_output_plugins_list.this_node) , struct LogpipeOutputPlugin , this_node )
			{
				nret = p_logpipe_output_plugin->pfuncAfterWriteOutputPlugin( p_env , p_logpipe_output_plugin , p_logpipe_output_plugin->context ) ;
				INFOLOG( "[%s]->pfuncAfterWriteOutputPlugin return[%d]" , p_logpipe_output_plugin->so_filename , nret );
			}
			return 0;
		}
		else
		{
			INFOLOG( "[%s]->pfuncReadInputPlugin ok" , p_logpipe_input_plugin->so_filename );
		}
		
		/* 执行所有输出端写函数 */
		list_for_each_entry( p_logpipe_output_plugin , & (p_env->logpipe_output_plugins_list.this_node) , struct LogpipeOutputPlugin , this_node )
		{
			nret = p_logpipe_output_plugin->pfuncWriteOutputPlugin( p_env , p_logpipe_output_plugin , p_logpipe_output_plugin->context , block_len , block_buf ) ;
			if( nret < 0 )
			{
				ERRORLOG( "[%s]->pfuncWriteOutputPlugin failed[%d]" , p_logpipe_output_plugin->so_filename , nret );
				return -1;
			}
			else if( nret > 0 )
			{
				INFOLOG( "[%s]->pfuncWriteOutputPlugin return[%d]" , p_logpipe_output_plugin->so_filename , nret );
				list_for_each_entry( p_logpipe_output_plugin , & (p_env->logpipe_output_plugins_list.this_node) , struct LogpipeOutputPlugin , this_node )
				{
					nret = p_logpipe_output_plugin->pfuncAfterWriteOutputPlugin( p_env , p_logpipe_output_plugin , p_logpipe_output_plugin->context ) ;
					INFOLOG( "[%s]->pfuncAfterWriteOutputPlugin return[%d]" , p_logpipe_output_plugin->so_filename , nret );
				}
				return 0;
			}
			else
			{
				INFOLOG( "[%s]->pfuncWriteOutputPlugin ok" , p_logpipe_output_plugin->so_filename );
			}
		}
	}
	
	/* 执行所有输出端写后函数 */
	list_for_each_entry( p_logpipe_output_plugin , & (p_env->logpipe_output_plugins_list.this_node) , struct LogpipeOutputPlugin , this_node )
	{
		nret = p_logpipe_output_plugin->pfuncAfterWriteOutputPlugin( p_env , p_logpipe_output_plugin , p_logpipe_output_plugin->context ) ;
		if( nret < 0 )
		{
			ERRORLOG( "[%s]->pfuncAfterWriteOutputPlugin failed[%d]" , p_logpipe_output_plugin->so_filename , nret );
			return -1;
		}
		else if( nret > 0 )
		{
			INFOLOG( "[%s]->pfuncAfterWriteOutputPlugin return[%d]" , p_logpipe_output_plugin->so_filename , nret );
		}
		else
		{
			INFOLOG( "[%s]->pfuncAfterWriteOutputPlugin ok" , p_logpipe_output_plugin->so_filename );
		}
	}
	
	return 0;
}
