#include "logpipe_api.h"

/* communication protocol :
	|'@'(1byte)|filename_len(2bytes)|file_name|file_block_len(2bytes)|file_block_data|...(other file blocks)...|\0\0\0\0|
*/

char	*__LOGPIPE_OUTPUT_TCP_VERSION = "0.1.0" ;

struct ForwardSession
{
	char			*ip ;
	int			port ;
	struct sockaddr_in   	addr ;
	int			sock ;
	time_t			enable_timestamp ;
	struct timeval		tv_begin_send_filename ;
	struct timeval		tv_end_send_filename ;
	struct timeval		tv_begin_send_block_len ;
	struct timeval		tv_end_send_block_len ;
	struct timeval		tv_begin_send_block ;
	struct timeval		tv_end_send_block ;
	struct timeval		tv_begin_send_eob ;
	struct timeval		tv_end_send_eob ;
} ;

#define IP_PORT_MAXCNT		8

#define DISABLE_TIMEOUT		60

struct OutputPluginContext
{
	struct ForwardSession	forward_session_array[IP_PORT_MAXCNT] ;
	int			forward_session_count ;
	struct ForwardSession	*p_forward_session ;
	int			forward_session_index ;
	int			disable_timeout ;
} ;

funcLoadOutputPluginConfig LoadOutputPluginConfig ;
int LoadOutputPluginConfig( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , struct LogpipePluginConfigItem *p_plugin_config_items , void **pp_context )
{
	struct OutputPluginContext	*p_plugin_ctx = NULL ;
	int				i ;
	struct ForwardSession		*p_forward_session = NULL ;
	char				*p = NULL ;
	
	/* �����ڴ��Դ�Ų�������� */
	p_plugin_ctx = (struct OutputPluginContext *)malloc( sizeof(struct OutputPluginContext) ) ;
	if( p_plugin_ctx == NULL )
	{
		ERRORLOG( "malloc failed , errno[%d]" , errno );
		return -1;
	}
	memset( p_plugin_ctx , 0x00 , sizeof(struct OutputPluginContext) );
	
	/* ����������� */
	for( i = 0 , p_forward_session = p_plugin_ctx->forward_session_array ; i < IP_PORT_MAXCNT ; i++ , p_forward_session++ )
	{
		if( i == 0 )
		{
			p_forward_session->ip = QueryPluginConfigItem( p_plugin_config_items , "ip" ) ;
			INFOLOG( "ip[%s]" , p_forward_session->ip )
		}
		else
		{
			p_forward_session->ip = QueryPluginConfigItem( p_plugin_config_items , "ip%d" , i+1 ) ;
			INFOLOG( "ip%d[%s]" , i+1 , p_forward_session->ip )
		}
		if( p_forward_session->ip == NULL || p_forward_session->ip[0] == '\0' )
			break;
		
		if( i == 0 )
			p = QueryPluginConfigItem( p_plugin_config_items , "port" ) ;
		else
			p = QueryPluginConfigItem( p_plugin_config_items , "port%d" , i+1 ) ;
		if( p == NULL || p[0] == '\0' )
		{
			ERRORLOG( "expect config for 'port'" );
			return -1;
		}
		p_forward_session->port = atoi(p) ;
		if( i == 0 )
		{
			INFOLOG( "port[%d]" , p_forward_session->port )
		}
		else
		{
			INFOLOG( "port%d[%d]" , i+1 , p_forward_session->port )
		}
		if( p_forward_session->port <= 0 )
		{
			ERRORLOG( "port[%s] invalid" , p );
			return -1;
		}
		
		p_plugin_ctx->forward_session_count++;
	}
	if( p_plugin_ctx->forward_session_count == 0 )
	{
		ERRORLOG( "expect config for 'ip'" );
		return -1;
	}
	
	p = QueryPluginConfigItem( p_plugin_config_items , "disable_timeout" ) ;
	if( p )
	{
		p_plugin_ctx->disable_timeout = atoi(p) ;
	}
	else
	{
		p_plugin_ctx->disable_timeout = DISABLE_TIMEOUT ;
	}
	
	/* ���ò������������ */
	(*pp_context) = p_plugin_ctx ;
	
	return 0;
}

