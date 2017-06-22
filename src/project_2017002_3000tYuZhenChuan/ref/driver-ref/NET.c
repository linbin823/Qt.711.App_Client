#include "usr.h"



/************************************************************
* VAR
************************************************************/
bool	g_flag_macBrk[2];
bool	g_flag_netLive[2];
bool	g_flag_netAlive[4];
bool	g_flag_interface_en[2];
bool	g_flag_log_macBrk[2];
bool	g_flag_log_netAlive[2];


UINT8	g_flag_work_mode;
UINT8	g_flag_log_work_mode;
bool	g_flag_net_buzz;
bool	g_flag_net_ack;
bool	g_flag_net_rst;
bool	g_flag_engine_happen_call;
bool	g_flag_bridge_happen_call;
bool	g_flag_happen_newAlm;
bool	g_flag_config_forbit;

bool	flag_net_cfg_fail = 0;
bool	flag_net_cfg_fail_log = 0;

UINT8	g_net_monitor_data[513][6];
UINT8	g_net_monitor_logdata[513][6];

struct sockaddr_in	g_caller_from[2][2];
socklen_t			g_caller_lens[2][2];
bool				g_caller_flag[2][2];
int					g_caller_sock[2];

struct sockaddr_in	g_TO_from[2];
socklen_t		g_TO_lens[2];
int					g_TO_sock[2];

UINT8				light_duty[32];

/************************************************************
* FUNC
************************************************************/
STATUS	usr_net_init(void);

void	usr_thread_network_config_1(void *p);
void	usr_thread_network_config_2(void *p);
void	usr_thread_network_data_1(void *p);
void	usr_thread_network_data_2(void *p);
void	usr_thread_network_pro_1			(void *p);
void	usr_thread_network_pro_2			(void *p);
void	usr_udp_single_send(struct sockaddr_in source, struct sockaddr_in dest, char *buf, int len);
void	usr_udp_broadcast_send(struct sockaddr_in source, struct sockaddr_in dest, char *buf, int len);

void	usr_network_config_analyze(int IP_interface_ID, int sock, char *buf, int len, struct sockaddr_in *from, socklen_t fromlen);
void	usr_network_data_analyze(int IP_interface_ID, int sock, char *buf, int len, struct sockaddr_in *from, socklen_t fromlen);
void	usr_network_data_analyze_tmp(int IP_interface_ID, int sock, char *buf, int len, struct sockaddr_in *from, socklen_t fromlen);

void	usr_network_recv_debug_print(int IP_interface_ID, char *buf, int len);


bool	usr_udp_protocol_V9_200(char *bfr, int len, UINT8  cmd);
bool	usr_udp_protocol_V9_202(char *bfr, int len);
bool	usr_udp_protocol_V9_212(char *bfr, int len, UINT32 *caller, UINT32 *beCaller);
bool	usr_udp_protocol_V9_213(char *bfr, int len);
bool	usr_udp_protocol_V9_214(char *bfr, int len, UINT8  *wm);
bool	usr_udp_protocol_V9_222(char *bfr, int len, bool *ld);
int	  usr_udp_protocol_V9_configure(char *bfr, int len);
bool	usr_udp_protocol_MAC_configure(char *bfr, int len);

bool	usr_net_isCmd_wm_Set(char *bfr, int len, UINT8 *wm);
bool	usr_net_isCmd_wm_Read(char *bfr, int len);
bool	usr_net_isCmd_buzz_Free(char *bfr, int len);
bool	usr_net_isCmd_buzz_Cancel(char *bfr, int len);
bool	usr_net_isCmd_sailor_CallorCancel(char *bfr, int len, UINT32 *caller, UINT32 *beCaller);
bool	usr_net_isCmd_net_Ack(char *bfr, int len);
bool	usr_net_isCmd_net_Rst(char *bfr, int len);
bool	usr_net_isCmd_comm_Test(char *bfr, int len);
bool	usr_net_isCmd_adjust_clock(char *bfr, int len);
bool	usr_net_isCmd_ask_cfg(char *bfr, int len);
bool  usr_net_isCmd_duty_light(char *bfr, int len, UINT8 *ld);

int		usr_net_answer_wm(int IP_interface_ID, char *bfr, UINT8 flag);
int		usr_net_answer_comm(int IP_interface_ID, char *bfr);
int 	usr_net_answer_TO(int IP_interface_ID, char *bfr);
int		usr_net_answer_MAC(int IP_interface_ID, char *bfr);
int		usr_net_answer_cfg_OK(int IP_interface_ID, char *bfr);
int		usr_net_answer_cfg_ERROR(int IP_interface_ID, char *bfr);
int		usr_net_answer_call(int IP_interface_ID, char *bfr, UINT32 beCaller);
int		usr_net_answer_cfg(int IP_interface_ID, int sock, struct sockaddr_in *from, socklen_t fromlen);

bool	usr_is_net_buzz_ctrl(void);
void	usr_net_buzz_Free_happen(void);
void	usr_net_buzz_Cancel_happen(void);
bool	usr_is_net_Ack_happen(void);
void	usr_net_Ack_happen(void);
void	usr_clr_net_Ack_happen(void);
bool	usr_is_net_Rst_happen(void);
void	usr_net_Rst_happen(void);
void	usr_clr_net_Rst_happen(void);
bool	usr_is_happen_sailor_engine_call(void);
void	usr_ack_sailor_engine_call(void);
void	usr_sailor_engine_happen_call(void);
bool	usr_is_happen_bridge_engine_call(void);
void	usr_ack_sailor_bridge_call(void);
void	usr_sailor_bridge_happen_call(void);
void	usr_easyalm_take_over(void);
bool	usr_is_happen_net_call(void);

bool	usr_is_happen_net_newAlm(void);
void	usr_net_happen_newAlm(void);
void	usr_ack_net_newAlm(void);

void	usr_net_data_transfer(char *bfr, int len);
void	usr_net_data_analyze(USR_TIME *time, UINT16 offset, UINT16 num);



/************************************************************
* usr_net_init
************************************************************/
STATUS usr_net_init(void)
{
	int i, j;

	for(i = 0; i < 513; i++)
		for(j = 0; j < 6; j++)
		{
			g_net_monitor_data[i][j]	= 0x00;
			g_net_monitor_logdata[i][j] = 0x00;
		}

	g_flag_macBrk[USR_IP_INTERFACE_1]		= true;
	g_flag_macBrk[USR_IP_INTERFACE_2]		= true;
	g_flag_netLive[USR_IP_INTERFACE_1]		= false;
	g_flag_netLive[USR_IP_INTERFACE_2]		= false;
	g_flag_netAlive[USR_IP_INTERFACE_1]		= false;
	g_flag_netAlive[USR_IP_INTERFACE_2]		= false;
	g_flag_netAlive[USR_IP_INTERFACE_1_LOG]		= false;
	g_flag_netAlive[USR_IP_INTERFACE_2_LOG]		= false;
	g_flag_interface_en[USR_IP_INTERFACE_1]	= false;
	g_flag_interface_en[USR_IP_INTERFACE_2]	= false;
	g_flag_log_macBrk[USR_IP_INTERFACE_1]	= true;
	g_flag_log_macBrk[USR_IP_INTERFACE_2]	= true;
	g_flag_log_netAlive[USR_IP_INTERFACE_1]	= false;
	g_flag_log_netAlive[USR_IP_INTERFACE_2]	= false;

	g_flag_work_mode				= USR_WORK_MODE_DUTY_OFF;
	g_flag_log_work_mode			= USR_WORK_MODE_DUTY_OFF;
	g_flag_net_buzz					= false;
	g_flag_net_ack					= false;
	g_flag_net_rst					= false;
	g_flag_engine_happen_call		= false;
	g_flag_bridge_happen_call		= false;
	g_flag_happen_newAlm			= false;
	g_flag_config_forbit			= false;

	g_caller_flag[USR_IP_INTERFACE_1][USR_CALL_ENGINE_ID] = false;
	g_caller_flag[USR_IP_INTERFACE_2][USR_CALL_ENGINE_ID] = false;
	g_caller_flag[USR_IP_INTERFACE_1][USR_CALL_BRIGDE_ID] = false;
	g_caller_flag[USR_IP_INTERFACE_2][USR_CALL_BRIGDE_ID] = false;

	g_caller_sock[USR_IP_INTERFACE_1] = -1;
	g_caller_sock[USR_IP_INTERFACE_2] = -1;

	g_TO_sock[USR_IP_INTERFACE_1] = -1;
	g_TO_sock[USR_IP_INTERFACE_2] = -1;

	return OK;
}

