#include "main.h"

#define MaxN 200
#define MinN 10


	float cal_k3,cal;	 
/*
algorithm 0 first version
algorithm 1 ����Ƕȵ�Ŀ�������

*/
#define algorithm 0		

/*		      		 0 1 2 3 4 x 6 7 8 9*/
/*		      		 0 1 2 3 4 5 6 7 8 9*/
uint8_t swt[10]={1,0,0,0,0,0,0,1,1,0}; //ȫ��һЩ״̬���صĿ���
float target_N[4]={10,10,10,10};			//0:˫����ǰ 1��������ǰ 2:˫���ź� 3�������ź� 
float ano_o_F1[14]={0};
float flag_fl;
float flag_fr;
float flag_bl;
float flag_br;
void send_F7(void){
//��Ŀ���� target_N[4]�������� hx711_N[4] ͨ������5������F7	float
//����5���ֽڷ��ͺ���	PcTx_Byte5(u8 data)		unsigned char
	PcTx_Byte5('T');
	
	PcTx_Byte5((u8)(hx711_N[0]/2));
	PcTx_Byte5((u8)(hx711_N[1]/2));
	PcTx_Byte5((u8)(hx711_N[2]/2));
	PcTx_Byte5((u8)(hx711_N[3]/2));
	
	PcTx_Byte5((u8)(target_N[0]/2));
	PcTx_Byte5((u8)(target_N[1]/2));
	PcTx_Byte5((u8)(target_N[2]/2));
	PcTx_Byte5((u8)(target_N[3]/2));
	
	PcTx_Byte5('E');
}

void com_F7(void){
	float temp_N[2]={0};
	static int flag1=0,flag2=0;
	static float max1=1,max2=1;
						
	
	if(flag1!=0&&flag2!=0){                   //�ǶȲ���
	cal=max2-max1;
	flag1=0;
	flag2=0;
}
	
	
	ano_o_F3[7] = state_S*5;	
	ano_o_F3[8] = state_F*6;
	
	temp_N[1] = generatrForce(state_F,cal_k3,stcIMU[3].x_w);
		if(state_F==1){											//״̬�л�
			state_F=12;
			flag1++;
			if(flag1==1){
				max1=stcIMU[3].x_Angle;
			}
			flag_br=1;
		}
		else
		if(state_F==2){
			state_F=23;
		}
		else
		if(state_F==3){
			state_F=34;
			flag_fr=1;
		}
		else
		if(state_F==4){
			state_F=41;
		}
	
	temp_N[0] = generatrForce2(state_S,stcIMU[4].x_Angle,stcIMU[4].x_w);
	
	if(state_S==1){
		state_S=12;
		flag2++;
		if(flag2==1){
			max2=stcIMU[4].x_Angle;
		}
		flag_bl=1;
	}
	else
	if(state_S==2){
		state_S=23;
	}
	else
	if(state_S==3){
		state_S=34;
		flag_fl=1;
	}
	else
	if(state_S==4){
		state_S=41;
	}
	
	if(state_F==12||state_F==23){					//����л�
		target_N[3] = temp_N[1];
		target_N[1] = MinN;
	}
	if(state_F==34||state_F==41){
		target_N[1] = temp_N[1];
		target_N[3] = MinN;
	}
	if(state_S==12||state_S==23){
		target_N[2] = temp_N[0];
		target_N[0] = MinN;
	}
	if(state_S==34||state_S==41){
		target_N[0] = temp_N[0];
		target_N[2] = MinN;
	}
	if(state_F==5){
		target_N[1]=MinN;
		target_N[3]=MinN;
		flag1=0;
		flag2=0;
	}
	if(state_S==5){
		target_N[0] = MinN;
		target_N[2] = MinN;
		flag1=0;
		flag2=0;
	}
	
	ano_o_F3[0] = target_N[0];						//�����������F3 0123
	ano_o_F3[1] = target_N[1];
	ano_o_F3[2] = target_N[2];
	ano_o_F3[3] = target_N[3];
	
	
	if(swt[6]){
		send_F7();
	}
}


