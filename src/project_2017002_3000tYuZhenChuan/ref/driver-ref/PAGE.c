#include "usr.h"

/************************************************************
* VAR
************************************************************/

ALM_SOLID		g_solid_alm[USR_SOLID_ALM_NUM];
ALM_BLOCK		g_block_alm[USR_BLOCK_ALM_NUM];
ALM_COPY_BLOCK	g_copy_block_alm[USR_COPY_BLOCK_ALM_NUM];

ALM_LIST		g_list_pool_alm_block;
ALM_LIST		g_list_total_alm;
ALM_LIST_GROUP	g_list_pool_alm_copy_block;
ALM_LIST_GROUP	g_list_group_copy_alm[USR_LIST_ALM_GROUP_NUM];


/************************************************************
* FUNC
************************************************************/
STATUS			usr_page_init(void);

void			usr_alm_solid_info_init(void);
void			usr_module_property_init(void);
void			usr_judge_duty_light(int offset);
void			usr_solid_debug_print(ALM_SOLID* p);

void			usr_list_pool_alm_init(void);
void			usr_list_total_alm_init(void);
ALM_BLOCK*		usr_give_block_pool_alm(void);
STATUS			usr_back_block_pool_alm(ALM_BLOCK* p);
ALM_BLOCK*		usr_find_block_in_list_alm(ALM_LIST* list, UINT32 tag_id, int level_alm);
STATUS			usr_add_block_in_list_alm(ALM_LIST* list, ALM_BLOCK* p);
STATUS			usr_del_block_in_list_alm(ALM_LIST* list, ALM_BLOCK* p);
ALM_BLOCK*		usr_find_lcd_head_block(void);
bool			usr_is_list_total_alm_change(void);
UINT16			usr_get_list_total_alm_len(void);
UINT16			usr_get_list_total_alm_lcd_head(void);
void			usr_block_alm_debug_print(ALM_BLOCK* p);
void			usr_block_all_alm_debug_print(void);
void			usr_list_total_alm_debug_print(void);

void			usr_list_pool_copy_alm_init(void);
void			usr_list_group_copy_alm_init(void);
ALM_COPY_BLOCK*	usr_give_copy_block_pool_alm(void);
STATUS			usr_back_copy_block_pool_alm(ALM_COPY_BLOCK* p);
ALM_COPY_BLOCK*	usr_find_copy_block_in_list_alm(ALM_LIST_GROUP* list, ALM_BLOCK* block);
STATUS			usr_add_copy_block_in_list_alm(ALM_LIST_GROUP* list, ALM_COPY_BLOCK *p);
STATUS			usr_del_copy_block_in_list_alm(ALM_LIST_GROUP* list, ALM_COPY_BLOCK *p);
ALM_COPY_BLOCK*	usr_find_lcd_head_copy_block(int ID);
bool			usr_is_list_group_alm_change(int ID);
UINT16			usr_get_list_group_alm_len(int ID);
UINT16			usr_get_list_group_alm_lcd_head(int ID);
void			usr_copy_block_alm_debug_print(ALM_COPY_BLOCK* p);
void			usr_copy_block_all_alm_debug_print(int ID);
void			usr_list_group_alm_debug_print(int ID);
void			usr_list_alm_all_debug_print(void);

void			usr_list_rst_readyRst_alm(void);

STATUS			usr_add_alm(UINT32 tag_id, int level_alm);
STATUS			usr_del_alm(UINT32 tag_id, int level_alm);
STATUS			usr_refurbish_alm_happen_time		(UINT32 tag_id, int level_alm, USR_TIME *time);
STATUS			usr_refurbish_alm_real_time_value	(UINT32 tag_id, int level_alm, UINT16 value);
STATUS			usr_refurbish_alm_limit_value		(UINT32 tag_id, int level_alm, UINT16 value);
STATUS			usr_refurbish_alm_flag_wait_rdyRst	(UINT32 tag_id, int level_alm, bool flag);
bool			usr_get_alm_flag_wait_rdyRst		(UINT32 tag_id, int level_alm);
STATUS			usr_net_group_sort_newAlm			(UINT32 tag_id, int level_alm);

void			usr_page_debug(void);



/************************************************************
* usr_page_init() 
************************************************************/
STATUS usr_page_init(void)
{
	if(usr_is_default_status())
		return ERROR;

	usr_alm_solid_info_init();
	usr_module_property_init();

	usr_list_pool_alm_init();
	usr_list_total_alm_init();

	usr_list_pool_copy_alm_init();
	usr_list_group_copy_alm_init();

	return OK;
}


