#include "usr.h"


/************************************************************
* VAR
************************************************************/
struct netif	server_netif_1;
struct netif	server_netif_2;
USR_TIME		g_current_time;
bool			g_flag_oneSec_overturn;
bool			g_flag_500ms_overturn;

/************************************************************
* FUNC
************************************************************/
int		main_thread(void);
STATUS	usr_start_init(void);
void	usr_thread_main(void *p);
void	usr_thread_network_mac_1(void *p);
void	usr_thread_network_mac_2(void *p);
void	usr_thread_clocktick(void *p);
STATUS	usr_thread_network_mac_isbreak(int *sec, int v);
STATUS	usr_thread_network_interface_choice(int *sec, int v);

void	usr_clock_init(void);
bool	usr_get_flag_oneSec_overturn(void);
int		usr_get_now_year(void);
int		usr_get_now_month(void);
int		usr_get_now_date(void);
int		usr_get_now_hour(void);
int		usr_get_now_minute(void);
int		usr_get_now_second(void);
int		usr_get_now_second(void);
STATUS	usr_clock_set_time(int year, int month, int date, int hour, int minute, int second);
void	usr_clock_debug_XMD_print(void);


/************************************************************
* main
************************************************************/
int main()
{
	microblaze_init_icache_range(0, XPAR_MICROBLAZE_0_CACHE_BYTE_SIZE);
	microblaze_init_dcache_range(0, XPAR_MICROBLAZE_0_DCACHE_BYTE_SIZE);
	//microblaze_enable_exceptions();

	/* enable caches */
	XCACHE_ENABLE_ICACHE();
	XCACHE_ENABLE_DCACHE();

	/* start the kernel - does not return */
	xilkernel_main();

	return 0;
}


/************************************************************
* xilkernel_main() default first run here
************************************************************/
int main_thread(void)
{
	//xil_printf("LIRR PROJECT 2012-7-13 \n");

	if(usr_start_init() == ERROR)
		return 0;

	/* initialize lwIP before calling sys_thread_new */
	lwip_init();

//xil_printf("start \n");

	sys_thread_new("usr_main_loop",		usr_thread_main,			NULL, PTHREAD_STACK_SIZE, DEFAULT_THREAD_PRIO);
	sys_thread_new("usr_network_1",		usr_thread_network_mac_1,	NULL, PTHREAD_STACK_SIZE, DEFAULT_THREAD_PRIO);
	sys_thread_new("usr_network_2",		usr_thread_network_mac_2,	NULL, PTHREAD_STACK_SIZE, DEFAULT_THREAD_PRIO);
	sys_thread_new("usr_clock",			usr_thread_clocktick,		NULL, PTHREAD_STACK_SIZE, DEFAULT_THREAD_PRIO);

	return 0;
}



/************************************************************
* usr_thread_main
************************************************************/
void usr_thread_main(void *p)
{
	FOREVER
	{
		usr_pannel_key_input();


#ifdef USR_GPIO_DEBUG
		usr_gpio_debug();
#endif

#ifdef USR_LCD_DEBUG
		usr_lcd_debug();
#endif
		
#ifdef USR_PAGE_DEBUG
		usr_page_debug();
#endif

		usr_pannel_lcd_deal_with();
		usr_pannel_gpio_deal_with();
		
		usr_pannel_gpio_output();
		
		//xil_printf("%x\n",g_GPIO_OUT[USR_GPIO_OUT_CH_1]);

		sleep(THREAD_MAIN_TICK);
	}
}

