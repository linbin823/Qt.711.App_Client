#include "usr.h"

/************************************************************
* VAR
************************************************************/
Xuint8		g_CONFIGURE[USR_FLASH_DATA_LEN_BYTE];
bool		g_net_configure_group[USR_NET_CONFIGURE_GROUP_NUM];

bool		g_default_mac;
bool		g_default_par;
bool		g_default_status;


UINT8		g_SIPACP_IP[2][4];		//延伸报警模块IP地址[双网段]
UINT32		g_SIPACP_PORT[2];		//延伸报警模块配置端口
UINT8 		g_SMIPAP_IP[2][4]; 		//延伸报警模块组播地址[双网段]
UINT32		g_SMIPAP_PORT[2];		//延伸报警模块组播端口
UINT32		g_SSSIGN;				//延伸报警模块所属分组标志
UINT8		g_SPNAME_CH[513][24];	//延伸报警模块参数中文名称[513个]
UINT8		g_SPNAME_EN[513][36];	//延伸报警模块参数英文名称[513个]
UINT8		g_SPUNIT[513][5];		//延伸报警模块参数单位[513个]
UINT8		g_SPDAPW_P[513];		//延伸报警模块参数阶码[513个]
UINT8		g_SPDAPW_F[513];		//延伸报警模块参数显示方式[513个]
UINT8		g_SPMPN[513][8];		//延伸报警模块参数测点号[513个]
UINT8		g_SPICTN[30][24];		//延伸报警模块图形显示页标题名称[30]
UINT16		g_SPPPIA[30][30];		//延伸报警模块图形及特殊显示页参数索引地址
UINT8		g_SECNAME[32][20];		//轮机员呼叫成员名单[32个]
UINT32		g_SECSGN;				//延伸报警模块轮机员呼叫标志
UINT8		g_SHNAME[20];			//延伸报警主机名称
UINT32		g_SEAID;				//延伸报警模块ID号
UINT8		g_SACKRSF;				//全网应答全网复位处理标志
UINT8		g_TGBZ;					//调光标志	
UINT32		g_MONITOR_IP[20];		//标显台IP地址
UINT8 		g_MONITOR_NAME[10][20]; //标显台名称
	


u32_t		g_flash_module_net_ip[2];
u32_t		g_flash_module_broadcast_ip[2];
u16_t		g_flash_module_config_port[2];
u16_t		g_flash_module_data_port[2];
UINT32		g_flash_module_group;
UINT8		g_flash_module_identity_name[20];
UINT32		g_flash_module_identity_id;
int			g_flash_module_identity_light;
UINT8		g_flash_module_name[20];
UINT32		g_flash_module_id;
bool		g_flash_flag_net_ack;				
bool		g_flash_flag_net_setLight;	
u32_t		g_flash_bridge_net_ip[2];
u32_t		g_flash_engine_net_ip[2];
u8_t		g_interface_mac[2][6];


/************************************************************
* FUNC
************************************************************/
void		drv_flash_init(void);
void		drv_flash_sector_erase(Xuint8 sector);
void 		drv_flash_erase(void);
void 		drv_flash_demo_test(void);
void 		drv_flash_write_page_256byte(Xuint16 sector, Xuint16 page, Xuint8 *bfr);
void 		drv_flash_read_page_256byte(Xuint16 sector, Xuint16 page, Xuint8 *bfr);
void		drv_debug_prt(Xuint16 sector, Xuint16 page);

STATUS		usr_flash_first_write(void);
STATUS		usr_configure_status_init(void);
STATUS		usr_flash_init(void);
STATUS		usr_flash_data_get(void);
STATUS		usr_flash_data_set(void);
STATUS		usr_flash_data_analyze(void);
Xuint8		usr_flash_analyze_1byte(Xuint8 temp);
Xuint8		usr_flash_analyze_2byte(UINT16 offset);
void		usr_flash_analyze_SIPACP_IP(void);		//延伸报警模块IP地址[双网段]
void		usr_flash_analyze_SIPACP_PORT(void);	//延伸报警模块配置端口	
void		usr_flash_analyze_SMIPAP_IP(void);		//延伸报警模块组播地址[双网段]
void		usr_flash_analyze_SMIPAP_PORT(void);	//延伸报警模块组播端口
void		usr_flash_analyze_SSSIGN(void);			//延伸报警模块所属分组标志
void		usr_flash_analyze_SPNAME_CH(void);		//延伸报警模块参数中文名称[513个]
void		usr_flash_analyze_SPNAME_EN(void);		//延伸报警模块参数英文名称[513个]
void		usr_flash_analyze_SPUNIT(void);			//延伸报警模块参数单位[513个]
void		usr_flash_analyze_SPDAPW_P(void);		//延伸报警模块参数阶码[513个]
void		usr_flash_analyze_SPDAPW_F(void);		//延伸报警模块参数显示方式[513个]
void		usr_flash_analyze_SPMPN(void);			//延伸报警模块参数测点号[513个]
void		usr_flash_analyze_SPICTN(void); 		//延伸报警模块图形显示页标题名称[30]	
void		usr_flash_analyze_SPPPIA(void);			//延伸报警模块图形及特殊显示页参数索引地址
void		usr_flash_analyze_SECNAME(void);		//轮机员呼叫成员名单[32个]
void		usr_flash_analyze_SECSGN(void);			//延伸报警模块轮机员呼叫标志
void		usr_flash_analyze_SHNAME(void);			//延伸报警主机名称
void		usr_flash_analyze_SEAID(void);			//延伸报警模块ID号
void		usr_flash_analyze_SACKRSF(void);		//全网应答全网复位处理标志
void		usr_flash_analyze_TGBZ(void);			//调光标志	
void		usr_flash_analyze_MONITOR_IP(void);		//标显台IP地址	
void		usr_flash_analyze_MONITOR_NAME(void);	//标显台名称	

void		usr_flash_Wr_Rd_debug(void);

