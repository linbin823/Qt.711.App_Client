#include "usr.h"

/************************************************************
* VAR
************************************************************/
XGpio		GpioChannel;	// The driver instance for GPIO Device 
XSpi		Spi;			// The driver instance for SPI Device 
Xuint32		GpioOutReg;		// The Output Register for GPIO Device's O/P 
Xuint32		GpioInReg;		// The Input Register for GPIO Device's I/P 

UINT32		g_GPIO_IN[2]; 
UINT32		g_GPIO_OUT[2]; 

UINT32		g_key_flag_input; 
UINT32		g_key_flag_log; 
UINT8		g_key_flag_status[32]; 

UINT32		g_led_flag_output; 
UINT32		g_led_flag_overturn; 
int			g_level_lightshin;
bool		g_flag_testlight; 
bool		g_flag_mute; 

bool    flag_IO_buzz = 0;
/************************************************************
* FUNC
************************************************************/
void	drv_gpio_init(void);
UINT32	drv_gpio_input(int devID);
void	drv_gpio_output(int devID);


void	usr_gpio_init(void);
bool	usr_ackbit16(UINT16 tmp, int bit);
bool	usr_ackbit(UINT32 tmp, int bit);
void	usr_setbit(UINT32 *pt, int bit);
void	usr_clrbit(UINT32 *pt, int bit);
bool	usr_gpio_get_input(int devID,  int bit);
void	usr_gpio_set_output(int devID, int bit);
void	usr_gpio_clr_output(int devID, int bit);

bool	usr_key_get_flag_input(int ID);
void	usr_gpio_key_trigger(void);
bool	usr_key_isup(int ID);
bool	usr_key_isdown(int ID);
bool	usr_key_ishigh(int ID);
bool	usr_key_islow(int ID);

bool	usr_led_get_flag_output(int ID);
void	usr_led_set_flag_output(int ID);
void	usr_led_clr_flag_output(int ID);
bool	usr_led_get_flag_overturn(int ID);
void	usr_led_set_flag_overturn(int ID);
void	usr_led_clr_flag_overturn(int ID);
bool	usr_led_get_testlight(void);
void	usr_led_on_testlight(void);
void	usr_led_off_testlight(void);
int		usr_led_get_lightlevel(void);
void	usr_led_set_lightlevel(int level);
bool	usr_buzz_get_mute(void);
void	usr_buzz_on_mute(void);
void	usr_buzz_off_mute(void);
void	usr_gpio_led_output(void);
void	usr_gpio_buzz_output(void);

bool	usr_gpio_isMac1Break(void);
bool	usr_gpio_isMac2Break(void);

void	usr_gpio_debug(void);

void	usr_pannel_key_input(void);
void	usr_pannel_gpio_output(void);
void 	usr_gpio_relay_output(void);


void	usr_pannel_gpio_deal_with(void);
void	usr_pannel_gpio_deal_lampTest(void);
void	usr_pannel_gpio_deal_dimmer(void);
void	usr_pannel_gpio_deal_mute(void);
void	usr_pannel_gpio_deal_language(void);
void	usr_pannel_gpio_deal_almRst(void);
void	usr_pannel_gpio_deal_soundoff(void);
void	usr_pannel_gpio_deal_ack_callfromBridge(void);
void	usr_pannel_gpio_deal_ack_callfromEngine(void);
void	usr_pannel_gpio_deal_alm_status(void);
void	usr_pannel_gpio_deal_duty_status(void);
void	usr_pannel_gpio_deal_unit_status(void);
void	usr_pannel_gpio_debug_call(void);

bool	usr_is_app_light_flash(void);

void usr_pannel_gpio_deal_check(void);
void usr_pannel_gpio_deal_clrLcd(void);

/************************************************************
* drv_gpio_init() 
************************************************************/
void drv_gpio_init(void)
{
	Xuint32		RegData = 0;
	Xuint32*	pOutReg;
	
	pOutReg = &GpioOutReg;
	RegData = (Xuint32)(*pOutReg);

	XU_GPIO_Out32(GPIO_CHA_TRI_ADDR,	GPIO_CHANNELA_DIR);
	XU_GPIO_Out32(GPIO_CHB_TRI_ADDR,	GPIO_CHANNELB_DIR);
	XU_GPIO_Out32(GPIO_0_CHA_TRI_ADDR,	GPIO_CHANNELB_DIR);	

	RegData = GPIO_CHANNELA_DFT;
	XU_GPIO_Out32(GPIO_OUT_ADDR,RegData);
			
	*pOutReg = (Xuint32)RegData;

}


/************************************************************
* drv_gpio_input() 
************************************************************/
UINT32 drv_gpio_input(int devID)
{
	UINT32 tmp = 0x00;
	switch(devID)
	{
	case USR_GPIO_IN_CH_1:
		g_GPIO_IN[USR_GPIO_IN_CH_1] = (UINT32)(XU_GPIO_In32(USR_GPIO_IN_ADDR_1));
		tmp = g_GPIO_IN[USR_GPIO_IN_CH_1];
		break;

	case USR_GPIO_IN_CH_2:
		g_GPIO_IN[USR_GPIO_IN_CH_2] = (UINT32)(XU_GPIO_In32(USR_GPIO_IN_ADDR_2));
		tmp = g_GPIO_IN[USR_GPIO_IN_CH_2];
		break;

	default:
		; 
	}

	return tmp;
}


/************************************************************
* drv_gpio_output() 
************************************************************/
void drv_gpio_output(int devID)
{
	switch(devID)
	{
	case USR_GPIO_OUT_CH_1:
		XU_GPIO_Out32(USR_GPIO_OUT_ADDR_1, g_GPIO_OUT[USR_GPIO_OUT_CH_1]);	
		break;

	case USR_GPIO_OUT_CH_2:
		XU_GPIO_Out32(USR_GPIO_OUT_ADDR_2, g_GPIO_OUT[USR_GPIO_OUT_CH_2]);	
		break;

	default:
		;
	}
}