/************************************************************
* usr_thread_network_config_1
************************************************************/
void usr_thread_network_config_1(void *p)
{
	struct sockaddr_in	address;
	struct sockaddr_in	from;
	socklen_t			fromlen;
	char				recv_buf[RECV_BUF_SIZE];
	int					recvNum;
	int					sock, i;

#ifdef USR_NET_RECV_DEBUG
				//xil_printf("usr_thread_network_config_1 begin\n");
#endif

	FOREVER
	{
		for(i = 0; i < RECV_BUF_SIZE; i++)recv_buf[i] = 0;
		// create a UCP socket 
		if ((sock = lwip_socket(AF_INET, SOCK_DGRAM, 0)) < 0)
			return;
			
		address.sin_family		= AF_INET;
		address.sin_port		= g_flash_module_config_port[USR_IP_INTERFACE_1];
		address.sin_addr.s_addr = g_flash_module_net_ip[USR_IP_INTERFACE_1]; 
		
		// bind socket
		if (lwip_bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
			return;
	
		fromlen = sizeof(struct sockaddr_in);
		FOREVER
		{
			recvNum = lwip_recvfrom (sock, recv_buf, RECV_BUF_SIZE, 0, (struct sockaddr *)&from, &fromlen);
	
			if(recvNum <= 0)
				break;
#ifdef USR_NET_RECV_DEBUG
				//usr_network_recv_debug_print(USR_IP_INTERFACE_1, recv_buf, recvNum);
				//xil_printf("usr_thread_network_config_1 recv config %d\n",recvNum);
#endif
	
			if(recvNum > 0)
			{
				usr_network_config_analyze(USR_IP_INTERFACE_1, sock, recv_buf, recvNum, &from, fromlen);
			}
		}
		lwip_close(sock);
#ifdef USR_NET_RECV_DEBUG
				//xil_printf("usr_thread_network_config_1 thread rst\n");
#endif
		//lwip_init();
		sleep(1000);
	}
}


/************************************************************
* usr_thread_network_config_2
************************************************************/
void usr_thread_network_config_2(void *p)
{
	struct sockaddr_in	address;
	struct sockaddr_in	from;
	socklen_t			fromlen;
	char				recv_buf[RECV_BUF_SIZE];
	int					recvNum;
	int					sock, i;

	FOREVER
	{
		for(i = 0; i < RECV_BUF_SIZE; i++)recv_buf[i] = 0;
		// create a UCP socket 
		if ((sock = lwip_socket(AF_INET, SOCK_DGRAM, 0)) < 0)
			return;
			
		address.sin_family		= AF_INET;
		address.sin_port		= g_flash_module_config_port[USR_IP_INTERFACE_2];
		address.sin_addr.s_addr = g_flash_module_net_ip[USR_IP_INTERFACE_2]; 
		
		// bind socket
		if (lwip_bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
			return;
	
		fromlen = sizeof(struct sockaddr_in);
		FOREVER
		{
			recvNum = lwip_recvfrom (sock, recv_buf, RECV_BUF_SIZE, 0, (struct sockaddr *)&from, &fromlen);
	
			if(recvNum <= 0)
				break;
#ifdef USR_NET_RECV_DEBUG
				//usr_network_recv_debug_print(USR_IP_INTERFACE_2, recv_buf, recvNum);
				//xil_printf("usr_thread_network_config_2 recv config %d\n",recvNum);
#endif
	
			if(recvNum > 0)
			{
				usr_network_config_analyze(USR_IP_INTERFACE_2, sock, recv_buf, recvNum, &from, fromlen);
			}
		}
		lwip_close(sock);
#ifdef USR_NET_RECV_DEBUG
				//xil_printf("usr_thread_network_config_2 thread rst\n");
#endif
		//lwip_init();
		sleep(1000);
	}
}


/************************************************************
* usr_thread_network_data_1
************************************************************/
void usr_thread_network_data_1(void *p)
{
	struct sockaddr_in	address;
	struct sockaddr_in	from;
	socklen_t			fromlen;
	char				recv_buf[RECV_BUF_SIZE];
	int					recvNum;
	int					sock, i;
	int					relen;

#ifdef USR_NET_RECV_DEBUG
				//xil_printf("usr_thread_network_data_1 begin\n");
#endif

	FOREVER
	{
		for(i = 0; i < RECV_BUF_SIZE; i++)recv_buf[i] = 0;
		// create a UCP socket 
		if ((sock = lwip_socket(AF_INET, SOCK_DGRAM, 0)) < 0)
			return;
			
		address.sin_family		= AF_INET;
		address.sin_port		= g_flash_module_data_port[USR_IP_INTERFACE_1];
		address.sin_addr.s_addr = /*g_flash_module_broadcast_ip*/g_flash_module_net_ip[USR_IP_INTERFACE_1]; 
		
		// bind socket
		if (lwip_bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
			return;
	

		fromlen = sizeof(struct sockaddr_in);
		
		usr_network_data_analyze_tmp(USR_IP_INTERFACE_1, sock, recv_buf, 10, &address, fromlen);
		FOREVER
		{
			recvNum = lwip_recvfrom (sock, recv_buf, RECV_BUF_SIZE, 0, (struct sockaddr *)&from, &fromlen);
		   //sleep(5);
			if(recvNum <= 0)
				break;
#ifdef USR_NET_RECV_DEBUG
				//usr_network_recv_debug_print(USR_IP_INTERFACE_1, recv_buf, recvNum);
				//xil_printf("usr_thread_network_data_1 recv data %d\n",recvNum);
#endif
	
			if(recvNum > 0)
			{					 
				usr_network_data_analyze_tmp(USR_IP_INTERFACE_1, sock, recv_buf, recvNum, &from, fromlen);
				if((g_flag_interface_en[USR_IP_INTERFACE_1])&&(!g_flag_config_forbit))
					usr_network_data_analyze(USR_IP_INTERFACE_1, sock, recv_buf, recvNum, &from, fromlen);
			}
		}
		lwip_close(sock);
#ifdef USR_NET_RECV_DEBUG
		//xil_printf("usr_thread_network_data_1 thread rst\n");
#endif
		//lwip_init();
		sleep(1000);
	}
}


/************************************************************
* usr_thread_network_data_2
************************************************************/
void usr_thread_network_data_2(void *p)
{
	struct sockaddr_in	address;
	struct sockaddr_in	from;
	socklen_t			fromlen;
	char				recv_buf[RECV_BUF_SIZE];
	int					recvNum;
	int					sock, i;
	int					relen;

	FOREVER
	{
		for(i = 0; i < RECV_BUF_SIZE; i++)recv_buf[i] = 0;
		// create a UCP socket 
		if ((sock = lwip_socket(AF_INET, SOCK_DGRAM, 0)) < 0)
			return;
			
		address.sin_family		= AF_INET;
		address.sin_port		= g_flash_module_data_port[USR_IP_INTERFACE_2];
		address.sin_addr.s_addr = /*g_flash_module_broadcast_ip*/g_flash_module_net_ip[USR_IP_INTERFACE_2]; 
		
		// bind socket
		if (lwip_bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
			return;
	
		fromlen = sizeof(struct sockaddr_in);
		
		usr_network_data_analyze_tmp(USR_IP_INTERFACE_2, sock, recv_buf, 10, &address, fromlen);
		FOREVER
		{
			recvNum = lwip_recvfrom (sock, recv_buf, RECV_BUF_SIZE, 0, (struct sockaddr *)&from, &fromlen);
		   //sleep(5);
			if(recvNum <= 0)
				break;
	#ifdef USR_NET_RECV_DEBUG
				//usr_network_recv_debug_print(USR_IP_INTERFACE_2, recv_buf, recvNum);
				//xil_printf("usr_thread_network_data_2 recv data %d\n",recvNum);
	#endif
	
			if(recvNum > 0)
			{
				usr_network_data_analyze_tmp(USR_IP_INTERFACE_2, sock, recv_buf, recvNum, &from, fromlen);
				if(g_flag_interface_en[USR_IP_INTERFACE_2])
					if(!g_flag_config_forbit)
						usr_network_data_analyze(USR_IP_INTERFACE_2, sock, recv_buf, recvNum, &from, fromlen);
			}
		}
		lwip_close(sock);
#ifdef USR_NET_RECV_DEBUG
				//xil_printf("usr_thread_network_data_2 thread rst\n");
#endif
		//lwip_init();
		sleep(1000);
	}
}


/************************************************************
* usr_thread_network_pro_1
************************************************************/
void usr_thread_network_pro_1(void *p)
{
	struct sockaddr_in	address;
	struct sockaddr_in	from;
	socklen_t			fromlen;
	char				recv_buf[RECV_BUF_SIZE];
	int					recvNum;
	int					sock, i;
	char				rebuf[100];
	int relen;

	FOREVER
	{
		for(i = 0; i < RECV_BUF_SIZE; i++)recv_buf[i] = 0;
		// create a UCP socket 
		if ((sock = lwip_socket(AF_INET, SOCK_DGRAM, 0)) < 0)
			return;
			
		address.sin_family		= AF_INET;
		address.sin_port		= 41013;
		address.sin_addr.s_addr = g_flash_module_net_ip[USR_IP_INTERFACE_1]; 
		
		// bind socket
		if (lwip_bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
			return;
	
		fromlen = sizeof(struct sockaddr_in);
		
		FOREVER
		{
			recvNum = lwip_recvfrom (sock, recv_buf, RECV_BUF_SIZE, 0, (struct sockaddr *)&from, &fromlen);
			
			if(recvNum <= 0)
				break;
	
			if(recvNum > 0)
			{
				if(usr_net_pro_transfer(recv_buf, recvNum, rebuf, &relen))
				{
					lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *) &from, sizeof (struct sockaddr_in)); 
					//lwip_sendto(sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen);
				}
			}
		}
		lwip_close(sock);

		sleep(1000);
	}
}

/************************************************************
* usr_thread_network_pro_2
************************************************************/
void usr_thread_network_pro_2(void *p)
{
	struct sockaddr_in	address;
	struct sockaddr_in	from;
	socklen_t			fromlen;
	char				recv_buf[RECV_BUF_SIZE];
	int					recvNum;
	int					sock, i;
	char				rebuf[100];
	int relen;

	FOREVER
	{
		for(i = 0; i < RECV_BUF_SIZE; i++)recv_buf[i] = 0;
		// create a UCP socket 
		if ((sock = lwip_socket(AF_INET, SOCK_DGRAM, 0)) < 0)
			return;
			
		address.sin_family		= AF_INET;
		address.sin_port		= 41013;
		address.sin_addr.s_addr = g_flash_module_net_ip[USR_IP_INTERFACE_2]; 
		
		// bind socket
		if (lwip_bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
			return;
	
		fromlen = sizeof(struct sockaddr_in);
		
		FOREVER
		{
			recvNum = lwip_recvfrom (sock, recv_buf, RECV_BUF_SIZE, 0, (struct sockaddr *)&from, &fromlen);
			
			if(recvNum <= 0)
				break;
	
			if(recvNum > 0)
			{
				if(usr_net_pro_transfer(recv_buf, recvNum, rebuf, &relen))
				{
						lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *) &from, sizeof (struct sockaddr_in)); 
					//lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen);
				}
			}
		}
		lwip_close(sock);

		sleep(1000);
	}
}

/************************************************************
* usr_udp_single_send
************************************************************/
void usr_udp_single_send(struct sockaddr_in source, struct sockaddr_in dest, char *buf, int len)
{
	int					sock;
	struct sockaddr_in	address;
	struct sockaddr_in	remote;

	// create a UCP socket 
	if ((sock = lwip_socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return;

	address.sin_family		= AF_INET;
	address.sin_port		= source.sin_port;
	address.sin_addr.s_addr = source.sin_addr.s_addr; 
	
	// bind socket
	if (lwip_bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
		return;

	remote.sin_family		= AF_INET;
	remote.sin_port			= dest.sin_port;
	remote.sin_addr.s_addr	= dest.sin_addr.s_addr;

	lwip_sendto (sock, buf, len, 0, (struct sockaddr *) &remote, sizeof (struct sockaddr_in)); 

	close(sock);

}


/************************************************************
* usr_udp_broadcast_send
************************************************************/
void usr_udp_broadcast_send(struct sockaddr_in source, struct sockaddr_in dest, char *buf, int len)
{
	int					sock;
	struct sockaddr_in	address;
	struct sockaddr_in	remote;
	int					on;

	if ((sock = lwip_socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return;
		
	on = 1; 
    if (lwip_setsockopt (sock, SOL_SOCKET, SO_BROADCAST, (char *) &on, sizeof(int))  < 0)
		return ;

	address.sin_family		= AF_INET;
	address.sin_port		= source.sin_port;
	address.sin_addr.s_addr = source.sin_addr.s_addr; 
	
	// bind socket
	if (lwip_bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
		return;
		
	remote.sin_family		= AF_INET;
	remote.sin_port			= dest.sin_port;
	remote.sin_addr.s_addr	= dest.sin_addr.s_addr;
	
	lwip_sendto (sock, buf, len, 0, (struct sockaddr *) &remote, sizeof (struct sockaddr_in)); 
	
	close(sock);

}


/************************************************************
* usr_network_config_analyze
************************************************************/
void usr_network_config_analyze(int IP_interface_ID, int sock, char *buf, int len,
								struct sockaddr_in *from, socklen_t fromlen)
{
	char	rebuf[RECV_BUF_SIZE];
	int		relen;
	UINT32	caller, beCaller;
	struct sockaddr_in *p;

	if(usr_net_isCmd_ask_cfg(buf, len)) // ask configure data need to send
	{
		usr_net_answer_cfg(IP_interface_ID, sock, from, fromlen);
	}
	else if(usr_udp_protocol_MAC_configure(buf, len))	// net configure mac need to deal with
	{
		relen = usr_net_answer_MAC(IP_interface_ID, rebuf);
		lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen); 
	}
	else 
		;
		
	if(g_current_page == USR_LCD_PAGE_CONFIG_SUCCESS)
		return;
		
	if(usr_udp_protocol_V9_configure(buf, len))			// net configure data need to deal with
	{
		relen = usr_net_answer_cfg_OK(IP_interface_ID, rebuf);
		lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen); 
	}
	else
	{
		if((flag_net_cfg_fail == 1)&&(flag_net_cfg_fail_log == 0))
		{
			relen = usr_net_answer_cfg_ERROR(IP_interface_ID, rebuf);
			lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen); 
		}
	}
	flag_net_cfg_fail_log = flag_net_cfg_fail;
	
	if(usr_net_isCmd_comm_Test(buf, len))			// cmd need to answer comm test
	{
		relen = usr_net_answer_comm(IP_interface_ID, rebuf);
		lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen); 
		g_flag_netLive[IP_interface_ID] = true;

		p = &(g_TO_from[IP_interface_ID]);memcpy(p, from, sizeof(struct sockaddr_in));
		g_TO_lens[IP_interface_ID] = fromlen;
		g_TO_sock[IP_interface_ID] = sock;
	}
	else if(usr_net_isCmd_wm_Read(buf, len))			// cmd need to answer module work mode
	{
		relen = usr_net_answer_wm(IP_interface_ID, rebuf, g_flag_work_mode);
		lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen); 
	}
	else if(usr_net_isCmd_sailor_CallorCancel(buf, len, &caller, &beCaller))	// cmd need ack call
	{
		if((g_flash_module_identity_id&beCaller) == g_flash_module_identity_id)
		{
			if(usr_is_happen_sailor_engine_call())
			{
				//g_caller_flag[IP_interface_ID][USR_CALL_ENGINE_ID] = false;
				//usr_ack_sailor_engine_call();
				;
			}
			else 
			{
				g_caller_flag[IP_interface_ID][USR_CALL_ENGINE_ID] = true;
				p = &(g_caller_from[IP_interface_ID][USR_CALL_ENGINE_ID]);memcpy(p, from, sizeof(struct sockaddr_in));
				g_caller_lens[IP_interface_ID][USR_CALL_ENGINE_ID] = fromlen;
				g_caller_sock[IP_interface_ID] = sock;
				usr_sailor_engine_happen_call();
			}
		}
		if(beCaller == 0)
		{
			if(usr_is_happen_sailor_engine_call())
			{
				g_caller_flag[IP_interface_ID][USR_CALL_ENGINE_ID] = false;
				usr_ack_sailor_engine_call();
			}
		}
	}
	else if(usr_net_isCmd_adjust_clock(buf, len))		// cmd need to adjust real time
		;
	else if(usr_net_isCmd_wm_Set(buf, len, &g_flag_work_mode))	// cmd need to set work mode
	{
		relen = usr_net_answer_wm(IP_interface_ID, rebuf, g_flag_work_mode);
		lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen);
	}
	else if(usr_net_isCmd_net_Ack(buf, len))			// cmd need to ack(net)
	{
		usr_net_Ack_happen();
		usr_net_buzz_Cancel_happen();
	}
	else if(usr_net_isCmd_net_Rst(buf, len))			// cmd need to rst(net)
		usr_net_Rst_happen();
	else if(usr_net_isCmd_buzz_Free(buf, len))			// cmd need free buzz(net)
		usr_net_buzz_Free_happen();
	else if(usr_net_isCmd_buzz_Cancel(buf, len))		// cmd need cancel buzz(net)
		usr_net_buzz_Cancel_happen();
	else if(usr_net_isCmd_duty_light(buf, len, light_duty))		// cmd light duty(net)
		;
	else
		;
}

/************************************************************
* usr_network_data_analyze
************************************************************/
void usr_network_data_analyze(	int IP_interface_ID, int sock, char *buf, int len, 
								struct sockaddr_in *from, socklen_t fromlen)
{
	char	rebuf[RECV_BUF_SIZE];
	int		relen;	// data need to deal with
	if(usr_udp_protocol_V9_213(buf, len))
	{
		if(g_flag_work_mode != USR_WORK_MODE_READY)
			usr_net_data_transfer(buf, len);
		
//		xil_printf("rev data \n");
		relen = usr_net_answer_comm(IP_interface_ID, rebuf);
		lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen); 
		//sleep(1);
	}
}


void usr_network_data_analyze_tmp(	int IP_interface_ID, int sock, char *buf, int len, 
								struct sockaddr_in *from, socklen_t fromlen)
{
	char	rebuf[RECV_BUF_SIZE];
	int		relen;	// data need to deal with	
	
		relen = usr_net_answer_comm(IP_interface_ID, rebuf);
		lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen); 
		
}

