#ifndef __INCfunch
#define __INCfunch

#define MAX_NUM_POINT		500		//测点最大值
#define ACT_NUM_POINT		245		//实际测点值


//中文还是英文
//#define _IS_LANGUAGE_CH
#define _IS_LANGUAGE_EN

//是否有呼叫应答
#define _IS_CALL_ACK
//#undef _IS_CALL_ACK

//是否有网络应答
#define _IS_NET_ACK
//#undef _IS_NET_ACK

//是否有网络复位
#define _IS_NET_RST
//#undef _IS_NET_RST

//是否有值班
#define _IS_NET_DUTY
//#undef _IS_NET_DUTY

//是否有网络报警接管
#define _IS_NET_BUZZ
//#undef _IS_NET_BUZZ


//是否有特殊继电器直接输出
//#define _IS_IO_RELAY
#undef _IS_IO_RELAY

#define IO_NUMBER		6
#define _IS_IO_LOW		// 低有效
//#undef _IS_IO_LOW// 高有效


#endif	//__INCfunch


