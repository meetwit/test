#ifndef __ANO_H
#define __ANO_H	

 /*meetwit*/
typedef   signed          char s8;
typedef   signed short     int s16;
typedef   signed           int s32;

typedef unsigned          char u8;
typedef unsigned short     int u16;
typedef unsigned           int u32;

extern float ano_o_F2[10];
extern float ano_o_F3[10];
extern float ano_o_F1[14];

void ANO_send(u8 fun,unsigned char * sp,u8 sizenum,u8 len);
	
#endif 