/************************************************************
* usr_gpio_init() 
************************************************************/
void usr_gpio_init(void)
{
	int i;

	g_GPIO_IN[USR_GPIO_IN_CH_1]		= 0x00000000;
	g_GPIO_IN[USR_GPIO_IN_CH_2]		= 0x00000000;
	
	g_GPIO_OUT[USR_GPIO_OUT_CH_1]	= 0x00000000;
	g_GPIO_OUT[USR_GPIO_OUT_CH_2]	= 0x00000000;
	
	drv_gpio_output(USR_GPIO_OUT_CH_1);
	drv_gpio_output(USR_GPIO_OUT_CH_2);


	g_key_flag_input	= 0x00000000; 
	g_key_flag_log		= 0x00000000; 
	for(i = 0; i < 32; i++)g_key_flag_status[i] = USR_KEY_STATUS_LOW;

	g_led_flag_output	= 0x00000000;  
	g_led_flag_overturn	= 0x00000000; 

	g_level_lightshin	= USR_LED_LIGHT_LEVEL_DEFAULT;

	g_flag_testlight	= false; 

	g_flag_mute			= false; 

	usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_BUZZ_ID);
}



/************************************************************
* usr_ackbit16() 
************************************************************/
bool usr_ackbit16(UINT16 tmp, int bit)
{
	UINT16 mask = 0x00;
	mask = (0x01 << bit);

	if((tmp&mask) == 0x0000)
		return false;
	else
		return true;
}

/************************************************************
* usr_ackbit() 
************************************************************/
bool usr_ackbit(UINT32 tmp, int bit)
{
	UINT32 mask = 0x00;
	mask = (0x01 << bit);

	if((tmp&mask) == 0x00000000)
		return false;
	else
		return true;
}

/************************************************************
* usr_setbit() 
************************************************************/
void usr_setbit(UINT32 *pt, int bit)
{
	UINT32 mask = 0x00;
	mask = (0x01 << bit);
	(*pt) |= mask;
}

/************************************************************
* usr_clrbit() 
************************************************************/
void usr_clrbit(UINT32 *pt, int bit)
{
	UINT32 mask = 0x00;
	mask = (0x01 << bit);
	(*pt) &= ~(mask);
}


/************************************************************
* usr_gpio_get_input() 
************************************************************/
bool usr_gpio_get_input(int devID,  int bit)
{
	return usr_ackbit(g_GPIO_IN[devID], bit);
}

/************************************************************
* usr_gpio_set_output() 
************************************************************/
void usr_gpio_set_output(int devID, int bit)
{
	usr_setbit(&(g_GPIO_OUT[devID]), bit);
}

/************************************************************
* usr_gpio_clr_output() 
************************************************************/
void usr_gpio_clr_output(int devID, int bit)
{
	usr_clrbit(&(g_GPIO_OUT[devID]), bit);
}


/************************************************************
* usr_key_get_flag_input() 
************************************************************/
bool usr_key_get_flag_input(int ID)
{
	return usr_ackbit(g_key_flag_input, ID);
}

/************************************************************
* usr_gpio_key_trigger() 
************************************************************/
void usr_gpio_key_trigger(void)
{
	int		i;
	bool	temp,last;
	for(i = 0; i <= 29; i++)
	{
		temp = usr_ackbit(g_key_flag_input, i);
		last = usr_ackbit(g_key_flag_log, i);

		if((last == false) && (temp == true))
			g_key_flag_status[i] = USR_KEY_STATUS_UP;
		else if((last == true) && (temp == false))
			g_key_flag_status[i] = USR_KEY_STATUS_DOWN;
		else if((last == false) && (temp == false))
			g_key_flag_status[i] = USR_KEY_STATUS_LOW;
		else if((last == true) && (temp == true))
			g_key_flag_status[i] = USR_KEY_STATUS_HIGH;
		else
			;
	}

	g_key_flag_log = g_key_flag_input;
}


/************************************************************
* usr_key_isup() 
************************************************************/
bool usr_key_isup(int ID)
{
	return (g_key_flag_status[ID] == USR_KEY_STATUS_UP);
}

/************************************************************
* usr_key_isdown() 
************************************************************/
bool usr_key_isdown(int ID)
{
	return (g_key_flag_status[ID] == USR_KEY_STATUS_DOWN);
}

/************************************************************
* usr_key_ishigh() 
************************************************************/
bool usr_key_ishigh(int ID)
{
	return (g_key_flag_status[ID] == USR_KEY_STATUS_HIGH);
}

/************************************************************
* usr_key_islow() 
************************************************************/
bool usr_key_islow(int ID)
{
	return (g_key_flag_status[ID] == USR_KEY_STATUS_LOW);
}





/************************************************************
* usr_led_get_flag_output() 
************************************************************/
bool usr_led_get_flag_output(int ID)
{
	return usr_ackbit(g_led_flag_output, ID);
}

/************************************************************
* usr_led_set_flag_output() 
************************************************************/
void usr_led_set_flag_output(int ID)
{
	usr_setbit(&(g_led_flag_output), ID);
}

/************************************************************
* usr_led_clr_flag_output() 
************************************************************/
void usr_led_clr_flag_output(int ID)
{
	usr_clrbit(&(g_led_flag_output), ID);
}

/************************************************************
* usr_led_get_flag_overturn() 
************************************************************/
bool usr_led_get_flag_overturn(int ID)
{
	return usr_ackbit(g_led_flag_overturn, ID);
}

/************************************************************
* usr_led_set_flag_overturn() 
************************************************************/
void usr_led_set_flag_overturn(int ID)
{
	usr_setbit(&(g_led_flag_overturn), ID);
}

/************************************************************
* usr_led_clr_flag_overturn() 
************************************************************/
void usr_led_clr_flag_overturn(int ID)
{
	usr_clrbit(&(g_led_flag_overturn), ID);
}


/************************************************************
* usr_led_get_testlight() 
************************************************************/
bool usr_led_get_testlight(void)
{
	return g_flag_testlight;
}

/************************************************************
* usr_led_on_testlight() 
************************************************************/
void usr_led_on_testlight(void)
{
	g_flag_testlight = true;
}

/************************************************************
* usr_led_off_testlight() 
************************************************************/
void usr_led_off_testlight(void)
{
	g_flag_testlight = false;
}


/************************************************************
* usr_led_get_lightlevel() 
************************************************************/
int usr_led_get_lightlevel(void)
{
	return g_level_lightshin;
}