/************************************************************
* usr_network_recv_debug_print
************************************************************/
void usr_network_recv_debug_print(int IP_interface_ID, char *buf, int len)
{
	int i,j;
	//xil_printf("\n Interface_ID_%d recv: %d",IP_interface_ID, len);

	for(i = 0, j = 0; i < len; i++)
	{
		//xil_printf(" %x",buf[i]);
		j++;
		if(j == 20)
		{
			j = 0;
			//xil_printf("\n");
		}
	}

}



/************************************************************
* usr_udp_protocol_V9_200
************************************************************/
bool usr_udp_protocol_V9_200(char *bfr, int len, UINT8  cmd)
{
	UINT16	data_len;
	int		i;
	UINT32	sum;
	
	data_len = (bfr[6]&0xff) + ((bfr[7]&0xff) << 8);
	if(data_len != USR_V9_CMD_200_FRAME_LEN)
		return false;

	if(((bfr[0]&0xff) != USR_V9_HEAD) || ((bfr[USR_V9_CMD_200_FRAME_LEN + 7 - 1]&0xff) != USR_V9_TAIL))
		return false;

	if((bfr[5]&0xff) != cmd)
		return false;

	sum = 0;
	for(i = 8; i < (USR_V9_CMD_200_FRAME_LEN + 7 - 2); i++)sum+=(bfr[i]&0xff);
	if((sum&0xff) != (bfr[USR_V9_CMD_200_FRAME_LEN + 7 - 2]&0xff))
		return false;

	return true;		
}


