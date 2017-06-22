#include "usr.h"


/************************************************************
* VAR
************************************************************/
UINT8	g_lcd_contest[LCD_BFR];
int		g_current_page;
int		g_current_list;
int		g_flag_lcd_display_word;

int		G_Alm_num;
int		G_YN_IOin;
int		G_YN_IOout;
int		G_no_IO;

int 	flag_check_led[16];

UINT8	G_bfr_Alm_total_ch[LCD_BFR] = "  目前所有测点没有报警  ";
UINT8	G_bfr_Alm_1_ch[LCD_BFR] = "  目前该组测点没有报警  ";
UINT8	G_bfr_Alm_2_ch[LCD_BFR] = "  目前该组测点没有报警  ";
UINT8	G_bfr_Alm_3_ch[LCD_BFR] = "  目前该组测点没有报警  ";
UINT8	G_bfr_Alm_4_ch[LCD_BFR] = "  目前该组测点没有报警  ";
UINT8	G_bfr_Alm_5_ch[LCD_BFR] = "  目前该组测点没有报警  ";
UINT8	G_bfr_Alm_6_ch[LCD_BFR] = "  目前该组测点没有报警  ";
UINT8	G_bfr_Alm_7_ch[LCD_BFR] = "  目前该组测点没有报警  ";
UINT8	G_bfr_Alm_8_ch[LCD_BFR] = "  目前该组测点没有报警  ";

UINT8	G_bfr_Alm_total_en[LCD_BFR] = "    NO ANY ALARM NOW    ";
UINT8	G_bfr_Alm_1_en[LCD_BFR] = "NO ALM OF THIS GROUP NOW";
UINT8	G_bfr_Alm_2_en[LCD_BFR] = "NO ALM OF THIS GROUP NOW";
UINT8	G_bfr_Alm_3_en[LCD_BFR] = "NO ALM OF THIS GROUP NOW";
UINT8	G_bfr_Alm_4_en[LCD_BFR] = "NO ALM OF THIS GROUP NOW";
UINT8	G_bfr_Alm_5_en[LCD_BFR] = "NO ALM OF THIS GROUP NOW";
UINT8	G_bfr_Alm_6_en[LCD_BFR] = "NO ALM OF THIS GROUP NOW";
UINT8	G_bfr_Alm_7_en[LCD_BFR] = "NO ALM OF THIS GROUP NOW";
UINT8	G_bfr_Alm_8_en[LCD_BFR] = "NO ALM OF THIS GROUP NOW";

//UINT8	g_lcd_scren_context[LCD_BFR]    = "一二三四五六七八九十一二一二三四五六七八九十一二一二三四五六七八九十一二一二三四五六七八九十一二";
UINT8	g_lcd_scren_context[LCD_BFR]    = "延伸报警模块初始页面测试上海七一一研究所自动化部闵行区华宁路第三一一一号一二三四五六七八九十一二";
UINT8	g_lcd_debug_context[LCD_BFR]    = " Welcome To My Machine   Are You Ready To Scream------------------------                        ";
UINT8	g_lcd_start_context_en[LCD_BFR] = "  Your requirement is     our aspire and honour ------------------------              711_SMDERI";
UINT8	g_lcd_start_context_ch[LCD_BFR] = "    全力满足您的需求      会是我们的渴望与荣耀  ------------------------            七一一研究所";

UINT8	g_lcd_config_ing_context_en[LCD_BFR]		= "========================  Now you are configure  The process is not end ========================";
UINT8	g_lcd_config_ing_context_ch[LCD_BFR]		= "========================  您现在正进行参数配置    全配置过程还没有结束  ========================";
UINT8	g_lcd_config_success_context_en[LCD_BFR]	= "======================== Your configure success  Please restart the dev ========================";
UINT8	g_lcd_config_success_context_ch[LCD_BFR]	= "========================  您的配置已经成功下载    请您断电后重启该模块  ========================";

UINT8	g_lcd_start_info_err_til_ch[LCD_LINE_LENTH] = "延伸报警模块处于初始状态";
UINT8	g_lcd_start_info_err_mac_ch[LCD_LINE_LENTH] = "MAC 地址未重新下载配置  ";
UINT8	g_lcd_start_info_err_net_ch[LCD_LINE_LENTH] = "IP  地址未重新下载配置  ";
UINT8	g_lcd_start_info_err_par_ch[LCD_LINE_LENTH] = "各类参数未重新下载配置  ";

UINT8	g_lcd_start_info_err_til_en[LCD_LINE_LENTH] = "INITIALIZE STATUS NOW   ";
UINT8	g_lcd_start_info_err_mac_en[LCD_LINE_LENTH] = "MAC ADDRESS NEED SET    ";
UINT8	g_lcd_start_info_err_net_en[LCD_LINE_LENTH] = "IPv4 ADDRESS NEED SET   ";
UINT8	g_lcd_start_info_err_par_en[LCD_LINE_LENTH] = "VARIOUS PARS NEED SET   ";


/************************************************************
* FUNC
************************************************************/
UINT8 	lcd_db_read		(void);
void 	lcd_db_write	(UINT32 *regData, UINT8 content);
void 	lcd_rs_data		(UINT32 *regData);
void 	lcd_rs_cmd		(UINT32 *regData); 
void 	lcd_rw_read		(UINT32 *regData); 
void 	lcd_rw_write	(UINT32 *regData); 
void 	lcd_enable		(UINT32 *regData, int i); 
void 	lcd_disable		(UINT32 *regData, int i); 
UINT8	lcd_busy		(void); 

void 	lcd_init 		(UINT32 *regData);
void 	lcd_clr_ram 	(UINT32 *regData);
void 	lcd_clr_cgram 	(UINT32 *regData);
void 	lcd_wr 			(UINT32 *regData, int i, UINT8 dat_comm,UINT8 content);
void 	lcd_chk_busy 	(UINT32 *regData, int i);
void 	lcd_delay 		(UINT8 us);

void 	lcd_chn_disp_line 			(UINT32 *regData, UINT8 *bfr, UINT8 line);
void 	lcd_chn_disp_all 			(UINT32 *regData, UINT8 *bfr);
void 	lcd_line_backlight_on 		(UINT32 *regData, UINT8 line);
void 	lcd_line_backlight_off 		(UINT32 *regData, UINT8 line);
void 	lcd_move_to_left_once		(UINT32 *regData, int i);
void 	lcd_move_to_right_once		(UINT32 *regData, int i);
void 	lcd_point_to_left_once		(UINT32 *regData, int i);
void 	lcd_point_to_right_once		(UINT32 *regData, int i);
void 	lcd_point_off				(UINT32 *regData, int i);
void 	lcd_sleep 					(UINT32 *regData);
void 	lcd_wakeup 					(UINT32 *regData);
void 	lcd_on 						(UINT32 *regData);
void	lcd_off 					(UINT32 *regData);
void 	lcd_lat_disp 				(UINT32 *regData, int i, UINT8 data);
void 	lcd_con_disp 				(UINT32 *regData, int i, UINT8 data1,UINT8 data2,UINT8 x0,UINT8 y0,UINT8 xl,UINT8 yl);


void	drv_lcd_init(void);
void	usr_lcd_init(void);

void	usr_range_add(UINT16 *range_tag, UINT16 range_min, UINT16 range_max);
void	usr_range_sub(UINT16 *range_tag, UINT16 range_min, UINT16 range_max);
void	usr_lcd_int_cov(UINT8 *lcd_bfr, int data, int data_digit);
void	usr_lcd_float_cov(UINT8 *lcd_bfr, UINT16 data, UINT8 data_power);
void	usr_lcd_alm_level_cov(UINT8 *lcd_bfr, int alm_level);
void	usr_lcd_clr_line(int line);
void	usr_lcd_clr_screen(void);
void	usr_lcd_debug(void);
void	usr_lcd_page_info_debug_print(void);
bool	usr_lcd_is_display_ch(void);
bool	usr_lcd_is_display_en(void);
void	usr_lcd_set_display_lang(int language_type);

void	usr_lcd_page_display(void);
void	usr_lcd_page_list_info(ALM_BLOCK* p, int line);
void	usr_lcd_page_detail_info(ALM_BLOCK* p);
void 	usr_lcd_page_info(void);
void	usr_lcd_page_start(void);
void	usr_lcd_page_config_ing(void);
void	usr_lcd_page_config_success(void);
void	usr_lcd_page_tag_lists(void);
void	usr_lcd_page_tag_details(void);
void	usr_lcd_page_sleep(void);
void	usr_lcd_page_uint_fail(void);
void	usr_lcd_page_list_total_alm(void);
void	usr_lcd_page_list_1_alm(void);
void	usr_lcd_page_list_2_alm(void);
void	usr_lcd_page_list_3_alm(void);
void	usr_lcd_page_list_4_alm(void);
void	usr_lcd_page_list_5_alm(void);
void	usr_lcd_page_list_6_alm(void);
void	usr_lcd_page_list_7_alm(void);
void	usr_lcd_page_list_8_alm(void);
void	usr_lcd_page_cfgcheck(void);

void	usr_lcd_page_list_total_null(int ID);

void	usr_lcd_page_operate(void);
void	usr_lcd_page_machine_start_operate(void);
void	usr_lcd_page_tag_lists_operate(void);
void	usr_lcd_page_tag_details_operate(void);
void	usr_lcd_page_sleep_operate(void);
void	usr_lcd_page_uint_fail_operate(void);
void	usr_lcd_page_list_total_operate(void);
void	usr_lcd_page_list_1_operate(void);
void	usr_lcd_page_list_2_operate(void);
void	usr_lcd_page_list_3_operate(void);
void	usr_lcd_page_list_4_operate(void);
void	usr_lcd_page_list_5_operate(void);
void	usr_lcd_page_list_6_operate(void);
void	usr_lcd_page_list_7_operate(void);
void	usr_lcd_page_list_8_operate(void);
void	usr_lcd_page_cfgcheck_operate(void);

void	usr_pannel_lcd_deal_with(void);
void	usr_pannel_lcd_output(void);


/************************************************************
************************************************************/
UINT8 lcd_db_read(void)
	{
	UINT32 data;
	XU_GPIO_Out32(GPIO_0_CHA_TRI_ADDR,0xffff);
	data = (UINT32)(XU_GPIO_In32(GPIO_0_IN_ADDR));
	XU_GPIO_Out32(GPIO_0_CHA_TRI_ADDR,0x0000);
	return ((UINT8)(data & LCD_DB));
	}

void lcd_db_write(UINT32 *regData, UINT8 content)
	{
	(*regData) &= 0xffffff00;
	(*regData) |= content;
	XU_GPIO_Out32(GPIO_0_OUT_ADDR,(*regData));
	}

void lcd_rs_data(UINT32 *regData) 
	{
  	(*regData) |= LCD_RS_DATA;
	XU_GPIO_Out32(GPIO_0_OUT_ADDR,(*regData));
	}
	
void lcd_rs_cmd(UINT32 *regData) 
	{
	(*regData) &= LCD_RS_COMMAND;
	XU_GPIO_Out32(GPIO_0_OUT_ADDR,(*regData));
	}

void lcd_rw_read(UINT32 *regData) 
	{
	(*regData) |= LCD_RW_READ;
	XU_GPIO_Out32(GPIO_0_OUT_ADDR,(*regData));
	}
	
void lcd_rw_write(UINT32 *regData) 
	{
	(*regData) &= LCD_RW_WRITE;
	XU_GPIO_Out32(GPIO_0_OUT_ADDR,(*regData));
	}

void lcd_enable(UINT32 *regData, int i) 
	{
	if(i == 1)
		(*regData) |= LCD_ENABLE_1;
	else if(i == 2)
		(*regData) |= LCD_ENABLE_2;
	else 
		return;
	
	XU_GPIO_Out32(GPIO_0_OUT_ADDR,(*regData));
	}