/************************************************************
* usr_list_pool_alm_init() 
************************************************************/
void usr_alm_solid_info_init(void)
{
	int i,j;

#ifdef USR_DATA_FLASH_DOWMLOAD
	for(i = 0; i < USR_SOLID_ALM_NUM; i++)
	{
		g_solid_alm[i].id			= i;
		g_solid_alm[i].tag_id		= i;
		g_solid_alm[i].tag_power	= g_SPDAPW_P[i];

		for(j = 0; j < 5; j++)g_solid_alm[i].tag_uint_name[j] = g_SPUNIT[i][j];
		for(j = 0; j < 8; j++)g_solid_alm[i].tag_easy_id[j]	= g_SPMPN[i][j];
		for(j = 0; j < 24; j++)g_solid_alm[i].tag_name_ch[j] = g_SPNAME_CH[i][j];
		for(j = 0; j < 36; j++)g_solid_alm[i].tag_name_en[j] = g_SPNAME_EN[i][j];
	}

	for(i = 0; i < 30; i++)
	{
		if((g_SPICTN[i][0] == 0x70) && (g_SPICTN[i][1] == 0x61) && (g_SPICTN[i][2] == 0x67) && (g_SPICTN[i][3] == 0x65))
		{
			if((g_SPICTN[i][4] == 0x31)&&(g_SPICTN[i][5] == 0x36))//page16
			{
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_16;
			}
			else if((g_SPICTN[i][4] == 0x31)&&(g_SPICTN[i][5] == 0x35))//page15
			{
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_15;
			}
			else if((g_SPICTN[i][4] == 0x31)&&(g_SPICTN[i][5] == 0x34))//page14
			{
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_14;
			}
			else if((g_SPICTN[i][4] == 0x31)&&(g_SPICTN[i][5] == 0x33))//page13
			{
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_13;
			}
			else if((g_SPICTN[i][4] == 0x31)&&(g_SPICTN[i][5] == 0x32))//page12
			{
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_12;
			}
			else if((g_SPICTN[i][4] == 0x31)&&(g_SPICTN[i][5] == 0x31))//page11
			{
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_11;
			}
			else if((g_SPICTN[i][4] == 0x31)&&(g_SPICTN[i][5] == 0x30))//page10
			{
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_10;
			}
			else
			{
				switch(g_SPICTN[i][4])
				{
				case 0x31:	//page1
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_1;
					break;
	
				case 0x32:	//page2
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_2;
					break;
	
				case 0x33:	//page3
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_3;
					break;
	
				case 0x34:	//page4
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_4;
					break;
	
				case 0x35:	//page5
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_5;
					break;
					
				case 0x36:	//page6
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_6;
					break;	
					
				case 0x37:	//page7
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_7;
					break;					
					
				case 0x38:	//page8
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_8;
					break;
					
				case 0x39:	//page9
					for(j = 0; j < 30; j++)
						if((g_SPPPIA[i][j] >= 1) && (g_SPPPIA[i][j] <= 513))
							g_solid_alm[g_SPPPIA[i][j]].group_id = USR_LIST_ALM_GROUP_9;
					break;
															
				default:
					;
				}
			}
		}
	}
#endif


#ifdef USR_FLASH_USE_DEBUG
	for(i = 33; i < 133; i++)
	{
		/*
		xil_printf("alm_%d: tag_id:%d, power:%x, group:%d  easy_id: ", 
			g_solid_alm[i].id,g_solid_alm[i].tag_id,g_solid_alm[i].tag_power,g_solid_alm[i].group_id);
		for(j = 0; j  < 8; j++)xil_printf("%x ",g_solid_alm[i].tag_easy_id[j]);
		xil_printf("\n");
		*/
		;		

		//xil_printf("ch_name: ");
		//for(j = 0; j < 24; j++)xil_printf("%x ",g_solid_alm[i].tag_name_ch[j]);
		//xil_printf("\n");

		//xil_printf("en_name: ");
		//for(j = 0; j  < 36; j++)xil_printf("%x ",g_solid_alm[i].tag_name_en[j]);
		//xil_printf("\n");

		//xil_printf("uint_name: ");
		//for(j = 0; j  < 5; j++)xil_printf("%x ",g_solid_alm[i].tag_uint_name[j]);
		//xil_printf("\n");
		
	}

#endif

}