void send_ANO(void){
	ano_o_F1[0] = hx711_N[2];
	ano_o_F1[1] = hx711_N[3];
	ano_o_F1[2] = hx711_N[0];
	ano_o_F1[3] = hx711_N[1];
	ano_o_F1[4] = target_N[0];
	ano_o_F1[5] = target_N[2];
	ano_o_F1[6] = target_N[1];
	ano_o_F1[7] = target_N[3];
	ano_o_F1[8] = stcIMU[3].x_Angle;
	ano_o_F1[9] = stcIMU[4].x_Angle;
	ano_o_F1[10] = flag_fl;
	ano_o_F1[11] = flag_bl;
//	ano_o_F1[12] = flag_fr;
//	ano_o_F1[13] = flag_br;
	//ANO_send(0xf1,(unsigned char *)ano_o_F1,sizeof(float),sizeof(ano_o_F1));
	if(swt[5]){
		ANO_send(0xf2,(unsigned char *)ano_o_F2,sizeof(float),sizeof(ano_o_F2));
		ANO_send(0xf3,(unsigned char *)ano_o_F3,sizeof(float),sizeof(ano_o_F3));
	}
}



void swt_f(int num,int val){
		swt[num] = val;
		switch(num){
//			case 0 :
//				if(val){
//					rt_kprintf("ȫ��ʱ�Ӵ�ӡ����\r\n");
//				}else{
//					rt_kprintf("ȫ��ʱ�Ӵ�ӡ�ر�\r\n");
//				}
//			break;
				
			case 1 :
				if(val){
					rt_kprintf("����������com1��������\r\n"); 
				}else{
					rt_kprintf("����������com1�رղ���\r\n");
				}
			break;
				
			case 2 :
				if(val){
					rt_kprintf("����������com2��������\r\n");
				}else{
					rt_kprintf("����������com2�رղ���\r\n");
				}
			break;
				
			case 3 :
				if(val){
					rt_kprintf("����������com3��������\r\n");
				}else{
					rt_kprintf("����������com3�رղ���\r\n");
				}
			break;
				
			case 4 :
				if(val){
					rt_kprintf("����������com4��������\r\n");
				}else{
					rt_kprintf("����������com4�رղ���\r\n");
				}
			break;
				
			case 5 :
				if(val){
					rt_kprintf("������λ��������ʾ\r\n");
				}else{
					rt_kprintf("������λ���ر���ʾ\r\n");
				}
			break;
				
			case 6 :
				if(val){
					rt_kprintf("F4�����������ݵ�F7\r\n");
				}else{
					rt_kprintf("F4�����������ݵ�F7\r\n");
				}
			break;
				
			case 7 :
				if(val){
					rt_kprintf("imu�Ƕȿ�������\r\n");
				}else{
					rt_kprintf("imu�Ƕȹرղ���\r\n");
				}
			break;
				
			case 8 :
				if(val){
					rt_kprintf("imu���ٶȿ�������\r\n");
				}else{
					rt_kprintf("imu���ٶȹرղ���\r\n");
				}
			break;
				
			case 9 :
				if(val){
					rt_kprintf("imu���ٶȿ�������\r\n");
				}else{
					rt_kprintf("imu���ٶȹرղ���\r\n");
				}
			break;
		}
	
}