static int CheckAndConnectForwardSocket( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , struct OutputPluginContext *p_plugin_ctx , int forward_session_index )
{
	struct ForwardSession	*p_forward_session = NULL ;
	
	int			i ;
	
	int			nret = 0 ;
	
	DEBUGLOG( "forward_session_index[%d]" , forward_session_index )
	
	while(1)
	{
		for( i = 0 ; i < p_plugin_ctx->forward_session_count ; i++ )
		{
			if( forward_session_index >= 0 )
			{
				p_forward_session = p_plugin_ctx->forward_session_array + forward_session_index ;
			}
			else
			{
				p_plugin_ctx->forward_session_index++;
				if( p_plugin_ctx->forward_session_index >= p_plugin_ctx->forward_session_count )
					p_plugin_ctx->forward_session_index = 0 ;
				DEBUGLOG( "p_plugin_ctx->forward_session_index[%d]" , p_plugin_ctx->forward_session_index )
				p_plugin_ctx->p_forward_session = p_forward_session = p_plugin_ctx->forward_session_array + p_plugin_ctx->forward_session_index ;
			}
			
			if( forward_session_index < 0 )
			{
				if( p_forward_session->enable_timestamp > 0 )
				{
					if( time(NULL) < p_forward_session->enable_timestamp )
						continue;
					else
						p_forward_session->enable_timestamp = 0 ;
				}
				
				if( p_forward_session->sock >= 0 )
					return 0;
			}
			
			/* �����׽��� */
			p_forward_session->sock = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP ) ;
			if( p_forward_session->sock == -1 )
			{
				ERRORLOG( "socket failed , errno[%d]" , errno );
				return -1;
			}
			
			/* �����׽���ѡ�� */
			{
				int	onoff = 1 ;
				setsockopt( p_forward_session->sock , SOL_SOCKET , SO_REUSEADDR , (void *) & onoff , sizeof(int) );
			}
			
			{
				int	onoff = 1 ;
				setsockopt( p_forward_session->sock , IPPROTO_TCP , TCP_NODELAY , (void*) & onoff , sizeof(int) );
			}
			
			/* ���ӵ�����������˿� */
			nret = connect( p_forward_session->sock , (struct sockaddr *) & (p_forward_session->addr) , sizeof(struct sockaddr) ) ;
			if( nret == -1 )
			{
				ERRORLOG( "connect[%s:%d] failed , errno[%d]" , p_forward_session->ip , p_forward_session->port , errno );
				close( p_forward_session->sock ); p_forward_session->sock = -1 ;
				if( forward_session_index < 0 )
					p_forward_session->enable_timestamp = time(NULL) + p_plugin_ctx->disable_timeout ;
			}
			else
			{
				INFOLOG( "connect[%s:%d] ok , sock[%d]" , p_forward_session->ip , p_forward_session->port , p_forward_session->sock );
				/* �������������� */
				AddOutputPluginEvent( p_env , p_logpipe_output_plugin , p_forward_session->sock );
				return 0;
			}
		}
		
		sleep(1);
	}
}