/************************************************************
* usr_module_property_init() 
************************************************************/
void usr_module_property_init(void)
{
	int i, j;	

	if(!usr_is_default_status())
	{
		g_interface_mac[USR_IP_INTERFACE_1][0]	= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 0];
		g_interface_mac[USR_IP_INTERFACE_1][1]	= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 1];
		g_interface_mac[USR_IP_INTERFACE_1][2]	= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 2];
		g_interface_mac[USR_IP_INTERFACE_1][3]	= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 3];
		g_interface_mac[USR_IP_INTERFACE_1][4]	= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 4];
		g_interface_mac[USR_IP_INTERFACE_1][5]	= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 5];
		g_interface_mac[USR_IP_INTERFACE_2][0]	= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 6];
		g_interface_mac[USR_IP_INTERFACE_2][1]	= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 7];
		g_interface_mac[USR_IP_INTERFACE_2][2]	= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 8];
		g_interface_mac[USR_IP_INTERFACE_2][3]	= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 9];
		g_interface_mac[USR_IP_INTERFACE_2][4]	= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 10];
		g_interface_mac[USR_IP_INTERFACE_2][5]	= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 11];

		g_flash_module_net_ip[USR_IP_INTERFACE_1]		= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 15]&0xff); 
		g_flash_module_net_ip[USR_IP_INTERFACE_1]		+= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 14]&0xff)<<8;
		g_flash_module_net_ip[USR_IP_INTERFACE_1]		+= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 13]&0xff)<<16;
		g_flash_module_net_ip[USR_IP_INTERFACE_1]		+= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 12]&0xff)<<24;
		g_flash_module_broadcast_ip[USR_IP_INTERFACE_1]	= g_flash_module_net_ip[USR_IP_INTERFACE_1] | 0xff;

		g_flash_module_net_ip[USR_IP_INTERFACE_2]		= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 19]&0xff); 
		g_flash_module_net_ip[USR_IP_INTERFACE_2]		+= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 18]&0xff)<<8;
		g_flash_module_net_ip[USR_IP_INTERFACE_2]		+= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 17]&0xff)<<16;
		g_flash_module_net_ip[USR_IP_INTERFACE_2]		+= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 16]&0xff)<<24;
		g_flash_module_broadcast_ip[USR_IP_INTERFACE_2]	= g_flash_module_net_ip[USR_IP_INTERFACE_2] | 0xff;
		
		
		G_Alm_num		= g_CONFIGURE[USR_FLASH_DATA_OFFSET_GROUP + 0]&0xff;
		G_YN_IOin		= g_CONFIGURE[USR_FLASH_DATA_OFFSET_GROUP + 1]&0xff;
		if(G_YN_IOin > 0)
		{
			G_YN_IOout	= g_CONFIGURE[USR_FLASH_DATA_OFFSET_GROUP + 2]&0xff;
			G_no_IO			= g_CONFIGURE[USR_FLASH_DATA_OFFSET_GROUP + 3]&0xff;
		}
		else
		{
			G_YN_IOout	= -1;
			G_no_IO			= -1;
		}
		
		/*
		j = USR_FLASH_DATA_OFFSET_GROUP + 1;
		for(i=0; i<24; i++)
			G_bfr_Alm_total_ch[i] = g_CONFIGURE[j++];
		for(i=0; i<24; i++)
			G_bfr_Alm_total_en[i] = g_CONFIGURE[j++];		
			
		for(i=0; i<24; i++)
			G_bfr_Alm_1_ch[i] = g_CONFIGURE[j++];
		for(i=0; i<24; i++)
			G_bfr_Alm_1_en[i] = g_CONFIGURE[j++];	
			
		for(i=0; i<24; i++)
			G_bfr_Alm_2_ch[i] = g_CONFIGURE[j++];
		for(i=0; i<24; i++)
			G_bfr_Alm_2_en[i] = g_CONFIGURE[j++];	
			
		for(i=0; i<24; i++)
			G_bfr_Alm_3_ch[i] = g_CONFIGURE[j++];
		for(i=0; i<24; i++)
			G_bfr_Alm_3_en[i] = g_CONFIGURE[j++];	
		 
		for(i=0; i<24; i++)
			G_bfr_Alm_4_ch[i] = g_CONFIGURE[j++];
		for(i=0; i<24; i++)
			G_bfr_Alm_4_en[i] = g_CONFIGURE[j++];	
			
		for(i=0; i<24; i++)
			G_bfr_Alm_5_ch[i] = g_CONFIGURE[j++];
		for(i=0; i<24; i++)
			G_bfr_Alm_5_en[i] = g_CONFIGURE[j++];	
			
		for(i=0; i<24; i++)
			G_bfr_Alm_6_ch[i] = g_CONFIGURE[j++];
		for(i=0; i<24; i++)
			G_bfr_Alm_6_en[i] = g_CONFIGURE[j++];	
			
		for(i=0; i<24; i++)
			G_bfr_Alm_7_ch[i] = g_CONFIGURE[j++];
		for(i=0; i<24; i++)
			G_bfr_Alm_7_en[i] = g_CONFIGURE[j++];	
		 
		for(i=0; i<24; i++)
			G_bfr_Alm_8_ch[i] = g_CONFIGURE[j++];
		for(i=0; i<24; i++)
			G_bfr_Alm_8_en[i] = g_CONFIGURE[j++];				
		*/	
						
		for(i = 0; i < 2; i++)
		{
			//g_flash_module_net_ip[i]	= (g_SIPACP_IP[i][3]&0xff);
			//g_flash_module_net_ip[i]	+= (g_SIPACP_IP[i][2]&0xff)<<8;
			//g_flash_module_net_ip[i]	+= (g_SIPACP_IP[i][1]&0xff)<<16;
			//g_flash_module_net_ip[i]	+= (g_SIPACP_IP[i][0]&0xff)<<24;
			//g_flash_module_broadcast_ip[i] 	= g_flash_module_net_ip[i] | 0xff;

			g_flash_module_config_port[i]	= (u16_t)(g_SIPACP_PORT[i]&0xffff);
			g_flash_module_data_port[i]		= (u16_t)(g_SMIPAP_PORT[i]&0xffff);
		}

		/*
		xil_printf(" using mac1: %x-%x-%x-%x-%x-%x,  mac2: %x-%x-%x-%x-%x-%x, \n", 
			g_interface_mac[USR_IP_INTERFACE_1][0], g_interface_mac[USR_IP_INTERFACE_1][1], g_interface_mac[USR_IP_INTERFACE_1][2],
			g_interface_mac[USR_IP_INTERFACE_1][3], g_interface_mac[USR_IP_INTERFACE_1][4], g_interface_mac[USR_IP_INTERFACE_1][5],
			g_interface_mac[USR_IP_INTERFACE_2][0], g_interface_mac[USR_IP_INTERFACE_2][1], g_interface_mac[USR_IP_INTERFACE_2][2],
			g_interface_mac[USR_IP_INTERFACE_2][3], g_interface_mac[USR_IP_INTERFACE_2][4], g_interface_mac[USR_IP_INTERFACE_2][5]);

		xil_printf(" using ip1: %x, broadcast_ip1: %x, Cport1: %d, Dport1: %d\n using ip2: %x, broadcast_ip2: %x, Cport2: %d, Dport2: %d \n", 
			g_flash_module_net_ip[USR_IP_INTERFACE_1], g_flash_module_broadcast_ip[USR_IP_INTERFACE_1], g_flash_module_config_port[USR_IP_INTERFACE_1], g_flash_module_data_port[USR_IP_INTERFACE_1],
			g_flash_module_net_ip[USR_IP_INTERFACE_2], g_flash_module_broadcast_ip[USR_IP_INTERFACE_2], g_flash_module_config_port[USR_IP_INTERFACE_2], g_flash_module_data_port[USR_IP_INTERFACE_2]);
		
		xil_printf(" G_Alm_num: %d, G_YN_IOin: %d, G_YN_IOout: %d, G_no_IO: %d\n",
			G_Alm_num, G_YN_IOin, G_YN_IOout, G_no_IO);
		*/
	}

	


	g_flash_module_group = g_SSSIGN;

	for(j = 0; j < 32; j++)
		if(usr_ackbit(g_SECSGN, j))
			break;
	if(j < 32)
	{
		usr_judge_duty_light(j);
		for(i = 0; i < 20; i++)
			g_flash_module_identity_name[i] = g_SECNAME[j][i];
			
	}
	g_flash_module_identity_id = g_SECSGN;

	for(i = 0; i < 20; i++)g_flash_module_name[i] = g_SHNAME[i];
	g_flash_module_id = g_SEAID;

	g_flash_flag_net_ack		= g_SACKRSF;				
	g_flash_flag_net_setLight	= g_TGBZ;	

	g_flash_bridge_net_ip[0] = g_MONITOR_IP[0];
	g_flash_bridge_net_ip[1] = g_MONITOR_IP[1];
	g_flash_engine_net_ip[0] = g_MONITOR_IP[2];
	g_flash_engine_net_ip[1] = g_MONITOR_IP[3];



#ifdef USR_FLASH_USE_DEBUG

	//延伸报警模块mac地址
	/*
	xil_printf("mac1: %x-%x-%x-%x-%x-%x,  mac2: %x-%x-%x-%x-%x-%x \n",
				g_interface_mac[0][0], g_interface_mac[0][1], g_interface_mac[0][2], g_interface_mac[0][3], g_interface_mac[0][4], g_interface_mac[0][5], 
				g_interface_mac[1][0], g_interface_mac[1][1], g_interface_mac[1][2], g_interface_mac[1][3], g_interface_mac[1][4], g_interface_mac[1][5]);
	*/
	//延伸报警模块IP地址 + 配置端口
	/*
	xil_printf("IP + CPORT + DPORT: %x + %d + %d   %x + %d + %d\n",
				g_flash_module_net_ip[0], g_flash_module_config_port[0], g_flash_module_data_port[0],
				g_flash_module_net_ip[1], g_flash_module_config_port[1], g_flash_module_data_port[1]);
	*/
	//延伸报警模块所属分组标志
	//xil_printf("GROUP ID: %d \n", g_flash_module_group);

	//轮机员呼叫成员名单 + 轮机员呼叫标志
	/*
	xil_printf("SAILOR_NAME: ");
	for(i = 0; i < 20; i++)xil_printf("%x ",g_flash_module_identity_name[i]);
	xil_printf("\n SAILOR_ID: %x\n",g_flash_module_identity_id);
	*/

	//延伸报警主机名称 + 模块ID号
	/*
	xil_printf("MODULE_NAME: ");
	for(i = 0; i < 20; i++)xil_printf("%x ",g_flash_module_name[i]);
	xil_printf("\n MODULE_ID: %x\n",g_flash_module_id);
	*/

	//全网应答全网复位处理标志
	//xil_printf("NET_ACK: %d \n",g_flash_flag_net_ack);

	//调獗曛?
	//xil_printf("NET_LIGHT: %d \n",g_flash_flag_net_setLight);

	//BRIDGE IP
	//xil_printf("IP: %x  %x \n",g_flash_bridge_net_ip[0], g_flash_bridge_net_ip[1]);

	//ENGINE IP
	//xil_printf("IP: %x  %x \n",g_flash_engine_net_ip[0], g_flash_engine_net_ip[1]);

