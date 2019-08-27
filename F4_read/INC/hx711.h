#ifndef __HX711_H
#define __HX711_H

#include "sys.h"

#define DAT1 PFin(0)
#define SCK1 PFout(1)
#define DAT2 PFin(2)
#define SCK2 PFout(3)
#define DAT3 PFin(4)
#define SCK3 PFout(5)
#define DAT4 PFin(6)
#define SCK4 PFout(7)

extern float hx711_N[4];
	
void hx711_init(void);
int32_t ReadCountA(void);
int32_t ReadCountB(void);
int32_t ReadCountC(void);
int32_t ReadCountD(void);
void read_Hx711(void);

#endif
