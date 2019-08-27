#include "main.h"

int main(void)
{			
	u32 hb=0;
	Stm32_Clock_Init(336,8,2,7);		//ϵͳʱ�ӳ�ʼ��
	delay_init(168);								//��ʱ������ʼ��
	hx711_init();										//GPIOF0-7������ʼ��
	uart_init(84,460800);						//
	RS485_Init(42,115200);					//��ʼ��RS485 usart2
	RS485_Init2(42,115200);					//��ʼ��RS485 usart3
	uart4_init(42,460800);					//ZD_left
	uart5_init(42,460800);					//ZD_right
	printf("\r\nsys init\r\n\r\n");
	
	while(1){
		hb++;													
		read_Imu();										//swt 789 ���ƽǶ� ���ٶ� ���ٶ�	
		imu_find_point();							//��Ҫ��imu�߼� idΪ������imu��������
		imu_find_point2();						//idΪ˫����imu��������
		//send_ANO();										//������ӡ
		com_F7();											//���䵽F7
		//if(hb%2) read_Hx711();				//swt 0123����hx711 1234
		printf("%.2f %.2f %.2f %.2f %.2f %.2f \r\n",m[0],m[1],stcIMU[3].x_Angle,stcIMU[4].x_Angle,target_N[2],target_N[3]);
	}
	
}