void lcd_disable(UINT32 *regData, int i) 
	{
	if(i == 1)
		(*regData) &= LCD_DISABLE_1;
	else if(i == 2)
		(*regData) &= LCD_DISABLE_2;
	else 
		return;
		
	XU_GPIO_Out32(GPIO_0_OUT_ADDR,(*regData));
	}
	
UINT8 lcd_busy(void) 
	{
	return (lcd_db_read() & LCD_BUSY);
	}

void lcd_rst(UINT32 *regData) 
	{
	int i;
	(*regData) &= LCD_RST_L;
	XU_GPIO_Out32(GPIO_0_OUT_ADDR,(*regData));
	//sleep(1);
	for(i=0;i<5000;i++)i=i;
	(*regData) |= LCD_RST_H;
	XU_GPIO_Out32(GPIO_0_OUT_ADDR,(*regData));
	}
	
	
void lcd_init (UINT32 *regData)
{
	lcd_wr (regData, 1, LCD_COMM, 0x34);
	lcd_wr (regData, 1, LCD_COMM, 0x34);
	lcd_wr (regData, 1, LCD_COMM, 0x30);
	lcd_wr (regData, 1, LCD_COMM, 0x30);
	
	lcd_wr (regData, 2, LCD_COMM, 0x34);
	lcd_wr (regData, 2, LCD_COMM, 0x34);
	lcd_wr (regData, 2, LCD_COMM, 0x30);
	lcd_wr (regData, 2, LCD_COMM, 0x30);
		
	
	
  lcd_wr (regData, 1, LCD_COMM,0x30);  
  lcd_wr (regData, 1, LCD_COMM,0x01);  
  lcd_wr (regData, 1, LCD_COMM,0x06); 
  lcd_wr (regData, 1, LCD_COMM,0x0c);  

  
  lcd_wr (regData, 2, LCD_COMM,0x30);  
  lcd_wr (regData, 2, LCD_COMM,0x01);  
  lcd_wr (regData, 2, LCD_COMM,0x06); 
  lcd_wr (regData, 2, LCD_COMM,0x0c);  
  
 
  lcd_clr_ram(regData);
  lcd_clr_cgram(regData);
}


void lcd_chn_disp_line (UINT32 *regData, UINT8 *bfr, UINT8 line)
{
  int i;

  switch(line)
  {
	case 1:
		lcd_wr (regData, 1, LCD_COMM, 0x30);
	lcd_wr (regData, 1, LCD_COMM, 0x80);
		for (i=0;i<24;i++)
			lcd_wr (regData, 1, LCD_DATA, bfr[i]);
			


	break;
	
	case 2:
		lcd_wr (regData, 1, LCD_COMM, 0x30);
	lcd_wr (regData, 1, LCD_COMM, 0x90);
	for (i=0;i<24;i++)
			lcd_wr (regData, 1, LCD_DATA, bfr[i]);

	
	break;
	
	case 3:
		lcd_wr (regData, 2, LCD_COMM, 0x30);
	lcd_wr (regData, 2, LCD_COMM, 0x80);
	for (i=0;i<24;i++)
			lcd_wr (regData, 2, LCD_DATA, bfr[i]);

	
	break;
	
	case 4:
		lcd_wr (regData, 2, LCD_COMM, 0x30);
	lcd_wr (regData, 2, LCD_COMM, 0x90);
	for (i=0;i<24;i++)
			lcd_wr (regData, 2, LCD_DATA, bfr[i]);

	
	break;
	
	default:
	return;
	}
	
	
}

void lcd_chn_disp_all (UINT32 *regData, UINT8 *bfr)
{
	//lcd_clr_ram(regData);
	//lcd_clr_cgram(regData);
  
	lcd_chn_disp_line(regData, bfr, 	1);
	lcd_chn_disp_line(regData, bfr+24, 	2);
	lcd_chn_disp_line(regData, bfr+48, 	3);
	lcd_chn_disp_line(regData, bfr+72, 	4);
}


void lcd_clr_ram (UINT32 *regData)
{
  UINT16 i;
  
  for(i=0; i<10 ;i++)
  {
  lcd_wr (regData, 1, LCD_COMM, 0x30);
  lcd_wr (regData, 1, LCD_COMM, 0x01);
  
  
  lcd_wr (regData, 2, LCD_COMM, 0x30);
  lcd_wr (regData, 2, LCD_COMM, 0x01);
  
  }
}


void lcd_clr_cgram (UINT32 *regData)
{
  lcd_lat_disp(regData, 1, 0x00);
  
  lcd_lat_disp(regData, 2, 0x00);
  
}


void lcd_move_to_left_once(UINT32 *regData, int i)
{
  	lcd_wr (regData, i, LCD_COMM, 0x30);
	lcd_wr (regData, i, LCD_COMM, 0x18);
}

void lcd_move_to_right_once(UINT32 *regData, int i)
{
  	lcd_wr (regData, i, LCD_COMM, 0x30);
	lcd_wr (regData, i, LCD_COMM, 0x1c);
}


void lcd_point_to_left_once(UINT32 *regData, int i)
{
  	lcd_wr (regData, i, LCD_COMM, 0x30);
	lcd_wr (regData, i, LCD_COMM, 0x0f); 
	lcd_wr (regData, i, LCD_COMM, 0x10);
}

void lcd_point_to_right_once(UINT32 *regData, int i)
{
  	lcd_wr (regData, i, LCD_COMM, 0x30);
   lcd_wr (regData, i, LCD_COMM, 0x0f); 
	lcd_wr (regData, i, LCD_COMM, 0x14);
}


void 	lcd_point_off(UINT32 *regData, int i)
{
  	lcd_wr (regData, i, LCD_COMM, 0x30);
   lcd_wr (regData, i, LCD_COMM, 0x0c); 
}


void lcd_wr (UINT32 *regData, int i, UINT8 dat_comm,UINT8 content)
{
	int j;
  lcd_chk_busy (regData, i);
  if(dat_comm)
   {
    lcd_rs_data(regData);   //data
	// sleep(1);
	for(j=0;j<100;j++);
    lcd_rw_write(regData);   //write
   }
  else 
   {
    lcd_rs_cmd(regData);   //command
	 //sleep(1);
	 for(j=0;j<100;j++);
    lcd_rw_write(regData);   //write
   }
  lcd_db_write	(regData, content);
  lcd_enable(regData, i);
  //for(j=0;j<5000;j++)
  	//j=j;
  lcd_disable(regData, i);
}

void lcd_chk_busy (UINT32 *regData, int i)
{
  lcd_db_write	(regData, 0xff);
  lcd_rs_cmd(regData);
  lcd_rw_read(regData);
  lcd_enable(regData, i);
  while(lcd_busy());
  lcd_disable(regData, i);
}

void lcd_delay (UINT8 us)   //delay time
{
  while(us--);
}


void lcd_line_backlight_on (UINT32 *regData, UINT8 line)
{
		if( (line >= 1) && (line <= 2) )
		{
			lcd_wr (regData, 1, LCD_COMM, 0x34);
			lcd_wr (regData, 1, LCD_COMM, (0x04 + line - 1));
		}
		else if( (line >= 3) && (line <= 4) )
		{
			lcd_wr (regData, 2, LCD_COMM, 0x34);
			lcd_wr (regData, 2, LCD_COMM, (0x04 + line - 3));
		}
		else
			;
			
}

void lcd_line_backlight_off (UINT32 *regData, UINT8 line)
{
		lcd_line_backlight_on(regData, line);
}


void lcd_sleep (UINT32 *regData)
{
		lcd_wr (regData, 1, LCD_COMM, 0x34);
		lcd_wr (regData, 1, LCD_COMM, 0x08);
		
		lcd_wr (regData, 2, LCD_COMM, 0x34);
		lcd_wr (regData, 2, LCD_COMM, 0x08);
}

void lcd_wakeup (UINT32 *regData)
{
		lcd_wr (regData, 1, LCD_COMM, 0x34);
		lcd_wr (regData, 1, LCD_COMM, 0x0c);
		
		lcd_wr (regData, 2, LCD_COMM, 0x34);
		lcd_wr (regData, 2, LCD_COMM, 0x0c);
}


void lcd_on (UINT32 *regData)
{
		lcd_init(regData);
}

void lcd_off (UINT32 *regData)
{
		lcd_wr (regData, 1, LCD_COMM, 0x30);
		lcd_wr (regData, 1, LCD_COMM, 0x08);
		lcd_wr (regData, 2, LCD_COMM, 0x30);
		lcd_wr (regData, 2, LCD_COMM, 0x08);
}



void lcd_lat_disp (UINT32 *regData, int i, UINT8 data)
{
  UINT8 l,j;

/* X(0x80 - 0x8a), Y(0x80-0xa0)*/
  for(j=0;j<32;j++)
  {
		for(l=0;l<16;l++)
      {
        lcd_wr (regData, i, LCD_COMM, 0x34);
        lcd_wr (regData, i, LCD_COMM, 0x80+j);
        lcd_wr (regData, i, LCD_COMM, 0x80+l);
        lcd_wr (regData, i, LCD_COMM, 0x30);
        lcd_wr (regData, i, LCD_DATA, data);
		lcd_wr (regData, i, LCD_DATA, data);
      }
  }
  lcd_wr (regData, i, LCD_COMM, 0x36);
}



void lcd_con_disp (UINT32 *regData, int i, UINT8 data1,UINT8 data2,UINT8 x0,UINT8 y0,UINT8 xl,UINT8 yl)
{
  UINT8 l,j;
  for(j=0;j<yl;j++)
  {
    for(l=0;l<xl;l++)
    {
      lcd_wr (regData, i, LCD_COMM, 0x34);
      lcd_wr (regData, i, LCD_COMM, y0+j);
      lcd_wr (regData, i, LCD_COMM, x0+l);
      lcd_wr (regData, i, LCD_COMM, 0x30);
      lcd_wr (regData, i, LCD_DATA, data1);
      lcd_wr (regData, i, LCD_DATA, data2);
    }
  }
  lcd_wr (regData, i, LCD_COMM, 0x36);
}



void write_ram (UINT32 *regData, int i, UINT8 *bfr, UINT8 adr, UINT16 num, UINT8 fill)
{
  int j;

  lcd_wr (regData, i, LCD_COMM, 0x34);
  lcd_wr (regData, i, LCD_COMM, 0x02);
  lcd_wr (regData, i, LCD_COMM, 0x30);
  lcd_wr (regData, i, LCD_COMM, adr);
  	
	for (j=0;j<num;j++)
			lcd_wr (regData, i, LCD_DATA, bfr[j]);
			
	if(fill)
	{
	for (j=num;j<32;i++)
			lcd_wr (regData, i, LCD_DATA, 0x20);
	}
}

void set_Y_move (UINT32 *regData, int i, UINT8 adr)
{
  lcd_wr (regData, i, LCD_COMM, 0x34);
  lcd_wr (regData, i, LCD_COMM, 0x03);
  lcd_wr (regData, i, LCD_COMM, adr);
}


/************************************************************
* drv_lcd_init
************************************************************/
void drv_lcd_init(void)
{
	int i;
	usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_BUZZ_ID); // BUZZ down
	usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_LCD_ID_LIGHT);	// LCD_BACK_LIGHT	
	
	if(G_YN_IOout == 1)
		usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_LED_ID_RELAY);
	else if(G_YN_IOout == 2)
		usr_gpio_clr_output(USR_GPIO_OUT_CH_1, USR_LED_ID_RELAY);
	else
		;
	drv_gpio_output(USR_GPIO_OUT_CH_1);
	
	g_GPIO_OUT[USR_GPIO_OUT_CH_2] |= LCD_RST_H;
	lcd_init(&g_GPIO_OUT[USR_GPIO_OUT_CH_2]);
	
	
	for(i=0;i<100;i++)
		lcd_chn_disp_all(&g_GPIO_OUT[USR_GPIO_OUT_CH_2], g_lcd_scren_context);

	lcd_init(&g_GPIO_OUT[USR_GPIO_OUT_CH_2]);
	lcd_init(&g_GPIO_OUT[USR_GPIO_OUT_CH_2]);
	
}