#endif

}


/************************************************************
* usr_judge_duty_light() 
************************************************************/
void usr_judge_duty_light(int offset)
{
	switch(offset)
	{
	case 2:
		g_flash_module_identity_light = USR_LED_ID_L1;
		break;
	case 3:
		g_flash_module_identity_light = USR_LED_ID_L2;
		break;
	case 4:
		g_flash_module_identity_light = USR_LED_ID_L3;
		break;
	case 5:
		g_flash_module_identity_light = USR_LED_ID_L4;
		break;
	case 6:
		g_flash_module_identity_light = USR_LED_ID_L5;
		break;
	case 7:
		g_flash_module_identity_light = USR_LED_ID_L6;
		break;
	case 8:
		g_flash_module_identity_light = USR_LED_ID_L7;
		break;
	case 9:
		g_flash_module_identity_light = USR_LED_ID_L8;
		break;
	case 10:
		g_flash_module_identity_light = USR_LED_ID_R8;
		break;
	case 11:
		g_flash_module_identity_light = USR_LED_ID_R7;
		break;
	case 12:
		g_flash_module_identity_light = USR_LED_ID_R6;
		break;	
	case 13:
		g_flash_module_identity_light = USR_LED_ID_R5;
		break;	
	case 14:
		g_flash_module_identity_light = USR_LED_ID_R4;
		break;
	case 15:
		g_flash_module_identity_light = USR_LED_ID_R3;
		break;
	case 16:
		g_flash_module_identity_light = USR_LED_ID_R2;
		break;
	case 17:
		g_flash_module_identity_light = USR_LED_ID_R1;
		break;
	default:
		;
	}
	
}




/************************************************************
* usr_list_pool_alm_init() 
************************************************************/
void usr_solid_debug_print(ALM_SOLID* p)
{
	/*
	int j;

	xil_printf("SOLID id:          %d\n", p->id);
	xil_printf("SOLID tag_id:      %x\n", p->tag_id);
	xil_printf("SOLID group_id:    %d\n", p->group_id);
	xil_printf("SOLID tag_power:   %x\n", p->tag_power);

	xil_printf("SOLID easy_id: ");
	for(j = 0; j < 8; j++)xil_printf("%x ",p->tag_easy_id[j]);
	xil_printf("\n");

	xil_printf("SOLID ch_name: ");
	for(j = 0; j < 24; j++)xil_printf("%x ",p->tag_name_ch[j]);
	xil_printf("\n");

	xil_printf("SOLID en_name: ");
	for(j = 0; j  < 36; j++)xil_printf("%x ",p->tag_name_en[j]);
	xil_printf("\n");

	xil_printf("SOLID uint_name: ");
	for(j = 0; j  < 5; j++)xil_printf("%x ",p->tag_uint_name[j]);
	xil_printf("\n");
	*/
}


/************************************************************
* usr_list_pool_alm_init() 
************************************************************/
void usr_list_pool_alm_init(void)
{
	ALM_BLOCK* p;
	int i;

	for(i = 0; i < USR_BLOCK_ALM_NUM; i++)
	{
		g_block_alm[i].id				= i;
		g_block_alm[i].pSource			= (ALM_SOLID*)NULL;
		g_block_alm[i].alm_level		= -1;
		g_block_alm[i].real_time_value	= 0;
		g_block_alm[i].alm_limit_value	= 0;
		g_block_alm[i].flag_wait_rdyRst	= false;
	}

	g_list_pool_alm_block.list_len		= USR_BLOCK_ALM_NUM;

	g_list_pool_alm_block.head	= g_block_alm;
	p		= &(g_block_alm[0]);
	p->prev	= (ALM_BLOCK*)NULL;
	p->next	= &(g_block_alm[1]);
	p		= p->next;
	for(i = 2; i < USR_BLOCK_ALM_NUM; i++)
	{
		p->prev = &(g_block_alm[i-2]);
		p->next = &(g_block_alm[i]);
		p = p->next;
	}
	p->prev = &(g_block_alm[USR_BLOCK_ALM_NUM - 2]);
	p->next = (ALM_BLOCK*)NULL;
	g_list_pool_alm_block.tail = &(g_block_alm[USR_BLOCK_ALM_NUM - 1]);
}


/************************************************************
* usr_list_total_alm_init() 
************************************************************/
void usr_list_total_alm_init(void)
{
	g_list_total_alm.list_len		= 0;
	g_list_total_alm.list_lcd_head	= 0;
	g_list_total_alm.log_list_len	= 0;
	g_list_total_alm.log_list_lcd_head = 0;
	g_list_total_alm.head			= (ALM_BLOCK*)NULL;
	g_list_total_alm.tail			= (ALM_BLOCK*)NULL;
}


/************************************************************
* usr_give_block_pool_alm() 
************************************************************/
ALM_BLOCK* usr_give_block_pool_alm(void)
{
	ALM_BLOCK* p;
	if(g_list_pool_alm_block.list_len == 0)
		return (ALM_BLOCK*)NULL;
	
	p = g_list_pool_alm_block.head;
	g_list_pool_alm_block.head = p->next;
	g_list_pool_alm_block.list_len--;

	return p;
}


/************************************************************
* usr_back_block_pool_alm() 
************************************************************/
STATUS usr_back_block_pool_alm(ALM_BLOCK* p)
{
	g_list_pool_alm_block.tail->next = p;
	g_list_pool_alm_block.tail = p;
	g_list_pool_alm_block.list_len++;
	return OK;
}

