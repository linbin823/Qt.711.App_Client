/* $Id: gpio_header.h,v 1.1.2.1 2008/02/12 13:28:38 svemula Exp $ */

#ifndef __INCgpioctrh
#define __INCgpioctrh
/***************************** Include Files ********************************/
#include <xbasic_types.h>
#include <xstatus.h>
#include <xparameters.h>

/************************** Constant Definitions ****************************/

//#define GPIO_STD_FUNC

#define GPIO_CHANNELA		1
#define GPIO_CHANNELA_DIR	0x0000
#define GPIO_CHANNELA_DFT	0x0000
#define GPIO_CHANNELB		2
#define GPIO_CHANNELB_DIR	0xFFFF

#define GPIO_CHA_TRI_ADDR	(XPAR_XPS_GPIO_0_BASEADDR+0x04)
#define GPIO_CHB_TRI_ADDR	(XPAR_XPS_GPIO_0_BASEADDR+0x0C)
#define GPIO_OUT_ADDR		(XPAR_XPS_GPIO_0_BASEADDR+0x00)
#define GPIO_IN_ADDR		(XPAR_XPS_GPIO_0_BASEADDR+0x08)

#define GPIO_0_CHA_TRI_ADDR	(XPAR_XPS_GPIO_1_BASEADDR+0x04)
#define GPIO_0_OUT_ADDR		(XPAR_XPS_GPIO_1_BASEADDR+0x00)
#define GPIO_0_IN_ADDR		(XPAR_XPS_GPIO_1_BASEADDR+0x08)

#define HS_RESET_FIELD		0x0040
#define HS_TSET_FIELD		0x0020

#define HS_RESET_MASK		0x00DF
#define HS_TSET_MASK		0x00BF

#define DATA_LSB_FIELD		0x0001
#define DATA_LBYTE_FIELD	0x00FF
#define DATA_HBYTE_FIELD	0xFF00

#define XU_GPIO_Out32(OutputPtr, Value) \
	(*(volatile u32 *)((OutputPtr)) = (Value))
	
#define XU_GPIO_In32(InputPtr)  (*(volatile u32 *)(InputPtr))

/************************** Function Prototypes ****************************/


#endif	//__INCgpioctrh