/************************************************************
* usr_lcd_init
************************************************************/
void usr_lcd_init(void)
{
	g_current_page			= USR_LCD_PAGE_MACHINE_START;
	g_current_list			= USR_LCD_PAGE_LIST_ALM_TOTAL;
	g_flag_lcd_display_word	= USR_LCD_DISPLAY_WORD_DEFAULT;
}


/************************************************************
* usr_range_add
************************************************************/
void usr_range_add(UINT16 *range_tag, UINT16 range_min, UINT16 range_max)
{
	if(*range_tag == range_max)
		*range_tag = range_min;
	else
		*range_tag = *range_tag + 1;
}


/************************************************************
* usr_range_sub
************************************************************/
void usr_range_sub(UINT16 *range_tag, UINT16 range_min, UINT16 range_max)
{
	if(*range_tag == range_min)
		*range_tag = range_max;
	else
		*range_tag = *range_tag - 1;
}


/************************************************************
* usr_lcd_int_cov
************************************************************/
void usr_lcd_int_cov(UINT8 *lcd_bfr, int data, int data_digit)
{
	UINT8 tmp;
	while(data_digit)
	{
		tmp = data % 10;
		lcd_bfr[data_digit - 1] = 0x30 + tmp;
		data /= 10;
		data_digit--;
	}
}


/************************************************************
* usr_lcd_float_cov
************************************************************/
void usr_lcd_float_cov(UINT8 *lcd_bfr, UINT16 data, UINT8 data_power)
{
	UINT16	value_org;
	int		pre, pre_org;
	UINT8	tmp[5];
	int		len;
	int		i, cnt, j;
	UINT8	point = 0x2e; // .
	
	if(data_power > 0x80)
		pre = data_power - 0x80;
	else
	{
		for(i = 0; i < data_power; i++)
			data = data*10;
		pre = 0;
	}

	value_org	= data;
	pre_org		= pre;

	for(i = 0; i < 5; i++)
	{
		tmp[i] = data%10;
		data /= 10;
	}
	
	if(pre == 0)
	{
		cnt = 4;
		while(cnt)
		{
			if(tmp[cnt] != 0)
				break;
			cnt--;
		}
		for(i = 0; i <= cnt; i++)lcd_bfr[4 - i] = 0x30 + tmp[i];
	}
	else
	{
		cnt = 4;
		while(cnt)
		{
			if( (tmp[cnt] == 0) && (tmp[cnt - 1] != 0) )
				break;
			cnt--;
		}
		
		if(pre > cnt)
		{
			len = pre - cnt;// fill 0
			for(i = 0; i < cnt; i++)lcd_bfr[5 - i] = 0x30 + tmp[i];
			for(j = 0; j < len; j++,i++)lcd_bfr[5 - i] = 0x30;
			lcd_bfr[5 - i] = point;
			lcd_bfr[4 - i] = 0x30;
		}
		else if(pre == cnt)
		{
			for(i = 0; i < cnt; i++)lcd_bfr[5 - i] = 0x30 + tmp[i];
			lcd_bfr[5 - i] = point;
			lcd_bfr[4 - i] = 0x30;
		}
		else
		{
			for(i = 0; i < pre; i++)
			lcd_bfr[5 - i] = 0x30 + tmp[i];

			lcd_bfr[5 - pre] = point;

			for(j = 0; j < (cnt - pre); j++,i++)lcd_bfr[4 - i] = 0x30 + tmp[i];
		}
	}
}


/************************************************************
* usr_lcd_alm_level_cov
************************************************************/
void usr_lcd_alm_level_cov(UINT8 *lcd_bfr, int alm_level)
{
	int		i,j;
	UINT8	bf_j_ch[] = "级";
	UINT8	bf_d_ch[] = "断线";
	UINT8	bf_j_en[] = "lv";
	UINT8	bf_d_en[] = " brk";
	
	if(usr_lcd_is_display_ch())
	{
		switch(alm_level)
		{
		case 1:
		
			lcd_bfr[0] = 0x31;
			for(i = 0; i < 2; i++)lcd_bfr[i+2] = bf_j_ch[i];
			break;
			
		case 2:
		
			lcd_bfr[0] = 0x32;
			for(i = 0; i < 2; i++)lcd_bfr[i+2] = bf_j_ch[i];
			break;
			
		case 3:
		
			lcd_bfr[0] = 0x33;
			for(i = 0; i < 2; i++)lcd_bfr[i+2] = bf_j_ch[i];
			break;
			
		case 4:
		
			for(i = 0; i < 4; i++)lcd_bfr[i] = bf_d_ch[i];
			break;
			
		default:
			;
		}
	}
	else if(usr_lcd_is_display_en())
	{
		switch(alm_level)
		{
		case 1:
		
			lcd_bfr[0] = 0x31;
			for(i = 0; i < 2; i++)lcd_bfr[i+2] = bf_j_en[i];
			break;
			
		case 2:
		
			lcd_bfr[0] = 0x32;
			for(i = 0; i < 2; i++)lcd_bfr[i+2] = bf_j_en[i];
			break;
			
		case 3:
		
			lcd_bfr[0] = 0x33;
			for(i = 0; i < 2; i++)lcd_bfr[i+2] = bf_j_en[i];
			break;
			
		case 4:
		
			for(i = 0; i < 4; i++)lcd_bfr[i] = bf_d_en[i];
			break;
			
		default:
			;
		}
	}
	else
		;
		 
}


/************************************************************
* usr_lcd_clr_line
************************************************************/
void usr_lcd_clr_line(int line)
{
	int		i;
	UINT8	*bfr;
	bfr  = g_lcd_contest + (line * LCD_LINE_LENTH); 
	for(i = 0; i < LCD_LINE_LENTH; i++)bfr[i] = 0x20;
}


/************************************************************
* usr_lcd_clr_screen
************************************************************/
void usr_lcd_clr_screen(void)
{
	int i;
	for(i = 0; i < LCD_BFR; i++)g_lcd_contest[i] = 0x20;
}


/************************************************************
* usr_lcd_debug
************************************************************/
void usr_lcd_debug(void)
{
	int		i;
	UINT8	*bfr;

	usr_lcd_clr_screen();
	bfr = g_lcd_contest; 
	for(i = 0; i < LCD_BFR; i++)bfr[i] = g_lcd_debug_context[i];
	
	bfr = g_lcd_contest + (3 * LCD_LINE_LENTH); 

	usr_lcd_int_cov(bfr+0,	usr_get_now_year(),		4);
	usr_lcd_int_cov(bfr+5,	usr_get_now_month(),	2);
	usr_lcd_int_cov(bfr+8,	usr_get_now_date(),		2);
	bfr[4] = '-';
	bfr[7] = '-';

	usr_lcd_int_cov(bfr+11,	usr_get_now_hour(),		2);
	usr_lcd_int_cov(bfr+14,	usr_get_now_minute(),	2);
	usr_lcd_int_cov(bfr+17,	usr_get_now_second(),	2);
	bfr[13] = ':';
	bfr[16] = ':';
}


/************************************************************
* usr_lcd_page_info_debug_print
************************************************************/
void usr_lcd_page_info_debug_print(void)
{
	switch(g_current_page)
	{
	case USR_LCD_PAGE_MACHINE_START:
		//xil_printf("\n USR_LCD_PAGE_MACHINE_START  ");
		break;

	case USR_LCD_PAGE_LIST_ALM:
		//xil_printf("\n USR_LCD_PAGE_LIST_ALM  ");
		break;

	case USR_LCD_PAGE_TAG_DETAIL:
		//xil_printf("\n USR_LCD_PAGE_TAG_DETAIL  ");
		break;

	case USR_LCD_PAGE_SLEEP:
		//xil_printf("\n USR_LCD_PAGE_SLEEP  ");
		break;

	case USR_LCD_PAGE_CONFIG_ING:
		//xil_printf("\n USR_LCD_PAGE_CONFIG_ING  ");
		break;

	case USR_LCD_PAGE_CONFIG_SUCCESS:
		//xil_printf("\n USR_LCD_PAGE_CONFIG_SUCCESS  ");
		break;

	default:
		;
	}

	switch(g_current_list)
	{
	case USR_LCD_PAGE_LIST_ALM_TOTAL:
		//xil_printf("USR_LCD_PAGE_LIST_ALM_TOTAL \n");
		break;

	case USR_LCD_PAGE_LIST_ALM_1:
		//xil_printf("USR_LCD_PAGE_LIST_ALM_1 \n");
		break;

	case USR_LCD_PAGE_LIST_ALM_2:
		//xil_printf("USR_LCD_PAGE_LIST_ALM_2 \n");
		break;

	case USR_LCD_PAGE_LIST_ALM_3:
		//xil_printf("USR_LCD_PAGE_LIST_ALM_3 \n");
		break;

	case USR_LCD_PAGE_LIST_ALM_4:
		//xil_printf("USR_LCD_PAGE_LIST_ALM_4 \n");
		break;

	case USR_LCD_PAGE_LIST_ALM_5:
		//xil_printf("USR_LCD_PAGE_LIST_ALM_5 \n");
		break;

	default:
		;
	}
}


/************************************************************
* usr_lcd_is_display_ch
************************************************************/
bool usr_lcd_is_display_ch(void)
{
	return (g_flag_lcd_display_word == USR_LCD_DISPLAY_WORD_CH);
}


/************************************************************
* usr_lcd_is_display_en
************************************************************/
bool usr_lcd_is_display_en(void)
{
	return (g_flag_lcd_display_word == USR_LCD_DISPLAY_WORD_EN);
}


/************************************************************
* usr_lcd_set_display_lang
************************************************************/
void usr_lcd_set_display_lang(int language_type)
{
	g_flag_lcd_display_word = language_type;
}


/************************************************************
* usr_lcd_page_display
************************************************************/
void usr_lcd_page_display(void)
{
	if(usr_led_get_testlight())
	{
		usr_lcd_page_info();
	}
	else
	{
		switch(g_current_page)
		{
		case USR_LCD_PAGE_MACHINE_START:
	
			usr_lcd_page_start();
			break;
	
		case USR_LCD_PAGE_LIST_ALM:
	
			usr_lcd_page_tag_lists();
			break;
	
		case USR_LCD_PAGE_TAG_DETAIL:
	
			usr_lcd_page_tag_details();
			break;
	
		case USR_LCD_PAGE_SLEEP:
	
			usr_lcd_page_sleep();
			break;
	
		case USR_LCD_PAGE_UINT_FAIL:
	
			usr_lcd_page_uint_fail();
			break;
	
		case USR_LCD_PAGE_CONFIG_ING:
	
			usr_lcd_page_config_ing();
			break;
	
		case USR_LCD_PAGE_CONFIG_SUCCESS:
	
			usr_lcd_page_config_success();
			break;
			
		case USR_LCD_PAGE_CFG_CHECK:
			
			usr_lcd_page_cfgcheck();
			break;
	
		default:
			;
		}
	}
}

/************************************************************
* usr_lcd_page_list_info
************************************************************/
void usr_lcd_page_list_info(ALM_BLOCK* p, int line)
{
	int		i;
	UINT8	*bfr;
	UINT8	bf_r_ch[] = "日";
	UINT8	bf_r_en[] = ".D";
	//UINT8	bf_j[] = "级";
	UINT8	mh = 0x3a; //:

	usr_lcd_clr_line(line);

	if(p == (ALM_BLOCK*)NULL)
		return;
	
	bfr = g_lcd_contest + (line * LCD_LINE_LENTH); 

	usr_lcd_int_cov(bfr,	p->alm_happen_time.date,	2);
	usr_lcd_int_cov(bfr+5,	p->alm_happen_time.hrs,		2);
	usr_lcd_int_cov(bfr+8,	p->alm_happen_time.mins,	2);
	usr_lcd_int_cov(bfr+11, p->alm_happen_time.secs,	2);
	//usr_lcd_int_cov(bfr+14, p->pSource->tag_easy_id,	4);
	bfr[14] = p->pSource->tag_easy_id[0];
	bfr[15] = p->pSource->tag_easy_id[1];
	bfr[16] = p->pSource->tag_easy_id[2];
	bfr[17] = p->pSource->tag_easy_id[3];
	usr_lcd_alm_level_cov(bfr+20, p->alm_level);
	
	if(usr_lcd_is_display_ch())
	{
		bfr[2]	= bf_r_ch[0];
		bfr[3]	= bf_r_ch[1];
	}
	else if(usr_lcd_is_display_en())
	{
		bfr[2]	= bf_r_en[0];
		bfr[3]	= bf_r_en[1];
	}
	else
		;

	bfr[7]	= mh;
	bfr[10] = mh;
}

