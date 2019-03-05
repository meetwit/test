#include "stm32f10x.h"
#include "tim2.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "stdio.h" 
#include "math.h" 


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