bool		usr_flash_is_default_mac(void);
STATUS		usr_flash_over_default_mac(void);
bool		usr_flash_is_default_par(void);
STATUS		usr_flash_over_default_par(void);
STATUS		usr_flash_rst_default(void);
bool		usr_is_default_mac(void);
bool		usr_is_default_par(void);
bool		usr_is_default_status(void);

/************************************************************
* drv_flash_init
************************************************************/
void drv_flash_init(void)
{
	Initialize_Spi_Controller(XPAR_SPI_FLASH_BASEADDR);
}


/************************************************************
* drv_flash_sector_erase
************************************************************/
void drv_flash_sector_erase(Xuint8 sector)
{
	M25P16_sector_erase (XPAR_SPI_FLASH_BASEADDR, sector);
}


/************************************************************
* drv_flash_erase
************************************************************/
void drv_flash_erase(void)
{
	Xuint8*  ddr_data = (Xuint8 *) XPAR_DDR_SDRAM_MPMC_BASEADDR;
	Xuint8   send_data[16], recv_data[16];
	Xuint8   NumBytesRcvd, mfg_id, mem_type, mem_capacity, error;
	Xuint16  sector_select, page_select;
	Xuint16  spi_control_reg;
	Xuint32  spi_ss_reg, i, j;

	XSpi_mEnable(XPAR_SPI_FLASH_BASEADDR);

	XSpi_mSetSlaveSelectReg(XPAR_SPI_FLASH_BASEADDR, SPI_NONE_SELECT);

	// Perform a Chip Erase
	//xil_printf("\n\rChip Erase Starting\n\r");
	M25P16_bulk_erase(XPAR_SPI_FLASH_BASEADDR);
	//xil_printf("Chip Erase Complete\n\r");

	// Verify Chip Erase
	M25P16_start_read(XPAR_SPI_FLASH_BASEADDR, 0x00, 0x00, 0x00, SCK_FASTER_THAN_20MHz);
	error = 0;
	for (i = 0; i < M25P16_BYTES/16; i++)
	{
		spi_transfer(send_data, recv_data, 16);
		for (j = 0; j < 16; j++)
		{
		  if(recv_data[j] != 0xFF)
		  {
			//xil_printf("Error at i = %x, j = %x\n\r", i,j);
			j = 16;
			i = M25P16_BYTES;
			error = 1;
		  }
		}
	}
	if(!error)
		;//xil_printf("Erase verified\n\r");
	else
		;//xil_printf("Erase failed\n\r");

	M25P16_end_read (XPAR_SPI_FLASH_BASEADDR);
	// Disable the SPI Controller
	XSpi_mDisable(XPAR_SPI_FLASH_BASEADDR);
}


/************************************************************
* drv_flash_demo_test
************************************************************/
void drv_flash_demo_test(void)
{
	Xuint8*  ddr_data = (Xuint8 *) XPAR_DDR_SDRAM_MPMC_BASEADDR;
	Xuint8   send_data[16], recv_data[16];
	Xuint8   NumBytesRcvd, mfg_id, mem_type, mem_capacity, error;
	Xuint16  sector_select, page_select;
	Xuint16  spi_control_reg;
	Xuint32  spi_ss_reg, i, j;

	Initialize_Spi_Controller(XPAR_SPI_FLASH_BASEADDR);
	XSpi_mEnable(XPAR_SPI_FLASH_BASEADDR);

	// Get the Manufacturer's ID
	// Send the Read ID op-code -- ignoring recv_data
	XSpi_mSetSlaveSelectReg(XPAR_SPI_FLASH_BASEADDR, SPI_FLASH_SELECT);
	send_data[0] = RDID;
	spi_transfer(send_data, recv_data, 1);

	// Get the 3-byte ID -- send_data is don't care
	spi_transfer(send_data, recv_data, 3);

	// Deselect flash
	XSpi_mSetSlaveSelectReg(XPAR_SPI_FLASH_BASEADDR, SPI_NONE_SELECT);

	mfg_id       = recv_data[0];
	mem_type     = recv_data[1];
	mem_capacity = recv_data[2];

	//xil_printf("Manufacturer Code = : %x \n\r", mfg_id);
	//xil_printf("Memory Type       = : %x \n\r", mem_type);
	//xil_printf("Memory Capacity   = : %x \n\r", mem_capacity);

	// Perform a Chip Erase
	//xil_printf("\n\rChip Erase Starting\n\r");
	M25P16_bulk_erase(XPAR_SPI_FLASH_BASEADDR);
	//xil_printf("Chip Erase Complete - verifying\n\r");

	// Verify Chip Erase
	M25P16_start_read(XPAR_SPI_FLASH_BASEADDR, 0x00, 0x00, 0x00, SCK_FASTER_THAN_20MHz);
	error = 0;
	for (i = 0; i < M25P16_BYTES/16; i++)
	{
		spi_transfer(send_data, recv_data, 16);
		for (j = 0; j < 16; j++)
		{
		  if(recv_data[j] != 0xFF)
		  {
			//xil_printf("Error at i = %x, j = %x\n\r", i,j);
			j = 16;
			i = M25P16_BYTES;
			error = 1;
		  }
		}
	}
	if(!error)
		;//xil_printf("Erase verified\n\r");
	else 
		;//xil_printf("Erase failed\n\r");
	M25P16_end_read (XPAR_SPI_FLASH_BASEADDR);


	// Write Test
	//xil_printf("\n\rData being written\n\r");
	for(sector_select = 0; sector_select < M25P16_SECTORS; sector_select++)
		for(page_select = 0; page_select < M25P16_PAGES_PER_SECTOR; page_select++)
		{
			M25P16_start_page_program (XPAR_SPI_FLASH_BASEADDR, sector_select, page_select, 0x00);
			for (i = 0; i < M25P16_BYTES_PER_PAGE; i+=16)
			{
				for(j = 0; j < 16; j++)
				  send_data[j]  = i + j;
				spi_transfer(send_data, recv_data, 16);
			}
			M25P16_end_page_program (XPAR_SPI_FLASH_BASEADDR);
		}
	//xil_printf("Test Data Written; reading back\n\n\r", i);
  
  
	// Readback all bytes
	M25P16_start_read (XPAR_SPI_FLASH_BASEADDR, 0x00, 0x00, 0x00, SCK_FASTER_THAN_20MHz);
	error = 0;
	for (i = 0; i < M25P16_BYTES; i+=16)
	{
		spi_transfer(send_data, recv_data, 16);
		for(j = 0; j < 16; j++)
		{
			if(recv_data[j] != ((i + j) & 0xFF))
			{
				//xil_printf("Error at i = %x, j = %x\n\r", i,j);
				//xil_printf("Data read was %x\n\r", recv_data[j]);
				//xil_printf("Exp. data was %x\n\r", (i+j));
				j = 16;
				i = M25P16_BYTES;
				error = 1;
			}
		}
	}
	if(!error)
		;//xil_printf("Flash Test PASSED!\n\r");
	else
		;//xil_printf("Flash Test not PASSED!\n\r");

	M25P16_end_read (XPAR_SPI_FLASH_BASEADDR);
  
	// Disable the SPI Controller
	XSpi_mDisable(XPAR_SPI_FLASH_BASEADDR);
}