funcInitOutputPluginContext InitOutputPluginContext ;
int InitOutputPluginContext( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , void *p_context )
{
	struct OutputPluginContext	*p_plugin_ctx = (struct OutputPluginContext *)p_context ;
	
	int				i ;
	struct ForwardSession		*p_forward_session = NULL ;
	
	int				nret = 0 ;
	
	/* ��ʼ����������ڲ����� */
	
	/* �������жԶ�logpipe������ */
	for( i = 0 , p_forward_session = p_plugin_ctx->forward_session_array ; i < p_plugin_ctx->forward_session_count ; i++ , p_forward_session++ )
	{
		memset( & (p_forward_session->addr) , 0x00 , sizeof(struct sockaddr_in) );
		p_forward_session->addr.sin_family = AF_INET ;
		if( p_forward_session->ip[0] == '\0' )
			p_forward_session->addr.sin_addr.s_addr = INADDR_ANY ;
		else
			p_forward_session->addr.sin_addr.s_addr = inet_addr(p_forward_session->ip) ;
		p_forward_session->addr.sin_port = htons( (unsigned short)(p_forward_session->port) );
		
		/* ���ӷ���� */
		p_forward_session->sock = -1 ;
		nret = CheckAndConnectForwardSocket( p_env , p_logpipe_output_plugin , p_plugin_ctx , i ) ;
		if( nret )
		{
			ERRORLOG( "CheckAndConnectForwardSocket failed[%d]" , nret );
			return -1;
		}
	}
	
	p_plugin_ctx->forward_session_index = -1 ;
	
	/* ������ӣ��������ʧЧ������ */
	nret = CheckAndConnectForwardSocket( p_env , p_logpipe_output_plugin , p_plugin_ctx , -1 ) ;
	if( nret )
	{
		ERRORLOG( "CheckAndConnectForwardSocket failed[%d]" , nret );
		return nret;
	}
	
	return 0;
}

funcOnOutputPluginEvent OnOutputPluginEvent;
int OnOutputPluginEvent( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , void *p_context )
{
	struct OutputPluginContext	*p_plugin_ctx = (struct OutputPluginContext *)p_context ;
	
	int				i ;
	struct ForwardSession		*p_forward_session = NULL ;
	fd_set				read_fds , except_fds ;
	int				max_fd ;
	struct timeval			timeout ;
	
	int				nret = 0 ;
	
	DEBUGLOG( "OnOutputPluginEvent" )
	
	/* ������жԶ�logpipe���������� */
	FD_ZERO( & read_fds );
	FD_ZERO( & except_fds );
	max_fd = -1 ;
	for( i = 0 , p_forward_session = p_plugin_ctx->forward_session_array ; i < p_plugin_ctx->forward_session_count ; i++ , p_forward_session++ )
	{
		if( p_forward_session->sock < 0 )
			continue;
		
		FD_SET( p_forward_session->sock , & read_fds );
		FD_SET( p_forward_session->sock , & except_fds );
		if( p_forward_session->sock > max_fd )
			max_fd = p_forward_session->sock ;
		DEBUGLOG( "add fd[%d] to select fds" , p_forward_session->sock )
	}
	
	timeout.tv_sec = 0 ;
	timeout.tv_usec = 0 ;
	nret = select( max_fd+1 , & read_fds , NULL , & except_fds , & timeout ) ;
	if( nret > 0 )
	{
		for( i = 0 , p_forward_session = p_plugin_ctx->forward_session_array ; i < p_plugin_ctx->forward_session_count ; i++ , p_forward_session++ )
		{
			if( p_forward_session->sock < 0 )
				continue;
			
			if( FD_ISSET( p_forward_session->sock , & read_fds ) || FD_ISSET( p_forward_session->sock , & except_fds ) )
			{
				DEBUGLOG( "select fd[%d] hited" , p_forward_session->sock )
				
				/* �ر����� */
				DeleteOutputPluginEvent( p_env , p_logpipe_output_plugin , p_forward_session->sock );
				ERRORLOG( "remote socket closed , close forward sock[%d]" , p_forward_session->sock )
				close( p_forward_session->sock ); p_forward_session->sock = -1 ;
			}
		}
	}
	
	return 0;
}