void swt_help(){
//		rt_kprintf("������ĸ�ʽ���ԣ��ο����£�\r\n");
//		rt_kprintf("swt_f 0 0 ȫ��ʱ�Ӵ�ӡ�ر�\r\n");
//		rt_kprintf("swt_f 0 1 ȫ��ʱ�Ӵ�ӡ����\r\n");
		rt_kprintf("\r\nswt_f 1 0 ����������com1�رղ���\r\n");
		rt_kprintf("swt_f 1 1 ����������com1��������\r\n");
		rt_kprintf("swt_f 2 0 ����������com2�رղ���\r\n");
		rt_kprintf("swt_f 2 1 ����������com2��������\r\n");
		rt_kprintf("swt_f 3 0 ����������com3�رղ���\r\n");
		rt_kprintf("swt_f 3 1 ����������com3��������\r\n");
		rt_kprintf("swt_f 4 0 ����������com4�رղ���\r\n");
		rt_kprintf("swt_f 4 1 ����������com4��������\r\n");
		rt_kprintf("swt_f 5 0 ������λ���ر���ʾ\r\n");
		rt_kprintf("swt_f 5 1 ������λ��������ʾ\r\n");
		rt_kprintf("swt_f 6 0 F4�����������ݵ�F7\r\n");
		rt_kprintf("swt_f 6 1 F4�����������ݵ�F7\r\n");
		rt_kprintf("swt_f 7 0 imu�Ƕȹرղ���\r\n");
		rt_kprintf("swt_f 7 1 imu�Ƕȿ�������\r\n");
		rt_kprintf("swt_f 8 0 imu���ٶȹرղ���\r\n");
		rt_kprintf("swt_f 8 1 imu���ٶȿ�������\r\n");
		rt_kprintf("swt_f 9 0 imu���ٶȹرղ���\r\n");
		rt_kprintf("swt_f 9 1 imu���ٶȿ�������\r\n");
}

void swt_check(){
		rt_kprintf("\r\n\n״̬��ѯ��\r\n");
//		if(swt[0]){
//					rt_kprintf("ȫ��ʱ�Ӵ�ӡ�ѿ���\r\n");
//				}else{
//					rt_kprintf("ȫ��ʱ�Ӵ�ӡ�ѹر�\r\n");
//				}
		if(swt[1]){
					rt_kprintf("����������com1�ѿ�������\r\n");
				}else{
					rt_kprintf("����������com1�ѹرղ���\r\n");
				}
		if(swt[2]){
					rt_kprintf("����������com2�ѿ�������\r\n");
				}else{
					rt_kprintf("����������com2�ѹرղ���\r\n");
				}
		if(swt[3]){
					rt_kprintf("����������com3�ѿ�������\r\n");
				}else{
					rt_kprintf("����������com3�ѹرղ���\r\n");
				}
		if(swt[4]){
					rt_kprintf("����������com4�ѿ�������\r\n");
				}else{
					rt_kprintf("����������com4�ѹرղ���\r\n");
				}
		if(swt[5]){
					rt_kprintf("������λ���ѿ�����ʾ\r\n");
				}else{
					rt_kprintf("������λ���ѹر���ʾ\r\n");
				}
		if(swt[6]){
					rt_kprintf("F4�����������ݵ�F7\r\n");
				}else{
					rt_kprintf("F4�����������ݵ�F7\r\n");
				}
		if(swt[7]){
					rt_kprintf("imu�Ƕ��ѿ�������\r\n");
				}else{
					rt_kprintf("imu�Ƕ��ѹرղ���\r\n");
				}
		if(swt[8]){
					rt_kprintf("imu���ٶ��ѿ�������\r\n");
				}else{
					rt_kprintf("imu���ٶ��ѹرղ���\r\n");
				}
		if(swt[9]){
					rt_kprintf("imu���ٶ��ѿ�������\r\n");
				}else{
					rt_kprintf("imu���ٶ��ѹرղ���\r\n");
				}
}


