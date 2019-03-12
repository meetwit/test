#include "stm32f10x.h"
#include "tim2.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "stdio.h" 
#include "math.h" 
/*
interrupt����{
////�����ж�
c();	

}

void c(unsigned char date)
{
//date���жϽ��յ���һ���ֽ�
static unsigned char date_buf[6]=0,cmd_flag=0,cmd_index=0;
	switch(date){
		case 'd':
			cmd_flag=1;
			cmd_index=0;
		break;

		case '#'
			cmd_flag=0;
			if(cmd_index==8){
				printf("time:%d%d%d%d-%d%d-%d%d\r\n",date_buf[0],date_buf[1],date_buf[2],date_buf[3],date_buf[4],date_buf[5],date_buf[6],date_buf[7]);
			}
		break;

		default :
			if(cmd_flag)
			{
				date_buf[cmd_index++]=date;
			}
		break;
	}
	
}
*/

int main(){
	
	/*��ʱ��2��ʼ��*/
	init_tim2(5);			//��ʱ��2���ж�ʱ��������λms
	
	/*���ڳ�ʼ����������Ϊ115200*/
	usart_init(115200);				//����1
	usart2_init(115200);			//����2
	usart3_init(115200);			//����3
	
	printf("start\r\n");
	
	while(1){
		
		Task_Pc();
		Task_Pc2();
		Task_Pc3();


		
	}

}