/************************************************************
* usr_led_set_lightlevel() 
************************************************************/
void usr_led_set_lightlevel(int level)
{
	int		i;
	Xuint32 tmp = 0x00;
	g_level_lightshin = level;
	
	for(i = USR_LED_LIGHT_LEVEL_MIN,  tmp = 0x00; i <= USR_LED_LIGHT_LEVEL_MAX; i++)
	{
		tmp = tmp + 1;
		if(level == i)
			break;
	}

	if(usr_ackbit(tmp, 0))
		usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_LED_LIGHT_LEVEL_BIT0);
	else
		usr_gpio_clr_output(USR_GPIO_OUT_CH_1, USR_LED_LIGHT_LEVEL_BIT0);

	if(usr_ackbit(tmp, 1))
		usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_LED_LIGHT_LEVEL_BIT1);
	else
		usr_gpio_clr_output(USR_GPIO_OUT_CH_1, USR_LED_LIGHT_LEVEL_BIT1);

	if(usr_ackbit(tmp, 2))
		usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_LED_LIGHT_LEVEL_BIT2);
	else
		usr_gpio_clr_output(USR_GPIO_OUT_CH_1, USR_LED_LIGHT_LEVEL_BIT2);

	if(usr_ackbit(tmp, 3))
		usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_LED_LIGHT_LEVEL_BIT3);
	else
		usr_gpio_clr_output(USR_GPIO_OUT_CH_1, USR_LED_LIGHT_LEVEL_BIT3);

}


/************************************************************
* usr_gpio_led_output() 
************************************************************/
void usr_gpio_led_output(void)
{
	int i;
	for(i = 0; i <= 24; i++)
	{
		if(usr_led_get_testlight())
		{
			//usr_gpio_set_output(USR_GPIO_OUT_CH_1, i);
			if(usr_get_flag_oneSec_overturn())
				usr_gpio_clr_output(USR_GPIO_OUT_CH_1, i);
			else
				usr_gpio_set_output(USR_GPIO_OUT_CH_1, i);
		}
		else if(usr_led_get_flag_overturn(i))
		{
			if(usr_get_flag_oneSec_overturn())
				usr_gpio_set_output(USR_GPIO_OUT_CH_1, i);
			else
				usr_gpio_clr_output(USR_GPIO_OUT_CH_1, i);
		}
		else 
		{
			if(usr_led_get_flag_output(i))
				usr_gpio_set_output(USR_GPIO_OUT_CH_1, i);
			else
				usr_gpio_clr_output(USR_GPIO_OUT_CH_1, i);
		}
	}
}

/************************************************************
* usr_buzz_get_mute() 
************************************************************/
bool usr_buzz_get_mute(void)
{
	return g_flag_mute;
}

/************************************************************
* usr_buzz_on_mute() 
************************************************************/
void usr_buzz_on_mute(void)
{
	g_flag_mute = true;
}

/************************************************************
* usr_buzz_off_mute() 
************************************************************/
void usr_buzz_off_mute(void)
{
	g_flag_mute = false;
}


/************************************************************
* usr_is_app_light_flash() 
************************************************************/
bool usr_is_app_light_flash(void)
{
	if(usr_led_get_flag_overturn(USR_LED_ID_R1)&&(G_no_IO != 1))
		return true;

	if(usr_led_get_flag_overturn(USR_LED_ID_R2)&&(G_no_IO != 2))
		return true;

	if(usr_led_get_flag_overturn(USR_LED_ID_R3)&&(G_no_IO != 3))
		return true;

	if(usr_led_get_flag_overturn(USR_LED_ID_R4)&&(G_no_IO != 4))
		return true;

	if(usr_led_get_flag_overturn(USR_LED_ID_R5)&&(G_no_IO != 5))
		return true;
		
	if(usr_led_get_flag_overturn(USR_LED_ID_R6)&&(G_no_IO != 6))
		return true;
		
	if(usr_led_get_flag_overturn(USR_LED_ID_R7)&&(G_no_IO != 7))
		return true;
		
	if(usr_led_get_flag_overturn(USR_LED_ID_R8)&&(G_no_IO != 8))
		return true;
		
	return false;
}

bool usr_is_app_call_flash(void)
{
	if(usr_led_get_flag_overturn(USR_LED_ID_CALLFROM_BRIDGE))
		return true;

	if(usr_led_get_flag_overturn(USR_LED_ID_CALLFROM_ENGINE))
		return true;

	if(usr_led_get_flag_overturn(USR_LED_ID_UNIT_FAIL))
		return true;
		
	return false;
}

bool usr_is_app_IO_flash(void)
{
	if(G_YN_IOin <= 0)
		return false;
		
	if(usr_led_get_flag_overturn(USR_LED_ID_R1)&&(G_no_IO == 1))
		return true;

	if(usr_led_get_flag_overturn(USR_LED_ID_R2)&&(G_no_IO == 2))
		return true;

	if(usr_led_get_flag_overturn(USR_LED_ID_R3)&&(G_no_IO == 3))
		return true;

	if(usr_led_get_flag_overturn(USR_LED_ID_R4)&&(G_no_IO == 4))
		return true;

	if(usr_led_get_flag_overturn(USR_LED_ID_R5)&&(G_no_IO == 5))
		return true;
		
	if(usr_led_get_flag_overturn(USR_LED_ID_R6)&&(G_no_IO == 6))
		return true;
		
	if(usr_led_get_flag_overturn(USR_LED_ID_R7)&&(G_no_IO == 7))
		return true;
		
	if(usr_led_get_flag_overturn(USR_LED_ID_R8)&&(G_no_IO == 8))
		return true;
		
	return false;

}