UINT8 changePower(UINT8 power)
{
	if(power == 0x00)
		return 0x00;
	else if(power > 0x80)
		return (power - 0x80);
	else
		return (power + 0x80);
}


/************************************************************
* usr_lcd_page_detail_info
************************************************************/
void usr_lcd_page_detail_info(ALM_BLOCK* p)
{
	int			i,j;
	UINT8		*bfr_1;
	UINT8		*bfr_2;
	UINT8		*bfr_3;
	UINT8		bf_b_ch[] = "报警限值";
	UINT8		bf_d_ch[] = "当前状态";
	UINT8		bf_b_en[] = "AlmLimit";
	UINT8		bf_d_en[] = "NowValue";
	UINT8		mh = 0x3a; //:
	UINT8		flag = 0;
	UINT8		pow;

	usr_lcd_clr_line(1);
	usr_lcd_clr_line(2);
	usr_lcd_clr_line(3);

	if(p == (ALM_BLOCK *)NULL)
		return;

	bfr_1 = g_lcd_contest + (1 * LCD_LINE_LENTH); 
	bfr_2 = g_lcd_contest + (2 * LCD_LINE_LENTH); 
	bfr_3 = g_lcd_contest + (3 * LCD_LINE_LENTH); 

	if(usr_lcd_is_display_ch())
	{
		//for(i = 0; i < LCD_LINE_LENTH; i++)bfr_1[i] = p->pSource->tag_name_ch[i];
		
		for(i = 0, j = 0; i < LCD_LINE_LENTH; j++)
		{
			if(i%2 == 0)
			{
				bfr_1[i] = p->pSource->tag_name_ch[j];
				if(bfr_1[i] < 0xA0)
					flag = 1;
				else
					flag = 0;
				i++;
			}
			else
			{
				if((flag == 1)&&(p->pSource->tag_name_ch[j] >= 0xA0))
					i++;
				bfr_1[i] = p->pSource->tag_name_ch[j];
				flag = 0;
				i++;
			}
		}
		
		/*
		xil_printf("%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x \n",
							bfr_1[0], bfr_1[1], bfr_1[2], bfr_1[3], bfr_1[4], bfr_1[5], bfr_1[6], bfr_1[7], bfr_1[8], bfr_1[9], 
							bfr_1[10],bfr_1[11],bfr_1[12],bfr_1[13],bfr_1[14],bfr_1[15],bfr_1[16],bfr_1[17],bfr_1[18],bfr_1[19], 
							bfr_1[20],bfr_1[21],bfr_1[22],bfr_1[23]);
							*/
	}
	else if(usr_lcd_is_display_en())
		for(i = 0; i < LCD_LINE_LENTH; i++)bfr_1[i] = p->pSource->tag_name_en[i];
	else
		;
	pow = changePower(p->pSource->tag_power);
	usr_lcd_float_cov(bfr_2+10, p->alm_limit_value, pow);
	usr_lcd_float_cov(bfr_3+10, p->real_time_value, pow);

	for(i = 0; i < 8; i++)
	{
		if(usr_lcd_is_display_ch())
		{
			bfr_2[i] = bf_b_ch[i];
			bfr_3[i] = bf_d_ch[i];
		}
		else if(usr_lcd_is_display_en())
		{
			bfr_2[i] = bf_b_en[i];
			bfr_3[i] = bf_d_en[i];
		}
		else
			;

	}
	bfr_2[8] = mh;
	bfr_3[8] = mh;

	if((p->pSource->tag_uint_name[0] == 0xa1)&&(p->pSource->tag_uint_name[1] == 0xe6))
	{
		p->pSource->tag_uint_name[0] = 0x20;
		p->pSource->tag_uint_name[1] = 0xa1;
		p->pSource->tag_uint_name[2] = 0xe6;
	}
	
	for(i = 0; i < 5; i++)
	{
		bfr_2[19 + i] = p->pSource->tag_uint_name[i];
		bfr_3[19 + i] = p->pSource->tag_uint_name[i];
	}
}


/************************************************************
* usr_lcd_page_info
* 显示模块信息界面
************************************************************/
void usr_lcd_page_info(void)
{
	int		i;
	UINT8	*bfr_1;
	UINT8	*bfr_2;
	UINT8	*bfr_3;
	UINT8	*bfr_4;
	int id;
	int ip[2][4];

	//UINT8	bfr_1_ch_t[] = "123456789012345678901234";
	UINT8	bfr_1_ch[] = "网络模块no              ";
	UINT8	bfr_2_ch[] = "网络接口IP              ";

	UINT8	bfr_1_en[] = "module ID               ";
	UINT8	bfr_2_en[] = "module IP               ";

	if(g_default_status)
	{
			;
	}
	else
	{
		usr_lcd_clr_screen();
		
		bfr_1 = g_lcd_contest + (0 * LCD_LINE_LENTH); 
		bfr_2 = g_lcd_contest + (1 * LCD_LINE_LENTH); 
		bfr_3 = g_lcd_contest + (2 * LCD_LINE_LENTH); 
		bfr_4 = g_lcd_contest + (3 * LCD_LINE_LENTH); 

		ip[0][0] = (g_flash_module_net_ip[USR_IP_INTERFACE_1]>>24)&0xff;
		ip[0][1] = (g_flash_module_net_ip[USR_IP_INTERFACE_1]>>16)&0xff;
		ip[0][2] = (g_flash_module_net_ip[USR_IP_INTERFACE_1]>>8)&0xff;
		ip[0][3] = (g_flash_module_net_ip[USR_IP_INTERFACE_1])&0xff;
		ip[1][0] = (g_flash_module_net_ip[USR_IP_INTERFACE_2]>>24)&0xff;
		ip[1][1] = (g_flash_module_net_ip[USR_IP_INTERFACE_2]>>16)&0xff;
		ip[1][2] = (g_flash_module_net_ip[USR_IP_INTERFACE_2]>>8)&0xff;
		ip[1][3] = (g_flash_module_net_ip[USR_IP_INTERFACE_2])&0xff;

		for(i = 0; i < LCD_LINE_LENTH; i++)
		{
			if(usr_lcd_is_display_ch())
			{
					//bfr_1[i] = bfr_1_ch[i];
					bfr_2[i] = bfr_2_ch[i];
			}
			else if(usr_lcd_is_display_en())
			{
					//bfr_1[i] = bfr_1_en[i];
					bfr_2[i] = bfr_2_en[i];
			}
			else
				;
		}
		for(i = 0; i < 20; i++)bfr_1[i] = g_SHNAME[i];
		//usr_lcd_int_cov(bfr_1+11,	id,	2);
		usr_lcd_int_cov(bfr_3+0,	ip[0][0],	3);bfr_3[3] = '.';
		usr_lcd_int_cov(bfr_3+4,	ip[0][1],	3);bfr_3[7] = '.';
		usr_lcd_int_cov(bfr_3+8,	ip[0][2],	3);bfr_3[11] = '.';
		usr_lcd_int_cov(bfr_3+12,	ip[0][3],	3);
		usr_lcd_int_cov(bfr_4+0,	ip[1][0],	3);bfr_4[3] = '.';
		usr_lcd_int_cov(bfr_4+4,	ip[1][1],	3);bfr_4[7] = '.';
		usr_lcd_int_cov(bfr_4+8,	ip[1][2],	3);bfr_4[11] = '.';
		usr_lcd_int_cov(bfr_4+12,	ip[1][3],	3);	
	}

}

/************************************************************
* usr_lcd_page_start
************************************************************/
void usr_lcd_page_start(void)
{
	int		i;
	UINT8	*bfr;
	UINT8	*bfr_1;
	UINT8	*bfr_2;
	UINT8	*bfr_3;
	UINT8	*bfr_4;
	int ip[2][4];


		
	usr_lcd_clr_screen();

	bfr	  = g_lcd_contest;
	bfr_1 = g_lcd_contest + (0 * LCD_LINE_LENTH); 
	bfr_2 = g_lcd_contest + (1 * LCD_LINE_LENTH); 
	bfr_3 = g_lcd_contest + (2 * LCD_LINE_LENTH); 
	bfr_4 = g_lcd_contest + (3 * LCD_LINE_LENTH); 

	if(usr_is_default_status())
	{
		if(usr_lcd_is_display_ch())
		{
			for(i = 0; i < LCD_LINE_LENTH; i++)bfr_1[i] = g_lcd_start_info_err_til_ch[i];
			if(usr_is_default_mac())
			{
				for(i = 0; i < LCD_LINE_LENTH; i++)
				{
					bfr_2[i] = g_lcd_start_info_err_mac_ch[i];
					bfr_3[i] = g_lcd_start_info_err_net_ch[i];
				}
			}
			else {
					ip[0][0] = (g_flash_module_net_ip[USR_IP_INTERFACE_1]>>24)&0xff;
					ip[0][1] = (g_flash_module_net_ip[USR_IP_INTERFACE_1]>>16)&0xff;
					ip[0][2] = (g_flash_module_net_ip[USR_IP_INTERFACE_1]>>8)&0xff;
					ip[0][3] = (g_flash_module_net_ip[USR_IP_INTERFACE_1])&0xff;
					ip[1][0] = (g_flash_module_net_ip[USR_IP_INTERFACE_2]>>24)&0xff;
					ip[1][1] = (g_flash_module_net_ip[USR_IP_INTERFACE_2]>>16)&0xff;
					ip[1][2] = (g_flash_module_net_ip[USR_IP_INTERFACE_2]>>8)&0xff;
					ip[1][3] = (g_flash_module_net_ip[USR_IP_INTERFACE_2])&0xff;
		
					usr_lcd_int_cov(bfr_2+0,	ip[0][0],	3);bfr_2[3] = '.';
					usr_lcd_int_cov(bfr_2+4,	ip[0][1],	3);bfr_2[7] = '.';
					usr_lcd_int_cov(bfr_2+8,	ip[0][2],	3);bfr_2[11] = '.';
					usr_lcd_int_cov(bfr_2+12,	ip[0][3],	3);
					usr_lcd_int_cov(bfr_3+0,	ip[1][0],	3);bfr_3[3] = '.';
					usr_lcd_int_cov(bfr_3+4,	ip[1][1],	3);bfr_3[7] = '.';
					usr_lcd_int_cov(bfr_3+8,	ip[1][2],	3);bfr_3[11] = '.';
					usr_lcd_int_cov(bfr_3+12,	ip[1][3],	3);	
			}

			if(usr_is_default_par())
			{
				for(i = 0; i < LCD_LINE_LENTH; i++)
				{
					bfr_4[i] = g_lcd_start_info_err_par_ch[i];
				}
			}
		}
		else if(usr_lcd_is_display_en())
		{
			for(i = 0; i < LCD_LINE_LENTH; i++)bfr_1[i] = g_lcd_start_info_err_til_en[i];
			if(usr_is_default_mac())
			{
				for(i = 0; i < LCD_LINE_LENTH; i++)
				{
					bfr_2[i] = g_lcd_start_info_err_mac_en[i];
					bfr_3[i] = g_lcd_start_info_err_net_en[i];
				}
			}
			else {
					ip[0][0] = (g_flash_module_net_ip[USR_IP_INTERFACE_1]>>24)&0xff;
					ip[0][1] = (g_flash_module_net_ip[USR_IP_INTERFACE_1]>>16)&0xff;
					ip[0][2] = (g_flash_module_net_ip[USR_IP_INTERFACE_1]>>8)&0xff;
					ip[0][3] = (g_flash_module_net_ip[USR_IP_INTERFACE_1])&0xff;
					ip[1][0] = (g_flash_module_net_ip[USR_IP_INTERFACE_2]>>24)&0xff;
					ip[1][1] = (g_flash_module_net_ip[USR_IP_INTERFACE_2]>>16)&0xff;
					ip[1][2] = (g_flash_module_net_ip[USR_IP_INTERFACE_2]>>8)&0xff;
					ip[1][3] = (g_flash_module_net_ip[USR_IP_INTERFACE_2])&0xff;
		
					usr_lcd_int_cov(bfr_2+0,	ip[0][0],	3);bfr_2[3] = '.';
					usr_lcd_int_cov(bfr_2+4,	ip[0][1],	3);bfr_2[7] = '.';
					usr_lcd_int_cov(bfr_2+8,	ip[0][2],	3);bfr_2[11] = '.';
					usr_lcd_int_cov(bfr_2+12,	ip[0][3],	3);
					usr_lcd_int_cov(bfr_3+0,	ip[1][0],	3);bfr_3[3] = '.';
					usr_lcd_int_cov(bfr_3+4,	ip[1][1],	3);bfr_3[7] = '.';
					usr_lcd_int_cov(bfr_3+8,	ip[1][2],	3);bfr_3[11] = '.';
					usr_lcd_int_cov(bfr_3+12,	ip[1][3],	3);	
				}

			if(usr_is_default_par())
			{
				for(i = 0; i < LCD_LINE_LENTH; i++)
				{
					bfr_4[i] = g_lcd_start_info_err_par_en[i];
				}
			}
		}
		else
			;
	}
	else
	{
		if(usr_lcd_is_display_ch())
			for(i = 0; i < LCD_BFR; i++)bfr[i] = g_lcd_start_context_ch[i];
		else if(usr_lcd_is_display_en())
			for(i = 0; i < LCD_BFR; i++)bfr[i] = g_lcd_start_context_en[i];
		else
		;
	}
}