/************************************************************
* usr_find_block_in_list_alm() 
************************************************************/
ALM_BLOCK* usr_find_block_in_list_alm(ALM_LIST* list, UINT32 tag_id, int level_alm)
{		
	ALM_BLOCK*	p;
	int			cnt,i;

	p	= list->head;
	cnt = list->list_len;

	while((p != (ALM_BLOCK*)NULL) && (cnt != 0))
	{
		if((tag_id == p->pSource->tag_id) && (level_alm == p->alm_level))
			return p;
		cnt--;
		p = p->next;
	}
	return (ALM_BLOCK*)NULL;
}


/************************************************************
* usr_add_block_in_list_alm() 
************************************************************/
STATUS usr_add_block_in_list_alm(ALM_LIST* list, ALM_BLOCK* p)
{
	if(list->list_len == 0)
	{
		list->head		= p;
		list->tail		= p;
		p->prev			= (ALM_BLOCK *)NULL;
		p->next			= (ALM_BLOCK *)NULL;
		list->list_len	= 1;
	}
	else 
	{
		list->head->prev	= p;
		p->next				= list->head;
		list->head			= p;
		p->prev				= list->tail;
		list->tail->next	= list->head;
		list->list_len++;
	}
	return OK;
}


/************************************************************
* usr_del_block_in_list_alm() 
************************************************************/
STATUS usr_del_block_in_list_alm(ALM_LIST* list, ALM_BLOCK* p)
{
	if(list->head == list->tail)
	{
		list->head			= (ALM_BLOCK*)NULL;
		list->tail			= (ALM_BLOCK*)NULL;
		list->list_len		= 0;
	}
	else if(list->head == p)
	{
		list->head			= p->next;
		list->head->prev	= list->tail;
		list->tail->next	= list->head;
		list->list_len--;
	}
	else if(list->tail == p)
	{
		list->tail			= p->prev;
		list->tail->next	= list->head;
		list->head->prev	= list->tail;
		list->list_len--;
	}
	else
	{
		p->prev->next		= p->next;
		p->next->prev		= p->prev;
		list->list_len--;
	}
	return OK;
}

/************************************************************
* usr_find_lcd_head_block() 
************************************************************/
ALM_BLOCK* usr_find_lcd_head_block(void)
{
	ALM_BLOCK	*p;
	int			cnt, i;

	if(g_list_total_alm.list_lcd_head < 1)
		return (ALM_BLOCK*)NULL;

	if(g_list_total_alm.list_lcd_head == 1)
		return g_list_total_alm.head;

	cnt = g_list_total_alm.list_lcd_head - 1;
	p = g_list_total_alm.head;

	for(i = 0; i < cnt; i++)p = p->next;

	return p;
}


/************************************************************
* usr_get_list_total_alm_len() 
************************************************************/
bool usr_is_list_total_alm_change(void)
{
	bool flag = false;
	if((g_list_total_alm.list_len != g_list_total_alm.log_list_len) ||
	   (g_list_total_alm.list_lcd_head != g_list_total_alm.log_list_lcd_head))
		flag = true;

	g_list_total_alm.log_list_len		= g_list_total_alm.list_len;
	g_list_total_alm.log_list_lcd_head	= g_list_total_alm.list_lcd_head;

	return flag;
}


/************************************************************
* usr_get_list_total_alm_len() 
************************************************************/
UINT16 usr_get_list_total_alm_len(void)
{
	return g_list_total_alm.list_len;
}


/************************************************************
* usr_get_list_total_alm_lcd_head() 
************************************************************/
UINT16 usr_get_list_total_alm_lcd_head(void)
{
	return g_list_total_alm.list_lcd_head;
}

/************************************************************
* usr_block_alm_debug_print() 
************************************************************/
void usr_block_alm_debug_print(ALM_BLOCK* p)
{
	/*
	xil_printf("ALM_BLOCK id:               %d\n", p->id);
	xil_printf("ALM_BLOCK alm_level:        %d\n", p->alm_level);
	xil_printf("ALM_BLOCK real_time_value:  %d\n", p->real_time_value);
	xil_printf("ALM_BLOCK alm_limit_value:  %d\n", p->alm_limit_value);
	xil_printf("ALM_BLOCK flag_wait_rdyRst: %d\n", p->flag_wait_rdyRst);
	usr_solid_debug_print(p->pSource);
	*/
}


/************************************************************
* usr_block_alm_debug_print() 
************************************************************/
void usr_block_all_alm_debug_print(void)
{
	UINT16		i;
	ALM_BLOCK	*p;
	p = g_list_total_alm.head;
	for(i = 0; i < usr_get_list_total_alm_len(); i++)
	{
		usr_block_alm_debug_print(p);
		p = p->next;
	}
}



/************************************************************
* usr_list_total_alm_debug_print() 
************************************************************/
void usr_list_total_alm_debug_print(void)
{
	//xil_printf("list_total_alm  len: %d,  lcd_head: %d\n", usr_get_list_total_alm_len(), usr_get_list_total_alm_lcd_head());
}


/************************************************************
* usr_list_pool_copy_alm_init() 
************************************************************/
void usr_list_pool_copy_alm_init(void)
{
	ALM_COPY_BLOCK* p;
	int i;

	for(i = 0; i < USR_COPY_BLOCK_ALM_NUM; i++)
	{
		g_copy_block_alm[i].id		= i;
		g_copy_block_alm[i].pSource	= (ALM_BLOCK*)NULL;
	}

	g_list_pool_alm_copy_block.list_len = USR_COPY_BLOCK_ALM_NUM;

	g_list_pool_alm_copy_block.head	= g_copy_block_alm;
	p		= &(g_copy_block_alm[0]);
	p->prev	= (ALM_COPY_BLOCK*)NULL;
	p->next	= &(g_copy_block_alm[1]);
	p		= p->next;
	for(i = 2; i < USR_COPY_BLOCK_ALM_NUM; i++)
	{
		p->prev = &(g_copy_block_alm[i-2]);
		p->next = &(g_copy_block_alm[i]);
		p = p->next;
	}
	p->prev = &(g_copy_block_alm[USR_COPY_BLOCK_ALM_NUM - 2]);
	p->next = (ALM_COPY_BLOCK*)NULL;
	g_list_pool_alm_copy_block.tail = &(g_copy_block_alm[USR_COPY_BLOCK_ALM_NUM - 1]);
}