/************************************************************
* drv_flash_write_page_256byte
************************************************************/
void drv_flash_write_page_256byte(Xuint16 sector, Xuint16 page, Xuint8 *bfr)
{
	Xuint8*  ddr_data = (Xuint8 *) XPAR_DDR_SDRAM_MPMC_BASEADDR;
	Xuint8   send_data[16], recv_data[16];
	Xuint8   NumBytesRcvd, mfg_id, mem_type, mem_capacity, error;
	Xuint16  sector_select, page_select;
	Xuint16  spi_control_reg;
	Xuint32  spi_ss_reg, i, j, k;

	XSpi_mEnable(XPAR_SPI_FLASH_BASEADDR);
	XSpi_mSetSlaveSelectReg(XPAR_SPI_FLASH_BASEADDR, SPI_FLASH_SELECT);
  
	M25P16_start_page_program (XPAR_SPI_FLASH_BASEADDR, sector, page, 0x00);
	for (i = 0; i < 256; i+=16)
	{
	  for(j = 0; j < 16; j++)
	  {
		 send_data[j]  = bfr[i + j];
		}
	  spi_transfer(send_data, recv_data, 16);
	}
	
	M25P16_end_page_program (XPAR_SPI_FLASH_BASEADDR);
	XSpi_mDisable(XPAR_SPI_FLASH_BASEADDR);
}


/************************************************************
* drv_flash_read_page_256byte
************************************************************/
void drv_flash_read_page_256byte(Xuint16 sector, Xuint16 page, Xuint8 *bfr)
{
	Xuint8*  ddr_data = (Xuint8 *) XPAR_DDR_SDRAM_MPMC_BASEADDR;
	Xuint8   send_data[16], recv_data[16];
	Xuint8   NumBytesRcvd, mfg_id, mem_type, mem_capacity, error;
	Xuint16  sector_select, page_select;
	Xuint16  spi_control_reg;
	Xuint32  spi_ss_reg, i, j, m = 0;

	XSpi_mEnable(XPAR_SPI_FLASH_BASEADDR);

	XSpi_mSetSlaveSelectReg(XPAR_SPI_FLASH_BASEADDR, SPI_FLASH_SELECT);
  
	M25P16_start_read (XPAR_SPI_FLASH_BASEADDR, sector, page, 0x00, SCK_FASTER_THAN_20MHz);
	for (i = 0; i < 256; i+=16)
	{
	 spi_transfer(send_data, recv_data, 16);
	 for(j = 0; j < 16; j++)
	 {
		bfr[m++]	= recv_data[j];
	 }
	}
  
	M25P16_end_read (XPAR_SPI_FLASH_BASEADDR);

	XSpi_mDisable(XPAR_SPI_FLASH_BASEADDR);
}


/************************************************************
* drv_debug_prt
************************************************************/
void drv_debug_prt(Xuint16 sector, Xuint16 page)
{
	Xuint8*  ddr_data = (Xuint8 *) XPAR_DDR_SDRAM_MPMC_BASEADDR;
	Xuint8   send_data[16], recv_data[16];
	Xuint8   NumBytesRcvd, mfg_id, mem_type, mem_capacity, error;
	Xuint16  sector_select, page_select;
	Xuint16  spi_control_reg;
	Xuint32  spi_ss_reg, i, j;

	//xil_printf("\n HERE IS sector_%d page_%d \n",sector,page);

	XSpi_mEnable(XPAR_SPI_FLASH_BASEADDR);
	XSpi_mSetSlaveSelectReg(XPAR_SPI_FLASH_BASEADDR, SPI_FLASH_SELECT);

	// Readback all bytes
	M25P16_start_read (XPAR_SPI_FLASH_BASEADDR, sector, page, 0x00, SCK_FASTER_THAN_20MHz);
	for (i = 0; i < 256; i+=16)
	{
		spi_transfer(send_data, recv_data, 16);
		for(j = 0; j < 16; j++)
			;//xil_printf("%x ",recv_data[j]);
		//xil_printf("\n");
	}
	M25P16_end_read (XPAR_SPI_FLASH_BASEADDR);

	// Disable the SPI Controller
	XSpi_mDisable(XPAR_SPI_FLASH_BASEADDR);
}








/************************************************************
* usr_flash_first_write
************************************************************/
STATUS usr_flash_first_write(void)
{
	return OK;
}