/************************************************************
* usr_lcd_page_config_ing
************************************************************/
void usr_lcd_page_config_ing(void)
{
	int		i;
	UINT8	*bfr;

	usr_lcd_clr_screen();
	bfr	  = g_lcd_contest;

	if(usr_lcd_is_display_ch())
		for(i = 0; i < LCD_BFR; i++)bfr[i] = g_lcd_config_ing_context_ch[i];
	else if(usr_lcd_is_display_en())
		for(i = 0; i < LCD_BFR; i++)bfr[i] = g_lcd_config_ing_context_en[i];
	else
	;
}


/************************************************************
* usr_lcd_page_config_success
************************************************************/
void usr_lcd_page_config_success(void)
{
	int		i;
	UINT8	*bfr;

	usr_lcd_clr_screen();
	bfr	  = g_lcd_contest;

	if(usr_lcd_is_display_ch())
		for(i = 0; i < LCD_BFR; i++)bfr[i] = g_lcd_config_success_context_ch[i];
	else if(usr_lcd_is_display_en())
		for(i = 0; i < LCD_BFR; i++)bfr[i] = g_lcd_config_success_context_en[i];
	else
	;
}


/************************************************************
* usr_lcd_page_tag_lists
************************************************************/
void usr_lcd_page_tag_lists(void)
{
	switch(g_current_list)
	{
	case USR_LCD_PAGE_LIST_ALM_TOTAL:

		usr_lcd_page_list_total_alm();
		break;

	case USR_LCD_PAGE_LIST_ALM_1:

		usr_lcd_page_list_1_alm();
		break;

	case USR_LCD_PAGE_LIST_ALM_2:

		usr_lcd_page_list_2_alm();
		break;

	case USR_LCD_PAGE_LIST_ALM_3:

		usr_lcd_page_list_3_alm();
		break;

	case USR_LCD_PAGE_LIST_ALM_4:

		usr_lcd_page_list_4_alm();
		break;

	case USR_LCD_PAGE_LIST_ALM_5:

		usr_lcd_page_list_5_alm();
		break;

	case USR_LCD_PAGE_LIST_ALM_6:

		usr_lcd_page_list_6_alm();
		break;
		
	case USR_LCD_PAGE_LIST_ALM_7:

		usr_lcd_page_list_7_alm();
		break;
		
	case USR_LCD_PAGE_LIST_ALM_8:

		usr_lcd_page_list_8_alm();
		break;
				
	default:
		;
	}
}


/************************************************************
* usr_lcd_page_tag_details
************************************************************/
void usr_lcd_page_tag_details(void)
{
	ALM_BLOCK* p;
	ALM_COPY_BLOCK*	q;

	switch(g_current_list)
	{
	case USR_LCD_PAGE_LIST_ALM_TOTAL:

		p = usr_find_lcd_head_block();
		break;

	case USR_LCD_PAGE_LIST_ALM_1:

		q = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_1);
		if(q == (ALM_COPY_BLOCK*)NULL)
			p = (ALM_BLOCK*)NULL;
		else
			p = q->pSource;
		break;

	case USR_LCD_PAGE_LIST_ALM_2:

		q = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_2);
		if(q == (ALM_COPY_BLOCK*)NULL)
			p = (ALM_BLOCK*)NULL;
		else
			p = q->pSource;
		break;

	case USR_LCD_PAGE_LIST_ALM_3:

		q = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_3);
		if(q == (ALM_COPY_BLOCK*)NULL)
			p = (ALM_BLOCK*)NULL;
		else
			p = q->pSource;
		break;

	case USR_LCD_PAGE_LIST_ALM_4:

		q = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_4);
		if(q == (ALM_COPY_BLOCK*)NULL)
			p = (ALM_BLOCK*)NULL;
		else
			p = q->pSource;
		break;

	case USR_LCD_PAGE_LIST_ALM_5:

		q = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_5);
		if(q == (ALM_COPY_BLOCK*)NULL)
			p = (ALM_BLOCK*)NULL;
		else
			p = q->pSource;
		break;
		
	case USR_LCD_PAGE_LIST_ALM_6:

		q = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_6);
		if(q == (ALM_COPY_BLOCK*)NULL)
			p = (ALM_BLOCK*)NULL;
		else
			p = q->pSource;
		break;
		
	case USR_LCD_PAGE_LIST_ALM_7:

		q = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_7);
		if(q == (ALM_COPY_BLOCK*)NULL)
			p = (ALM_BLOCK*)NULL;
		else
			p = q->pSource;
		break;
		
	case USR_LCD_PAGE_LIST_ALM_8:

		q = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_8);
		if(q == (ALM_COPY_BLOCK*)NULL)
			p = (ALM_BLOCK*)NULL;
		else
			p = q->pSource;
		break;

	default:
		p = (ALM_BLOCK*)NULL;
	}

	usr_lcd_page_list_info(p, 0);
	usr_lcd_page_detail_info(p);
}

/************************************************************
* usr_lcd_page_sleep
************************************************************/
void usr_lcd_page_sleep(void)
{
	int		i;
	UINT8	*bfr_1;
	UINT8	*bfr_2;
	UINT8	*bfr_3;
	UINT8	*bfr_4;

	usr_lcd_clr_screen();
	
	bfr_1 = g_lcd_contest + (0 * LCD_LINE_LENTH); 
	bfr_2 = g_lcd_contest + (1 * LCD_LINE_LENTH); 
	bfr_3 = g_lcd_contest + (2 * LCD_LINE_LENTH); 
	bfr_4 = g_lcd_contest + (3 * LCD_LINE_LENTH); 


	usr_lcd_int_cov(bfr_2+7,	usr_get_now_year(),		4);
	usr_lcd_int_cov(bfr_2+12,	usr_get_now_month(),	2);
	usr_lcd_int_cov(bfr_2+15,	usr_get_now_date(),		2);
	bfr_2[11] = '-';
	bfr_2[14] = '-';

	usr_lcd_int_cov(bfr_3+8,	usr_get_now_hour(),		2);
	usr_lcd_int_cov(bfr_3+11,	usr_get_now_minute(),	2);
	usr_lcd_int_cov(bfr_3+14,	usr_get_now_second(),	2);
	bfr_3[10] = ':';
	bfr_3[13] = ':';

	for(i = 0; i < LCD_LINE_LENTH; i++)
	{
		bfr_1[i] = '-';
		bfr_4[i] = '-';
	}
}


/************************************************************
* usr_lcd_page_uint_fail
************************************************************/
void usr_lcd_page_uint_fail(void)
{
	int		i;
	UINT8	*bfr_1;
	UINT8	*bfr_2;
	UINT8	*bfr_3;
	UINT8	*bfr_4;

	//UINT8	bfr_1_ch_t[] = "123456789012345678901234";
	UINT8	bfr_1_ch_t[] = "网络接口01  网线  正常  ";
	UINT8	bfr_1_ch_f[] = "网络接口01  网线  断开  ";
	UINT8	bfr_2_ch_t[] = "网络接口02  网线  正常  ";
	UINT8	bfr_2_ch_f[] = "网络接口02  网线  断开  ";

	UINT8	bfr_3_ch_t[] = "网络通讯01  网络测试正常";
	UINT8	bfr_3_ch_f[] = "网络通讯01  网络测试错误";
	UINT8	bfr_4_ch_t[] = "网络通讯02  网络测试正常";
	UINT8	bfr_4_ch_f[] = "网络通讯02  网络测试错误";

	UINT8	bfr_1_en_t[] = "net mac 01  wire normal ";
	UINT8	bfr_1_en_f[] = "net mac 01  wire error  ";
	UINT8	bfr_2_en_t[] = "net mac 02  wire normal ";
	UINT8	bfr_2_en_f[] = "net mac 02  wire error  ";

	UINT8	bfr_3_en_t[] = "netComm 01  comm normal ";
	UINT8	bfr_3_en_f[] = "netComm 01  comm error  ";
	UINT8	bfr_4_en_t[] = "netComm 02  comm normal ";
	UINT8	bfr_4_en_f[] = "netComm 02  comm error  ";


	usr_lcd_clr_screen();
	
	bfr_1 = g_lcd_contest + (0 * LCD_LINE_LENTH); 
	bfr_2 = g_lcd_contest + (1 * LCD_LINE_LENTH); 
	bfr_3 = g_lcd_contest + (2 * LCD_LINE_LENTH); 
	bfr_4 = g_lcd_contest + (3 * LCD_LINE_LENTH); 


	for(i = 0; i < LCD_LINE_LENTH; i++)
	{
		if(usr_lcd_is_display_ch())
		{
			if(g_flag_macBrk[USR_IP_INTERFACE_1])
				bfr_1[i] = bfr_1_ch_f[i];
			else
				bfr_1[i] = bfr_1_ch_t[i];

/*
			if(g_flag_macBrk[USR_IP_INTERFACE_2])
				bfr_2[i] = bfr_2_ch_f[i];
			else
				bfr_2[i] = bfr_2_ch_t[i];
*/
			if(g_flag_netAlive[USR_IP_INTERFACE_1])
				bfr_3[i] = bfr_3_ch_t[i];
			else
				bfr_3[i] = bfr_3_ch_f[i];
/*
			if(g_flag_netAlive[USR_IP_INTERFACE_2])
				bfr_4[i] = bfr_4_ch_t[i];
			else
				bfr_4[i] = bfr_4_ch_f[i];
*/		
		}
		else if(usr_lcd_is_display_en())
		{
			if(g_flag_macBrk[USR_IP_INTERFACE_1])
				bfr_1[i] = bfr_1_en_f[i];
			else
				bfr_1[i] = bfr_1_en_t[i];
/*
			if(g_flag_macBrk[USR_IP_INTERFACE_2])
				bfr_2[i] = bfr_2_en_f[i];
			else
				bfr_2[i] = bfr_2_en_t[i];
*/
			if(g_flag_netAlive[USR_IP_INTERFACE_1])
				bfr_3[i] = bfr_3_en_t[i];
			else
				bfr_3[i] = bfr_3_en_f[i];
/*
			if(g_flag_netAlive[USR_IP_INTERFACE_2])
				bfr_4[i] = bfr_4_en_t[i];
			else
				bfr_4[i] = bfr_4_en_f[i];
*/
		}
		else
			;
	}
}



