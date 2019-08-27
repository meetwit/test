#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  


//模式控制
#define RS485_TX_EN		PAout(4)	//485模式控制.0,接收;1,发送.
#define RS485_TX_EN2		PEout(15)	//485模式控制.0,接收;1,发送.

														 
void RS485_Init(u32 pclk2,u32 bound);
void RS485_Send_Data(u8 *buf,u8 len); 
void RS485_Init2(u32 pclk2,u32 bound);
void RS485_Send_Data2(u8 *buf,u8 len); 
#endif	   
