/************************************************************
* usr_flash_first_write
************************************************************/
STATUS usr_configure_status_init(void)
{
	if(usr_flash_is_default_mac())
		g_default_mac = true;
	else
		g_default_mac = false;

	if(usr_flash_is_default_par())
		g_default_par = true;
	else
		g_default_par = false;

	if(g_default_mac || g_default_par)
		g_default_status = true;
	else
		g_default_status = false;

	//xil_printf("g_default_mac:%d, g_default_par:%d \n", g_default_mac, g_default_par);

	if(g_default_status)
	{
		if(g_default_mac)
		{
			g_interface_mac[USR_IP_INTERFACE_1][0]			= USR_DEFAULT_MAC1_1;
			g_interface_mac[USR_IP_INTERFACE_1][1]			= USR_DEFAULT_MAC1_2;
			g_interface_mac[USR_IP_INTERFACE_1][2]			= USR_DEFAULT_MAC1_3;
			g_interface_mac[USR_IP_INTERFACE_1][3]			= USR_DEFAULT_MAC1_4;
			g_interface_mac[USR_IP_INTERFACE_1][4]			= USR_DEFAULT_MAC1_5;
			g_interface_mac[USR_IP_INTERFACE_1][5]			= USR_DEFAULT_MAC1_6;
			g_flash_module_net_ip[USR_IP_INTERFACE_1]		= inet_addr(USR_DEFAULT_IP_INTERFACE_1); 
			g_flash_module_broadcast_ip[USR_IP_INTERFACE_1]	= inet_addr(USR_DEFAULT_BROAD_IP_INTERFACE_1); 
			g_flash_module_config_port[USR_IP_INTERFACE_1]	= USR_DEFAULT_CONFIG_PORT_1;
			g_flash_module_data_port[USR_IP_INTERFACE_1]	= USR_DEFAULT_CONFIG_DATA_1;


			g_interface_mac[USR_IP_INTERFACE_2][0]			= USR_DEFAULT_MAC2_1;
			g_interface_mac[USR_IP_INTERFACE_2][1]			= USR_DEFAULT_MAC2_2;
			g_interface_mac[USR_IP_INTERFACE_2][2]			= USR_DEFAULT_MAC2_3;
			g_interface_mac[USR_IP_INTERFACE_2][3]			= USR_DEFAULT_MAC2_4;
			g_interface_mac[USR_IP_INTERFACE_2][4]			= USR_DEFAULT_MAC2_5;
			g_interface_mac[USR_IP_INTERFACE_2][5]			= USR_DEFAULT_MAC2_6;
			g_flash_module_net_ip[USR_IP_INTERFACE_2]		= inet_addr(USR_DEFAULT_IP_INTERFACE_2); 
			g_flash_module_broadcast_ip[USR_IP_INTERFACE_2]	= inet_addr(USR_DEFAULT_BROAD_IP_INTERFACE_2); 
			g_flash_module_config_port[USR_IP_INTERFACE_2]	= USR_DEFAULT_CONFIG_PORT_2;
			g_flash_module_data_port[USR_IP_INTERFACE_2]	= USR_DEFAULT_CONFIG_DATA_2;
		}
		else
		{
			g_interface_mac[USR_IP_INTERFACE_1][0]			= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 0];
			g_interface_mac[USR_IP_INTERFACE_1][1]			= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 1];
			g_interface_mac[USR_IP_INTERFACE_1][2]			= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 2];
			g_interface_mac[USR_IP_INTERFACE_1][3]			= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 3];
			g_interface_mac[USR_IP_INTERFACE_1][4]			= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 4];
			g_interface_mac[USR_IP_INTERFACE_1][5]			= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 5];
			g_flash_module_net_ip[USR_IP_INTERFACE_1]		= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 15]&0xff); 
			g_flash_module_net_ip[USR_IP_INTERFACE_1]		+= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 14]&0xff)<<8;
			g_flash_module_net_ip[USR_IP_INTERFACE_1]		+= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 13]&0xff)<<16;
			g_flash_module_net_ip[USR_IP_INTERFACE_1]		+= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 12]&0xff)<<24;
			g_flash_module_broadcast_ip[USR_IP_INTERFACE_1]	= g_flash_module_net_ip[USR_IP_INTERFACE_1] | 0xff;
			g_flash_module_config_port[USR_IP_INTERFACE_1]	= USR_DEFAULT_CONFIG_PORT_1;
			g_flash_module_data_port[USR_IP_INTERFACE_1]	= USR_DEFAULT_CONFIG_DATA_1;


			g_interface_mac[USR_IP_INTERFACE_2][0]			= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 6];
			g_interface_mac[USR_IP_INTERFACE_2][1]			= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 7];
			g_interface_mac[USR_IP_INTERFACE_2][2]			= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 8];
			g_interface_mac[USR_IP_INTERFACE_2][3]			= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 9];
			g_interface_mac[USR_IP_INTERFACE_2][4]			= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 10];
			g_interface_mac[USR_IP_INTERFACE_2][5]			= g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 11];
			g_flash_module_net_ip[USR_IP_INTERFACE_2]		= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 19]&0xff); 
			g_flash_module_net_ip[USR_IP_INTERFACE_2]		+= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 18]&0xff)<<8;
			g_flash_module_net_ip[USR_IP_INTERFACE_2]		+= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 17]&0xff)<<16;
			g_flash_module_net_ip[USR_IP_INTERFACE_2]		+= (g_CONFIGURE[USR_FLASH_DATA_OFFSET_MAC + 16]&0xff)<<24;
			g_flash_module_broadcast_ip[USR_IP_INTERFACE_2]	= g_flash_module_net_ip[USR_IP_INTERFACE_2] | 0xff;
			g_flash_module_config_port[USR_IP_INTERFACE_2]	= USR_DEFAULT_CONFIG_PORT_2;
			g_flash_module_data_port[USR_IP_INTERFACE_2]	= USR_DEFAULT_CONFIG_DATA_2;
		}