/************************************************************
* usr_lcd_page_list_total_alm
************************************************************/
void usr_lcd_page_list_total_alm(void)
{
	ALM_BLOCK*	p;
	UINT16		cnt = 0;

	p = usr_find_lcd_head_block();
	while
		( 
		(p != (ALM_BLOCK*)NULL) &&
		(cnt < LCD_LINE) &&
		(cnt < usr_get_list_total_alm_len())
		)
	{
		usr_lcd_page_list_info(p, cnt);
		cnt++;
		p = p->next;
	}

	if(cnt)
	{
		while(cnt < LCD_LINE)
		{
			usr_lcd_page_list_info((ALM_BLOCK*)NULL, cnt);
			cnt++;
		}
	}
	else
		usr_lcd_page_list_total_null(USR_LCD_PAGE_LIST_ALM_TOTAL);
}


/************************************************************
* usr_lcd_page_list_1_alm
************************************************************/
void usr_lcd_page_list_1_alm(void)
{
	ALM_COPY_BLOCK*	p;
	UINT16			cnt = 0;

	p = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_1);
	while
		( 
		(p != (ALM_COPY_BLOCK*)NULL) &&
		(cnt < LCD_LINE) &&
		(cnt < usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_1))
		)
	{
		usr_lcd_page_list_info(p->pSource, cnt);
		cnt++;
		p = p->next;
	}

	if(cnt)
	{
		while(cnt < LCD_LINE)
		{
			usr_lcd_page_list_info((ALM_BLOCK*)NULL, cnt);
			cnt++;
		}
	}
	else
		usr_lcd_page_list_total_null(USR_LCD_PAGE_LIST_ALM_1);
}


/************************************************************
* usr_lcd_page_list_2_alm
************************************************************/
void usr_lcd_page_list_2_alm(void)
{
	ALM_COPY_BLOCK*	p;
	UINT16			cnt = 0;

	p = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_2);
	while
		( 
		(p != (ALM_COPY_BLOCK*)NULL) &&
		(cnt < LCD_LINE) &&
		(cnt < usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_2))
		)
	{
		usr_lcd_page_list_info(p->pSource, cnt);
		cnt++;
		p = p->next;
	}

	if(cnt)
	{
		while(cnt < LCD_LINE)
		{
			usr_lcd_page_list_info((ALM_BLOCK*)NULL, cnt);
			cnt++;
		}
	}
	else
		usr_lcd_page_list_total_null(USR_LCD_PAGE_LIST_ALM_2);

}

/************************************************************
* usr_lcd_page_list_3_alm
************************************************************/
void usr_lcd_page_list_3_alm(void)
{
	ALM_COPY_BLOCK*	p;
	UINT16			cnt = 0;

	p = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_3);
	while
		( 
		(p != (ALM_COPY_BLOCK*)NULL) &&
		(cnt < LCD_LINE) &&
		(cnt < usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_3))
		)
	{
		usr_lcd_page_list_info(p->pSource, cnt);
		cnt++;
		p = p->next;
	}

	if(cnt)
	{
		while(cnt < LCD_LINE)
		{
			usr_lcd_page_list_info((ALM_BLOCK*)NULL, cnt);
			cnt++;
		}
	}
	else
		usr_lcd_page_list_total_null(USR_LCD_PAGE_LIST_ALM_3);
}


/************************************************************
* usr_lcd_page_list_4_alm
************************************************************/
void usr_lcd_page_list_4_alm(void)
{
	ALM_COPY_BLOCK*	p;
	UINT16			cnt = 0;

	p = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_4);
	while
		( 
		(p != (ALM_COPY_BLOCK*)NULL) &&
		(cnt < LCD_LINE) &&
		(cnt < usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_4))
		)
	{
		usr_lcd_page_list_info(p->pSource, cnt);
		cnt++;
		p = p->next;
	}

	if(cnt)
	{
		while(cnt < LCD_LINE)
		{
			usr_lcd_page_list_info((ALM_BLOCK*)NULL, cnt);
			cnt++;
		}
	}
	else
		usr_lcd_page_list_total_null(USR_LCD_PAGE_LIST_ALM_4);
}


/************************************************************
* usr_lcd_page_list_5_alm
************************************************************/
void usr_lcd_page_list_5_alm(void)
{
	ALM_COPY_BLOCK*	p;
	UINT16			cnt = 0;

	p = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_5);
	while
		( 
		(p != (ALM_COPY_BLOCK*)NULL) &&
		(cnt < LCD_LINE) &&
		(cnt < usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_5))
		)
	{
		usr_lcd_page_list_info(p->pSource, cnt);
		cnt++;
		p = p->next;
	}

	if(cnt)
	{
		while(cnt < LCD_LINE)
		{
			usr_lcd_page_list_info((ALM_BLOCK*)NULL, cnt);
			cnt++;
		}
	}
	else
		usr_lcd_page_list_total_null(USR_LCD_PAGE_LIST_ALM_5);
}


/************************************************************
* usr_lcd_page_list_6_alm
************************************************************/
void usr_lcd_page_list_6_alm(void)
{
	ALM_COPY_BLOCK*	p;
	UINT16			cnt = 0;

	p = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_6);
	while
		( 
		(p != (ALM_COPY_BLOCK*)NULL) &&
		(cnt < LCD_LINE) &&
		(cnt < usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_6))
		)
	{
		usr_lcd_page_list_info(p->pSource, cnt);
		cnt++;
		p = p->next;
	}

	if(cnt)
	{
		while(cnt < LCD_LINE)
		{
			usr_lcd_page_list_info((ALM_BLOCK*)NULL, cnt);
			cnt++;
		}
	}
	else
		usr_lcd_page_list_total_null(USR_LCD_PAGE_LIST_ALM_6);
}

/************************************************************
* usr_lcd_page_list_7_alm
************************************************************/
void usr_lcd_page_list_7_alm(void)
{
	ALM_COPY_BLOCK*	p;
	UINT16			cnt = 0;

	p = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_7);
	while
		( 
		(p != (ALM_COPY_BLOCK*)NULL) &&
		(cnt < LCD_LINE) &&
		(cnt < usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_7))
		)
	{
		usr_lcd_page_list_info(p->pSource, cnt);
		cnt++;
		p = p->next;
	}

	if(cnt)
	{
		while(cnt < LCD_LINE)
		{
			usr_lcd_page_list_info((ALM_BLOCK*)NULL, cnt);
			cnt++;
		}
	}
	else
		usr_lcd_page_list_total_null(USR_LCD_PAGE_LIST_ALM_7);
}

/************************************************************
* usr_lcd_page_list_8_alm
************************************************************/
void usr_lcd_page_list_8_alm(void)
{
	ALM_COPY_BLOCK*	p;
	UINT16			cnt = 0;

	p = usr_find_lcd_head_copy_block(USR_LIST_ALM_GROUP_8);
	while
		( 
		(p != (ALM_COPY_BLOCK*)NULL) &&
		(cnt < LCD_LINE) &&
		(cnt < usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_8))
		)
	{
		usr_lcd_page_list_info(p->pSource, cnt);
		cnt++;
		p = p->next;
	}

	if(cnt)
	{
		while(cnt < LCD_LINE)
		{
			usr_lcd_page_list_info((ALM_BLOCK*)NULL, cnt);
			cnt++;
		}
	}
	else
		usr_lcd_page_list_total_null(USR_LCD_PAGE_LIST_ALM_8);
}


/************************************************************
* usr_lcd_page_list_total_null
************************************************************/
void usr_lcd_page_list_total_null(int ID)
{
	int		i;
	UINT8	*bfr_1;
	UINT8	*bfr_2;
	UINT8	*bfr_3;
	UINT8	*bfr_4;
	UINT8	bfr_line[LCD_BFR]			    = "------------------------";
	
	usr_lcd_clr_screen();
	
	bfr_1 = g_lcd_contest + (0 * LCD_LINE_LENTH); 
	bfr_2 = g_lcd_contest + (1 * LCD_LINE_LENTH); 
	bfr_3 = g_lcd_contest + (2 * LCD_LINE_LENTH); 
	bfr_4 = g_lcd_contest + (3 * LCD_LINE_LENTH); 


	for(i = 0; i < LCD_LINE_LENTH; i++)
	{
		bfr_1[i] = bfr_line[i];
		bfr_3[i] = bfr_line[i];

		if(usr_lcd_is_display_ch())
		{
			switch(ID)
			{
			case USR_LCD_PAGE_LIST_ALM_TOTAL:
				bfr_2[i] = G_bfr_Alm_total_ch[i];
				break;

			case USR_LCD_PAGE_LIST_ALM_1:
				bfr_2[i] = G_bfr_Alm_1_ch[i];
				break;

			case USR_LCD_PAGE_LIST_ALM_2:
				bfr_2[i] = G_bfr_Alm_2_ch[i];
				break;

			case USR_LCD_PAGE_LIST_ALM_3:
				bfr_2[i] = G_bfr_Alm_3_ch[i];
				break;

			case USR_LCD_PAGE_LIST_ALM_4:
				bfr_2[i] = G_bfr_Alm_4_ch[i];
				break;

			case USR_LCD_PAGE_LIST_ALM_5:
				bfr_2[i] = G_bfr_Alm_5_ch[i];
				break;

			case USR_LCD_PAGE_LIST_ALM_6:
				bfr_2[i] = G_bfr_Alm_6_ch[i];
				break;

			case USR_LCD_PAGE_LIST_ALM_7:
				bfr_2[i] = G_bfr_Alm_7_ch[i];
				break;
	
			case USR_LCD_PAGE_LIST_ALM_8:
				bfr_2[i] = G_bfr_Alm_8_ch[i];
				break;
											
			default:
				;
			}
		}
		else if(usr_lcd_is_display_en())
		{
			switch(ID)
			{
			case USR_LCD_PAGE_LIST_ALM_TOTAL:
				bfr_2[i] = G_bfr_Alm_total_en[i];
				break;

			case USR_LCD_PAGE_LIST_ALM_1:
				bfr_2[i] = G_bfr_Alm_1_en[i];
				break;

			case USR_LCD_PAGE_LIST_ALM_2:
				bfr_2[i] = G_bfr_Alm_2_en[i];
				break;

			case USR_LCD_PAGE_LIST_ALM_3:
				bfr_2[i] = G_bfr_Alm_3_en[i];
				break;

			case USR_LCD_PAGE_LIST_ALM_4:
				bfr_2[i] = G_bfr_Alm_4_en[i];
				break;

			case USR_LCD_PAGE_LIST_ALM_5:
				bfr_2[i] = G_bfr_Alm_5_en[i];
				break;

			case USR_LCD_PAGE_LIST_ALM_6:
				bfr_2[i] = G_bfr_Alm_6_en[i];
				break;
				
			case USR_LCD_PAGE_LIST_ALM_7:
				bfr_2[i] = G_bfr_Alm_7_en[i];
				break;		
				
			case USR_LCD_PAGE_LIST_ALM_8:
				bfr_2[i] = G_bfr_Alm_8_en[i];
				break;
						
			default:
				;
			}
		}
		else
			;

	}

	
	usr_lcd_int_cov(bfr_4+0,	usr_get_now_year(),		4);
	usr_lcd_int_cov(bfr_4+5,	usr_get_now_month(),	2);
	usr_lcd_int_cov(bfr_4+8,	usr_get_now_date(),		2);
	bfr_4[4] = '-';
	bfr_4[7] = '-';

	usr_lcd_int_cov(bfr_4+11,	usr_get_now_hour(),		2);
	usr_lcd_int_cov(bfr_4+14,	usr_get_now_minute(),	2);
	usr_lcd_int_cov(bfr_4+17,	usr_get_now_second(),	2);
	bfr_4[13] = ':';
	bfr_4[16] = ':';
}


