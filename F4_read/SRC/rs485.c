#include "sys.h"		    
#include "rs485.h"	 
#include "jy901.h"


void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART2->SR&(1<<5))//接收到数据
	{	 
		res=USART2->DR; 			 
		CopeSerialData(res);
	}  											 
} 
						
void USART3_IRQHandler(void)
{
	u8 res;	    
	if(USART3->SR&(1<<5))//接收到数据
	{	 
		res=USART3->DR; 			 
		CopeSerialData2(res);
	}  											 
} 

//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz),APB1一般为42Mhz
//bound:波特率	  
void RS485_Init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	
	RCC->AHB1ENR|=1<<0;   		//使能PORTA口时钟 
	GPIO_Set(GPIOA,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); 		//PA4推挽输出
	GPIO_Set(GPIOA,PIN2|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);	//PA2,PA3,复用功能,上拉 
 	GPIO_AF_Set(GPIOA,2,7);		//PA2,AF7
	GPIO_AF_Set(GPIOA,3,7);		//PA3,AF7  	   
 
	RCC->APB1ENR|=1<<17;  		//使能串口2时钟  
	RCC->APB1RSTR|=1<<17;   	//复位串口2
	RCC->APB1RSTR&=~(1<<17);	//停止复位	   	   
	//波特率设置
 	USART2->BRR=mantissa; 		// 波特率设置	 
	USART2->CR1|=0X200C;  		//1位停止,无校验位.
	
	//使能接收中断 
	USART2->CR1|=1<<2;  		//串口接收使能
	USART2->CR1|=1<<5;    		//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(1,1,USART2_IRQn,2);//组2，最低优先级 

	RS485_TX_EN=0;				//默认为接收模式	
}

//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;			//设置为发送模式
  	for(t=0;t<len;t++)		//循环发送数据
	{
		while((USART2->SR&0X40)==0);//等待发送结束		  
		USART2->DR=buf[t];
	}	 
	while((USART2->SR&0X40)==0);//等待发送结束	
	RS485_TX_EN=0;				//设置为接收模式	
}

//初始化IO 串口3 rs485init
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void RS485_Init2(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV@OVER8=0
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分@OVER8=0 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->AHB1ENR|=1<<1;   	//使能PORTB口时钟  
	RCC->AHB1ENR|=1<<4;   	//使能PORTE口时钟  
	RCC->APB1ENR|=1<<18;  	//使能串口3时钟 
	GPIO_Set(GPIOE,PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); 		//PE15推挽输出
	GPIO_Set(GPIOB,PIN11|PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);
 	GPIO_AF_Set(GPIOB,11,7);	//PB11,AF7
	GPIO_AF_Set(GPIOB,10,7);//PB10,AF7  	   
	//波特率设置
 	USART3->BRR=mantissa; 	//波特率设置	 
	USART3->CR1&=~(1<<15); 	//设置OVER8=0 
	USART3->CR1|=1<<3;  	//串口发送使能 
	
	USART3->CR1|=1<<2;  		//串口接收使能
	USART3->CR1|=1<<5;    		//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(1,1,USART3_IRQn,2);//组2，最低优先级 
	
	USART3->CR1|=1<<13;  	//串口使能
}

//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data2(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN2=1;			//设置为发送模式
  	for(t=0;t<len;t++)		//循环发送数据
	{
		while((USART3->SR&0X40)==0);//等待发送结束		  
		USART3->DR=buf[t];
	}	 
	while((USART3->SR&0X40)==0);//等待发送结束	
	RS485_TX_EN2=0;				//设置为接收模式	
}