/*
		xil_printf(" default mac1: %x-%x-%x-%x-%x-%x,  mac2: %x-%x-%x-%x-%x-%x, \n", 
			g_interface_mac[USR_IP_INTERFACE_1][0], g_interface_mac[USR_IP_INTERFACE_1][1], g_interface_mac[USR_IP_INTERFACE_1][2],
			g_interface_mac[USR_IP_INTERFACE_1][3], g_interface_mac[USR_IP_INTERFACE_1][4], g_interface_mac[USR_IP_INTERFACE_1][5],
			g_interface_mac[USR_IP_INTERFACE_2][0], g_interface_mac[USR_IP_INTERFACE_2][1], g_interface_mac[USR_IP_INTERFACE_2][2],
			g_interface_mac[USR_IP_INTERFACE_2][3], g_interface_mac[USR_IP_INTERFACE_2][4], g_interface_mac[USR_IP_INTERFACE_2][5]);

		xil_printf(" default ip1: %x, broadcast_ip1: %x, Cport1: %d, Dport1: %d\n default ip2: %x, broadcast_ip2: %x, Cport2: %d, Dport2: %d \n", 
		g_flash_module_net_ip[USR_IP_INTERFACE_1], g_flash_module_broadcast_ip[USR_IP_INTERFACE_1], g_flash_module_config_port[USR_IP_INTERFACE_1], g_flash_module_data_port[USR_IP_INTERFACE_1],
		g_flash_module_net_ip[USR_IP_INTERFACE_2], g_flash_module_broadcast_ip[USR_IP_INTERFACE_2], g_flash_module_config_port[USR_IP_INTERFACE_2], g_flash_module_data_port[USR_IP_INTERFACE_2]);
*/
	}

	return OK;
}



/************************************************************
* usr_flash_init
************************************************************/
STATUS usr_flash_init(void)
{
	UINT16 i,j;

	//if(usr_is_default_status())
	//	return ERROR;

	for(i = 0; i < USR_FLASH_DATA_LEN_BYTE; i++)g_CONFIGURE[i] = 0;
	for(i = 0; i < USR_NET_CONFIGURE_GROUP_NUM; i++)g_net_configure_group[i] = false;
	

	for(i = 0; i < 2; i++)
	{
		for(j = 0; j < 4; j++)
		{
			g_SIPACP_IP[i][j] = 0;
			g_SMIPAP_IP[i][j] = 0;
		}
	}

	for(i = 0; i < 2; i++)
	{
		g_SIPACP_PORT[i] = 0;
		g_SMIPAP_PORT[i] = 0;
	}		

	g_SSSIGN = 0;

	for(j = 0; j < 24; j++)
		for(i = 0; i < 513; i++)
			g_SPNAME_CH[i][j] = 0;	

	for(j = 0; j < 36; j++)
		for(i = 0; i < 513; i++)
			g_SPNAME_EN[i][j] = 0;	

	for(j = 0; j < 5; j++)
		for(i = 0; i < 513; i++)
			g_SPUNIT[i][j] = 0;	

	for(i = 0; i < 513; i++)
	{
		g_SPDAPW_P[i]	= 0;			
		g_SPDAPW_F[i]	= 0;		
	}

	for(i = 0; i < 513; i++)
		for(j = 0; j < 8; j++)
			g_SPMPN[i][j] = 0;

	for(j = 0; j < 24; j++)
		for(i = 0; i < 30; i++)
			g_SPICTN[i][j] = 0;	

	for(j = 0; j < 30; j++)
		for(i = 0; i < 30; i++)
			g_SPPPIA[i][j] = 0;	
	
	for(j = 0; j < 20; j++)
		for(i = 0; i < 32; i++)
			g_SECNAME[i][j] = 0;	

	g_SECSGN = 0;

	for(i = 0; i < 20; i++)g_SHNAME[i] = 0;	

	g_SEAID = 0;

	g_SACKRSF	= 0;
	g_TGBZ		= 0;

	for(i = 0; i < 20; i++)g_MONITOR_IP[i] = 0x00000000;

	for(j = 0; j < 20; j++)
		for(i = 0; i < 10; i++)
			g_MONITOR_NAME[i][j] = 0;

	usr_flash_data_get();
	usr_flash_data_analyze();

	return OK;
}

/************************************************************
* usr_flash_data_get
************************************************************/
STATUS usr_flash_data_get(void)
{
	int i;
	Xuint8 *bfr;
	int len = USR_FLASH_DATA_LEN_BYTE / 256 + 1;
	
	for(i = 0; i < len; i++)
	{
		bfr = g_CONFIGURE + (256 * i);
		drv_flash_read_page_256byte(USR_FLASH_DATA_SECTOR_START, i, bfr);
	}
}

/************************************************************
* usr_flash_data_set
************************************************************/
STATUS usr_flash_data_set(void)
{
	int i;
	Xuint8 *bfr;
	int len = USR_FLASH_DATA_LEN_BYTE / 256 + 1; //177
	
	drv_flash_sector_erase(USR_FLASH_DATA_SECTOR_START);
	for(i = 0; i < len; i++)
	{
		bfr = g_CONFIGURE + (256 * i);
		drv_flash_write_page_256byte(USR_FLASH_DATA_SECTOR_START, i, bfr);
	}
}

/************************************************************
* usr_flash_data_analyze
************************************************************/
STATUS usr_flash_data_analyze(void)
{
	usr_flash_analyze_SIPACP_IP();		//延伸报警模块IP地址[双网段]
	usr_flash_analyze_SIPACP_PORT();	//延伸报警模块配置端口	
	usr_flash_analyze_SMIPAP_IP();		//延伸报警模块组播地址[双网段]
	usr_flash_analyze_SMIPAP_PORT();	//延伸报警模块组播端口
	usr_flash_analyze_SSSIGN();			//延伸报警模块所属分组标志
	usr_flash_analyze_SPNAME_CH();		//延伸报警模块参数中文名称[513个]
	usr_flash_analyze_SPNAME_EN();		//延伸报警模块参数英文名称[513个]
	usr_flash_analyze_SPUNIT();			//延伸报警模块参数单位[513个]
	usr_flash_analyze_SPDAPW_P();		//延伸报警模块参数阶码[513个]
	usr_flash_analyze_SPDAPW_F();		//延伸报警模块参数显示方式[513个]
	usr_flash_analyze_SPMPN();			//延伸报警模块参数测点号[513个]
	usr_flash_analyze_SPICTN(); 		//延伸报警模块图形显示页标题名称[30]	
	usr_flash_analyze_SPPPIA();			//延伸报警模块图形及特殊显示页参数索引地址
	usr_flash_analyze_SECNAME();		//轮机员呼叫成员名单[32个]
	usr_flash_analyze_SECSGN();			//延伸报警模块轮机员呼叫标志
	usr_flash_analyze_SHNAME();			//延伸报警主机名称
	usr_flash_analyze_SEAID();			//延伸报警模块ID号
	usr_flash_analyze_SACKRSF();		//全网应答全网复位处理标志
	usr_flash_analyze_TGBZ();			//调光标志	
	usr_flash_analyze_MONITOR_IP();		//标显台IP地址	
	usr_flash_analyze_MONITOR_NAME();	//标显台名称

#ifdef USR_FLASH_DEBUG
	usr_flash_Wr_Rd_debug();
#endif

	return OK;
}