/************************************************************
* usr_lcd_page_operate
************************************************************/
void usr_lcd_page_operate(void)
{
	switch(g_current_page)
	{
	case USR_LCD_PAGE_MACHINE_START:

		usr_lcd_page_machine_start_operate();
		break;

	case USR_LCD_PAGE_LIST_ALM:

		usr_lcd_page_tag_lists_operate();
		break;

	case USR_LCD_PAGE_TAG_DETAIL:

		usr_lcd_page_tag_details_operate();
		break;

	case USR_LCD_PAGE_SLEEP:

		usr_lcd_page_sleep_operate();
		break;

	case USR_LCD_PAGE_UINT_FAIL:

		usr_lcd_page_uint_fail_operate();
		break;
		
	case USR_LCD_PAGE_CFG_CHECK:
		
		usr_lcd_page_cfgcheck_operate();
		break;
		
	default:
		;
	}
}

/************************************************************
* usr_lcd_page_cfgcheck
************************************************************/
void usr_lcd_page_cfgcheck(void)
{
	int			i,j,flag;
	UINT8		*bfr_1;
	UINT8		*bfr_2;
	UINT8		*bfr_3;
	UINT8		*bfr_4;
	ALM_SOLID* p;
	
	p = &g_solid_alm[g_current_list];

	usr_lcd_clr_screen();
	
	bfr_1 = g_lcd_contest + (0 * LCD_LINE_LENTH); 
	bfr_2 = g_lcd_contest + (1 * LCD_LINE_LENTH); 
	bfr_3 = g_lcd_contest + (2 * LCD_LINE_LENTH); 
	bfr_4 = g_lcd_contest + (3 * LCD_LINE_LENTH); 
	
	usr_lcd_int_cov(bfr_1+0,	p->id,	4);
	usr_lcd_int_cov(bfr_1+8,	(p->group_id+1),	2);

	for(i = 0; i < 8; i++)bfr_2[i] = p->tag_easy_id[i];
	usr_lcd_int_cov(bfr_2+10,	p->tag_power,	2);
	for(i = 18; i < 23; i++)bfr_2[i] = p->tag_uint_name[i-19];
	
	for(i = 0, j = 0; i < LCD_LINE_LENTH; j++)
	{
		if(i%2 == 0)
		{
			bfr_3[i] = p->tag_name_ch[j];
			if(bfr_3[i] < 0xA0)
				flag = 1;
			else
				flag = 0;
			i++;
		}
		else
		{
			if((flag == 1)&&(p->tag_name_ch[j] >= 0xA0))
				i++;
			bfr_3[i] = p->tag_name_ch[j];
			flag = 0;
			i++;
		}
	}
	for(i = 0; i < LCD_LINE_LENTH; i++)bfr_4[i] = p->tag_name_en[i];
	
	for(i=USR_LIST_ALM_GROUP_1;i<USR_LIST_ALM_GROUP_16;i++)
		flag_check_led[i] = 0;
	flag_check_led[p->group_id] = 1;
}


/************************************************************
* usr_lcd_page_cfgcheck_operate
************************************************************/
void usr_lcd_page_cfgcheck_operate(void)
{
	int i;
	if(usr_key_isup(USR_KEY_ID_DOWN))
	{
		g_current_page = USR_LCD_PAGE_CFG_CHECK;
		g_current_list++;
		if(g_current_list > USR_SOLID_ALM_NUM)
			g_current_list = 33;
	}
	else if(usr_key_isup(USR_KEY_ID_UP))
	{
		g_current_page = USR_LCD_PAGE_CFG_CHECK;
		g_current_list--;
		if(g_current_list < 33 )
			g_current_list = USR_SOLID_ALM_NUM - 1;
	}
	else if(usr_key_isup(USR_KEY_ID_ESC))
	{
		g_current_page = USR_LCD_PAGE_LIST_ALM;
		g_current_list = USR_LCD_PAGE_LIST_ALM_TOTAL;
		for(i=USR_LIST_ALM_GROUP_1;i<USR_LIST_ALM_GROUP_16;i++)
			flag_check_led[i] = 0;
	}
	else
		;
}


/************************************************************
* usr_lcd_page_machine_start_operate
************************************************************/
void usr_lcd_page_machine_start_operate(void)
{
	if(usr_is_default_status())
		return;

	if((usr_key_isup(USR_KEY_ID_UP)) || (usr_key_isup(USR_KEY_ID_DOWN)) || (usr_key_isup(USR_KEY_ID_ENTER)) || (usr_key_isup(USR_KEY_ID_ESC)))
	{
		g_current_page = USR_LCD_PAGE_LIST_ALM;
		g_current_list = USR_LCD_PAGE_LIST_ALM_TOTAL;
	}
}


/************************************************************
* usr_lcd_page_tag_lists_operate
************************************************************/
void usr_lcd_page_tag_lists_operate(void)
{
	switch(g_current_list)
	{
	case USR_LCD_PAGE_LIST_ALM_TOTAL:

		usr_lcd_page_list_total_operate();
		break;

	case USR_LCD_PAGE_LIST_ALM_1:

		if(G_no_IO != 1)
			usr_lcd_page_list_1_operate();
		break;

	case USR_LCD_PAGE_LIST_ALM_2:

		if(G_no_IO != 2)
			usr_lcd_page_list_2_operate();
		break;

	case USR_LCD_PAGE_LIST_ALM_3:

		if(G_no_IO != 3)
			usr_lcd_page_list_3_operate();
		break;

	case USR_LCD_PAGE_LIST_ALM_4:

		if(G_no_IO != 4)
			usr_lcd_page_list_4_operate();
		break;

	case USR_LCD_PAGE_LIST_ALM_5:

		if(G_no_IO != 5)
			usr_lcd_page_list_5_operate();
		break;

	case USR_LCD_PAGE_LIST_ALM_6:

		if(G_no_IO != 6)
			usr_lcd_page_list_6_operate();
		break;
		
	case USR_LCD_PAGE_LIST_ALM_7:

		if(G_no_IO != 7)
			usr_lcd_page_list_7_operate();
		break;
		
	case USR_LCD_PAGE_LIST_ALM_8:

		if(G_no_IO != 8)
			usr_lcd_page_list_8_operate();
		break;
	default:
		;
	}
}


/************************************************************
* usr_lcd_page_tag_lists_operate
************************************************************/
void usr_lcd_page_tag_details_operate(void)
{
	switch(g_current_list)
	{
	case USR_LCD_PAGE_LIST_ALM_TOTAL:

		if(usr_key_isup(USR_KEY_ID_UP))
			if(usr_get_list_total_alm_len())
				usr_range_add(&(g_list_total_alm.list_lcd_head), 1, g_list_total_alm.list_len);

		if(usr_key_isup(USR_KEY_ID_DOWN))
			if(usr_get_list_total_alm_len())
				usr_range_sub(&(g_list_total_alm.list_lcd_head), 1, g_list_total_alm.list_len);

		if(usr_key_isup(USR_KEY_ID_ESC))
			g_current_page = USR_LCD_PAGE_LIST_ALM;

		break;

	case USR_LCD_PAGE_LIST_ALM_1:

		if(usr_key_isup(USR_KEY_ID_UP))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_1))
				usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_1].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_1].list_len);

		if(usr_key_isup(USR_KEY_ID_DOWN))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_1))
				usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_1].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_1].list_len);

		if(usr_key_isup(USR_KEY_ID_ESC))
			g_current_page = USR_LCD_PAGE_LIST_ALM;

		break;

	case USR_LCD_PAGE_LIST_ALM_2:

		if(usr_key_isup(USR_KEY_ID_UP))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_2))
				usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_2].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_2].list_len);

		if(usr_key_isup(USR_KEY_ID_DOWN))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_2))
				usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_2].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_2].list_len);

		if(usr_key_isup(USR_KEY_ID_ESC))
			g_current_page = USR_LCD_PAGE_LIST_ALM;

		break;

	case USR_LCD_PAGE_LIST_ALM_3:

		if(usr_key_isup(USR_KEY_ID_UP))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_3))
				usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_3].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_3].list_len);

		if(usr_key_isup(USR_KEY_ID_DOWN))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_3))
				usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_3].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_3].list_len);

		if(usr_key_isup(USR_KEY_ID_ESC))
			g_current_page = USR_LCD_PAGE_LIST_ALM;

		break;

	case USR_LCD_PAGE_LIST_ALM_4:

		if(usr_key_isup(USR_KEY_ID_UP))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_4))
				usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_4].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_4].list_len);

		if(usr_key_isup(USR_KEY_ID_DOWN))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_4))
				usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_4].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_4].list_len);

		if(usr_key_isup(USR_KEY_ID_ESC))
			g_current_page = USR_LCD_PAGE_LIST_ALM;

		break;

	case USR_LCD_PAGE_LIST_ALM_5:

		if(usr_key_isup(USR_KEY_ID_UP))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_5))
				usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_5].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_5].list_len);

		if(usr_key_isup(USR_KEY_ID_DOWN))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_5))
				usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_5].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_5].list_len);

		if(usr_key_isup(USR_KEY_ID_ESC))
			g_current_page = USR_LCD_PAGE_LIST_ALM;

		break;

	case USR_LCD_PAGE_LIST_ALM_6:

		if(usr_key_isup(USR_KEY_ID_UP))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_6))
				usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_6].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_6].list_len);

		if(usr_key_isup(USR_KEY_ID_DOWN))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_6))
				usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_6].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_6].list_len);

		if(usr_key_isup(USR_KEY_ID_ESC))
			g_current_page = USR_LCD_PAGE_LIST_ALM;

		break;

	case USR_LCD_PAGE_LIST_ALM_7:

		if(usr_key_isup(USR_KEY_ID_UP))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_7))
				usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_7].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_7].list_len);

		if(usr_key_isup(USR_KEY_ID_DOWN))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_7))
				usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_7].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_7].list_len);

		if(usr_key_isup(USR_KEY_ID_ESC))
			g_current_page = USR_LCD_PAGE_LIST_ALM;

		break;

	case USR_LCD_PAGE_LIST_ALM_8:

		if(usr_key_isup(USR_KEY_ID_UP))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_8))
				usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_8].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_8].list_len);

		if(usr_key_isup(USR_KEY_ID_DOWN))
			if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_8))
				usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_8].list_lcd_head), 
								1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_8].list_len);

		if(usr_key_isup(USR_KEY_ID_ESC))
			g_current_page = USR_LCD_PAGE_LIST_ALM;

		break;
						
	default:
		;
	}
}


/************************************************************
* usr_lcd_page_sleep_operate
************************************************************/
void usr_lcd_page_sleep_operate(void)
{
	;
}



/************************************************************
* usr_lcd_page_uint_fail_operate
************************************************************/
void usr_lcd_page_uint_fail_operate(void)
{
	if(usr_key_isup(USR_KEY_ID_UINT_FAIL))
	{
		g_current_page = USR_LCD_PAGE_LIST_ALM;
		g_current_list = USR_LCD_PAGE_LIST_ALM_TOTAL;
	}
}


