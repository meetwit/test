#include "sys.h"		    
#include "rs485.h"	 
#include "jy901.h"


void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART2->SR&(1<<5))//���յ�����
	{	 
		res=USART2->DR; 			 
		CopeSerialData(res);
	}  											 
} 
						
void USART3_IRQHandler(void)
{
	u8 res;	    
	if(USART3->SR&(1<<5))//���յ�����
	{	 
		res=USART3->DR; 			 
		CopeSerialData2(res);
	}  											 
} 

//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz),APB1һ��Ϊ42Mhz
//bound:������	  
void RS485_Init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	
	RCC->AHB1ENR|=1<<0;   		//ʹ��PORTA��ʱ�� 
	GPIO_Set(GPIOA,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); 		//PA4�������
	GPIO_Set(GPIOA,PIN2|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);	//PA2,PA3,���ù���,���� 
 	GPIO_AF_Set(GPIOA,2,7);		//PA2,AF7
	GPIO_AF_Set(GPIOA,3,7);		//PA3,AF7  	   
 
	RCC->APB1ENR|=1<<17;  		//ʹ�ܴ���2ʱ��  
	RCC->APB1RSTR|=1<<17;   	//��λ����2
	RCC->APB1RSTR&=~(1<<17);	//ֹͣ��λ	   	   
	//����������
 	USART2->BRR=mantissa; 		// ����������	 
	USART2->CR1|=0X200C;  		//1λֹͣ,��У��λ.
	
	//ʹ�ܽ����ж� 
	USART2->CR1|=1<<2;  		//���ڽ���ʹ��
	USART2->CR1|=1<<5;    		//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(1,1,USART2_IRQn,2);//��2��������ȼ� 

	RS485_TX_EN=0;				//Ĭ��Ϊ����ģʽ	
}

//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;			//����Ϊ����ģʽ
  	for(t=0;t<len;t++)		//ѭ����������
	{
		while((USART2->SR&0X40)==0);//�ȴ����ͽ���		  
		USART2->DR=buf[t];
	}	 
	while((USART2->SR&0X40)==0);//�ȴ����ͽ���	
	RS485_TX_EN=0;				//����Ϊ����ģʽ	
}

//��ʼ��IO ����3 rs485init
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void RS485_Init2(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV@OVER8=0
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������@OVER8=0 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->AHB1ENR|=1<<1;   	//ʹ��PORTB��ʱ��  
	RCC->AHB1ENR|=1<<4;   	//ʹ��PORTE��ʱ��  
	RCC->APB1ENR|=1<<18;  	//ʹ�ܴ���3ʱ�� 
	GPIO_Set(GPIOE,PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); 		//PE15�������
	GPIO_Set(GPIOB,PIN11|PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);
 	GPIO_AF_Set(GPIOB,11,7);	//PB11,AF7
	GPIO_AF_Set(GPIOB,10,7);//PB10,AF7  	   
	//����������
 	USART3->BRR=mantissa; 	//����������	 
	USART3->CR1&=~(1<<15); 	//����OVER8=0 
	USART3->CR1|=1<<3;  	//���ڷ���ʹ�� 
	
	USART3->CR1|=1<<2;  		//���ڽ���ʹ��
	USART3->CR1|=1<<5;    		//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(1,1,USART3_IRQn,2);//��2��������ȼ� 
	
	USART3->CR1|=1<<13;  	//����ʹ��
}

//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data2(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN2=1;			//����Ϊ����ģʽ
  	for(t=0;t<len;t++)		//ѭ����������
	{
		while((USART3->SR&0X40)==0);//�ȴ����ͽ���		  
		USART3->DR=buf[t];
	}	 
	while((USART3->SR&0X40)==0);//�ȴ����ͽ���	
	RS485_TX_EN2=0;				//����Ϊ����ģʽ	
}

