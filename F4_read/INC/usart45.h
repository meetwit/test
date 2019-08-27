#ifndef __USART45_H
#define __USART45_H 
#include "sys.h"
#include "stdio.h"	  

extern double m[10];
void uart4_init(u32 pclk2,u32 bound);
void PcTx_Byte4(u8 Tx_data4);

void uart5_init(u32 pclk2,u32 bound);
void PcTx_Byte5(u8 Tx_data5);

void cope_uart5(u8 data);
#endif	   
















