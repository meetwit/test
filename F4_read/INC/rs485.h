#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  


//ģʽ����
#define RS485_TX_EN		PAout(4)	//485ģʽ����.0,����;1,����.
#define RS485_TX_EN2		PEout(15)	//485ģʽ����.0,����;1,����.

														 
void RS485_Init(u32 pclk2,u32 bound);
void RS485_Send_Data(u8 *buf,u8 len); 
void RS485_Init2(u32 pclk2,u32 bound);
void RS485_Send_Data2(u8 *buf,u8 len); 
#endif	   
