/************************************************************
* usr_udp_protocol_V9_202
************************************************************/
bool usr_udp_protocol_V9_202(char *bfr, int len)
{
	int		data_len;
	int		i;
	UINT32	sum;
	int		year, month, date, hour, minute, second;
	
	data_len = (bfr[6]&0xff) + ((bfr[7]&0xff) << 8);
	if(data_len != USR_V9_CMD_202_FRAME_LEN)
		return false;

	if(((bfr[0]&0xff) != USR_V9_HEAD) || ((bfr[USR_V9_CMD_202_FRAME_LEN + 7 - 1]&0xff) != USR_V9_TAIL))
		return false;

	if((bfr[5]&0xff) != USR_V9_TYPE_CLOCK_DATA)
		return false;

	sum = 0;
	for(i = 8; i < (USR_V9_CMD_202_FRAME_LEN + 7 - 2); i++)sum+=(bfr[i]&0xff);
	if((sum&0xff) != (bfr[USR_V9_CMD_202_FRAME_LEN + 7 - 2]&0xff))
		return false;

	year	= bfr[8]&0xff;
	year	+= (bfr[9]&0xff)*100;
	month	= bfr[10]&0xff;
	date	= bfr[11]&0xff;
	hour	= bfr[12]&0xff;
	minute	= bfr[13]&0xff;
	second	= bfr[14]&0xff;

	usr_clock_set_time(year, month, date, hour, minute, second);

	return true;		
}



/************************************************************
* usr_udp_protocol_V9_212
************************************************************/
bool usr_udp_protocol_V9_212(char *bfr, int len, UINT32 *caller, UINT32 *beCaller)
{
	int		data_len;
	int		i;
	UINT32	sum;
	
	data_len = (bfr[6]&0xff) + ((bfr[7]&0xff) << 8);
	if(data_len != USR_V9_CMD_212_FRAME_LEN)
		return false;

	if(((bfr[0]&0xff) != USR_V9_HEAD) || ((bfr[USR_V9_CMD_212_FRAME_LEN + 7 - 1]&0xff) != USR_V9_TAIL))
		return false;

	if((bfr[5]&0xff) != USR_V9_TYPE_SAILOR_CALL)
		return false;

	sum = 0;
	for(i = 8; i < (USR_V9_CMD_212_FRAME_LEN + 7 - 2); i++)sum+=(bfr[i]&0xff);
	if((sum&0xff) != (bfr[USR_V9_CMD_212_FRAME_LEN + 7 - 2]&0xff))
		return false;

	*beCaller	= 0x00000000;
	*beCaller	=  bfr[15]&0xff;
	*beCaller	+= ((bfr[16]&0xff) << 8);
	*beCaller	+= ((bfr[17]&0xff) << 8);
	*beCaller	+= ((bfr[18]&0xff) << 8);

	*caller		= 0x00000000;
	*caller		= bfr[19]&0xff;
	*caller		+= ((bfr[20]&0xff) << 8);
	*caller		+= ((bfr[21]&0xff) << 8);
	*caller		+= ((bfr[22]&0xff) << 8);

	return true;		
}


/************************************************************
* usr_udp_protocol_V9_213
************************************************************/
bool usr_udp_protocol_V9_213(char *bfr, int len)
{
	int		data_len;
	int		i;
	UINT32	sum;

	if(((bfr[0]&0xff) != USR_V9_HEAD) || ((bfr[len - 1]&0xff) != USR_V9_TAIL))
		return false;

	if((bfr[5]&0xff) != USR_V9_TYPE_M_COV_DATA)
		return false;


	data_len = (bfr[6]&0xff) + ((bfr[7]&0xff) << 8);
	if(data_len != (len - 7))
		return false;
	
	sum = 0;
	for(i = 8; i < (len - 2); i++)sum+=(bfr[i]&0xff);
	if((sum&0xff) != (bfr[len-2]&0xff))
		return false;

	return true;
}


/************************************************************
* usr_udp_protocol_V9_214
************************************************************/
bool usr_udp_protocol_V9_214(char *bfr, int len, UINT8  *wm)
{
	int		data_len;
	int		i;
	UINT32	sum;

	data_len = (bfr[6]&0xff) + ((bfr[7]&0xff) << 8);
	if(data_len != USR_V9_CMD_214_FRAME_LEN)
		return false;

	if(((bfr[0]&0xff) != USR_V9_HEAD) || ((bfr[USR_V9_CMD_214_FRAME_LEN + 7 - 1]&0xff) != USR_V9_TAIL))
		return false;
	
	if((bfr[5]&0xff) != USR_V9_TYPE_EXYEND_WM_SET)
		return false;

	sum = 0;
	for(i = 8; i < (USR_V9_CMD_214_FRAME_LEN + 7 - 2); i++)sum+=(bfr[i]&0xff);
	if((sum&0xff) != (bfr[USR_V9_CMD_214_FRAME_LEN + 7 - 2]&0xff))
		return false;

	//*wm = bfr[15]&0xff;
	return true;
}


/************************************************************
* usr_udp_protocol_V9_222
************************************************************/
bool usr_udp_protocol_V9_222(char *bfr, int len, UINT8 *ld)
{
	int		data_len;
	int		i;
	UINT32	sum;

	data_len = (bfr[6]&0xff) + ((bfr[7]&0xff) << 8);
	if(data_len != USR_V9_CMD_222_FRAME_LEN)
		return false;

	if(((bfr[0]&0xff) != USR_V9_HEAD) || ((bfr[USR_V9_CMD_222_FRAME_LEN + 7 - 1]&0xff) != USR_V9_TAIL))
		return false;
	
	if((bfr[5]&0xff) != USR_V9_TYPE_COMM_DL)
		return false;

	sum = 0;
	for(i = 8; i < (USR_V9_CMD_222_FRAME_LEN + 7 - 2); i++)sum+=(bfr[i]&0xff);
	if((sum&0xff) != (bfr[USR_V9_CMD_222_FRAME_LEN + 7 - 2]&0xff))
		return false;

	for(i=0;i<32;i++)
		ld[i] = bfr[15+i]&0xff;
	return true;
}


/************************************************************
* usr_udp_protocol_V9_configure
************************************************************/
int usr_udp_protocol_V9_configure(char *bfr, int len)
{
	int		data_len;
	int		i;
	UINT32	sum;
	UINT8	cnt;
	UINT8	offset;

	if(((bfr[0]&0xff) != USR_V9_HEAD) || ((bfr[len - 1]&0xff) != USR_V9_TAIL))
		return false;
		
	if((bfr[5]&0xff) != USR_V9_TYPE_EXYEND_CONFIGURE)
		return false;

	data_len = (bfr[6]&0xff) + ((bfr[7]&0xff) << 8);
	if(data_len != (len - 7))
		return false;

	sum = 0;
	for(i = 8; i < (len - 2); i++)sum+=(bfr[i]&0xff);
	if((sum&0xff) != (bfr[len-2]&0xff))
		return false;
	
	cnt		= bfr[17];	
	offset	= bfr[16];
	if(offset >= cnt)
		return false;
		
	if(offset == 0)
		flag_net_cfg_fail = 0;
	else
	{
		for(i = 0; i < offset; i++)
			if(!g_net_configure_group[i])
				flag_net_cfg_fail = 1;
	}
		
	g_flag_config_forbit = true;

	if(g_net_configure_group[offset])
		return false;
	g_net_configure_group[offset] = true;

	g_current_page = USR_LCD_PAGE_CONFIG_ING;
	g_current_list = USR_LCD_PAGE_LIST_ALM_TOTAL;

	// store
	data_len = data_len - 2 - 10 - 1;

	for(i = 0; i < data_len; i++)g_CONFIGURE[480 * offset + i] = bfr[18 + i];
	
//#ifdef USR_NET_FLASH_DEBUG
//	xil_printf("net configure pass group %d\n\r", offset);
//#endif

#ifdef USR_NET_FLASH_DEBUG
	//xil_printf("net configure pass group all\n\r");
#endif

	for(i = 0; i < cnt; i++)
		if(!g_net_configure_group[i])
				return false;

	for(i = 0; i < cnt; i++)
		g_net_configure_group[i] = false;
		
	usr_flash_data_set();
	usr_flash_over_default_par();

	g_current_page = USR_LCD_PAGE_CONFIG_SUCCESS;
	g_current_list = USR_LCD_PAGE_LIST_ALM_TOTAL;
	
	return true;		
}