Xuint8 usr_flash_analyze_1byte(Xuint8 temp)
{
	if( (temp >= 0x30) && (temp <= 0x39) )
		return (temp - 0x30);
		
	if( (temp >= 0x41) && (temp <= 0x46) )
		return (temp - 0x37);
		
	return temp;
}

Xuint8 usr_flash_analyze_2byte(UINT16 offset)
{
	Xuint8 *bfr, temp = 0x00;
	
	bfr = g_CONFIGURE + offset;
	temp = ((usr_flash_analyze_1byte(bfr[0])&0xff)*16)
			+ (usr_flash_analyze_1byte(bfr[1])&0xff);
		
	return temp;
}


void usr_flash_analyze_SIPACP_IP(void)
{
	g_SIPACP_IP[0][0] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 0);	
	g_SIPACP_IP[0][1] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 2);	
	g_SIPACP_IP[0][2] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 4);	
	g_SIPACP_IP[0][3] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 6);	

	g_SIPACP_IP[1][0] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 16);		
	g_SIPACP_IP[1][1] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 18);		
	g_SIPACP_IP[1][2] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 20);		
	g_SIPACP_IP[1][3] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 22);		
}

void usr_flash_analyze_SIPACP_PORT(void)
{
	UINT8 tmp[4];

	tmp[0] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 8);	
	tmp[1] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 10);	
	tmp[2] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 12);	
	tmp[3] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 14);	
	g_SIPACP_PORT[0] = (tmp[0] << 24) + (tmp[1] << 16) + (tmp[2] << 8) + tmp[3];

	tmp[0] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 24);	
	tmp[1] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 26);	
	tmp[2] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 28);	
	tmp[3] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SIPACP_IP_PORT + 30);	
	g_SIPACP_PORT[1] = (tmp[0] << 24) + (tmp[1] << 16) + (tmp[2] << 8) + tmp[3];
}	

void usr_flash_analyze_SMIPAP_IP(void)
{
	g_SMIPAP_IP[0][0] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 0);	
	g_SMIPAP_IP[0][1] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 2);	
	g_SMIPAP_IP[0][2] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 4);	
	g_SMIPAP_IP[0][3] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 6);	

	g_SMIPAP_IP[1][0] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 16);		
	g_SMIPAP_IP[1][1] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 18);		
	g_SMIPAP_IP[1][2] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 20);		
	g_SMIPAP_IP[1][3] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 22);		
}	

void usr_flash_analyze_SMIPAP_PORT(void)
{
	UINT8 tmp[4];

	tmp[0] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 8);	
	tmp[1] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 10);	
	tmp[2] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 12);	
	tmp[3] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 14);	
	g_SMIPAP_PORT[0] = (tmp[0] << 24) + (tmp[1] << 16) + (tmp[2] << 8) + tmp[3];

	tmp[0] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 24);	
	tmp[1] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 26);	
	tmp[2] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 28);	
	tmp[3] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MIPAP_IP_PORT + 30);	
	g_SMIPAP_PORT[1] = (tmp[0] << 24) + (tmp[1] << 16) + (tmp[2] << 8) + tmp[3];
}	
	
void usr_flash_analyze_SSSIGN(void)
{
	UINT8 tmp[4];

	tmp[0] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SSSIGN + 0);	
	tmp[1] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SSSIGN + 2);	
	tmp[2] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SSSIGN + 4);	
	tmp[3] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SSSIGN + 6);	
	g_SSSIGN = (tmp[0] << 24) + (tmp[1] << 16) + (tmp[2] << 8) + tmp[3];
}	
				
void usr_flash_analyze_SPNAME_CH(void)
{
	UINT16 i,j;

	for(i = 0; i < 513; i++)
		for(j = 0; j < 24; j++)
			g_SPNAME_CH[i][j] = g_CONFIGURE[USR_FLASH_DATA_OFFSET_SPNAME_CH + 24*i + j];
	
}	

void usr_flash_analyze_SPNAME_EN(void)
{
	UINT16 i,j;

	for(i = 0; i < 513; i++)
		for(j = 0; j < 36; j++)
			g_SPNAME_EN[i][j] = g_CONFIGURE[USR_FLASH_DATA_OFFSET_SPNAME_EN + 36*i + j];
}	

void usr_flash_analyze_SPUNIT(void)
{
	UINT16 i,j;

	for(i = 0; i < 513; i++)
		for(j = 0; j < 5; j++)
			g_SPUNIT[i][j] = g_CONFIGURE[USR_FLASH_DATA_OFFSET_SPUNIT + 5*i + j];

}	
		
void usr_flash_analyze_SPDAPW_P(void)
{
	UINT16 i;

	for(i = 0; i < 513; i++)
		g_SPDAPW_P[i] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SPDAPW + i*4 + 2);			
}	
		
void usr_flash_analyze_SPDAPW_F(void)
{
	UINT16 i;

	for(i = 0; i < 513; i++)
		g_SPDAPW_F[i] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SPDAPW + i*4);	
}	
	
		
void usr_flash_analyze_SPMPN(void)
{
	UINT16 i,j;

	for(i = 0; i < 513; i++)
		for(j = 0; j < 8; j++)
			g_SPMPN[i][j] = g_CONFIGURE[USR_FLASH_DATA_OFFSET_SPMPN + 8*i + j];
}	
				