funcBeforeWriteOutputPlugin BeforeWriteOutputPlugin ;
int BeforeWriteOutputPlugin( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , void *p_context , uint16_t filename_len , char *filename )
{
	struct OutputPluginContext	*p_plugin_ctx = (struct OutputPluginContext *)p_context ;
	
	uint16_t			*filename_len_htons = NULL ;
	char				comm_buf[ 1 + sizeof(uint16_t) + PATH_MAX ] ;
	int				len ;
	
	int				nret = 0 ;
	
_GOTO_RETRY_SEND :
	
	/* ������ӣ��������ʧЧ������ */
	nret = CheckAndConnectForwardSocket( p_env , p_logpipe_output_plugin , p_plugin_ctx , -1 ) ;
	if( nret )
	{
		ERRORLOG( "CheckAndConnectForwardSocket failed[%d]" , nret );
		return nret;
	}
	
	memset( comm_buf , 0x00 , sizeof(comm_buf) );
	comm_buf[0] = LOGPIPE_COMM_HEAD_MAGIC ;
	
	if( filename_len > PATH_MAX )
	{
		ERRORLOG( "filename length[%d] too long" , filename_len )
		return 1;
	}
	
	filename_len_htons = (uint16_t*)(comm_buf+1) ;
	(*filename_len_htons) = htons(filename_len) ;
	
	strncpy( comm_buf+1+sizeof(uint16_t) , filename , filename_len );
	
	/* ����ͨѶͷ���ļ��� */
	gettimeofday( & (p_plugin_ctx->p_forward_session->tv_begin_send_filename) , NULL );
	len = writen( p_plugin_ctx->p_forward_session->sock , comm_buf , 1+sizeof(uint16_t)+filename_len ) ;
	gettimeofday( & (p_plugin_ctx->p_forward_session->tv_end_send_filename) , NULL );
	if( len == -1 )
	{
		ERRORLOG( "send comm magic and filename[%.*s] to socket failed , errno[%d]" , filename_len , filename , errno )
		close( p_plugin_ctx->p_forward_session->sock ); p_plugin_ctx->p_forward_session->sock = -1 ;
		goto _GOTO_RETRY_SEND;
	}
	else
	{
		INFOLOG( "send comm magic and filename[%.*s] to socket ok , [%d]bytes" , filename_len , filename , 1+sizeof(uint16_t)+filename_len )
		DEBUGHEXLOG( comm_buf , len , NULL )
	}
	
	return 0;
}

funcWriteOutputPlugin WriteOutputPlugin ;
int WriteOutputPlugin( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , void *p_context , uint32_t file_offset , uint32_t file_line , uint32_t block_len , char *block_buf )
{
	struct OutputPluginContext	*p_plugin_ctx = (struct OutputPluginContext *)p_context ;
	
	uint32_t			block_len_htonl ;
	
	int				len ;
	
	/* �������ݿ鵽TCP */
	block_len_htonl = htonl(block_len) ;
	gettimeofday( & (p_plugin_ctx->p_forward_session->tv_begin_send_block_len) , NULL );
	len = writen( p_plugin_ctx->p_forward_session->sock , & block_len_htonl , sizeof(block_len_htonl) ) ;
	gettimeofday( & (p_plugin_ctx->p_forward_session->tv_end_send_block_len) , NULL );
	if( len == -1 )
	{
		ERRORLOG( "send block len to socket failed , errno[%d]" , errno )
		close( p_plugin_ctx->p_forward_session->sock ); p_plugin_ctx->p_forward_session->sock = -1 ;
		return 1;
	}
	else
	{
		INFOLOG( "send block len to socket ok , [%d]bytes" , sizeof(block_len_htonl) )
		DEBUGHEXLOG( (char*) & block_len_htonl , len , NULL )
	}
	
	gettimeofday( & (p_plugin_ctx->p_forward_session->tv_begin_send_block) , NULL );
	len = writen( p_plugin_ctx->p_forward_session->sock , block_buf , block_len ) ;
	gettimeofday( & (p_plugin_ctx->p_forward_session->tv_end_send_block) , NULL );
	if( len == -1 )
	{
		ERRORLOG( "send block data to socket failed , errno[%d]" , errno )
		close( p_plugin_ctx->p_forward_session->sock ); p_plugin_ctx->p_forward_session->sock = -1 ;
		return 1;
	}
	else
	{
		INFOLOG( "send block data to socket ok , [%d]bytes" , block_len )
		DEBUGHEXLOG( block_buf , len , NULL )
	}
	
	return 0;
}