/************************************************************
* usr_udp_protocol_MAC_configure
************************************************************/
bool usr_udp_protocol_MAC_configure(char *bfr, int len)
{
	int i,j,k,l;

	if(((bfr[0]&0xff) != USR_MAC_HEAD_1) || ((bfr[1]&0xff) != USR_MAC_HEAD_2) || ((bfr[2]&0xff) != USR_MAC_HEAD_3) || ((bfr[3]&0xff) != USR_MAC_HEAD_4))
		return false;

	if((bfr[4]&0xff) != USR_TYPE_MAC)
		return false;
		
	//if(len != USR_MAC_LEN)
	//	return false;

	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 0] = bfr[5]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 1] = bfr[6]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 2] = bfr[7]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 3] = bfr[8]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 4] = bfr[9]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 5] = bfr[10]&0xff;

	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 6] = bfr[11]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 7] = bfr[12]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 8] = bfr[13]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 9] = bfr[14]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 10] = bfr[15]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 11] = bfr[16]&0xff;

	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 12] = bfr[17]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 13] = bfr[18]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 14] = bfr[19]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 15] = bfr[20]&0xff;

	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 16] = bfr[21]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 17] = bfr[22]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 18] = bfr[23]&0xff;
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 19] = bfr[24]&0xff;
	
	
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_GROUP + 0] = bfr[25]&0xff; // G_Alm_num
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_GROUP + 1] = bfr[26]&0xff; // G_YesNo_IOin
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_GROUP + 2] = bfr[27]&0xff; // G_YesNo_IOout
	g_CONFIGURE[USR_FLASH_DATA_OFFSET_GROUP + 3] = bfr[28]&0xff; // G_Number_IOled
	
	/*
	k = USR_FLASH_DATA_OFFSET_GROUP + 1;
	l = 26;
	for(i=0;i<9;i++)				// Group Words
		for(j=0; j<48; j++)
		{
			g_CONFIGURE[k++] = bfr[l++];
		}
	*/	



#ifdef USR_NET_MAC_DEBUG
	//xil_printf("net mac pass\n\r");
	//xil_printf("mac1: %x-%x-%x-%x-%x-%x,  mac2: %x-%x-%x-%x-%x-%x \n",
				//bfr[5]&0xff, bfr[6]&0xff, bfr[7]&0xff, bfr[8]&0xff, bfr[9]&0xff, bfr[10]&0xff, 
				//bfr[11]&0xff, bfr[12]&0xff, bfr[13]&0xff, bfr[14]&0xff, bfr[15]&0xff, bfr[16]&0xff);

	//xil_printf("net mac pass\n\r");
	//xil_printf("ip1: %d.%d.%d.%d,  ip2: %d.%d.%d.%d \n",
				//bfr[17], bfr[18], bfr[19], bfr[20],
			//	bfr[21], bfr[22], bfr[23], bfr[24]);
#endif

	usr_flash_data_set();
	usr_flash_over_default_mac();


	return true;
}


/************************************************************
* usr_net_isCmd_XXX
************************************************************/
bool usr_net_isCmd_wm_Set(char *bfr, int len, UINT8 *wm)
{
	return usr_udp_protocol_V9_214(bfr, len, wm);
}

bool usr_net_isCmd_wm_Read(char *bfr, int len)
{
	return usr_udp_protocol_V9_200(bfr, len, USR_V9_TYPE_EXYEND_WM_READ);
}

bool usr_net_isCmd_buzz_Free(char *bfr, int len)
{
	return usr_udp_protocol_V9_200(bfr, len, USR_V9_TYPE_BUZZ_FREE);
}

bool usr_net_isCmd_buzz_Cancel(char *bfr, int len)
{
	return usr_udp_protocol_V9_200(bfr, len, USR_V9_TYPE_BUZZ_CANCEL);
}

bool usr_net_isCmd_sailor_CallorCancel(char *bfr, int len, UINT32 *caller, UINT32 *beCaller)
{
	return usr_udp_protocol_V9_212(bfr, len, caller, beCaller);
}

bool usr_net_isCmd_net_Ack(char *bfr, int len)
{
	return usr_udp_protocol_V9_200(bfr, len, USR_V9_TYPE_NET_ACK);
}

bool usr_net_isCmd_net_Rst(char *bfr, int len)
{
	return usr_udp_protocol_V9_200(bfr, len, USR_V9_TYPE_NET_RST);
}

bool usr_net_isCmd_comm_Test(char *bfr, int len)
{
	return usr_udp_protocol_V9_200(bfr, len, USR_V9_TYPE_COMM_TEST);
}

bool usr_net_isCmd_adjust_clock(char *bfr, int len)
{
	return usr_udp_protocol_V9_202(bfr, len);
}

bool usr_net_isCmd_ask_cfg(char *bfr, int len)
{
	return usr_udp_protocol_V9_200(bfr, len, USR_V9_TYPE_ASK_CONFIGURE);
}

bool usr_net_isCmd_duty_light(char *bfr, int len, UINT8 *ld)
{
	return usr_udp_protocol_V9_222(bfr, len, ld);
}


/************************************************************
* usr_net_answer_wm
************************************************************/
int usr_net_answer_wm(int IP_interface_ID, char *bfr, UINT8 flag)
{
	int i, sum;

	bfr[0]	= USR_V9_HEAD;
	bfr[1]	= (g_flash_module_net_ip[IP_interface_ID]>>24)&0xff;
	bfr[2]	= (g_flash_module_net_ip[IP_interface_ID]>>16)&0xff;
	bfr[3]	= (g_flash_module_net_ip[IP_interface_ID]>>8)&0xff;
	bfr[4]	= g_flash_module_net_ip[IP_interface_ID]&0xff;
	bfr[5]	= USR_V9_TYPE_EXYEND_WM;
	bfr[6]	= USR_V9_CMD_215_FRAME_LEN&0xff;
	bfr[7]	= (USR_V9_CMD_215_FRAME_LEN>>8)&0xff;
	bfr[9]	= usr_get_now_year() / 100;
	bfr[8]	= usr_get_now_year() - (bfr[9]*100);
	bfr[10]	= (usr_get_now_month())&0xff;
	bfr[11]	= (usr_get_now_date())&0xff;
	bfr[12]	= (usr_get_now_hour())&0xff;
	bfr[13]	= (usr_get_now_minute())&0xff;
	bfr[14]	= (usr_get_now_second())&0xff;
	bfr[15]	= flag;
	for(i = 8, sum = 0; i < 16; i++)sum	+= bfr[i];
	bfr[16]	= sum&0xff;
	bfr[17]	= USR_V9_TAIL;

	return 18;
}

/************************************************************
* usr_net_answer_comm
************************************************************/
int usr_net_answer_comm(int IP_interface_ID, char *bfr)
{
	int i, sum;

	bfr[0]	= USR_V9_HEAD;
	bfr[1]	= (g_flash_module_net_ip[IP_interface_ID]>>24)&0xff;
	bfr[2]	= (g_flash_module_net_ip[IP_interface_ID]>>16)&0xff;
	bfr[3]	= (g_flash_module_net_ip[IP_interface_ID]>>8)&0xff;
	bfr[4]	= g_flash_module_net_ip[IP_interface_ID]&0xff;
	bfr[5]	= USR_V9_TYPE_COMM_ACK;
	bfr[6]	= USR_V9_CMD_200_FRAME_LEN&0xff;
	bfr[7]	= (USR_V9_CMD_200_FRAME_LEN>>8)&0xff;
	bfr[9]	= usr_get_now_year() / 100;
	bfr[8]	= usr_get_now_year() - (bfr[9]*100);
	bfr[10]	= (usr_get_now_month())&0xff;
	bfr[11]	= (usr_get_now_date())&0xff;
	bfr[12]	= (usr_get_now_hour())&0xff;
	bfr[13]	= (usr_get_now_minute())&0xff;
	bfr[14]	= (usr_get_now_second())&0xff;
	for(i = 8, sum = 0; i < 15; i++)sum	+= bfr[i];
	bfr[15]	= sum&0xff;
	bfr[16]	= USR_V9_TAIL;

	return 17;
}


/************************************************************
* usr_net_answer_TO
************************************************************/
int usr_net_answer_TO(int IP_interface_ID, char *bfr)
{
	int i, sum;

	bfr[0]	= USR_V9_HEAD;
	bfr[1]	= (g_flash_module_net_ip[IP_interface_ID]>>24)&0xff;
	bfr[2]	= (g_flash_module_net_ip[IP_interface_ID]>>16)&0xff;
	bfr[3]	= (g_flash_module_net_ip[IP_interface_ID]>>8)&0xff;
	bfr[4]	= g_flash_module_net_ip[IP_interface_ID]&0xff;
	bfr[5]	= USR_V9_TYPE_COMM_TO;
	bfr[6]	= USR_V9_CMD_200_FRAME_LEN&0xff;
	bfr[7]	= (USR_V9_CMD_200_FRAME_LEN>>8)&0xff;
	bfr[9]	= usr_get_now_year() / 100;
	bfr[8]	= usr_get_now_year() - (bfr[9]*100);
	bfr[10]	= (usr_get_now_month())&0xff;
	bfr[11]	= (usr_get_now_date())&0xff;
	bfr[12]	= (usr_get_now_hour())&0xff;
	bfr[13]	= (usr_get_now_minute())&0xff;
	bfr[14]	= (usr_get_now_second())&0xff;
	for(i = 8, sum = 0; i < 15; i++)sum	+= bfr[i];
	bfr[15]	= sum&0xff;
	bfr[16]	= USR_V9_TAIL;

	return 17;
}



