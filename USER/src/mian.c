#include "stm32f10x.h"
#include "tim2.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "stdio.h" 
#include "math.h" 


int main(){
	
	/*定时器2初始化*/
	init_tim2(5);			//定时器2的中断时间间隔，单位ms
	
	/*串口初始化，波特率为115200*/
	usart_init(115200);				//串口1
	usart2_init(115200);			//串口2
	usart3_init(115200);			//串口3
	
	printf("start\r\n");
	
	while(1){
		
		Task_Pc();
		Task_Pc2();
		Task_Pc3();

		
	}

}