/************************************************************
* usr_gpio_buzz_output() 
************************************************************/
void usr_gpio_buzz_output(void)
{
	if(usr_buzz_get_mute())
	{
#ifdef USR_BUZZ_DEBUG
		usr_gpio_clr_output(USR_GPIO_OUT_CH_1, USR_DEBUG_BUZZ_LED);
#endif
		usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_BUZZ_ID);
	}
	else
	{
		//if(usr_led_get_testlight() || usr_is_happen_net_newAlm() || usr_is_happen_net_call())
		if(usr_led_get_testlight() || (usr_is_app_light_flash()&&(g_flag_net_buzz==true)&&(g_flag_work_mode != USR_WORK_MODE_READY)) || (usr_is_app_call_flash()) || (usr_is_app_IO_flash()))
		{
			if(flag_IO_buzz == 1) {
				if(usr_get_flag_500ms_overturn())
				{
	#ifdef USR_BUZZ_DEBUG
					usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_DEBUG_BUZZ_LED);
	#endif
					usr_gpio_clr_output(USR_GPIO_OUT_CH_1, USR_BUZZ_ID);
				}
				else
				{
	#ifdef USR_BUZZ_DEBUG
					usr_gpio_clr_output(USR_GPIO_OUT_CH_1, USR_DEBUG_BUZZ_LED);
	#endif
					usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_BUZZ_ID);
				}
			}
			else{
				if(usr_get_flag_oneSec_overturn())
				{
	#ifdef USR_BUZZ_DEBUG
					usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_DEBUG_BUZZ_LED);
	#endif
					usr_gpio_clr_output(USR_GPIO_OUT_CH_1, USR_BUZZ_ID);
				}
				else
				{
	#ifdef USR_BUZZ_DEBUG
					usr_gpio_clr_output(USR_GPIO_OUT_CH_1, USR_DEBUG_BUZZ_LED);
	#endif
					usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_BUZZ_ID);
				}
			}
		}
		else
		{
#ifdef USR_BUZZ_DEBUG
			usr_gpio_clr_output(USR_GPIO_OUT_CH_1, USR_DEBUG_BUZZ_LED);
#endif
			usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_BUZZ_ID);
		}
	}
}


/************************************************************
* usr_gpio_isMac1Break() 
************************************************************/
bool usr_gpio_isMac1Break(void)
{
	return usr_gpio_get_input(USR_GPIO_IN_CH_1, USR_KEY_ID_MAC1_BREAK);
}


/************************************************************
* usr_gpio_isMac2Break() 
************************************************************/
bool usr_gpio_isMac2Break(void)
{
	return usr_gpio_get_input(USR_GPIO_IN_CH_1, USR_KEY_ID_MAC2_BREAK);
}


/************************************************************
* usr_gpio_debug() 
************************************************************/
int usr_gpio_debug_1(int key_ID, int led_ID, bool flag_reverse)
{
	if(flag_reverse)
		if(usr_key_get_flag_input(key_ID))
			usr_led_clr_flag_output(led_ID);
		else
			usr_led_set_flag_output(led_ID);
	else
		if(usr_key_get_flag_input(key_ID))
			usr_led_set_flag_output(led_ID);
		else
			usr_led_clr_flag_output(led_ID);
}

void usr_gpio_debug(void)
{
	usr_gpio_debug_1(USR_KEY_ID_L1, USR_LED_ID_L1, 0);
	usr_gpio_debug_1(USR_KEY_ID_L2, USR_LED_ID_L2, 0);
	usr_gpio_debug_1(USR_KEY_ID_L3, USR_LED_ID_L3, 0);
	usr_gpio_debug_1(USR_KEY_ID_L4, USR_LED_ID_L4, 0);
	usr_gpio_debug_1(USR_KEY_ID_L5, USR_LED_ID_L5, 0);
	usr_gpio_debug_1(USR_KEY_ID_L6, USR_LED_ID_L6, 0);
	usr_gpio_debug_1(USR_KEY_ID_L7, USR_LED_ID_L7, 0);
	usr_gpio_debug_1(USR_KEY_ID_L8, USR_LED_ID_L8, 0);
	
	usr_gpio_debug_1(USR_KEY_ID_R1, USR_LED_ID_R1, 0);
	usr_gpio_debug_1(USR_KEY_ID_R2, USR_LED_ID_R2, 0);
	usr_gpio_debug_1(USR_KEY_ID_R3, USR_LED_ID_R3, 0);
	usr_gpio_debug_1(USR_KEY_ID_R4, USR_LED_ID_R4, 0);
	usr_gpio_debug_1(USR_KEY_ID_R5, USR_LED_ID_R5, 0);
	usr_gpio_debug_1(USR_KEY_ID_R6, USR_LED_ID_R6, 0);
	usr_gpio_debug_1(USR_KEY_ID_R7, USR_LED_ID_R7, 0);
	usr_gpio_debug_1(USR_KEY_ID_R8, USR_LED_ID_R8, 0);
	
	usr_gpio_debug_1(USR_KEY_ID_CALLFROM_BRIDGE,	USR_LED_ID_CALLFROM_BRIDGE, 0);
	usr_gpio_debug_1(USR_KEY_ID_CALLFROM_ENGINE,	USR_LED_ID_CALLFROM_ENGINE, 0);
	usr_gpio_debug_1(USR_KEY_ID_DIMMER_ADD,			USR_LED_ID_DIMMER_ADD,		0);
	usr_gpio_debug_1(USR_KEY_ID_DIMMER_SUB,			USR_LED_ID_DIMMER_SUB,		0);
	usr_gpio_debug_1(USR_KEY_ID_STANDBY1,			USR_LED_ID_STANDBY1,		0);
	usr_gpio_debug_1(USR_KEY_ID_STANDBY2,			USR_LED_ID_STANDBY2,		0);
	usr_gpio_debug_1(USR_KEY_ID_LAMPTEST,			USR_LED_ID_LAMPTEST,		0);
	usr_gpio_debug_1(USR_KEY_ID_MUTE,				USR_LED_ID_MUTE,			0);
	usr_gpio_debug_1(USR_KEY_ID_SOUNDOFF,			USR_LED_ID_SOUNDOFF,		0);
}



/************************************************************
* usr_pannel_key_input() 
************************************************************/
void usr_pannel_key_input(void)
{
	g_key_flag_input = drv_gpio_input(USR_GPIO_IN_CH_1);
	usr_gpio_key_trigger();
}


/************************************************************
* usr_pannel_gpio_output() 
************************************************************/
void usr_pannel_gpio_output(void)
{
	usr_gpio_led_output();
	usr_gpio_buzz_output();
	usr_gpio_relay_output();
	drv_gpio_output(USR_GPIO_OUT_CH_1);
}