void usr_flash_analyze_SPICTN(void)
{
	UINT16 i,j;

	for(i = 0; i < 30; i++)
		for(j = 0; j < 24; j++)
			g_SPICTN[i][j] = g_CONFIGURE[USR_FLASH_DATA_OFFSET_SPICTN + 24*i + j];
}


void usr_flash_analyze_SPPPIA(void)
{
	UINT16 i,j;
	UINT8 tmp[2];
		
	for(i = 0; i < 30; i++)
		for(j = 0; j < 30; j++)
		{
			tmp[0] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SPPPIA + i*120 + 4*j);	
			tmp[1] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SPPPIA + i*120 + 4*j + 2);	
			g_SPPPIA[i][j] = (tmp[0] << 8) + tmp[1];	
		}
}	
				
void usr_flash_analyze_SECNAME(void)
{
	UINT16 i,j;

	for(i = 0; i < 32; i++)
		for(j = 0; j < 20; j++)
			g_SECNAME[i][j] = g_CONFIGURE[USR_FLASH_DATA_OFFSET_SECNAME + 20*i + j];

}	
	
void usr_flash_analyze_SECSGN(void)
{
	UINT8 tmp[4];

	tmp[0] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SECSGN + 0);	
	tmp[1] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SECSGN + 2);	
	tmp[2] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SECSGN + 4);	
	tmp[3] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SECSGN + 6);	
	g_SECSGN = (tmp[0] << 24) + (tmp[1] << 16) + (tmp[2] << 8) + tmp[3];
}	
				
void usr_flash_analyze_SHNAME(void)
{
	UINT16 i;

	for(i = 0; i < 20; i++)g_SHNAME[i] = g_CONFIGURE[USR_FLASH_DATA_OFFSET_SHNAME + i];
}	
		
void usr_flash_analyze_SEAID(void)
{
	UINT8 tmp[4];

	tmp[0] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SEAID + 0);	
	tmp[1] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SEAID + 2);	
	tmp[2] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SEAID + 4);	
	tmp[3] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SEAID + 6);	
	g_SEAID = (tmp[0] << 24) + (tmp[1] << 16) + (tmp[2] << 8) + tmp[3];
}	

void usr_flash_analyze_SACKRSF(void)
{
	g_SACKRSF = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_SACKRSF);	
}

void usr_flash_analyze_TGBZ(void)
{
	g_TGBZ	= usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_TGBZ);	
}
				
void usr_flash_analyze_MONITOR_IP(void)
{
	UINT16 i;
	UINT8 tmp[4];
		
	for(i = 0; i < 20; i++)
	{
		tmp[0] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MONITOR_IP + 0 + i*8);	
		tmp[1] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MONITOR_IP + 2 + i*8);	
		tmp[2] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MONITOR_IP + 4 + i*8);	
		tmp[3] = usr_flash_analyze_2byte(USR_FLASH_DATA_OFFSET_MONITOR_IP + 6 + i*8);	
		g_MONITOR_IP[i] = (tmp[0] << 24) + (tmp[1] << 16) + (tmp[2] << 8) + tmp[3];
	}
}

void usr_flash_analyze_MONITOR_NAME(void)
{
	UINT16 i,j;

	for(i = 0; i < 10; i++)
		for(j = 0; j < 20; j++)
			g_MONITOR_NAME[i][j] = g_CONFIGURE[USR_FLASH_DATA_OFFSET_MONITOR_NAME + i*10 + j];
}



/************************************************************
* usr_flash_is_default_mac
************************************************************/
bool usr_flash_is_default_mac(void)
{
	Xuint8 bfr[256];
	drv_flash_read_page_256byte(USR_FLASH_DATA_DEFAULT_FLAG, 0, bfr);

	//xil_printf("mac flag: %x \n", bfr[0]);
	if((bfr[0]&0xff) == 0x01)
		return false;
	return true;
}


/************************************************************
* usr_flash_over_default_mac
************************************************************/
STATUS usr_flash_over_default_mac(void)
{
	Xuint8 bfr[256];
	drv_flash_read_page_256byte(USR_FLASH_DATA_DEFAULT_FLAG, 0, bfr);
	bfr[0] = 0x01;
	drv_flash_write_page_256byte(USR_FLASH_DATA_DEFAULT_FLAG, 0, bfr);

	return OK;
}


/************************************************************
* usr_flash_is_default_par
************************************************************/
bool usr_flash_is_default_par(void)
{
	Xuint8 bfr[256];
	drv_flash_read_page_256byte(USR_FLASH_DATA_DEFAULT_FLAG, 0, bfr);

	//xil_printf("ip&port flag: %x \n", bfr[1]);
	if((bfr[1]&0xff) == 0x01)
		return false;
	return true;
}


/************************************************************
* usr_flash_over_default_par
************************************************************/
STATUS usr_flash_over_default_par(void)
{
	Xuint8 bfr[256];
	drv_flash_read_page_256byte(USR_FLASH_DATA_DEFAULT_FLAG, 0, bfr);
	bfr[1] = 0x01;
	drv_flash_write_page_256byte(USR_FLASH_DATA_DEFAULT_FLAG, 0, bfr);

	return OK;
}



/************************************************************
* usr_flash_rst_default
************************************************************/
STATUS usr_flash_rst_default(void)
{
	drv_flash_sector_erase(USR_FLASH_DATA_DEFAULT_FLAG);

	return OK;
}



/************************************************************
* usr_is_default_mac
************************************************************/
bool usr_is_default_mac(void)
{
	return g_default_mac;
}


/************************************************************
* usr_is_default_par
************************************************************/
bool usr_is_default_par(void)
{
	return g_default_par;
}


/************************************************************
* usr_is_default_status
************************************************************/
bool usr_is_default_status(void)
{
	return g_default_status;
}