/************************************************************
* usr_net_answer_MAC
************************************************************/
int usr_net_answer_MAC(int IP_interface_ID, char *bfr)
{
	bfr[0]	= USR_MAC_HEAD_1;
	bfr[1]	= USR_MAC_HEAD_2;
	bfr[2]	= USR_MAC_HEAD_3;
	bfr[3]	= USR_MAC_HEAD_4;
	bfr[4]	= USR_TYPE_MAC;

	return 5;
}


/************************************************************
* usr_net_answer_cfg_OK
************************************************************/
int usr_net_answer_cfg_OK(int IP_interface_ID, char *bfr)
{
	int i, sum;

	bfr[0]	= USR_V9_HEAD;
	bfr[1]	= (g_flash_module_net_ip[IP_interface_ID]>>24)&0xff;
	bfr[2]	= (g_flash_module_net_ip[IP_interface_ID]>>16)&0xff;
	bfr[3]	= (g_flash_module_net_ip[IP_interface_ID]>>8)&0xff;
	bfr[4]	= g_flash_module_net_ip[IP_interface_ID]&0xff;
	bfr[5]	= USR_V9_TYPE_CONFIGURE_OK;
	bfr[6]	= USR_V9_CMD_200_FRAME_LEN&0xff;
	bfr[7]	= (USR_V9_CMD_200_FRAME_LEN>>8)&0xff;
	bfr[9]	= usr_get_now_year() / 100;
	bfr[8]	= usr_get_now_year() - (bfr[9]*100);
	bfr[10]	= (usr_get_now_month())&0xff;
	bfr[11]	= (usr_get_now_date())&0xff;
	bfr[12]	= (usr_get_now_hour())&0xff;
	bfr[13]	= (usr_get_now_minute())&0xff;
	bfr[14]	= (usr_get_now_second())&0xff;
	bfr[15]	= 0x00;
	bfr[16]	= 0x00;
	bfr[17]	= 0x00;
	bfr[18]	= 0x00;
	bfr[19]	= 0x00;
	for(i = 8, sum = 0; i < 20; i++)sum	+= bfr[i];
	bfr[20]	= sum&0xff;
	bfr[21]	= USR_V9_TAIL;

	return 22;
}

/************************************************************
* usr_net_answer_cfg_ERROR
************************************************************/
int usr_net_answer_cfg_ERROR(int IP_interface_ID, char *bfr)
{
	int i, sum;

	bfr[0]	= USR_V9_HEAD;
	bfr[1]	= (g_flash_module_net_ip[IP_interface_ID]>>24)&0xff;
	bfr[2]	= (g_flash_module_net_ip[IP_interface_ID]>>16)&0xff;
	bfr[3]	= (g_flash_module_net_ip[IP_interface_ID]>>8)&0xff;
	bfr[4]	= g_flash_module_net_ip[IP_interface_ID]&0xff;
	bfr[5]	= USR_V9_TYPE_CONFIGURE_OK;
	bfr[6]	= USR_V9_CMD_200_FRAME_LEN&0xff;
	bfr[7]	= (USR_V9_CMD_200_FRAME_LEN>>8)&0xff;
	bfr[9]	= usr_get_now_year() / 100;
	bfr[8]	= usr_get_now_year() - (bfr[9]*100);
	bfr[10]	= (usr_get_now_month())&0xff;
	bfr[11]	= (usr_get_now_date())&0xff;
	bfr[12]	= (usr_get_now_hour())&0xff;
	bfr[13]	= (usr_get_now_minute())&0xff;
	bfr[14]	= (usr_get_now_second())&0xff;
	bfr[15]	= 0x00;
	bfr[16]	= 0x00;
	bfr[17]	= 0x00;
	bfr[18]	= 0x00;
	bfr[19]	= 0x01;
	for(i = 8, sum = 0; i < 20; i++)sum	+= bfr[i];
	bfr[20]	= sum&0xff;
	bfr[21]	= USR_V9_TAIL;

	return 22;
}



/************************************************************
* usr_net_answer_call
************************************************************/
int usr_net_answer_call(int IP_interface_ID, char *bfr, UINT32 beCaller)
{
	int i, sum;

	bfr[0]	= USR_V9_HEAD;
	bfr[1]	= (g_flash_module_net_ip[IP_interface_ID]>>24)&0xff;
	bfr[2]	= (g_flash_module_net_ip[IP_interface_ID]>>16)&0xff;
	bfr[3]	= (g_flash_module_net_ip[IP_interface_ID]>>8)&0xff;
	bfr[4]	= g_flash_module_net_ip[IP_interface_ID]&0xff;
	bfr[5]	= USR_V9_TYPE_SAILOR_ACK;
	bfr[6]	= USR_V9_CMD_216_FRAME_LEN&0xff;
	bfr[7]	= (USR_V9_CMD_216_FRAME_LEN>>8)&0xff;
	bfr[9]	= usr_get_now_year() / 100;
	bfr[8]	= usr_get_now_year() - (bfr[9]*100);
	bfr[10]	= (usr_get_now_month())&0xff;
	bfr[11]	= (usr_get_now_date())&0xff;
	bfr[12]	= (usr_get_now_hour())&0xff;
	bfr[13]	= (usr_get_now_minute())&0xff;
	bfr[14]	= (usr_get_now_second())&0xff;
	bfr[15]	= beCaller&0xff;
	bfr[16]	= (beCaller>>8)&0xff;
	bfr[17]	= (beCaller>>16)&0xff;
	bfr[18]	= (beCaller>>24)&0xff;
	for(i = 8, sum = 0; i < 19; i++)sum	+= bfr[i];
	bfr[19]	= sum&0xff;
	bfr[20]	= USR_V9_TAIL;

	return 21;
}



/************************************************************
* usr_net_answer_cfg
************************************************************/
int usr_net_answer_cfg(int IP_interface_ID, int sock, struct sockaddr_in *from, socklen_t fromlen)
{
	int i,j,k,m,sum;
	Xuint8 bfr[RECV_BUF_SIZE];
	int len,all_len;
	
	
	bfr[0]	= USR_V9_HEAD;
	bfr[1]	= (g_flash_module_net_ip[IP_interface_ID]>>24)&0xff;
	bfr[2]	= (g_flash_module_net_ip[IP_interface_ID]>>16)&0xff;
	bfr[3]	= (g_flash_module_net_ip[IP_interface_ID]>>8)&0xff;
	bfr[4]	= g_flash_module_net_ip[IP_interface_ID]&0xff;
	bfr[5]	= USR_V9_TYPE_EXYEND_CONFIGURE;


	len = USR_FLASH_DATA_NUM/480;
	if(USR_FLASH_DATA_NUM%480)
		all_len = len+1;
		
	for(i = 0; i < len; i++)
	{
		bfr[9]	= usr_get_now_year() / 100;
		bfr[8]	= usr_get_now_year() - (bfr[9]*100);
		bfr[10]	= (usr_get_now_month())&0xff;
		bfr[11]	= (usr_get_now_date())&0xff;
		bfr[12]	= (usr_get_now_hour())&0xff;
		bfr[13]	= (usr_get_now_minute())&0xff;
		bfr[14]	= (usr_get_now_second())&0xff;
		bfr[15]	= 1;
		bfr[16]	= i;
		bfr[17]	= all_len;
		for(j = 0; j < 480; j++)bfr[18+j] = g_CONFIGURE[(480 * i) + j];
		for(k = 8, sum = 0; k < 498; k++)sum	+= bfr[k];
		bfr[18+j] = sum&0xff;
		bfr[19+j]	= USR_V9_TAIL;
		
		bfr[6]	= (13+j)&0xff;
		bfr[7]	= ((13+j)>>8)&0xff;
		
		lwip_sendto (sock, bfr, (j+20), 0, (struct sockaddr *)from, fromlen);
	}
	
	if(all_len > len)
	{
		m = USR_FLASH_DATA_NUM%480;
		bfr[9]	= usr_get_now_year() / 100;
		bfr[8]	= usr_get_now_year() - (bfr[9]*100);
		bfr[10]	= (usr_get_now_month())&0xff;
		bfr[11]	= (usr_get_now_date())&0xff;
		bfr[12]	= (usr_get_now_hour())&0xff;
		bfr[13]	= (usr_get_now_minute())&0xff;
		bfr[14]	= (usr_get_now_second())&0xff;
		bfr[15]	= 1;
		bfr[16]	= all_len-1;
		bfr[17]	= all_len;
		for(j = 0; j < m; j++)bfr[18+j] = g_CONFIGURE[(480 * len) + j];
		for(k = 8, sum = 0; k < (18+m); k++)sum	+= bfr[k];
		bfr[18+j] = sum&0xff;
		bfr[19+j]	= USR_V9_TAIL;
		
		bfr[6]	= (13+j)&0xff;
		bfr[7]	= ((13+j)>>8)&0xff;
		
		lwip_sendto (sock, bfr, (j+20), 0, (struct sockaddr *)from, fromlen);
	}
		
	return true;
}




/************************************************************
* usr_is_net_buzz_ctrl
************************************************************/
bool usr_is_net_buzz_ctrl(void)
{
	return g_flag_net_buzz;
}