/************************************************************
* usr_thread_network_mac_1
************************************************************/
void usr_thread_network_mac_1(void *p)
{
	unsigned char	mac_ethernet_address[6];
	struct netif	*netif;
	struct ip_addr	ipaddr, netmask, gw;
	u8_t			ip[4];
   
	// the mac address of the board. this should be unique per board 

	netif = &server_netif_1;

	mac_ethernet_address[0] = g_interface_mac[USR_IP_INTERFACE_1][0];
	mac_ethernet_address[1] = g_interface_mac[USR_IP_INTERFACE_1][1];
	mac_ethernet_address[2] = g_interface_mac[USR_IP_INTERFACE_1][2];
	mac_ethernet_address[3] = g_interface_mac[USR_IP_INTERFACE_1][3];
	mac_ethernet_address[4] = g_interface_mac[USR_IP_INTERFACE_1][4];
	mac_ethernet_address[5] = g_interface_mac[USR_IP_INTERFACE_1][5];

	ip[3] = g_flash_module_net_ip[USR_IP_INTERFACE_1]&0xff;
	ip[2] = (g_flash_module_net_ip[USR_IP_INTERFACE_1]>>8)&0xff;
	ip[1] = (g_flash_module_net_ip[USR_IP_INTERFACE_1]>>16)&0xff;
	ip[0] = (g_flash_module_net_ip[USR_IP_INTERFACE_1]>>24)&0xff;

	// initliaze IP addresses to be used 
	IP4_ADDR(&ipaddr,  ip[0],ip[1],ip[2],ip[3]);
	IP4_ADDR(&netmask, 255, 255, 255,  0);
	IP4_ADDR(&gw,      ip[0],ip[1],ip[2],1);

  	// Add network interface to the netif_list, and set it as default 
	if (!xemac_add(netif, &ipaddr, &netmask, &gw, mac_ethernet_address, EMAC_BASEADDR_1)) {
		return;
	}
	netif_set_default(netif);

	// specify that the network if is up 
	netif_set_up(netif);

	// start packet receive thread - required for lwIP operation 
	sys_thread_new("xemacif_input_thread_1", xemacif_input_thread, netif, PTHREAD_STACK_SIZE, DEFAULT_THREAD_PRIO);
	
	sys_thread_new("usr_net_config_1",	usr_thread_network_config_1,	0, PTHREAD_STACK_SIZE, DEFAULT_THREAD_PRIO);
	sys_thread_new("usr_net_data_1",	usr_thread_network_data_1,		0, PTHREAD_STACK_SIZE, DEFAULT_THREAD_PRIO);
	sys_thread_new("usr_net_pro_1",	usr_thread_network_pro_1,		0, PTHREAD_STACK_SIZE, DEFAULT_THREAD_PRIO);
}


/************************************************************
* usr_thread_network_mac_2
************************************************************/
void usr_thread_network_mac_2(void *p)
{
	unsigned char	mac_ethernet_address[6];
	struct netif	*netif;
	struct ip_addr	ipaddr, netmask, gw;
	u8_t			ip[4];

	// the mac address of the board. this should be unique per board    
	netif = &server_netif_2;

	mac_ethernet_address[0] = g_interface_mac[USR_IP_INTERFACE_2][0];
	mac_ethernet_address[1] = g_interface_mac[USR_IP_INTERFACE_2][1];
	mac_ethernet_address[2] = g_interface_mac[USR_IP_INTERFACE_2][2];
	mac_ethernet_address[3] = g_interface_mac[USR_IP_INTERFACE_2][3];
	mac_ethernet_address[4] = g_interface_mac[USR_IP_INTERFACE_2][4];
	mac_ethernet_address[5] = g_interface_mac[USR_IP_INTERFACE_2][5];

	ip[3] = g_flash_module_net_ip[USR_IP_INTERFACE_2]&0xff;
	ip[2] = (g_flash_module_net_ip[USR_IP_INTERFACE_2]>>8)&0xff;
	ip[1] = (g_flash_module_net_ip[USR_IP_INTERFACE_2]>>16)&0xff;
	ip[0] = (g_flash_module_net_ip[USR_IP_INTERFACE_2]>>24)&0xff;

	// initliaze IP addresses to be used */
	IP4_ADDR(&ipaddr,  ip[0],ip[1],ip[2],ip[3]);
	IP4_ADDR(&netmask, 255, 255, 255,  0);
	IP4_ADDR(&gw,      ip[0],ip[1],ip[2],1);
	
  	// Add network interface to the netif_list, and set it as default 
	if (!xemac_add(netif, &ipaddr, &netmask, &gw, mac_ethernet_address, EMAC_BASEADDR_2)) {
		return;		
	   }
	netif_set_default(netif);

	// specify that the network if is up 
	netif_set_up(netif);

	// start packet receive thread - required for lwIP operation 
  	sys_thread_new("xemacif_input_thread_2", xemacif_input_thread, netif, PTHREAD_STACK_SIZE, DEFAULT_THREAD_PRIO);
	
	sys_thread_new("usr_net_config_2",	usr_thread_network_config_2,	0, PTHREAD_STACK_SIZE, DEFAULT_THREAD_PRIO);
	sys_thread_new("usr_net_data_2",	usr_thread_network_data_2,		0, PTHREAD_STACK_SIZE, DEFAULT_THREAD_PRIO);
	sys_thread_new("usr_net_pro_2",	usr_thread_network_pro_2,		0, PTHREAD_STACK_SIZE, DEFAULT_THREAD_PRIO);
}


