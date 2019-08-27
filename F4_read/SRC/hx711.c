#include "hx711.h"
#include "stm32f4xx.h"


	float hx711_N[4] = {0};			//测量力
	
void hx711_init(void){
	RCC ->AHB1ENR |= (0x1<<5);//GPIOF使能
	GPIO_Set(GPIOF,PIN0|PIN2|PIN4|PIN6,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //in
	GPIO_Set(GPIOF,PIN1|PIN3|PIN5|PIN7,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //out
}

void delay(uint16_t i){
	while(i--);
}

int32_t ReadCountA(void)
{
 uint32_t Count;
 uint8_t i;
 uint16_t  return_count = 0;
	delay(1);
	SCK1 = 0;//LL_GPIO_ResetOutputPin(GPIOA,sck_pin);//ADSK=0; //使能AD（PD_SCK 置低）
	Count=0;
	delay(1);
 while(DAT1)//while(ADDO); //AD转换未结束则等待，否则开始读取
 {
//	 rt_kprintf("wait=%d...\r\n",return_count);
	 delay(10);
	 return_count++;
		if(return_count>1000){
			rt_kprintf("error A return\r\n");
			return 0;
		}
 }
	delay(1);
 for (i=0;i<24;i++)
 {
 SCK1 = 1;//LL_GPIO_SetOutputPin(GPIOA,sck_pin);//ADSK=1; //PD_SCK 置高（发送脉冲）
	 delay(1);
 Count=Count<<1; //下降沿来时变量Count左移一位，右侧补零
 SCK1 = 0;//LL_GPIO_ResetOutputPin(GPIOA,sck_pin);//ADSK=0; //PD_SCK 置低
	 delay(1);
 if(DAT1) Count++;
	 delay(1);
 }
 SCK1 = 1;//LL_GPIO_SetOutputPin(GPIOA,sck_pin);//ADSK=1;
 delay(1);
 Count=Count^0x800000;//第25个脉冲下降沿来时，转换数据
 SCK1 = 0;//LL_GPIO_ResetOutputPin(GPIOA,sck_pin);//ADSK=0;
 delay(1);
 return(Count);
}

int32_t ReadCountB(void)
{
 uint32_t Count;
 uint8_t i;
 uint16_t  return_count = 0;
	delay(1);
	SCK2 = 0;//LL_GPIO_ResetOutputPin(GPIOA,sck_pin);//ADSK=0; //使能AD（PD_SCK 置低）
	Count=0;
	delay(1);
 while(DAT2)//while(ADDO); //AD转换未结束则等待，否则开始读取
 {
//	 rt_kprintf("wait=%d...\r\n",return_count);
	 delay(20);
	 return_count++;
		if(return_count>1000){
			rt_kprintf("error B return\r\n");
			return 0;
		}
 }
	delay(1);
 for (i=0;i<24;i++)
 {
 SCK2 = 1;//LL_GPIO_SetOutputPin(GPIOA,sck_pin);//ADSK=1; //PD_SCK 置高（发送脉冲）
	 delay(1);
 Count=Count<<1; //下降沿来时变量Count左移一位，右侧补零
 SCK2 = 0;//LL_GPIO_ResetOutputPin(GPIOA,sck_pin);//ADSK=0; //PD_SCK 置低
	 delay(1);
 if(DAT2) Count++;
	 delay(1);
 }
 SCK2 = 1;//LL_GPIO_SetOutputPin(GPIOA,sck_pin);//ADSK=1;
 delay(1);
 Count=Count^0x800000;//第25个脉冲下降沿来时，转换数据
 SCK2 = 0;//LL_GPIO_ResetOutputPin(GPIOA,sck_pin);//ADSK=0;
 delay(1);
 return(Count);
}

int32_t ReadCountC(void)
{
 uint32_t Count;
 uint8_t i;
 uint16_t  return_count = 0;
	delay(1);
	SCK3 = 0;//LL_GPIO_ResetOutputPin(GPIOA,sck_pin);//ADSK=0; //使能AD（PD_SCK 置低）
	Count=0;
	delay(1);
 while(DAT3)//while(ADDO); //AD转换未结束则等待，否则开始读取
 {
//	 rt_kprintf("wait=%d...\r\n",return_count);
	 delay(10);
	 return_count++;
		if(return_count>1000){
			rt_kprintf("error C return\r\n");
			return 0;
		}
 }
	delay(1);
 for (i=0;i<24;i++)
 {
 SCK3 = 1;//LL_GPIO_SetOutputPin(GPIOA,sck_pin);//ADSK=1; //PD_SCK 置高（发送脉冲）
	 delay(1);
 Count=Count<<1; //下降沿来时变量Count左移一位，右侧补零
 SCK3 = 0;//LL_GPIO_ResetOutputPin(GPIOA,sck_pin);//ADSK=0; //PD_SCK 置低
	 delay(1);
 if(DAT3) Count++;
	 delay(1);
 }
 SCK3 = 1;//LL_GPIO_SetOutputPin(GPIOA,sck_pin);//ADSK=1;
 delay(1);
 Count=Count^0x800000;//第25个脉冲下降沿来时，转换数据
 SCK3 = 0;//LL_GPIO_ResetOutputPin(GPIOA,sck_pin);//ADSK=0;
 delay(1);
 return(Count);
}

int32_t ReadCountD(void)
{
 uint32_t Count;
 uint8_t i;
 uint16_t  return_count = 0;
	delay(1);
	SCK4 = 0;//LL_GPIO_ResetOutputPin(GPIOA,sck_pin);//ADSK=0; //使能AD（PD_SCK 置低）
	Count=0;
	delay(1);
 while(DAT4)//while(ADDO); //AD转换未结束则等待，否则开始读取
 {
//	 rt_kprintf("wait=%d...\r\n",return_count);
	 delay(10);
	 return_count++;
		if(return_count>1000){
			rt_kprintf("error D return\r\n");
			return 0;
		}
 }
	delay(1);
 for (i=0;i<24;i++)
 {
 SCK4 = 1;//LL_GPIO_SetOutputPin(GPIOA,sck_pin);//ADSK=1; //PD_SCK 置高（发送脉冲）
	 delay(1);
 Count=Count<<1; //下降沿来时变量Count左移一位，右侧补零
 SCK4 = 0;//LL_GPIO_ResetOutputPin(GPIOA,sck_pin);//ADSK=0; //PD_SCK 置低
	 delay(1);
 if(DAT4) Count++;
	 delay(1);
 }
 SCK4 = 1;//LL_GPIO_SetOutputPin(GPIOA,sck_pin);//ADSK=1;
 delay(1);
 Count=Count^0x800000;//第25个脉冲下降沿来时，转换数据
 SCK4 = 0;//LL_GPIO_ResetOutputPin(GPIOA,sck_pin);//ADSK=0;
 delay(1);
 return(Count);
}


void read_Hx711(void){
	uint32_t hx711_ans[4] = {0};
	 
		if(swt[1]){
			hx711_ans[0]=ReadCountA();
			hx711_N[0] = hx711_ans[0]*50/8388608.0*2.55*9.8-1249.5;
//			printf("A=%d,%f\r\n",hx711_ans[0],hx711_N[0]);
			if(hx711_ans[0])
				ano_o_F2[0]=hx711_N[0];
		}else{
				hx711_N[0] = 0;
				ano_o_F2[0]=0;
		}
		
		if(swt[2]){
			hx711_ans[1]=ReadCountB();
			hx711_N[1] = hx711_ans[1]*50/8388608.0*2.55*9.8-1249.5;
//			printf("B=%d,%f\r\n",hx711_ans[1],hx711_N[1]);
			if(hx711_ans[1])
				ano_o_F2[1]=hx711_N[1];
		}else{
				hx711_N[1] = 0;
				ano_o_F2[1]=0;
		}
		
		if(swt[3]){
			hx711_ans[2]=ReadCountC();
			hx711_N[2] = hx711_ans[2]*50/8388608.0*2.55*9.8-1249.5;
//			printf("C=%d,%f\r\n",hx711_ans[2],hx711_N[2]);
			if(hx711_ans[2])
				ano_o_F2[2]=hx711_N[2];
		}else{
				hx711_N[2] = 0;
				ano_o_F2[2]=0;
		}
		
		if(swt[4]){
			hx711_ans[3]=ReadCountD();
			hx711_N[3] = hx711_ans[3]*50/8388608.0*2.55*9.8-1249.5;
//			printf("D=%d,%f\r\n\r\n",hx711_ans[3],hx711_N[3]);
			if(hx711_ans[3])
				ano_o_F2[3]=hx711_N[3];
		}else{
				hx711_N[3] = 0;
				ano_o_F2[3]=0;
		}
		
}