/************************************************************
* usr_list_group_copy_alm_init() 
************************************************************/
void usr_list_group_copy_alm_init(void)
{
	int i;

	for(i = 0; i < USR_LIST_ALM_GROUP_NUM; i++)
	{
		g_list_group_copy_alm[i].list_len		= 0;
		g_list_group_copy_alm[i].list_lcd_head	= 0;
		g_list_group_copy_alm[i].log_list_len	= 0;
		g_list_group_copy_alm[i].log_list_lcd_head = 0;
		g_list_group_copy_alm[i].head			= (ALM_COPY_BLOCK*)NULL;
		g_list_group_copy_alm[i].tail			= (ALM_COPY_BLOCK*)NULL;
	}
}


/************************************************************
* usr_give_copy_block_pool_alm() 
************************************************************/
ALM_COPY_BLOCK*	usr_give_copy_block_pool_alm(void)
{
	ALM_COPY_BLOCK* p;
	if(g_list_pool_alm_copy_block.list_len == 0)
		return (ALM_COPY_BLOCK*)NULL;
	
	p = g_list_pool_alm_copy_block.head;
	g_list_pool_alm_copy_block.head = p->next;
	g_list_pool_alm_copy_block.list_len--;

	return p;
}


/************************************************************
* usr_back_copy_block_pool_alm() 
************************************************************/
STATUS usr_back_copy_block_pool_alm(ALM_COPY_BLOCK* p)
{
	g_list_pool_alm_copy_block.tail->next = p;
	g_list_pool_alm_copy_block.tail = p;
	g_list_pool_alm_copy_block.list_len++;
	return OK;
}


/************************************************************
* usr_find_copy_block_in_list_alm() 
************************************************************/
ALM_COPY_BLOCK*	usr_find_copy_block_in_list_alm(ALM_LIST_GROUP* list, ALM_BLOCK* block)
{		
	ALM_COPY_BLOCK*	p;
	int				cnt,i;

	p	= list->head;
	cnt = list->list_len;

	while((p != (ALM_COPY_BLOCK*)NULL) && (cnt != 0))
	{
		if(block == p->pSource)
			return p;
		cnt--;
		p = p->next;
	}
	return (ALM_COPY_BLOCK*)NULL;
}


/************************************************************
* usr_add_copy_block_in_list_alm() 
************************************************************/
STATUS usr_add_copy_block_in_list_alm(ALM_LIST_GROUP* list, ALM_COPY_BLOCK *p)
{
	if(list->list_len == 0)
	{
		list->head		= p;
		list->tail		= p;
		p->prev			= (ALM_COPY_BLOCK *)NULL;
		p->next			= (ALM_COPY_BLOCK *)NULL;
		list->list_len	= 1;
	}
	else 
	{
		list->head->prev	= p;
		p->next				= list->head;
		list->head			= p;
		p->prev				= list->tail;
		list->tail->next	= list->head;
		list->list_len++;
	}
	return OK;
}


/************************************************************
* usr_del_copy_block_in_list_alm() 
************************************************************/
STATUS usr_del_copy_block_in_list_alm(ALM_LIST_GROUP* list, ALM_COPY_BLOCK *p)
{
	if(list->head == list->tail)
	{
		list->head			= (ALM_COPY_BLOCK*)NULL;
		list->tail			= (ALM_COPY_BLOCK*)NULL;
		list->list_len		= 0;
	}
	else if(list->head == p)
	{
		list->head			= p->next;
		list->head->prev	= list->tail;
		list->tail->next	= list->head;
		list->list_len--;
	}
	else if(list->tail == p)
	{
		list->tail			= p->prev;
		list->tail->next	= list->head;
		list->head->prev	= list->tail;
		list->list_len--;
	}
	else
	{
		p->prev->next		= p->next;
		p->next->prev		= p->prev;
		list->list_len--;
	}
	return OK;
}


/************************************************************
* usr_find_lcd_head_copy_block() 
************************************************************/
ALM_COPY_BLOCK*	usr_find_lcd_head_copy_block(int ID)
{
	ALM_COPY_BLOCK	*p;
	int				cnt, i;

	if(g_list_group_copy_alm[ID].list_lcd_head < 1)
		return (ALM_COPY_BLOCK*)NULL;

	if(g_list_group_copy_alm[ID].list_lcd_head == 1)
		return g_list_group_copy_alm[ID].head;

	cnt = g_list_group_copy_alm[ID].list_lcd_head - 1;
	p = g_list_group_copy_alm[ID].head;

	for(i = 0; i < cnt; i++)p = p->next;

	return p;
}


/************************************************************
* usr_is_list_group_alm_change() 
************************************************************/
bool usr_is_list_group_alm_change(int ID)
{
	bool flag = false;
	if((g_list_group_copy_alm[ID].list_len != g_list_group_copy_alm[ID].log_list_len) ||
	   (g_list_group_copy_alm[ID].list_lcd_head != g_list_group_copy_alm[ID].log_list_lcd_head))
		flag = true;

	g_list_group_copy_alm[ID].log_list_len		= g_list_group_copy_alm[ID].list_len;
	g_list_group_copy_alm[ID].log_list_lcd_head	= g_list_group_copy_alm[ID].list_lcd_head;

	return flag;
}



/************************************************************
* usr_get_list_group_alm_len() 
************************************************************/
UINT16 usr_get_list_group_alm_len(int ID)
{
	return g_list_group_copy_alm[ID].list_len;
}


/************************************************************
* usr_get_list_group_alm_lcd_head() 
************************************************************/
UINT16 usr_get_list_group_alm_lcd_head(int ID)
{
	return g_list_group_copy_alm[ID].list_lcd_head;
}


/************************************************************
* usr_copy_block_alm_debug_print() 
************************************************************/
void usr_copy_block_alm_debug_print(ALM_COPY_BLOCK* p)
{
	usr_block_alm_debug_print(p->pSource);
}


/************************************************************
* usr_copy_block_alm_debug_print() 
************************************************************/
void usr_copy_block_all_alm_debug_print(int ID)
{
	UINT16		i;
	ALM_COPY_BLOCK	*p;
	p = g_list_group_copy_alm[ID].head;
	for(i = 0; i < usr_get_list_group_alm_len(ID); i++)
	{
		usr_copy_block_alm_debug_print(p);
		p = p->next;
	}
}


/************************************************************
* usr_list_group_alm_debug_print() 
************************************************************/
void usr_list_group_alm_debug_print(int ID)
{
	//xil_printf("list_group_copy_alm_%d  len: %d,  lcd_head: %d \n",ID, usr_get_list_group_alm_len(ID), usr_get_list_group_alm_lcd_head(ID));
}