/************************************************************
* usr_pannel_gpio_deal_with() 
************************************************************/
void usr_pannel_gpio_deal_with(void)
{
	usr_pannel_gpio_deal_mute();
	usr_pannel_gpio_deal_dimmer();
	usr_pannel_gpio_deal_lampTest();
	usr_pannel_gpio_deal_language();
	usr_pannel_gpio_deal_almRst();
	usr_pannel_gpio_deal_soundoff();
	usr_pannel_gpio_deal_ack_callfromBridge();
	usr_pannel_gpio_deal_ack_callfromEngine();


	usr_pannel_gpio_deal_alm_status();
	usr_pannel_gpio_deal_duty_status();
	usr_pannel_gpio_deal_unit_status();
	
	usr_pannel_gpio_deal_check();
	usr_pannel_gpio_deal_clrLcd();

#ifdef USR_CALL_DEBUG
	usr_pannel_gpio_debug_call();
#endif
}


/************************************************************
* usr_pannel_gpio_deal_lampTest() 
************************************************************/
void usr_pannel_gpio_deal_lampTest(void)
{
	if(usr_key_isup(USR_KEY_ID_LAMPTEST))
		if(usr_led_get_testlight())
			usr_led_off_testlight();
		else
			usr_led_on_testlight();
	else
		;
}


/************************************************************
* usr_pannel_gpio_deal_dimmer() 
************************************************************/
void usr_pannel_gpio_deal_dimmer(void)
{
	int level;

	if(usr_key_isup(USR_KEY_ID_DIMMER_ADD))
	{
		level = usr_led_get_lightlevel();
		if(level < USR_LED_LIGHT_LEVEL_MAX)
			level++;
		else
			level = USR_LED_LIGHT_LEVEL_MAX;
		usr_led_set_lightlevel(level);
	}
	else if(usr_key_isup(USR_KEY_ID_DIMMER_SUB))
	{
		level = usr_led_get_lightlevel();
		if(level > USR_LED_LIGHT_LEVEL_MIN)
			level--;
		else
			level = USR_LED_LIGHT_LEVEL_MIN;
		usr_led_set_lightlevel(level);
	}
	else
		;


	if(usr_key_ishigh(USR_KEY_ID_DIMMER_ADD))
		usr_led_set_flag_output(USR_LED_ID_DIMMER_ADD);
	else
		usr_led_clr_flag_output(USR_LED_ID_DIMMER_ADD);


	if(usr_key_ishigh(USR_KEY_ID_DIMMER_SUB))
		usr_led_set_flag_output(USR_KEY_ID_DIMMER_SUB);
	else
		usr_led_clr_flag_output(USR_LED_ID_DIMMER_SUB);

}


/************************************************************
* usr_pannel_gpio_deal_mute() 
************************************************************/
void usr_pannel_gpio_deal_mute(void)
{
	if(usr_key_isup(USR_KEY_ID_MUTE))
		if(usr_buzz_get_mute())
			usr_buzz_off_mute();
		else
			usr_buzz_on_mute();
	else
		;


	if(usr_buzz_get_mute())
		usr_led_set_flag_output(USR_LED_ID_MUTE);
	else
		usr_led_clr_flag_output(USR_LED_ID_MUTE);
}


/************************************************************
* usr_pannel_gpio_deal_language() 
************************************************************/
void usr_pannel_gpio_deal_language(void)
{
	if(usr_key_ishigh(USR_KEY_ID_DIMMER_ADD)&&usr_key_ishigh(USR_KEY_ID_DIMMER_SUB))
	{
		if(usr_lcd_is_display_ch())
			usr_lcd_set_display_lang(USR_LCD_DISPLAY_WORD_EN);
		else if(usr_lcd_is_display_en())
			usr_lcd_set_display_lang(USR_LCD_DISPLAY_WORD_CH);
		else
			;
	}
	else
		;
}


/************************************************************
* usr_pannel_gpio_deal_almRst() 
************************************************************/
void usr_pannel_gpio_deal_almRst(void)
{
	if((usr_key_isup(USR_KEY_ID_RST))||(usr_is_net_Rst_happen()))
	{
		usr_list_rst_readyRst_alm();
		usr_clr_net_Rst_happen();
	}
	else
		;

	if(usr_key_ishigh(USR_KEY_ID_RST))
		usr_led_set_flag_output(USR_LED_ID_STANDBY2);
	else
		usr_led_clr_flag_output(USR_LED_ID_STANDBY2);
}


/************************************************************
* usr_pannel_gpio_deal_soundoff() 
************************************************************/
void usr_pannel_gpio_deal_soundoff(void)
{
	if((usr_key_isup(USR_KEY_ID_SOUNDOFF))||(usr_is_net_Ack_happen()))
	{
		if(usr_is_happen_net_newAlm())
			usr_ack_net_newAlm();

		// ack 8 group light 
		if(usr_led_get_flag_overturn(USR_LED_ID_R1))
			usr_led_clr_flag_overturn(USR_LED_ID_R1);

		if(usr_led_get_flag_overturn(USR_LED_ID_R2))
			usr_led_clr_flag_overturn(USR_LED_ID_R2);

		if(usr_led_get_flag_overturn(USR_LED_ID_R3))
			usr_led_clr_flag_overturn(USR_LED_ID_R3);

		if(usr_led_get_flag_overturn(USR_LED_ID_R4))
			usr_led_clr_flag_overturn(USR_LED_ID_R4);

		if(usr_led_get_flag_overturn(USR_LED_ID_R5))
			usr_led_clr_flag_overturn(USR_LED_ID_R5);
			
		if(usr_led_get_flag_overturn(USR_LED_ID_R6))
			usr_led_clr_flag_overturn(USR_LED_ID_R6);

		if(usr_led_get_flag_overturn(USR_LED_ID_R7))
			usr_led_clr_flag_overturn(USR_LED_ID_R7);

		if(usr_led_get_flag_overturn(USR_LED_ID_R8))
			usr_led_clr_flag_overturn(USR_LED_ID_R8);

		if(usr_led_get_flag_overturn(USR_LED_ID_UNIT_FAIL))
			usr_led_clr_flag_overturn(USR_LED_ID_UNIT_FAIL);

		usr_clr_net_Ack_happen();
	}
	else
		;

	if(usr_key_ishigh(USR_KEY_ID_SOUNDOFF))
		usr_led_set_flag_output(USR_LED_ID_SOUNDOFF);
	else
		usr_led_clr_flag_output(USR_LED_ID_SOUNDOFF);
		
	if((usr_key_isup(USR_KEY_ID_SOUNDOFF))&&(g_flag_work_mode == USR_WORK_MODE_DUTY_ON))
		usr_easyalm_take_over();
		
	if(usr_key_isup(USR_KEY_ID_SOUNDOFF)) {
		usr_net_data_analyze2();
		flag_IO_buzz = 0;
	}
}