/************************************************************
* usr_flash_Wr_Rd_debug
************************************************************/
void usr_flash_Wr_Rd_debug(void)
{
	//Xuint8	TestData[256];
	int		i,j;

	//drv_flash_demo_test();

	//drv_flash_sector_erase(USR_FLASH_DATA_SECTOR_START);
	//for(i = 0; i < 256; i++)TestData[i] = i;
	//drv_flash_write_page_256byte(USR_FLASH_DATA_SECTOR_START, 0, TestData);
	//for(i = 0; i < 256; i++)TestData[255 - i] = i;
	//drv_flash_write_page_256byte(USR_FLASH_DATA_SECTOR_START, 1, TestData);

	//xil_printf("flash write over \n");

	//drv_debug_prt(USR_FLASH_DATA_SECTOR_START, 0);
	//drv_debug_prt(USR_FLASH_DATA_SECTOR_START, 1);

	//int i;
	//int len = USR_FLASH_DATA_LEN_BYTE / 256 + 1;
	//
	//for(i = 0; i < len; i++)
	//	drv_debug_prt(USR_FLASH_DATA_SECTOR_START, i);





	////UINT8		g_SIPACP_IP[2][4];		//延伸报警模块IP地址[双网段]
	//xil_printf("IP_1: %d.%d.%d.%d   IP_2: %d.%d.%d.%d\n",
	//			g_SIPACP_IP[0][0],g_SIPACP_IP[0][1],g_SIPACP_IP[0][2],g_SIPACP_IP[0][3],
	//			g_SIPACP_IP[1][0],g_SIPACP_IP[1][1],g_SIPACP_IP[1][2],g_SIPACP_IP[1][3]);

	////UINT32	g_SIPACP_PORT[2];		//延伸报警模块配置端口
	//xil_printf("PORT_1: %d   PORT_2: %d\n", g_SIPACP_PORT[0], g_SIPACP_PORT[1]);

	////UINT8 	g_SMIPAP_IP[2][4]; 		//延伸报警模块组播地址[双网段]
	//xil_printf("M_IP_1: %d.%d.%d.%d   M_IP_2: %d.%d.%d.%d\n",
	//			g_SMIPAP_IP[0][0],g_SMIPAP_IP[0][1],g_SMIPAP_IP[0][2],g_SMIPAP_IP[0][3],
	//			g_SMIPAP_IP[1][0],g_SMIPAP_IP[1][1],g_SMIPAP_IP[1][2],g_SMIPAP_IP[1][3]);

	////UINT32	g_SMIPAP_PORT[2];		//延伸报警模块组播端口
	//xil_printf("M_PORT_1: %d   M_PORT_2: %d\n", g_SMIPAP_PORT[0], g_SMIPAP_PORT[1]);

	////UINT32	g_SSSIGN;				//延伸报警模块所属分组标志
	//xil_printf("FLAG_GROUP: %d\n", g_SSSIGN);


	//UINT8		g_SPDAPW_P[513];		//延伸报警模块参数阶码[513个]
	//UINT8		g_SPDAPW_F[513];		//延伸报警模块参数显示方式[513个]
	for(i = 0; i < 50; i++)
	{
		;//xil_printf("%d: power %x, format: %x\n", i+1, g_SPDAPW_P[i], g_SPDAPW_F[i]);
	}

	////UINT16	g_SPPPIA[30][30];		//延伸报警模块图形及特殊显示页参数索引地址
	//for(i = 0; i < 30; i++)
	//{
	//	xil_printf("\n %d: ", i+1);
	//	for(j = 0; j < 30; j++)
	//		xil_printf("%d ", g_SPPPIA[i][j]);
	//}
	//xil_printf("\n");

	////UINT32	g_SECSGN;				//延伸报警模块轮机员呼叫标志
	//xil_printf("CALLER_FLAG: %d\n", g_SECSGN);

	////UINT32	g_SEAID;				//延伸报警模块ID号
	//xil_printf("MOUDLE_ID: %x\n", g_SEAID);

	////UINT32	g_MONITOR_IP[20];		//标显台IP地址
	//for(i = 0; i < 20; i++)
	//	xil_printf("%d: MONITOR_IP: %x\n", i+1, g_MONITOR_IP[i]);
	//
	////UINT8		g_SACKRSF;				//全网应答全网复位处理标志
	//xil_printf("ASK_FLAG: %d\n", g_SACKRSF);

	////UINT8		g_TGBZ;					//调光标志	
	//xil_printf("LIGHT_FLAG: %d\n", g_TGBZ);



	//xil_printf("g_CONFIGURE:");
	//for(i = 72; i < 1000; i++)xil_printf("%x ", g_CONFIGURE[i]);
	//xil_printf("\n");

	//for(i = 0; i < 40; i++)
	//{
	//	xil_printf("offset: %d\n", i);

	//	//UINT8		g_SPNAME_CH[513][24];	//延伸报警模块参数中文名称[513个]
	//	xil_printf("ch_name: ");
	//	for(j = 0; j < 24; j++)xil_printf("%x ",g_SPNAME_CH[i][j]);
	//	xil_printf("\n");

	//	////UINT8		g_SPNAME_EN[513][36];	//延伸报警模块参数英文名称[513个]
	//	//xil_printf("en_name: ");
	//	//for(j = 0; j  < 36; j++)xil_printf("%x ",g_SPNAME_EN[i][j]);
	//	//xil_printf("\n");

	//	////UINT8		g_SPUNIT[513][5];		//延伸报警模块参数单位[513个]
	//	//xil_printf("uint_name: ");
	//	//for(j = 0; j  < 5; j++)xil_printf("%x ",g_SPUNIT[i][j]);
	//	//xil_printf("\n");

	//	////UINT32	g_SPMPN[513];			//延伸报警模块参数测点号[513个]
	//	//xil_printf("easy_id: ");
	//	//for(j = 0; j  < 8; j++)xil_printf("%x ",g_SPMPN[i][j]);
	//	//xil_printf("\n");
	//}

	

//UINT8		g_SPICTN[30][24];		//延伸报警模块图形显示页标题名称[30]
//UINT8		g_SECNAME[32][20];		//轮机员呼叫成员名单[32个]
//UINT8		g_SHNAME[20];			//延伸报警主机名称
//UINT8 	g_MONITOR_NAME[10][20]; //标显台名称

}