funcAfterWriteOutputPlugin AfterWriteOutputPlugin ;
int AfterWriteOutputPlugin( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , void *p_context , uint16_t filename_len , char *filename )
{
	struct OutputPluginContext	*p_plugin_ctx = (struct OutputPluginContext *)p_context ;
	
	uint32_t			block_len_htonl ;
	
	struct timeval			tv_diff_send_filename ;
	struct timeval			tv_diff_send_block_len ;
	struct timeval			tv_diff_send_block ;
	struct timeval			tv_diff_send_eob ;
	
	int				len ;
	
	/* ����TCP�������� */
	block_len_htonl = htonl(0) ;
	gettimeofday( & (p_plugin_ctx->p_forward_session->tv_begin_send_eob) , NULL );
	len = writen( p_plugin_ctx->p_forward_session->sock , & block_len_htonl , sizeof(block_len_htonl) ) ;
	gettimeofday( & (p_plugin_ctx->p_forward_session->tv_end_send_eob) , NULL );
	if( len == -1 )
	{
		ERRORLOG( "send block len to socket failed , errno[%d]" , errno )
		close( p_plugin_ctx->p_forward_session->sock ); p_plugin_ctx->p_forward_session->sock = -1 ;
		return 1;
	}
	else
	{
		INFOLOG( "send block len to socket ok , [%d]bytes" , sizeof(block_len_htonl) )
		DEBUGHEXLOG( (char*) & block_len_htonl , len , NULL )
	}
	
	DiffTimeval( & (p_plugin_ctx->p_forward_session->tv_begin_send_filename) , & (p_plugin_ctx->p_forward_session->tv_end_send_filename) , & tv_diff_send_filename );
	DiffTimeval( & (p_plugin_ctx->p_forward_session->tv_begin_send_block_len) , & (p_plugin_ctx->p_forward_session->tv_end_send_block_len) , & tv_diff_send_block_len );
	DiffTimeval( & (p_plugin_ctx->p_forward_session->tv_begin_send_block) , & (p_plugin_ctx->p_forward_session->tv_end_send_block) , & tv_diff_send_block );
	DiffTimeval( & (p_plugin_ctx->p_forward_session->tv_begin_send_eob) , & (p_plugin_ctx->p_forward_session->tv_end_send_eob) , & tv_diff_send_eob );
	INFOLOG( "SEND-FILENAME[%ld.%06ld] SEND-BLOCK-LEN[%ld.%06ld] SEND-BLOCK[%ld.%06ld] SEND-EOB[%ld.%06ld]"
		, tv_diff_send_filename.tv_sec , tv_diff_send_filename.tv_usec
		, tv_diff_send_block_len.tv_sec , tv_diff_send_block_len.tv_usec
		, tv_diff_send_block.tv_sec , tv_diff_send_block.tv_usec
		, tv_diff_send_eob.tv_sec , tv_diff_send_eob.tv_usec )
	
	return 0;
}

funcCleanOutputPluginContext CleanOutputPluginContext ;
int CleanOutputPluginContext( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , void *p_context )
{
	struct OutputPluginContext	*p_plugin_ctx = (struct OutputPluginContext *)p_context ;
	
	if( p_plugin_ctx->p_forward_session->sock >= 0 )
	{
		INFOLOG( "close forward sock[%d]" , p_plugin_ctx->p_forward_session->sock )
		close( p_plugin_ctx->p_forward_session->sock ); p_plugin_ctx->p_forward_session->sock = -1 ;
	}
	
	return 0;
}

funcUnloadOutputPluginConfig UnloadOutputPluginConfig ;
int UnloadOutputPluginConfig( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , void **pp_context )
{
	struct OutputPluginContext	**pp_plugin_ctx = (struct OutputPluginContext **)pp_context ;
	
	/* �ͷ��ڴ��Դ�Ų�������� */
	free( (*pp_plugin_ctx) ); (*pp_plugin_ctx) = NULL ;
	
	return 0;
}