/************************************************************
* usr_pannel_gpio_deal_ack_callfromBridge() 
************************************************************/
void usr_pannel_gpio_deal_ack_callfromBridge()
{
	if(usr_key_isup(USR_KEY_ID_CALLFROM_BRIDGE))
		if(usr_is_happen_bridge_engine_call())
			usr_ack_sailor_bridge_call();
	else
		;

	if(usr_key_ishigh(USR_KEY_ID_CALLFROM_BRIDGE))
		usr_led_set_flag_output(USR_LED_ID_CALLFROM_BRIDGE);
	else
		usr_led_clr_flag_output(USR_LED_ID_CALLFROM_BRIDGE);	
}


/************************************************************
* usr_pannel_gpio_deal_ack_callfromEngine() 
************************************************************/
void usr_pannel_gpio_deal_ack_callfromEngine(void)
{
	if(usr_key_isup(USR_KEY_ID_CALLFROM_ENGINE))
		if(usr_is_happen_sailor_engine_call())
			usr_ack_sailor_engine_call();
	else
		;

	if(usr_key_ishigh(USR_KEY_ID_CALLFROM_ENGINE))
		usr_led_set_flag_output(USR_LED_ID_CALLFROM_ENGINE);
	else
		usr_led_clr_flag_output(USR_LED_ID_CALLFROM_ENGINE);
}


/************************************************************
* usr_gpio_relay_output() 
************************************************************/
void usr_gpio_relay_output(void)
{
	if(((G_YN_IOin == 1)&&(usr_key_ishigh(USR_KEY_ID_IOIN))) || ((G_YN_IOin == 2)&&(usr_key_islow(USR_KEY_ID_IOIN)))) 	
	{
		if(G_YN_IOout == 1)
			usr_gpio_clr_output(USR_GPIO_OUT_CH_1, USR_LED_ID_RELAY);
		else if(G_YN_IOout == 2)
			usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_LED_ID_RELAY);
		else
			;
		switch(G_no_IO)
		{
		case 1:
			usr_led_set_flag_output(USR_LED_ID_R1);
			break;

		case 2:
			usr_led_set_flag_output(USR_LED_ID_R2);
			break;

		case 3:
			usr_led_set_flag_output(USR_LED_ID_R3);
			break;

		case 4:
			usr_led_set_flag_output(USR_LED_ID_R4);
			break;

		case 5:
			usr_led_set_flag_output(USR_LED_ID_R5);
			break;

		case 6:
			usr_led_set_flag_output(USR_LED_ID_R6);
			break;

		case 7:
			usr_led_set_flag_output(USR_LED_ID_R7);
			break;

		case 8:
			usr_led_set_flag_output(USR_LED_ID_R8);
			break;

		default:
			;				
		}
	}
	else if(((G_YN_IOin == 1)&&(usr_key_islow(USR_KEY_ID_IOIN))) || ((G_YN_IOin == 2)&&(usr_key_ishigh(USR_KEY_ID_IOIN))))
	{
		if(G_YN_IOout == 1)
			usr_gpio_set_output(USR_GPIO_OUT_CH_1, USR_LED_ID_RELAY);
		else if(G_YN_IOout == 2)
			usr_gpio_clr_output(USR_GPIO_OUT_CH_1, USR_LED_ID_RELAY);
		else
			;
		switch(G_no_IO)
		{
		case 1:
			usr_led_clr_flag_output(USR_LED_ID_R1);
			break;

		case 2:
			usr_led_clr_flag_output(USR_LED_ID_R2);
			break;

		case 3:
			usr_led_clr_flag_output(USR_LED_ID_R3);
			break;

		case 4:
			usr_led_clr_flag_output(USR_LED_ID_R4);
			break;

		case 5:
			usr_led_clr_flag_output(USR_LED_ID_R5);
			break;

		case 6:
			usr_led_clr_flag_output(USR_LED_ID_R6);
			break;

		case 7:
			usr_led_clr_flag_output(USR_LED_ID_R7);
			break;

		case 8:
			usr_led_clr_flag_output(USR_LED_ID_R8);
			break;

		default:
			;				
		}
	}
	else
		;	
		
	
	if(((G_YN_IOin == 1)&&(usr_key_isup(USR_KEY_ID_IOIN))) || ((G_YN_IOin == 2)&&(usr_key_isdown(USR_KEY_ID_IOIN))))
	{
		flag_IO_buzz = 1;
		switch(G_no_IO)
		{
		case 1:
			usr_led_set_flag_overturn(USR_LED_ID_R1);
			break;

		case 2:
			usr_led_set_flag_overturn(USR_LED_ID_R2);
			break;

		case 3:
			usr_led_set_flag_overturn(USR_LED_ID_R3);
			break;

		case 4:
			usr_led_set_flag_overturn(USR_LED_ID_R4);
			break;

		case 5:
			usr_led_set_flag_overturn(USR_LED_ID_R5);
			break;

		case 6:
			usr_led_set_flag_overturn(USR_LED_ID_R6);
			break;

		case 7:
			usr_led_set_flag_overturn(USR_LED_ID_R7);
			break;

		case 8:
			usr_led_set_flag_overturn(USR_LED_ID_R8);
			break;

		default:
			;				
		}
	}
	else
		;
}