/************************************************************
* usr_thread_clocktick
************************************************************/
void usr_thread_clocktick(void *p)
{
	int macSec = 1;
	int interfaceSec = 40;
	int flag_tmp = 0;
	 
    FOREVER
	{
		sleep(500);
		flag_tmp++;
		
		
		if(g_flag_500ms_overturn)
				g_flag_500ms_overturn = false;
			else
				g_flag_500ms_overturn = true;
				
		
		if(flag_tmp == 2){
			flag_tmp = 0;
			g_current_time.tot_secs++;
			if (g_current_time.tot_secs == 86400)
				g_current_time.tot_secs = 0;
	
			g_current_time.hrs	= g_current_time.tot_secs / 3600;
			g_current_time.mins	= (g_current_time.tot_secs - g_current_time.hrs*3600) / 60;
			g_current_time.secs	= g_current_time.tot_secs - g_current_time.hrs*3600 - g_current_time.mins*60;
				
			if(g_flag_oneSec_overturn)
				g_flag_oneSec_overturn = false;
			else
				g_flag_oneSec_overturn = true;
	
			usr_pannel_lcd_output();
	
			usr_thread_network_mac_isbreak(&macSec, 1);
			usr_thread_network_interface_choice(&interfaceSec, 40);
	
	#ifdef USR_CLOCK_DEBUG
			usr_clock_debug_XMD_print();
	#endif
		}
    }
}



/************************************************************
* usr_start_init
************************************************************/
STATUS usr_start_init()
{
	//FLASH
	drv_flash_init();
	usr_flash_init();

	//usr_flash_rst_default();
	//return ERROR;

	usr_configure_status_init();
	
	//GPIO
	drv_gpio_init();
	usr_gpio_init();

	//LCD
	drv_lcd_init();
	usr_lcd_init();

	//CLOCK
	usr_clock_init();

	//PAGE
	usr_page_init();

	//NET
	usr_net_init();

	return OK;
}




/************************************************************
* usr_thread_network_mac_isbreak
************************************************************/
STATUS usr_thread_network_mac_isbreak(int *sec, int v)
{
	(*sec)--;
	if((*sec) == 0)
	{
		g_flag_macBrk[USR_IP_INTERFACE_1] = usr_gpio_isMac1Break();
		g_flag_macBrk[USR_IP_INTERFACE_2] = usr_gpio_isMac2Break();

		(*sec) = v;
	}
	return OK;
}