/************************************************************
* usr_net_buzz_Free_happen
************************************************************/
void usr_net_buzz_Free_happen(void)
{
	g_flag_net_buzz = true;
}


/************************************************************
* usr_net_buzz_Cancel_happen
************************************************************/
void usr_net_buzz_Cancel_happen(void)
{
	g_flag_net_buzz = false;
}


/************************************************************
* usr_is_net_Ack_happen
************************************************************/
bool usr_is_net_Ack_happen(void)
{
	return g_flag_net_ack;
}

/************************************************************
* usr_net_Ack_happen
************************************************************/
void usr_net_Ack_happen(void)
{
	g_flag_net_ack	= true;
}

/************************************************************
* usr_clr_net_Ack_happen
************************************************************/
void usr_clr_net_Ack_happen(void)
{
	g_flag_net_ack = false;
}


/************************************************************
* usr_is_net_Rst_happen
************************************************************/
bool usr_is_net_Rst_happen(void)
{
	return g_flag_net_rst;
}

/************************************************************
* usr_net_Rst_happen
************************************************************/
void usr_net_Rst_happen(void)
{
	g_flag_net_rst = true;
}

/************************************************************
* usr_clr_net_Rst_happen
************************************************************/
void usr_clr_net_Rst_happen(void)
{
	g_flag_net_rst = false;
}

/************************************************************
* usr_is_happen_sailor_engine_call
************************************************************/
bool usr_is_happen_sailor_engine_call(void)
{
	return g_flag_engine_happen_call;
}


/************************************************************
* usr_ack_sailor_engine_call
************************************************************/
void usr_ack_sailor_engine_call(void)
{
	struct sockaddr_in *from;
	socklen_t		fromlen;
	int				sock;
	char			rebuf[RECV_BUF_SIZE];
	int				relen;

	if(g_caller_flag[USR_IP_INTERFACE_1][USR_CALL_ENGINE_ID])
	{
		sock	= g_caller_sock[USR_IP_INTERFACE_1];
		relen	= usr_net_answer_call(USR_IP_INTERFACE_1, rebuf, g_flash_module_identity_id);
		from	= &g_caller_from[USR_IP_INTERFACE_1][USR_CALL_ENGINE_ID];
		fromlen	= g_caller_lens[USR_IP_INTERFACE_1][USR_CALL_ENGINE_ID];
		lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen); 
	}

	if(g_caller_flag[USR_IP_INTERFACE_2][USR_CALL_ENGINE_ID])
	{
		sock	= g_caller_sock[USR_IP_INTERFACE_2];
		relen	= usr_net_answer_call(USR_IP_INTERFACE_2, rebuf, g_flash_module_identity_id);
		from	= &g_caller_from[USR_IP_INTERFACE_2][USR_CALL_ENGINE_ID];
		fromlen	= g_caller_lens[USR_IP_INTERFACE_2][USR_CALL_ENGINE_ID];
		lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen); 
	}

	g_flag_engine_happen_call = false;
	usr_led_clr_flag_overturn(USR_LED_ID_CALLFROM_ENGINE);
}


/************************************************************
* usr_sailor_engine_happen_call
************************************************************/
void usr_sailor_engine_happen_call(void)
{
	g_flag_engine_happen_call = true;
	usr_led_set_flag_overturn(USR_LED_ID_CALLFROM_ENGINE);
}


/************************************************************
* usr_is_happen_bridge_engine_call
************************************************************/
bool usr_is_happen_bridge_engine_call(void)
{
	return g_flag_bridge_happen_call;
}

/************************************************************
* usr_ack_sailor_bridge_call
************************************************************/
void usr_ack_sailor_bridge_call(void)
{
	struct sockaddr_in *from;
	socklen_t		fromlen;
	int				sock;
	char			rebuf[RECV_BUF_SIZE];
	int				relen;

	if(g_caller_flag[USR_IP_INTERFACE_1][USR_CALL_BRIGDE_ID])
	{
		sock	= g_caller_sock[USR_IP_INTERFACE_1];
		relen	= usr_net_answer_call(USR_IP_INTERFACE_1, rebuf, g_flash_module_identity_id);
		from	= &g_caller_from[USR_IP_INTERFACE_1][USR_CALL_BRIGDE_ID];
		fromlen	= g_caller_lens[USR_IP_INTERFACE_1][USR_CALL_BRIGDE_ID];
		lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen); 
	}

	if(g_caller_flag[USR_IP_INTERFACE_2][USR_CALL_BRIGDE_ID])
	{
		sock	= g_caller_sock[USR_IP_INTERFACE_2];
		relen	= usr_net_answer_call(USR_IP_INTERFACE_2, rebuf, g_flash_module_identity_id);
		from	= &g_caller_from[USR_IP_INTERFACE_2][USR_CALL_BRIGDE_ID];
		fromlen	= g_caller_lens[USR_IP_INTERFACE_2][USR_CALL_BRIGDE_ID];
		lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen); 
	}

	g_flag_bridge_happen_call = false;
	usr_led_clr_flag_overturn(USR_LED_ID_CALLFROM_BRIDGE);
}

/************************************************************
* usr_sailor_bridge_happen_call
************************************************************/
void usr_sailor_bridge_happen_call(void)
{
	g_flag_bridge_happen_call = true;
	usr_led_set_flag_overturn(USR_LED_ID_CALLFROM_BRIDGE);
}


/************************************************************
* usr_easyalm_take_over
************************************************************/
void usr_easyalm_take_over(void)
{
	struct sockaddr_in *from;
	socklen_t		fromlen;
	int				sock;
	char			rebuf[RECV_BUF_SIZE];
	int				relen;

	sock	= g_TO_sock[USR_IP_INTERFACE_1];
	relen	= usr_net_answer_TO(USR_IP_INTERFACE_1, rebuf);
	from	= &g_TO_from[USR_IP_INTERFACE_1];
	fromlen	= g_TO_lens[USR_IP_INTERFACE_1];
	lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen); 

	sock	= g_TO_sock[USR_IP_INTERFACE_2];
	relen	= usr_net_answer_TO(USR_IP_INTERFACE_2, rebuf);
	from	= &g_TO_from[USR_IP_INTERFACE_2];
	fromlen	= g_TO_lens[USR_IP_INTERFACE_2];
	lwip_sendto (sock, rebuf, relen, 0, (struct sockaddr *)from, fromlen); 
}


/************************************************************
* usr_is_happen_net_newAlm
************************************************************/
bool usr_is_happen_net_newAlm(void)
{
	return g_flag_happen_newAlm;
}


/************************************************************
* usr_net_happen_newAlm
************************************************************/
void usr_net_happen_newAlm(void)
{
	g_flag_happen_newAlm	= true;

	if((g_current_page != USR_LCD_PAGE_SLEEP)&&(g_current_page != USR_LCD_PAGE_CONFIG_ING)&&(g_current_page != USR_LCD_PAGE_CONFIG_SUCCESS))
	{
		g_current_page = USR_LCD_PAGE_LIST_ALM;
		g_current_list = USR_LCD_PAGE_LIST_ALM_TOTAL;
	}
}


/************************************************************
* usr_ack_net_newAlm
************************************************************/
void usr_ack_net_newAlm(void)
{
	g_flag_happen_newAlm = false;
}


/************************************************************
* usr_is_happen_net_call
************************************************************/
bool usr_is_happen_net_call(void)
{
	return ((usr_is_happen_sailor_engine_call()) || (usr_is_happen_bridge_engine_call()));
}



/************************************************************
* usr_net_data_transfer
************************************************************/
void usr_net_data_transfer(char *bfr, int len)
{
	USR_TIME	time;
	UINT16		group, self_group;
	UINT16		offset;
	UINT8		num, i, j;

	group	= (bfr[16]&0xff) + ((bfr[15]&0xff) << 8);
	offset	= (bfr[18]&0xff) + ((bfr[17]&0xff) << 8);
	num		= (bfr[19]&0xff);

   //xil_printf("%d %d %d\n",group,offset,num);
	if(num > 80)
		return;
		
	if((offset+num)>513)
		return;

	self_group = (UINT16)(g_flash_module_group&0xffff);
	if(self_group != group)
		return;

	for(i = 0; i < num; i++)
		for(j = 0; j < 6; j++)
			g_net_monitor_data[offset + i][j] = bfr[20 + i*6 + j];

	time.year_l	= (bfr[8]&0xff);
	time.year_h = (bfr[9]&0xff);
	time.month	= (bfr[10]&0xff);
	time.date	= (bfr[11]&0xff);
	time.hrs	= (bfr[12]&0xff);
	time.mins	= (bfr[13]&0xff);
	time.secs	= (bfr[14]&0xff);

	usr_net_data_analyze(&time, offset, num);
}