/************************************************************
* usr_pannel_gpio_deal_alm_status() 
************************************************************/
void usr_pannel_gpio_deal_alm_status(void)
{		
	if(g_flag_work_mode == USR_WORK_MODE_READY)
	{
		if((G_no_IO != 1)&&(G_Alm_num >= 1))
		{
			usr_led_clr_flag_output(USR_LED_ID_R1);
			usr_led_clr_flag_overturn(USR_LED_ID_R1);
		}
		if((G_no_IO != 2)&&(G_Alm_num >= 2))
		{
			usr_led_clr_flag_output(USR_LED_ID_R2);
			usr_led_clr_flag_overturn(USR_LED_ID_R2);
		}
		if((G_no_IO != 3)&&(G_Alm_num >= 3))
		{
			usr_led_clr_flag_output(USR_LED_ID_R3);
			usr_led_clr_flag_overturn(USR_LED_ID_R3);
		}
		if((G_no_IO != 4)&&(G_Alm_num >= 4))
		{
			usr_led_clr_flag_output(USR_LED_ID_R4);
			usr_led_clr_flag_overturn(USR_LED_ID_R4);
		}
		if((G_no_IO != 5)&&(G_Alm_num >= 5))
		{
			usr_led_clr_flag_output(USR_LED_ID_R5);
			usr_led_clr_flag_overturn(USR_LED_ID_R5);
		}
		if((G_no_IO != 6)&&(G_Alm_num >= 6))
		{
			usr_led_clr_flag_output(USR_LED_ID_R6);
			usr_led_clr_flag_overturn(USR_LED_ID_R6);
		}
		if((G_no_IO != 7)&&(G_Alm_num >= 7))
		{
			usr_led_clr_flag_output(USR_LED_ID_R7);
			usr_led_clr_flag_overturn(USR_LED_ID_R7);
		}
		if((G_no_IO != 8)&&(G_Alm_num >= 8))
		{
			usr_led_clr_flag_output(USR_LED_ID_R8);
			usr_led_clr_flag_overturn(USR_LED_ID_R8);
		}
	}
	else
	{
		if(G_no_IO != 1)
		{
			if(((usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_1))&&(G_Alm_num >= 1))||(flag_check_led[USR_LIST_ALM_GROUP_1]))
				usr_led_set_flag_output(USR_LED_ID_R1);
			else
				usr_led_clr_flag_output(USR_LED_ID_R1);
		}
			
		if(G_no_IO != 2)
		{
			if(((usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_2))&&(G_Alm_num >= 2))||(flag_check_led[USR_LIST_ALM_GROUP_2]))
				usr_led_set_flag_output(USR_LED_ID_R2);
			else
				usr_led_clr_flag_output(USR_LED_ID_R2);
		}
	
		if(G_no_IO != 3)
		{		
			if(((usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_3))&&(G_Alm_num >= 3))||(flag_check_led[USR_LIST_ALM_GROUP_3]))
				usr_led_set_flag_output(USR_LED_ID_R3);
			else
				usr_led_clr_flag_output(USR_LED_ID_R3);
		}
			
		if(G_no_IO != 4)
		{
			if(((usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_4))&&(G_Alm_num >= 4))||(flag_check_led[USR_LIST_ALM_GROUP_4]))
				usr_led_set_flag_output(USR_LED_ID_R4);
			else
				usr_led_clr_flag_output(USR_LED_ID_R4);
		}
	
		if(G_no_IO != 5)
		{
			if(((usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_5))&&(G_Alm_num >= 5))||(flag_check_led[USR_LIST_ALM_GROUP_5]))
				usr_led_set_flag_output(USR_LED_ID_R5);
			else
				usr_led_clr_flag_output(USR_LED_ID_R5);
		}
			
		if(G_no_IO != 6)
		{
			if(((usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_6))&&(G_Alm_num >= 6))||(flag_check_led[USR_LIST_ALM_GROUP_6]))
				usr_led_set_flag_output(USR_LED_ID_R6);
			else
				usr_led_clr_flag_output(USR_LED_ID_R6);
		}
	
		if(G_no_IO != 7)
		{		
			if(((usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_7))&&(G_Alm_num >= 7))||(flag_check_led[USR_LIST_ALM_GROUP_7]))
				usr_led_set_flag_output(USR_LED_ID_R7);
			else
				usr_led_clr_flag_output(USR_LED_ID_R7);
		}
			
		if(G_no_IO != 8)
		{
			if(((usr_get_list_group_alm_len(USR_LIST_ALM_GROUP_8))&&(G_Alm_num >= 8))||(flag_check_led[USR_LIST_ALM_GROUP_8]))
				usr_led_set_flag_output(USR_LED_ID_R8);
			else
				usr_led_clr_flag_output(USR_LED_ID_R8);
		}
	}

/*
	if(g_current_list == USR_LCD_PAGE_LIST_ALM_TOTAL)
	{
		usr_led_clr_flag_output(USR_LED_ID_R1);
		usr_led_clr_flag_output(USR_LED_ID_R2);
		usr_led_clr_flag_output(USR_LED_ID_R3);
		usr_led_clr_flag_output(USR_LED_ID_R4);
		usr_led_clr_flag_output(USR_LED_ID_R5);
	}
	else if(g_current_list == USR_LCD_PAGE_LIST_ALM_1)
	{
		usr_led_set_flag_output(USR_LED_ID_R1);
		usr_led_clr_flag_output(USR_LED_ID_R2);
		usr_led_clr_flag_output(USR_LED_ID_R3);
		usr_led_clr_flag_output(USR_LED_ID_R4);
		usr_led_clr_flag_output(USR_LED_ID_R5);
	}
	else if(g_current_list == USR_LCD_PAGE_LIST_ALM_2)
	{
		usr_led_clr_flag_output(USR_LED_ID_R1);
		usr_led_set_flag_output(USR_LED_ID_R2);
		usr_led_clr_flag_output(USR_LED_ID_R3);
		usr_led_clr_flag_output(USR_LED_ID_R4);
		usr_led_clr_flag_output(USR_LED_ID_R5);
	}
	else if(g_current_list == USR_LCD_PAGE_LIST_ALM_3)
	{
		usr_led_clr_flag_output(USR_LED_ID_R1);
		usr_led_clr_flag_output(USR_LED_ID_R2);
		usr_led_set_flag_output(USR_LED_ID_R3);
		usr_led_clr_flag_output(USR_LED_ID_R4);
		usr_led_clr_flag_output(USR_LED_ID_R5);
	}
	else if(g_current_list == USR_LCD_PAGE_LIST_ALM_4)
	{
		usr_led_clr_flag_output(USR_LED_ID_R1);
		usr_led_clr_flag_output(USR_LED_ID_R2);
		usr_led_clr_flag_output(USR_LED_ID_R3);
		usr_led_set_flag_output(USR_LED_ID_R4);
		usr_led_clr_flag_output(USR_LED_ID_R5);
	}
	else if(g_current_list == USR_LCD_PAGE_LIST_ALM_5)
	{
		usr_led_clr_flag_output(USR_LED_ID_R1);
		usr_led_clr_flag_output(USR_LED_ID_R2);
		usr_led_clr_flag_output(USR_LED_ID_R3);
		usr_led_clr_flag_output(USR_LED_ID_R4);
		usr_led_set_flag_output(USR_LED_ID_R5);
	}
	else
	{
		usr_led_clr_flag_output(USR_LED_ID_R1);
		usr_led_clr_flag_output(USR_LED_ID_R2);
		usr_led_clr_flag_output(USR_LED_ID_R3);
		usr_led_clr_flag_output(USR_LED_ID_R4);
		usr_led_clr_flag_output(USR_LED_ID_R5);
	}
*/
}