/************************************************************
* usr_thread_network_interface_choice
************************************************************/
STATUS usr_thread_network_interface_choice(int *sec, int v)
{
	(*sec)--;
	if((*sec) == 0)
	{
		if(g_flag_netLive[USR_IP_INTERFACE_1])
		{
			g_flag_netLive[USR_IP_INTERFACE_1]	= false;
			g_flag_netAlive[USR_IP_INTERFACE_1] = true;
		}
		else
			g_flag_netAlive[USR_IP_INTERFACE_1] = false;

		/*
		if(g_flag_netLive[USR_IP_INTERFACE_2])
		{
			g_flag_netLive[USR_IP_INTERFACE_2]	= false;
			g_flag_netAlive[USR_IP_INTERFACE_2] = true;
		}
		else
			g_flag_netAlive[USR_IP_INTERFACE_2] = false;
		*/

		if((!g_flag_macBrk[USR_IP_INTERFACE_1]) && (g_flag_netAlive[USR_IP_INTERFACE_1]))
		{
			g_flag_interface_en[USR_IP_INTERFACE_1] = true;
			//g_flag_interface_en[USR_IP_INTERFACE_2] = false;
		}
		/*
		else if((!g_flag_macBrk[USR_IP_INTERFACE_2]) && (g_flag_netAlive[USR_IP_INTERFACE_2]))
		{
			g_flag_interface_en[USR_IP_INTERFACE_1] = false;
			g_flag_interface_en[USR_IP_INTERFACE_2] = true;
		}*/
		else
		{
			g_flag_interface_en[USR_IP_INTERFACE_1] = false;
			//g_flag_interface_en[USR_IP_INTERFACE_2] = false;
		}
		
		if((g_flag_netAlive[USR_IP_INTERFACE_1] == false)&&(g_flag_netAlive[USR_IP_INTERFACE_1_LOG] == true))
		{
			;//xil_printf("lwip_init rst\n");
			//lwip_init();
		}
		g_flag_netAlive[USR_IP_INTERFACE_1_LOG] = g_flag_netAlive[USR_IP_INTERFACE_1];
			

		//if(!g_flag_macBrk[USR_IP_INTERFACE_1])
		//{
		//	g_flag_interface_en[USR_IP_INTERFACE_1] = true;
		//	g_flag_interface_en[USR_IP_INTERFACE_2] = false;
		//}
		//else if(!g_flag_macBrk[USR_IP_INTERFACE_2])
		//{
		//	g_flag_interface_en[USR_IP_INTERFACE_1] = false;
		//	g_flag_interface_en[USR_IP_INTERFACE_2] = true;
		//}
		//else
		//{
		//	g_flag_interface_en[USR_IP_INTERFACE_1] = false;
		//	g_flag_interface_en[USR_IP_INTERFACE_2] = false;
		//}

		(*sec) = v;
	}
	return OK;
}


/************************************************************
* usr_clock_init
************************************************************/
void usr_clock_init(void)
{
	usr_clock_set_time(2011, 7, 13, 10, 38, 0);
}


/************************************************************
* usr_get_flag_oneSec_overturn
************************************************************/
bool usr_get_flag_oneSec_overturn(void)
{
	return g_flag_oneSec_overturn;
}

/************************************************************
* usr_get_flag_500ms_overturn
************************************************************/
bool usr_get_flag_500ms_overturn(void)
{
	return g_flag_500ms_overturn;
}


/************************************************************
* usr_get_now_ year/month/date/hour/minute/second
************************************************************/
int usr_get_now_year(void)
{
	return (g_current_time.year_l + (g_current_time.year_h << 8));
}

int usr_get_now_month(void)
{
	return g_current_time.month;
}

int usr_get_now_date(void)
{
	return g_current_time.date;
}

int usr_get_now_hour(void)
{
	return g_current_time.hrs;
}

int usr_get_now_minute(void)
{
	return g_current_time.mins;
}

int usr_get_now_second(void)
{
	return g_current_time.secs;
}


/************************************************************
* usr_clock_set_time
************************************************************/
STATUS usr_clock_set_time(int year, int month, int date, int hour, int minute, int second)
{
	g_current_time.year_l	= year&0xff;
	g_current_time.year_h	= (year>>8)&0xff;
	g_current_time.month	= month&0xff;
	g_current_time.date		= date&0xff;
	g_current_time.hrs		= hour&0xff;
	g_current_time.mins		= minute&0xff;
	g_current_time.secs		= second&0xff;
	
	g_current_time.tot_secs = ((g_current_time.hrs*60) + g_current_time.mins)*60 + g_current_time.secs;
	
	return OK;		
}




/************************************************************
* usr_clock_debug_XMD_print
************************************************************/
void usr_clock_debug_XMD_print(void)
{
	int year,month,date,hour,minute,second;

	year	= usr_get_now_year();
	month	= usr_get_now_month();
	date	= usr_get_now_date();
	hour	= usr_get_now_hour();
	minute	= usr_get_now_minute();
	second	= usr_get_now_second();

	//xil_printf("%d-%d-%d %d:%d:%d\n",year,month,date,hour,minute,second);
}