float generatrForce(int flag, float angle, float v){
	//Ԥ�����ŵĽǶȷ�Χ���������ʵʱ����ֵ������һ������
	static float max_angle=110,min_angle=60,mid2_angle=85,mid4_angle=85,N,buf12,buf41,k12,b12,k23,b23,k34,b34,k41,b41,max_N1=100,max_N3=60,percent23=10,percent41=50;	
	
	if(flag==1){															//��¼����λ�ýǶ�
		max_angle = angle;		
		k12=max_N1/(mid2_angle-max_angle);
		b12=-max_angle;
		return MinN;
	}else if(flag==3){
		min_angle = angle;
		k34=max_N3/(mid4_angle-min_angle);
		b34=-min_angle;
		return MinN;
	}else if(flag==2){
		mid2_angle = angle;
		k23=max_N1/((1-(percent23/100))*(mid2_angle-min_angle));
		b23=-((1-(percent23/100))*min_angle+(percent23/100)*mid2_angle);
		return N;
	}else if(flag==4){
		mid4_angle = angle;
		k41=max_N3/((1-(percent41/100))*(mid4_angle-max_angle));
		b41=-((percent41/100)*max_angle+(1-(percent41/100))*mid4_angle);
		return N;
	}else if(flag==5){
		return MinN;
	}
	
	if(flag==12){
		buf12=N;
		N=k12*(angle+b12);							//����12��Ŀ����
		if(N<buf12)N=buf12;							//����
	}
	else
	if(flag==23){
		buf12=0;													//������ʼ��
		N=k23*(angle+b23);								//����23��Ŀ����
		//if(v>-30&&angle<(-b23)) N=MinN;		//��ֹ��������
	}
	else
	if(flag==34){
		N=k34*(angle+b34);								//����34��Ŀ����
		buf41=500;												//������ʼ��
	}
	else
	if(flag==41){
		buf41=N;													//����41��Ŀ����
		N=k41*(angle+b41);								//����
		if(N>buf41) N=buf41;
		//if(v<30&&angle>(-b41)) N=MinN;		//��ֹ��������
	}
	if(flag==12||flag==23){							//�����ź������
		if(N>max_N1)N=max_N1;
	}
	if(flag==34||flag==41){							//������ǰ�����
		if(N>max_N3)N=max_N3;
	}
	if(N<MinN) N = MinN;								//������С��

	return N;
}

/*
��������generatrForce2
�� 	����int flag, 		״̬��״̬
				float angle,	�Ƕ� 
				float v				���ٶ�
����ֵ��(float)				Ŀ����
*/
float generatrForce2(int flag, float angle, float v){	
		static float max_angle=110,min_angle=60,mid2_angle=85,mid4_angle=85,N,buf12,buf41,k12,b12,k23,b23,k34,b34,k41,b41,max_N1=100,max_N3=60,percent23=10,percent41=50;	
	
	if(flag==1){															//��¼����λ�ýǶ�
		max_angle = angle;		
		k12=max_N1/(mid2_angle-max_angle);
		b12=-max_angle;
		return MinN;
	}else if(flag==3){
		min_angle = angle;
		k34=max_N3/(mid4_angle-min_angle);
		b34=-min_angle;
		return MinN;
	}else if(flag==2){
		mid2_angle = angle;
		k23=max_N1/((1-(percent23/100))*(mid2_angle-min_angle));
		b23=-((1-(percent23/100))*min_angle+(percent23/100)*mid2_angle);
		return N;
	}else if(flag==4){
		mid4_angle = angle;
		k41=max_N3/((1-(percent41/100))*(mid4_angle-max_angle));
		b41=-((percent41/100)*max_angle+(1-(percent41/100))*mid4_angle);
		return N;
	}else if(flag==5){
		return MinN;
	}
	
	if(flag==12){
		buf12=N;
		N=k12*(angle+b12);							//����12��Ŀ����
		if(N<buf12)N=buf12;							//����
	}
	else
	if(flag==23){
		buf12=0;													//������ʼ��
		N=k23*(angle+b23);								//����23��Ŀ����
		if(v>-30&&angle<(-b23)) N=MinN;		//��ֹ��������
	}
	else
	if(flag==34){
		N=k34*(angle+b34);								//����34��Ŀ����
		buf41=500;												//������ʼ��
	}
	else
	if(flag==41){
		buf41=N;													//����41��Ŀ����
		N=k41*(angle+b41);								//����
		if(N>buf41) N=buf41;
		if(v<30&&angle>(-b41)) N=MinN;		//��ֹ��������
	}
	if(flag==12||flag==23){							//�����ź������
		if(N>max_N1)N=max_N1;
	}
	if(flag==34||flag==41){							//������ǰ�����
		if(N>max_N3)N=max_N3;
	}
	if(N<MinN) N = MinN;								//������С��

	return N;
}