/************************************************************
* usr_lcd_page_list_total_operate
************************************************************/
void usr_lcd_page_list_total_operate(void)
{
	if(usr_key_isup(USR_KEY_ID_UP))
		if(usr_get_list_total_alm_len())
			usr_range_add(&(g_list_total_alm.list_lcd_head), 1, g_list_total_alm.list_len);

	if(usr_key_isup(USR_KEY_ID_DOWN))
		if(usr_get_list_total_alm_len())
			usr_range_sub(&(g_list_total_alm.list_lcd_head), 1, g_list_total_alm.list_len);

	if(usr_key_isup(USR_KEY_ID_ENTER))
		if(usr_get_list_total_alm_len())
			g_current_page = USR_LCD_PAGE_TAG_DETAIL;

	if(usr_key_isup(USR_KEY_ID_UINT_FAIL))
			g_current_page = USR_LCD_PAGE_UINT_FAIL;

	//if(usr_key_isup(USR_KEY_ID_ESC)) 
	//	g_current_page = USR_LCD_PAGE_SLEEP;

	if((usr_key_isup(USR_KEY_ID_R1))&&(G_Alm_num >= 1)&&(G_no_IO != 1))
		g_current_list = USR_LCD_PAGE_LIST_ALM_1;

	if((usr_key_isup(USR_KEY_ID_R2))&&(G_Alm_num >= 2)&&(G_no_IO != 2))
		g_current_list = USR_LCD_PAGE_LIST_ALM_2;

	if((usr_key_isup(USR_KEY_ID_R3))&&(G_Alm_num >= 3)&&(G_no_IO != 3))
		g_current_list = USR_LCD_PAGE_LIST_ALM_3;

	if((usr_key_isup(USR_KEY_ID_R4))&&(G_Alm_num >= 4)&&(G_no_IO != 4))
		g_current_list = USR_LCD_PAGE_LIST_ALM_4;

	if((usr_key_isup(USR_KEY_ID_R5))&&(G_Alm_num >= 5)&&(G_no_IO != 5))
		g_current_list = USR_LCD_PAGE_LIST_ALM_5;
		
	if((usr_key_isup(USR_KEY_ID_R6))&&(G_Alm_num >= 6)&&(G_no_IO != 6))
		g_current_list = USR_LCD_PAGE_LIST_ALM_6;
		
	if((usr_key_isup(USR_KEY_ID_R7))&&(G_Alm_num >= 7)&&(G_no_IO != 7))
		g_current_list = USR_LCD_PAGE_LIST_ALM_7;
		
	if((usr_key_isup(USR_KEY_ID_R8))&&(G_Alm_num >= 8)&&(G_no_IO != 8))
		g_current_list = USR_LCD_PAGE_LIST_ALM_8;
}


/************************************************************
* usr_lcd_page_list_1_operate
************************************************************/
void usr_lcd_page_list_1_operate(void)
{
	if(usr_key_isup(USR_KEY_ID_UP))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_1))
			usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_1].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_1].list_len);

	if(usr_key_isup(USR_KEY_ID_DOWN))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_1))
			usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_1].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_1].list_len);

	if(usr_key_isup(USR_KEY_ID_ENTER))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_1))
			g_current_page = USR_LCD_PAGE_TAG_DETAIL;

	if((usr_key_isup(USR_KEY_ID_R1)) || (usr_key_isup(USR_KEY_ID_ESC)))
		g_current_list = USR_LCD_PAGE_LIST_ALM_TOTAL;

/*
	if(usr_key_isup(USR_KEY_ID_R2))
		g_current_list = USR_LCD_PAGE_LIST_ALM_2;

	if(usr_key_isup(USR_KEY_ID_R3))
		g_current_list = USR_LCD_PAGE_LIST_ALM_3;

	if(usr_key_isup(USR_KEY_ID_R4))
		g_current_list = USR_LCD_PAGE_LIST_ALM_4;

	if(usr_key_isup(USR_KEY_ID_R5))
		g_current_list = USR_LCD_PAGE_LIST_ALM_5;
*/
}

/************************************************************
* usr_lcd_page_list_2_operate
************************************************************/
void usr_lcd_page_list_2_operate(void)
{
	if(usr_key_isup(USR_KEY_ID_UP))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_2))
			usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_2].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_2].list_len);

	if(usr_key_isup(USR_KEY_ID_DOWN))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_2))
			usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_2].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_2].list_len);

	if(usr_key_isup(USR_KEY_ID_ENTER))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_2))
			g_current_page = USR_LCD_PAGE_TAG_DETAIL;

	if((usr_key_isup(USR_KEY_ID_R2)) || (usr_key_isup(USR_KEY_ID_ESC)))
		g_current_list = USR_LCD_PAGE_LIST_ALM_TOTAL;

/*
	if(usr_key_isup(USR_KEY_ID_R1))
		g_current_list = USR_LCD_PAGE_LIST_ALM_1;

	if(usr_key_isup(USR_KEY_ID_R3))
		g_current_list = USR_LCD_PAGE_LIST_ALM_3;

	if(usr_key_isup(USR_KEY_ID_R4))
		g_current_list = USR_LCD_PAGE_LIST_ALM_4;

	if(usr_key_isup(USR_KEY_ID_R5))
		g_current_list = USR_LCD_PAGE_LIST_ALM_5;
*/
}

/************************************************************
* usr_lcd_page_list_3_operate
************************************************************/
void usr_lcd_page_list_3_operate(void)
{
	if(usr_key_isup(USR_KEY_ID_UP))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_3))
			usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_3].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_3].list_len);

	if(usr_key_isup(USR_KEY_ID_DOWN))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_3))
			usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_3].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_3].list_len);

	if(usr_key_isup(USR_KEY_ID_ENTER))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_3))
			g_current_page = USR_LCD_PAGE_TAG_DETAIL;

	if((usr_key_isup(USR_KEY_ID_R3)) || (usr_key_isup(USR_KEY_ID_ESC)))
		g_current_list = USR_LCD_PAGE_LIST_ALM_TOTAL;

/*
	if(usr_key_isup(USR_KEY_ID_R1))
		g_current_list = USR_LCD_PAGE_LIST_ALM_1;

	if(usr_key_isup(USR_KEY_ID_R2))
		g_current_list = USR_LCD_PAGE_LIST_ALM_2;

	if(usr_key_isup(USR_KEY_ID_R4))
		g_current_list = USR_LCD_PAGE_LIST_ALM_4;

	if(usr_key_isup(USR_KEY_ID_R5))
		g_current_list = USR_LCD_PAGE_LIST_ALM_5;
*/
}

/************************************************************
* usr_lcd_page_list_4_operate
************************************************************/
void usr_lcd_page_list_4_operate(void)
{
	if(usr_key_isup(USR_KEY_ID_UP))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_4))
			usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_4].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_4].list_len);

	if(usr_key_isup(USR_KEY_ID_DOWN))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_4))
			usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_4].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_4].list_len);

	if(usr_key_isup(USR_KEY_ID_ENTER))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_4))
			g_current_page = USR_LCD_PAGE_TAG_DETAIL;

	if((usr_key_isup(USR_KEY_ID_R4)) || (usr_key_isup(USR_KEY_ID_ESC)))
		g_current_list = USR_LCD_PAGE_LIST_ALM_TOTAL;
/*
	if(usr_key_isup(USR_KEY_ID_R1))
		g_current_list = USR_LCD_PAGE_LIST_ALM_1;

	if(usr_key_isup(USR_KEY_ID_R2))
		g_current_list = USR_LCD_PAGE_LIST_ALM_2;

	if(usr_key_isup(USR_KEY_ID_R3))
		g_current_list = USR_LCD_PAGE_LIST_ALM_3;

	if(usr_key_isup(USR_KEY_ID_R5))
		g_current_list = USR_LCD_PAGE_LIST_ALM_5;
*/
}

/************************************************************
* usr_lcd_page_list_5_operate
************************************************************/
void usr_lcd_page_list_5_operate(void)
{
	if(usr_key_isup(USR_KEY_ID_UP))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_5))
			usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_5].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_5].list_len);

	if(usr_key_isup(USR_KEY_ID_DOWN))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_5))
			usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_5].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_5].list_len);

	if(usr_key_isup(USR_KEY_ID_ENTER))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_5))
			g_current_page = USR_LCD_PAGE_TAG_DETAIL;

	if((usr_key_isup(USR_KEY_ID_R5)) || (usr_key_isup(USR_KEY_ID_ESC)))
		g_current_list = USR_LCD_PAGE_LIST_ALM_TOTAL;
/*
	if(usr_key_isup(USR_KEY_ID_R1))
		g_current_list = USR_LCD_PAGE_LIST_ALM_1;

	if(usr_key_isup(USR_KEY_ID_R2))
		g_current_list = USR_LCD_PAGE_LIST_ALM_2;

	if(usr_key_isup(USR_KEY_ID_R3))
		g_current_list = USR_LCD_PAGE_LIST_ALM_3;

	if(usr_key_isup(USR_KEY_ID_R4))
		g_current_list = USR_LCD_PAGE_LIST_ALM_4;
*/
}


/************************************************************
* usr_lcd_page_list_6_operate
************************************************************/
void usr_lcd_page_list_6_operate(void)
{
	if(usr_key_isup(USR_KEY_ID_UP))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_6))
			usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_6].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_6].list_len);

	if(usr_key_isup(USR_KEY_ID_DOWN))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_6))
			usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_6].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_6].list_len);

	if(usr_key_isup(USR_KEY_ID_ENTER))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_6))
			g_current_page = USR_LCD_PAGE_TAG_DETAIL;

	if((usr_key_isup(USR_KEY_ID_R6)) || (usr_key_isup(USR_KEY_ID_ESC)))
		g_current_list = USR_LCD_PAGE_LIST_ALM_TOTAL;
}


/************************************************************
* usr_lcd_page_list_7_operate
************************************************************/
void usr_lcd_page_list_7_operate(void)
{
	if(usr_key_isup(USR_KEY_ID_UP))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_7))
			usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_7].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_7].list_len);

	if(usr_key_isup(USR_KEY_ID_DOWN))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_7))
			usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_7].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_7].list_len);

	if(usr_key_isup(USR_KEY_ID_ENTER))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_7))
			g_current_page = USR_LCD_PAGE_TAG_DETAIL;

	if((usr_key_isup(USR_KEY_ID_R7)) || (usr_key_isup(USR_KEY_ID_ESC)))
		g_current_list = USR_LCD_PAGE_LIST_ALM_TOTAL;
}


/************************************************************
* usr_lcd_page_list_8_operate
************************************************************/
void usr_lcd_page_list_8_operate(void)
{
	if(usr_key_isup(USR_KEY_ID_UP))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_8))
			usr_range_add(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_8].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_8].list_len);

	if(usr_key_isup(USR_KEY_ID_DOWN))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_8))
			usr_range_sub(&(g_list_group_copy_alm[USR_LIST_ALM_GROUP_8].list_lcd_head), 
							1, g_list_group_copy_alm[USR_LIST_ALM_GROUP_8].list_len);

	if(usr_key_isup(USR_KEY_ID_ENTER))
		if(usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_8))
			g_current_page = USR_LCD_PAGE_TAG_DETAIL;

	if((usr_key_isup(USR_KEY_ID_R8)) || (usr_key_isup(USR_KEY_ID_ESC)))
		g_current_list = USR_LCD_PAGE_LIST_ALM_TOTAL;
}


/************************************************************
* usr_pannel_lcd_deal_with
************************************************************/
void usr_pannel_lcd_deal_with(void)
{
	if(!g_flag_config_forbit)
	{
		if((g_current_page != USR_LCD_PAGE_CONFIG_ING)&&(g_current_page != USR_LCD_PAGE_CONFIG_SUCCESS))
		{
			if(g_flag_work_mode == USR_WORK_MODE_READY)
				g_current_page = USR_LCD_PAGE_SLEEP;
			else
			{
				if(g_flag_log_work_mode == USR_WORK_MODE_READY)
				{
					g_current_page = USR_LCD_PAGE_LIST_ALM;
					g_current_list = USR_LCD_PAGE_LIST_ALM_TOTAL;
				}

				usr_lcd_page_operate();
				
			}
		}
		g_flag_log_work_mode = g_flag_work_mode;
	}

	usr_lcd_page_display();
	
	usr_lcd_page_info_debug_print();
}



/************************************************************
* usr_pannel_lcd_output
************************************************************/
void usr_pannel_lcd_output(void)
{
	//lcd_rst(&g_GPIO_OUT[USR_GPIO_OUT_CH_2]);
	lcd_chn_disp_all(&g_GPIO_OUT[USR_GPIO_OUT_CH_2], g_lcd_contest);
	//lcd_rst(&g_GPIO_OUT[USR_GPIO_OUT_CH_2]);
	
}