/************************************************************
* usr_pannel_gpio_deal_duty_status() 
************************************************************/
void usr_pannel_gpio_deal_duty_status(void)
{
	int i,j;
	int light;
	
	if(g_current_page == USR_LCD_PAGE_CFG_CHECK)
		usr_led_set_flag_output(g_flash_module_identity_light);
	else
		usr_led_clr_flag_output(g_flash_module_identity_light);
		
	j = 16-G_Alm_num;
	if(j>0)
	{
		for(i=0;i<j;i++)
		{
			switch(i)
			{
			case 0:
					light = USR_LED_ID_L1;
					break;
	
			case 1:
					light = USR_LED_ID_L2;
					break;
					
			case 2:
					light = USR_LED_ID_L3;
					break;				
					
			case 3:
					light = USR_LED_ID_L4;
					break;
					
			case 4:
					light = USR_LED_ID_L5;
					break;
	
			case 5:
					light = USR_LED_ID_L6;
					break;				
					
			case 6:
					light = USR_LED_ID_L7;
					break;	
					
			case 7:
					light = USR_LED_ID_L8;
					break;
					
			case 8:
					light = USR_LED_ID_R8;
					break;				
					
			case 9:
					light = USR_LED_ID_R7;
					break;				
					
			case 10:
					light = USR_LED_ID_R6;
					break;
					
			case 11:
					light = USR_LED_ID_R5;
					break;				
															
			case 12:
					light = USR_LED_ID_R4;
					break;
					
			case 13:
					light = USR_LED_ID_R3;
					break;				
					
			case 14:
					light = USR_LED_ID_R2;
					break;	
					
			case 15:
					light = USR_LED_ID_R1;
					break;
					
			default:
				light = -1;
			}				
								
			if(light == -1)
				;		
			else if(light_duty[i] == 0)//no use
				usr_led_clr_flag_output(light);
			else
			{
				if(light_duty[i] == 2) // duty
					usr_led_set_flag_output(light);
				else if(light_duty[i] == 1) // no duty
					usr_led_clr_flag_output(light);
				else
					usr_led_clr_flag_output(light);
					
				if(g_flash_module_identity_light == light)	
				{
					if(light_duty[i] == 2)
						g_flag_work_mode = USR_WORK_MODE_DUTY_ON;
					else
						g_flag_work_mode = USR_WORK_MODE_DUTY_OFF;
				}
			}
		}
	}
}


/************************************************************
* usr_pannel_gpio_deal_unit_status() 
************************************************************/
void usr_pannel_gpio_deal_unit_status(void)
{
	if(
		((g_flag_macBrk[USR_IP_INTERFACE_1] == true)&&(g_flag_log_macBrk[USR_IP_INTERFACE_1] == false)) ||
		/*((g_flag_macBrk[USR_IP_INTERFACE_2] == true)&&(g_flag_log_macBrk[USR_IP_INTERFACE_2] == false)) ||*/
		((g_flag_log_netAlive[USR_IP_INTERFACE_1] == false)&&(g_flag_log_netAlive[USR_IP_INTERFACE_1] == true)) /*||
		((g_flag_log_netAlive[USR_IP_INTERFACE_2] == false)&&(g_flag_log_netAlive[USR_IP_INTERFACE_2] == true))*/
		)
		usr_led_set_flag_overturn(USR_LED_ID_UNIT_FAIL);

	if(
		(g_flag_macBrk[USR_IP_INTERFACE_1] == true) ||
		/*(g_flag_macBrk[USR_IP_INTERFACE_2] == true) ||*/
		(g_flag_log_netAlive[USR_IP_INTERFACE_1] == false) /*||
		(g_flag_log_netAlive[USR_IP_INTERFACE_2] == false) */
		)
		usr_led_set_flag_output(USR_LED_ID_UNIT_FAIL);
	else
		usr_led_clr_flag_output(USR_LED_ID_UNIT_FAIL);

	g_flag_log_macBrk[USR_IP_INTERFACE_1]	= g_flag_macBrk[USR_IP_INTERFACE_1];
	//g_flag_log_macBrk[USR_IP_INTERFACE_2]	= g_flag_macBrk[USR_IP_INTERFACE_2];
	g_flag_log_netAlive[USR_IP_INTERFACE_1]	= g_flag_netAlive[USR_IP_INTERFACE_1];
	//g_flag_log_netAlive[USR_IP_INTERFACE_2]	= g_flag_netAlive[USR_IP_INTERFACE_2];
}


/************************************************************
* usr_pannel_gpio_debug_call() 
************************************************************/
void usr_pannel_gpio_debug_call(void)
{
	if(usr_key_isup(USR_DEBUG_ENGINE_CALL_KEY))
		usr_sailor_engine_happen_call();

	if(usr_key_isup(USR_DEBUG_BRIDGE_CALL_KEY))
		usr_sailor_bridge_happen_call();
}


/************************************************************
* usr_pannel_gpio_deal_check() 
************************************************************/
void usr_pannel_gpio_deal_check(void)
{
	int i;
	if(usr_key_ishigh(USR_KEY_ID_UP)&&usr_key_ishigh(USR_KEY_ID_DOWN))
	{
		g_current_page = USR_LCD_PAGE_CFG_CHECK;
		g_current_list = 33;
		for(i=USR_LIST_ALM_GROUP_1;i<USR_LIST_ALM_GROUP_16;i++)
			flag_check_led[i] = 0;
	}
	else
		;
}

/************************************************************
* usr_pannel_gpio_deal_clrLcd() 
************************************************************/
void usr_pannel_gpio_deal_clrLcd(void)
{
	if(usr_key_isup(USR_KEY_ID_ESC)&&usr_led_get_testlight())
	{
		lcd_init(&g_GPIO_OUT[USR_GPIO_OUT_CH_2]);  
	}
	else
		;
}