/************************************************************
* usr_net_data_analyze
************************************************************/
void usr_net_data_analyze(USR_TIME *time, UINT16 offset, UINT16 num)
{
	UINT16	i;
	UINT32	tag_id;
	UINT16	alarm, log_alm;
	UINT16	value;
	UINT16	limit;
	bool	lv_1, log_lv_1; // alm level 1
	bool	lv_2, log_lv_2; // alm level 2
	bool	lv_3, log_lv_3; // alm level 3
	bool	lv_4, log_lv_4; // alm level break

	for(i = 0; i < num; i++)
	{
		value	= g_net_monitor_data[offset + i][0] + (g_net_monitor_data[offset + i][1] << 8);
		alarm	= g_net_monitor_data[offset + i][2] + (g_net_monitor_data[offset + i][3] << 8);
		limit	= g_net_monitor_data[offset + i][4] + (g_net_monitor_data[offset + i][5] << 8);
		tag_id	= offset + i;
		log_alm	= g_net_monitor_logdata[offset + i][2] + (g_net_monitor_logdata[offset + i][3] << 8);

		if(usr_ackbit16(alarm, 5))
			alarm = 0;
			
		if((usr_ackbit16(alarm, 0)) || (usr_ackbit16(alarm, 8)))
			lv_1 = true;
		else
			lv_1 = false;

		if((usr_ackbit16(alarm, 1)) || (usr_ackbit16(alarm, 9)))
			lv_2 = true;
		else
			lv_2 = false;

		if((usr_ackbit16(alarm, 2)) || (usr_ackbit16(alarm, 10)))
			lv_3 = true;
		else
			lv_3 = false;

		if(usr_ackbit16(alarm, 3))
			lv_4 = true;
		else
			lv_4 = false;

		if((usr_ackbit16(log_alm, 0)) || (usr_ackbit16(log_alm, 8)))
			log_lv_1 = true;
		else
			log_lv_1 = false;

		if((usr_ackbit16(log_alm, 1)) || (usr_ackbit16(log_alm, 9)))
			log_lv_2 = true;
		else
			log_lv_2 = false;

		if((usr_ackbit16(log_alm, 2)) || (usr_ackbit16(log_alm, 10)))
			log_lv_3 = true;
		else
			log_lv_3 = false;

		if(usr_ackbit16(log_alm, 3))
			log_lv_4 = true;
		else
			log_lv_4 = false;

		if((lv_1)&&(!log_lv_1))
		{
			usr_del_alm(tag_id, 1);
			usr_add_alm(tag_id, 1);
			usr_refurbish_alm_happen_time(tag_id, 1, time);
			usr_net_happen_newAlm();
			usr_net_group_sort_newAlm(tag_id, 1);
		}
		else if((!lv_1)&&(log_lv_1))
			;//usr_del_alm(tag_id, 1);
		else if(lv_1)
		{
			usr_refurbish_alm_real_time_value(tag_id, 1, value);
			usr_refurbish_alm_limit_value(tag_id, 1, limit);
		}
		else
			;

		if((lv_2)&&(!log_lv_2))
		{
			usr_del_alm(tag_id, 2);
			usr_add_alm(tag_id, 2);
			usr_refurbish_alm_happen_time(tag_id, 2, time);
			usr_net_happen_newAlm();
			usr_net_group_sort_newAlm(tag_id, 2);
		}
		else if((!lv_2)&&(log_lv_2))
			;//usr_del_alm(tag_id, 2);
		else if(lv_2)
		{
			usr_refurbish_alm_real_time_value(tag_id, 2, value);
			usr_refurbish_alm_limit_value(tag_id, 2, limit);
		}
		else
			;

		if((lv_3)&&(!log_lv_3)&&(!(usr_get_alm_flag_wait_rdyRst(tag_id, 3))))
		{
			usr_add_alm(tag_id, 3);
			usr_refurbish_alm_happen_time(tag_id, 3, time);
			usr_net_happen_newAlm();
			usr_net_group_sort_newAlm(tag_id, 3);
		}
		else if((lv_3)&&(!log_lv_3)&&((usr_get_alm_flag_wait_rdyRst(tag_id, 3))))
		{
			usr_del_alm(tag_id, 3);
			usr_add_alm(tag_id, 3);
			usr_refurbish_alm_happen_time(tag_id, 3, time);
			usr_net_happen_newAlm();
			usr_net_group_sort_newAlm(tag_id, 3);
		}
		else if((!lv_3)&&(log_lv_3))
		{
			usr_refurbish_alm_flag_wait_rdyRst(tag_id, 3, true);
		}
		else if((lv_3) || (usr_get_alm_flag_wait_rdyRst(tag_id, 3)))
		{
			usr_refurbish_alm_real_time_value(tag_id, 3, value);
			usr_refurbish_alm_limit_value(tag_id, 3, limit);
		}
		else
			;

		if((lv_4)&&(!log_lv_4))
		{
			usr_del_alm(tag_id, 4);
			usr_add_alm(tag_id, 4);
			usr_refurbish_alm_happen_time(tag_id, 4, time);
			usr_net_happen_newAlm();
			usr_net_group_sort_newAlm(tag_id, 4);
		}
		else if((!lv_4)&&(log_lv_4))
			;//usr_del_alm(tag_id, 4);
		else if(lv_4)
		{
			usr_refurbish_alm_real_time_value(tag_id, 4, value);
			usr_refurbish_alm_limit_value(tag_id, 4, limit);
		}
		else
			;

		g_net_monitor_logdata[offset + i][0] = g_net_monitor_data[offset + i][0];
		g_net_monitor_logdata[offset + i][1] = g_net_monitor_data[offset + i][1];
		g_net_monitor_logdata[offset + i][2] = g_net_monitor_data[offset + i][2];
		g_net_monitor_logdata[offset + i][3] = g_net_monitor_data[offset + i][3];
		g_net_monitor_logdata[offset + i][4] = g_net_monitor_data[offset + i][4];
		g_net_monitor_logdata[offset + i][5] = g_net_monitor_data[offset + i][5];
	}

}



/************************************************************
* usr_net_data_analyze2
************************************************************/
void usr_net_data_analyze2(void)
{
	int offset = 0;
	UINT16	i;
	UINT32	tag_id;
	UINT16	alarm, log_alm;
	UINT16	value;
	UINT16	limit;
	bool	lv_1;// alm level 1
	bool	lv_2;// alm level 2
	bool	lv_4;// alm level break
	
	for(i = 0; i < 480; i++)
	{
		alarm	= g_net_monitor_data[offset + i][2] + (g_net_monitor_data[offset + i][3] << 8);
		tag_id	= offset + i;

		if((usr_ackbit16(alarm, 0)) || (usr_ackbit16(alarm, 8)))
			lv_1 = true;
		else
			lv_1 = false;

		if((usr_ackbit16(alarm, 1)) || (usr_ackbit16(alarm, 9)))
			lv_2 = true;
		else
			lv_2 = false;

		if(usr_ackbit16(alarm, 3))
			lv_4 = true;
		else
			lv_4 = false;

		if(!lv_1)
			usr_del_alm(tag_id, 1);
		
		if(!lv_2)
			usr_del_alm(tag_id, 2);
		
		if(!lv_4)
			usr_del_alm(tag_id, 4);
		
		if(usr_ackbit16(alarm, 5)) {
			usr_del_alm(tag_id, 1);
			usr_del_alm(tag_id, 2);
			usr_del_alm(tag_id, 4);
		}
	}

}


//**********************************************************************
//* Function:		usr_net_pro_transfer
//* Description: 	接受上位机下载命令
//*	return:			OK
char	flag_dl[10000];
int 	flag_sector;
int usr_net_pro_transfer(char *bfr, int len, char *rebuf, int *relen)
{
	int 			i,j;
	Xuint8		m;
	unsigned int	id;
	Xuint8		buf[300],tsum,sum;
	int				sector,page;

	if((bfr[0] == 0x22)&&(bfr[1] == 0x44)&&(bfr[2] == 0x66))
	{
		buf[0] = bfr[3];
		buf[1] = bfr[4];
		buf[2] = bfr[5];
		buf[3] = bfr[6];
		id = buf[3] + (buf[2]<<8) + (buf[1]<<16) + (buf[0]<<24); 
		//xil_printf("len: %d  id: %d\n",len,id);
		
		sum = 0;
		for(i=0; i<256;i++)
		{
			buf[i]=bfr[7+i];
			sum += bfr[7+i];
		}
	/*
		xil_printf("\n\nrev form YY\n\n");
		for(i=0;i<16;i++)
		{
			for(j=0;j<16;j++)
			{
			xil_printf("%x ",buf[i*16+j]);
			}
			xil_printf("\n");
		}
	*/
		tsum = bfr[263];
		if(tsum == sum)
		{
				//xil_printf("id: %d pass\n",id);
				
			sector	= (id-1)/256;
			page	= (id-1)%256;
			if((sector==0)&&(page == 0))
				{
				for(i=0;i<10000;i++)flag_dl[i] = 0;
				}
	
			if(flag_dl[id-1] == 0)
			{
				if((page == 0)&&(flag_sector == 0))
					{
					for(m=0;m<64;m++)
						drv_flash_sector_erase(m);
					flag_sector = 1;
					return 0;
					}
				
				drv_flash_write_page_256byte(sector, page, buf);
				flag_dl[id-1] = 1;
				//usr_flash_data_set();
			}
			rebuf[0] = 0x24;
			rebuf[1] = 0x4F;
			rebuf[2] = 0x4B;
			rebuf[3] = (id>>24)&0xff;
			rebuf[4] = (id>>16)&0xff;
			rebuf[5] = (id>>8)&0xff;
			rebuf[6] = (id)&0xff;
			rebuf[7] = 0x2A;
			rebuf[8] = 0x44;
			rebuf[9] = 0x4C;
			*relen = 10;
			return 1;
		}
	}
		
	return 0;
}