/************************************************************
* usr_list_alm_all_debug_print() 
************************************************************/
void usr_list_alm_all_debug_print(void)
{
	usr_list_total_alm_debug_print();
	usr_list_group_alm_debug_print(USR_LIST_ALM_GROUP_1);
	usr_list_group_alm_debug_print(USR_LIST_ALM_GROUP_2);
	usr_list_group_alm_debug_print(USR_LIST_ALM_GROUP_3);
	usr_list_group_alm_debug_print(USR_LIST_ALM_GROUP_4);
	usr_list_group_alm_debug_print(USR_LIST_ALM_GROUP_5);
}



/************************************************************
* usr_list_rst_readyRst_alm() 
************************************************************/
void usr_list_rst_readyRst_alm(void)
{
	ALM_BLOCK*	p;
	ALM_BLOCK*	q;
	int			cnt,i;

	p		= g_list_total_alm.head;
	cnt		= g_list_total_alm.list_len;

	while((p != (ALM_BLOCK*)NULL) && (cnt != 0) )
	{
		q = p->next;
		if(p->flag_wait_rdyRst)
		{
			p->flag_wait_rdyRst = false;
			usr_del_alm(p->pSource->tag_id, p->alm_level);
		}
			
		cnt--;
		p = q;
	}
	
	if(g_list_total_alm.head == g_list_total_alm.tail)
		g_list_total_alm.list_lcd_head = 1;
	else if(g_list_total_alm.head == (ALM_BLOCK*)NULL)
		g_list_total_alm.list_lcd_head = 0;
	else
		;
}



/************************************************************
* usr_add_alm() 
************************************************************/
STATUS usr_add_alm(UINT32 tag_id, int level_alm)
{
	ALM_BLOCK		*p;
	ALM_COPY_BLOCK  *q;
	int				i;
	bool			find = false;

	p = usr_find_block_in_list_alm(&g_list_total_alm, tag_id, level_alm);
	if(p != (ALM_BLOCK *)NULL)
		return ERROR;

	for(i = 0; i < USR_SOLID_ALM_NUM; i++)
	{
		if(g_solid_alm[i].tag_id == tag_id)
		{
			p = usr_give_block_pool_alm();
			if(p == (ALM_BLOCK *)NULL)
				return ERROR;

			q = usr_give_copy_block_pool_alm();
			if(q == (ALM_COPY_BLOCK *)NULL)
			{
				usr_back_block_pool_alm(p);
				return ERROR;
			}

			p->pSource		= &(g_solid_alm[i]);
			p->alm_level	= level_alm;
			usr_add_block_in_list_alm(&g_list_total_alm, p);

			q->pSource		= p;
			usr_add_copy_block_in_list_alm(&(g_list_group_copy_alm[p->pSource->group_id]), q);

			find = true;
			break;
		}
	}
	if(!find)
		return ERROR;

	g_list_total_alm.list_lcd_head									= 1;
	g_list_group_copy_alm[p->pSource->group_id].list_lcd_head	= 1;

	return OK;
}


/************************************************************
* usr_add_alm() 
************************************************************/
STATUS usr_del_alm(UINT32 tag_id, int level_alm)
{
	ALM_BLOCK		*p;
	ALM_COPY_BLOCK  *q;

	p = usr_find_block_in_list_alm(&g_list_total_alm, tag_id, level_alm);
	if(p == (ALM_BLOCK *)NULL)
		return ERROR;

	q = usr_find_copy_block_in_list_alm(&(g_list_group_copy_alm[p->pSource->group_id]), p);
	if(q == (ALM_COPY_BLOCK *)NULL)
		return ERROR;

	usr_del_block_in_list_alm(&g_list_total_alm, p);
	usr_back_block_pool_alm(p);

	usr_del_copy_block_in_list_alm(&(g_list_group_copy_alm[p->pSource->group_id]), q);
	usr_back_copy_block_pool_alm(q);

	if(g_list_total_alm.head == g_list_total_alm.tail)
		g_list_total_alm.list_lcd_head = 1;
	else if(g_list_total_alm.head == (ALM_BLOCK *)NULL)
		g_list_total_alm.list_lcd_head = 0;
	else
		;

	if(g_list_group_copy_alm[p->pSource->group_id].head == g_list_group_copy_alm[p->pSource->group_id].tail)
		g_list_group_copy_alm[p->pSource->group_id].list_lcd_head	= 1;
	else if(g_list_group_copy_alm[p->pSource->group_id].head == (ALM_COPY_BLOCK *)NULL)
		g_list_group_copy_alm[p->pSource->group_id].list_lcd_head	= 0;
	else
		;

	return OK;
}


/************************************************************
* usr_refurbish_alm_happen_time() 
************************************************************/
STATUS usr_refurbish_alm_happen_time(UINT32 tag_id, int level_alm, USR_TIME *time)
{
	ALM_BLOCK *p;
	
	p = usr_find_block_in_list_alm(&g_list_total_alm, tag_id, level_alm);
	if(p == (ALM_BLOCK *)NULL)
		return ERROR;

	p->alm_happen_time.secs		= time->secs;
	p->alm_happen_time.mins		= time->mins;
	p->alm_happen_time.hrs		= time->hrs;
	p->alm_happen_time.date		= time->date;
	p->alm_happen_time.month	= time->month;
	p->alm_happen_time.year_l	= time->year_l;
	p->alm_happen_time.year_h	= time->year_h;

	return OK;
}


/************************************************************
* usr_refurbish_alm_real_time_value() 
************************************************************/
STATUS usr_refurbish_alm_real_time_value(UINT32 tag_id, int level_alm, UINT16 value)
{
	ALM_BLOCK *p;
	
	p = usr_find_block_in_list_alm(&g_list_total_alm, tag_id, level_alm);
	if(p == (ALM_BLOCK *)NULL)
		return ERROR;

	p->real_time_value = value;

	return OK;
}

/************************************************************
* usr_refurbish_alm_limit_value() 
************************************************************/
STATUS usr_refurbish_alm_limit_value(UINT32 tag_id, int level_alm, UINT16 value)
{
	ALM_BLOCK *p;
	
	p = usr_find_block_in_list_alm(&g_list_total_alm, tag_id, level_alm);
	if(p == (ALM_BLOCK *)NULL)
		return ERROR;

	p->alm_limit_value = value;

	return OK;
}

/************************************************************
* usr_refurbish_alm_flag_wait_rdyRst() 
************************************************************/
STATUS usr_refurbish_alm_flag_wait_rdyRst(UINT32 tag_id, int level_alm, bool flag)
{
	ALM_BLOCK *p;
	
	p = usr_find_block_in_list_alm(&g_list_total_alm, tag_id, level_alm);
	if(p == (ALM_BLOCK *)NULL)
		return ERROR;

	p->flag_wait_rdyRst = flag;

	return OK;
}


/************************************************************
* usr_get_alm_flag_wait_rdyRst() 
************************************************************/
bool usr_get_alm_flag_wait_rdyRst(UINT32 tag_id, int level_alm)
{
	ALM_BLOCK *p;
	
	p = usr_find_block_in_list_alm(&g_list_total_alm, tag_id, level_alm);
	if(p == (ALM_BLOCK *)NULL)
		return false;

	return p->flag_wait_rdyRst;
}


/************************************************************
* usr_net_group_sort_newAlm() 
************************************************************/
STATUS usr_net_group_sort_newAlm(UINT32 tag_id, int level_alm)
{
	ALM_BLOCK *p;
	
	p = usr_find_block_in_list_alm(&g_list_total_alm, tag_id, level_alm);
	if(p == (ALM_BLOCK *)NULL)
		return ERROR;

	switch(p->pSource->group_id)
	{
	case USR_LIST_ALM_GROUP_1:
		usr_led_set_flag_overturn(USR_LED_ID_R1);
		break;

	case USR_LIST_ALM_GROUP_2:
		usr_led_set_flag_overturn(USR_LED_ID_R2);
		break;

	case USR_LIST_ALM_GROUP_3:
		usr_led_set_flag_overturn(USR_LED_ID_R3);
		break;

	case USR_LIST_ALM_GROUP_4:
		usr_led_set_flag_overturn(USR_LED_ID_R4);
		break;

	case USR_LIST_ALM_GROUP_5:
		usr_led_set_flag_overturn(USR_LED_ID_R5);
		break;
		
	case USR_LIST_ALM_GROUP_6:
		usr_led_set_flag_overturn(USR_LED_ID_R6);
		break;

	case USR_LIST_ALM_GROUP_7:
		usr_led_set_flag_overturn(USR_LED_ID_R7);
		break;
		
	case USR_LIST_ALM_GROUP_8:
		usr_led_set_flag_overturn(USR_LED_ID_R8);
		break;
		
	case USR_LIST_ALM_GROUP_9:
		usr_led_set_flag_overturn(USR_LED_ID_L8);
		break;
		
	case USR_LIST_ALM_GROUP_10:
		usr_led_set_flag_overturn(USR_LED_ID_L7);
		break;
		
	case USR_LIST_ALM_GROUP_11:
		usr_led_set_flag_overturn(USR_LED_ID_L6);
		break;
		
	case USR_LIST_ALM_GROUP_12:
		usr_led_set_flag_overturn(USR_LED_ID_L5);
		break;
		
	case USR_LIST_ALM_GROUP_13:
		usr_led_set_flag_overturn(USR_LED_ID_L4);
		break;
		
	case USR_LIST_ALM_GROUP_14:
		usr_led_set_flag_overturn(USR_LED_ID_L3);
		break;

	case USR_LIST_ALM_GROUP_15:
		usr_led_set_flag_overturn(USR_LED_ID_L2);
		break;
		
	case USR_LIST_ALM_GROUP_16:
		usr_led_set_flag_overturn(USR_LED_ID_L1);
		break;
					
	default:
		;
	}

	return OK;
}



/************************************************************
* usr_page_debug
*
* USR_KEY_ID_L1 : add 0x70001001 (G_1)
* USR_KEY_ID_L2 : add 0x70001007 (G_1)
* USR_KEY_ID_L3 : add 0x70002008 (G_2)
* USR_KEY_ID_L4 : add 0x70003001 (G_3)
* USR_KEY_ID_L5 : add 0x70003202 (G_3)
* USR_KEY_ID_L6 : add 0x70004002 (G_4)
* USR_KEY_ID_L7 : add 0x70004110 (G_4)
* USR_KEY_ID_L8 : add 0x70005001 (G_5)
* USR_KEY_ID_CALLFROM_BRIDGE : del 0x70001007 (G_1)
* USR_KEY_ID_CALLFROM_ENGINE : del 0x70003001 (G_3)
* USR_KEY_ID_DIMMER_ADD : del 0x70003202 (G_3)
* USR_KEY_ID_DIMMER_SUB : del 0x70004002 (G_4) 
* USR_KEY_ID_STANDBY1 : del 0x70004110 (G_4)
* USR_KEY_ID_STANDBY2 : del 0x70005001 (G_5)
************************************************************/
void usr_page_debug(void)
{
	if(usr_key_isup(USR_KEY_ID_L1))
		usr_add_alm(0x70001001, 1);

	if(usr_key_isup(USR_KEY_ID_L2))
		usr_add_alm(0x70001007, 1);

	if(usr_key_isup(USR_KEY_ID_L3))
		usr_add_alm(0x70002008, 1);

	if(usr_key_isup(USR_KEY_ID_L4))
		usr_add_alm(0x70003001, 1);

	if(usr_key_isup(USR_KEY_ID_L5))
		usr_add_alm(0x70003202, 1);

	if(usr_key_isup(USR_KEY_ID_L6))
		usr_add_alm(0x70004002, 1);

	if(usr_key_isup(USR_KEY_ID_L7))
		usr_add_alm(0x70004110, 1);

	if(usr_key_isup(USR_KEY_ID_L8))
		usr_add_alm(0x70005001, 1);

	if(usr_key_isup(USR_KEY_ID_CALLFROM_BRIDGE))
		usr_del_alm(0x70001007, 1);

	if(usr_key_isup(USR_KEY_ID_CALLFROM_ENGINE))
		usr_del_alm(0x70003001, 1);

	if(usr_key_isup(USR_KEY_ID_DIMMER_ADD))
		usr_del_alm(0x70003202, 1);

	if(usr_key_isup(USR_KEY_ID_DIMMER_SUB))
		usr_del_alm(0x70004002, 1);

	if(usr_key_isup(USR_KEY_ID_STANDBY1))
		usr_del_alm(0x70004110, 1);

	if(usr_key_isup(USR_KEY_ID_STANDBY2))
		usr_del_alm(0x70005001, 1);
}



