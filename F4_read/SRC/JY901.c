#include <string.h>
#include "JY901.h"
#include "stdio.h"
#include "rs485.h"
#include "ANO.h"
#include "thread_mw.h"
u8 state_F=0,state_S=0;
/**********************************************************************************************
jy901 modbus Э�����
�����ļ���jy901.h,jy901.c
��ֲ�٣��޸�ģ�鲨���ʺͳ���485�����ʣ�ʹ֮���
		�ڣ�����RS485_Send_Data����ʵ�壬����ΪҪ���͵��ֽ�
		�ۣ��ڴ����жϵ���CopeSerialData������λ�յ����ֽ�
		�ܣ��������modbus�ڵ�������дimuNum�ĺ궨��
		�ݣ�������ڵ���дallowAddeId�������ݣ����밴��˳�����ȥ����
		�ޣ�������ĵ��ã�ʹ��stcIMU[]�ṹ�����������Ӧ����
		
���ڣ�	2019��1��23��
���ߣ�	meetwit
***********************************************************************************************/

struct STime		stcTime={0};
struct SAcc 		stcAcc={0};
struct SGyro 		stcGyro={0};
struct SAngle 		stcAngle={0};
struct SMag 		stcMag={0};
struct SDStatus 	stcDStatus={0};
struct SPress 		stcPress={0};
struct SLonLat 		stcLonLat={0};
struct SGPSV 		stcGPSV={0};

struct IMU			stcIMU[imuNum]={0};												//������øýṹ�弴��
struct IMU			stcIMU_history[imuNum][3]={0};												//������øýṹ�弴��

unsigned char allowAddeId[imuNum]={0x50,0x51,0x52,0x53,0x54,0x55};				//ϵͳ��modbus�ڵ��ַ

static uint16_t const CRC16Table[256] = {   
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};
 
/*******************************************************************************
*   ��������: CRC16
*   ��������: �������CRC16.
*   �������: dataIn -- ��У������
*             length -- ���ݳ���
*   �� �� ֵ: У��ֵ
*******************************************************************************/
uint16_t CRC16(uint8_t* dataIn, int length)
{  
    uint16_t result  = 0xffff;
    uint16_t tableNo = 0;
 
    for(int i = 0; i < length; i++)
    {
        tableNo = ((result & 0xff) ^ (dataIn[i] & 0xff));
        result  = ((result >> 8) & 0xff) ^ CRC16Table[tableNo];
    }
 
    return result;  
}

/*
��������	send485
��	�Σ�	data
����ֵ��	void
		��	�ã�	485����ʵ�庯��
��	�ڣ�	2019��1��22��
��	�ߣ�	meetwit		
*/		
void send485(unsigned char data){
		RS485_Send_Data(&data,sizeof(unsigned char));	
}


/*
��������	send4852
��	�Σ�	data
����ֵ��	void
		��	�ã�	485����ʵ�庯��
��	�ڣ�	2019��1��22��
��	�ߣ�	meetwit		
*/		
void send4852(unsigned char data){
		RS485_Send_Data2(&data,sizeof(unsigned char));	
}


/*
��������	CopeSerialData
��	�Σ�	ucData �����յ����ֽ�
����ֵ��	void
��	�ã�	����jy901���յ�������
��	�ڣ�	2019��1��22��
��	�ߣ�	meetwit
*/
void CopeSerialData(unsigned char ucData)
{
	static unsigned char ucRxBuffer[11];
	static unsigned char ucRxCnt = 0;	
	static unsigned char ucRxLen = 0;	
	static float temp1 = 208.98;
	static float temp2 = 16.384;
	static float temp3 = 182.04;
	static unsigned char  index = 0;
		
	ucRxBuffer[ucRxCnt++]=ucData;
	if(ucRxCnt==1){
		return;
	}
	if(ucRxCnt==2){
		if (ucRxBuffer[1]!=0x03)
		{
			ucRxCnt=0;																															  
		}
			return;		
	}
	if(ucRxCnt==3){
			ucRxLen = ucRxBuffer[2]+5;
			return;		
	}
	if (ucRxCnt<ucRxLen) {
		return;
		}
	else
	{
			for(index=0;index<imuNum;index++){
				if(stcIMU[index].addrId==ucRxBuffer[0]){
					break;
				}
			}
		if(stcIMU[index].addrReg==Roll){				//�Ƕ�
			stcAngle.Angle[0] = ((unsigned short)ucRxBuffer[3]<<8)|ucRxBuffer[4];
			stcAngle.Angle[1] = ((unsigned short)ucRxBuffer[5]<<8)|ucRxBuffer[6];
			stcAngle.Angle[2] = ((unsigned short)ucRxBuffer[7]<<8)|ucRxBuffer[8];
		
			stcIMU_history[index][2].x_Angle = stcIMU_history[index][1].x_Angle;
			stcIMU_history[index][1].x_Angle = stcIMU_history[index][0].x_Angle;
			stcIMU_history[index][0].x_Angle = stcAngle.Angle[0]/temp3;
			stcIMU[index].x_Angle = stcIMU_history[index][0].x_Angle*0.5+stcIMU_history[index][1].x_Angle*0.3+stcIMU_history[index][2].x_Angle*0.2;
			
			stcIMU_history[index][2].y_Angle = stcIMU_history[index][1].y_Angle;
			stcIMU_history[index][1].y_Angle = stcIMU_history[index][0].y_Angle;
			stcIMU_history[index][0].y_Angle = stcAngle.Angle[1]/temp3;
			stcIMU[index].y_Angle = stcIMU_history[index][0].y_Angle*0.5+stcIMU_history[index][1].y_Angle*0.3+stcIMU_history[index][2].y_Angle*0.2;
			
			stcIMU_history[index][2].z_Angle = stcIMU_history[index][1].z_Angle;
			stcIMU_history[index][1].z_Angle = stcIMU_history[index][0].z_Angle;
			stcIMU_history[index][0].z_Angle = stcAngle.Angle[2]/temp3;
			stcIMU[index].z_Angle = stcIMU_history[index][0].z_Angle*0.5+stcIMU_history[index][1].z_Angle*0.3+stcIMU_history[index][2].z_Angle*0.2;
			
			//ANO printf
			if(swt[7]){
				ano_o_F2[index%2+4] = stcIMU[index].x_Angle;
			}else{
				ano_o_F2[index%2+4] = 0;
			}
			
		}else if(stcIMU[index].addrReg==AX){		//���ٶ�
			stcAcc.a[0] = ((unsigned short)ucRxBuffer[3]<<8)|ucRxBuffer[4];
			stcAcc.a[1] = ((unsigned short)ucRxBuffer[5]<<8)|ucRxBuffer[6];
			stcAcc.a[2] = ((unsigned short)ucRxBuffer[7]<<8)|ucRxBuffer[8];
			
			stcIMU_history[index][2].x_a = stcIMU_history[index][1].x_a;
			stcIMU_history[index][1].x_a = stcIMU_history[index][0].x_a;
			stcIMU_history[index][0].x_a = stcAcc.a[0]/temp1;
			stcIMU[index].x_a = stcIMU_history[index][0].x_a*0.5+stcIMU_history[index][1].x_a*0.3+stcIMU_history[index][2].x_a*0.2;
			
			stcIMU_history[index][2].y_a = stcIMU_history[index][1].y_a;
			stcIMU_history[index][1].y_a = stcIMU_history[index][0].y_a;
			stcIMU_history[index][0].y_a = stcAcc.a[1]/temp1;
			stcIMU[index].y_a = stcIMU_history[index][0].y_a*0.5+stcIMU_history[index][1].y_a*0.3+stcIMU_history[index][2].y_a*0.2;
			
			stcIMU_history[index][2].z_a = stcIMU_history[index][1].z_a;
			stcIMU_history[index][1].z_a = stcIMU_history[index][0].z_a;
			stcIMU_history[index][0].z_a = stcAcc.a[2]/temp1;
			stcIMU[index].z_a = stcIMU_history[index][0].z_a*0.5+stcIMU_history[index][1].z_a*0.3+stcIMU_history[index][2].z_a*0.2;
			
			//no ANO printf 
		}else if(stcIMU[index].addrReg==GX){		//���ٶ�
			stcGyro.w[0] = ((unsigned short)ucRxBuffer[3]<<8)|ucRxBuffer[4];
			stcGyro.w[1] = ((unsigned short)ucRxBuffer[5]<<8)|ucRxBuffer[6];
			stcGyro.w[2] = ((unsigned short)ucRxBuffer[7]<<8)|ucRxBuffer[8];
			
			stcIMU_history[index][2].x_w = stcIMU_history[index][1].x_w;
			stcIMU_history[index][1].x_w = stcIMU_history[index][0].x_w;
			stcIMU_history[index][0].x_w = stcGyro.w[0]/temp2;
			stcIMU[index].x_w = stcIMU_history[index][0].x_w*0.5+stcIMU_history[index][1].x_w*0.3+stcIMU_history[index][2].x_w*0.2;
			
			stcIMU_history[index][2].y_w = stcIMU_history[index][1].y_w;
			stcIMU_history[index][1].y_w = stcIMU_history[index][0].y_w;
			stcIMU_history[index][0].y_w = stcGyro.w[1]/temp2;
			stcIMU[index].y_w = stcIMU_history[index][0].y_w*0.5+stcIMU_history[index][1].y_w*0.3+stcIMU_history[index][2].y_w*0.2;
			
			stcIMU_history[index][2].z_w = stcIMU_history[index][1].z_w;
			stcIMU_history[index][1].z_w = stcIMU_history[index][0].z_w;
			stcIMU_history[index][0].z_w = stcGyro.w[2]/temp2;
			stcIMU[index].z_w = stcIMU_history[index][0].z_w*0.5+stcIMU_history[index][1].z_w*0.3+stcIMU_history[index][2].z_w*0.2;
			
			//ANO printf
			if(swt[8]){
				ano_o_F2[index%2+6] = stcIMU[index].x_w;
			}else{
				ano_o_F2[index%2+6] = 0;
			}
			
		}
		ucRxCnt=0;
	}
}


/*
��������	ModbusRWReg
��	����	Addr					���ӻ���ַ		
					R_W						��==0x03/0x06		������/д����
					usReg					����/д	�Ĵ�����ַ			
					usRegNumDate	��д������/���ĸ���
����ֵ��	void
��	�ã�	modbus��д��Э��
ʱ	�䣺	2019��1��22��
��	�ߣ�	meetwit
*/
void 	ModbusRWReg(unsigned short Addr, unsigned short R_W, unsigned short usReg, unsigned short usRegNumDate){
			char i=0,j=0;
      char cIndex=0;
	    unsigned char record[8];
	    short checkCRC;
			for(j=0;j<imuNum;j++){
				if(allowAddeId[j]==Addr){
					stcIMU[j].addrId = Addr;
					stcIMU[j].addrReg = usReg;
					break;
				}
			}
	    record[cIndex++]=Addr;
	    record[cIndex++]=R_W;
	    record[cIndex++]=usReg<<8;
	    record[cIndex++]=usReg&0xff;
	    record[cIndex++]=usRegNumDate<<8;
	    record[cIndex++]=usRegNumDate&0xff;
	    checkCRC=CRC16(record,cIndex);
	    record[cIndex++]=checkCRC&0xff; 
	    record[cIndex++]=(checkCRC>>8)&0xff;
		  for(i=0;i<=cIndex;i++)
			send4852(record[i]);
}


/*
��������	CopeSerialData2
��	�Σ�	ucData �����յ����ֽ�
����ֵ��	void
��	�ã�	����jy901���յ�������
��	�ڣ�	2019��1��22��
��	�ߣ�	meetwit
*/
void CopeSerialData2(unsigned char ucData)
{
	static unsigned char ucRxBuffer[11];
	static unsigned char ucRxCnt = 0;	
	static unsigned char ucRxLen = 0;	
	static float temp1 = 208.98;
	static float temp2 = 16.384;
	static float temp3 = 182.04;
	static unsigned char  index = 0;
		
	ucRxBuffer[ucRxCnt++]=ucData;
	if(ucRxCnt==1){
		return;
	}
	if(ucRxCnt==2){
		if (ucRxBuffer[1]!=0x03)
		{
			ucRxCnt=0;																															  
		}
			return;		
	}
	if(ucRxCnt==3){
			ucRxLen = ucRxBuffer[2]+5;
			return;		
	}
	if (ucRxCnt<ucRxLen) {
		return;
		}
	else
	{
			for(index=0;index<imuNum;index++){
				if(stcIMU[index].addrId==ucRxBuffer[0]){
					break;
				}
			}
		if(stcIMU[index].addrReg==Roll){				//�Ƕ�
			stcAngle.Angle[0] = ((unsigned short)ucRxBuffer[3]<<8)|ucRxBuffer[4];
			stcAngle.Angle[1] = ((unsigned short)ucRxBuffer[5]<<8)|ucRxBuffer[6];
			stcAngle.Angle[2] = ((unsigned short)ucRxBuffer[7]<<8)|ucRxBuffer[8];
		
			stcIMU_history[index][2].x_Angle = stcIMU_history[index][1].x_Angle;
			stcIMU_history[index][1].x_Angle = stcIMU_history[index][0].x_Angle;
			stcIMU_history[index][0].x_Angle = stcAngle.Angle[0]/temp3;
			stcIMU[index].x_Angle = stcIMU_history[index][0].x_Angle*0.5+stcIMU_history[index][1].x_Angle*0.3+stcIMU_history[index][2].x_Angle*0.2;
			
			stcIMU_history[index][2].y_Angle = stcIMU_history[index][1].y_Angle;
			stcIMU_history[index][1].y_Angle = stcIMU_history[index][0].y_Angle;
			stcIMU_history[index][0].y_Angle = stcAngle.Angle[1]/temp3;
			stcIMU[index].y_Angle = stcIMU_history[index][0].y_Angle*0.5+stcIMU_history[index][1].y_Angle*0.3+stcIMU_history[index][2].y_Angle*0.2;
			
			stcIMU_history[index][2].z_Angle = stcIMU_history[index][1].z_Angle;
			stcIMU_history[index][1].z_Angle = stcIMU_history[index][0].z_Angle;
			stcIMU_history[index][0].z_Angle = stcAngle.Angle[2]/temp3;
			stcIMU[index].z_Angle = stcIMU_history[index][0].z_Angle*0.5+stcIMU_history[index][1].z_Angle*0.3+stcIMU_history[index][2].z_Angle*0.2;
			
			//ANO printf
			if(swt[7]){
				ano_o_F2[index%2+4] = stcIMU[index].x_Angle;
			}else{
				ano_o_F2[index%2+4] = 0;
			}
			
			
		}else if(stcIMU[index].addrReg==AX){		//���ٶ�
			stcAcc.a[0] = ((unsigned short)ucRxBuffer[3]<<8)|ucRxBuffer[4];
			stcAcc.a[1] = ((unsigned short)ucRxBuffer[5]<<8)|ucRxBuffer[6];
			stcAcc.a[2] = ((unsigned short)ucRxBuffer[7]<<8)|ucRxBuffer[8];
			
			stcIMU_history[index][2].x_a = stcIMU_history[index][1].x_a;
			stcIMU_history[index][1].x_a = stcIMU_history[index][0].x_a;
			stcIMU_history[index][0].x_a = stcAcc.a[0]/temp1;
			stcIMU[index].x_a = stcIMU_history[index][0].x_a*0.5+stcIMU_history[index][1].x_a*0.3+stcIMU_history[index][2].x_a*0.2;
			
			stcIMU_history[index][2].y_a = stcIMU_history[index][1].y_a;
			stcIMU_history[index][1].y_a = stcIMU_history[index][0].y_a;
			stcIMU_history[index][0].y_a = stcAcc.a[1]/temp1;
			stcIMU[index].y_a = stcIMU_history[index][0].y_a*0.5+stcIMU_history[index][1].y_a*0.3+stcIMU_history[index][2].y_a*0.2;
			
			stcIMU_history[index][2].z_a = stcIMU_history[index][1].z_a;
			stcIMU_history[index][1].z_a = stcIMU_history[index][0].z_a;
			stcIMU_history[index][0].z_a = stcAcc.a[2]/temp1;
			stcIMU[index].z_a = stcIMU_history[index][0].z_a*0.5+stcIMU_history[index][1].z_a*0.3+stcIMU_history[index][2].z_a*0.2;
			
			
		}else if(stcIMU[index].addrReg==GX){		//���ٶ�
			stcGyro.w[0] = ((unsigned short)ucRxBuffer[3]<<8)|ucRxBuffer[4];
			stcGyro.w[1] = ((unsigned short)ucRxBuffer[5]<<8)|ucRxBuffer[6];
			stcGyro.w[2] = ((unsigned short)ucRxBuffer[7]<<8)|ucRxBuffer[8];
			
			stcIMU_history[index][2].x_w = stcIMU_history[index][1].x_w;
			stcIMU_history[index][1].x_w = stcIMU_history[index][0].x_w;
			stcIMU_history[index][0].x_w = stcGyro.w[0]/temp2;
			stcIMU[index].x_w = stcIMU_history[index][0].x_w*0.5+stcIMU_history[index][1].x_w*0.3+stcIMU_history[index][2].x_w*0.2;
			
			stcIMU_history[index][2].y_w = stcIMU_history[index][1].y_w;
			stcIMU_history[index][1].y_w = stcIMU_history[index][0].y_w;
			stcIMU_history[index][0].y_w = stcGyro.w[1]/temp2;
			stcIMU[index].y_w = stcIMU_history[index][0].y_w*0.5+stcIMU_history[index][1].y_w*0.3+stcIMU_history[index][2].y_w*0.2;
			
			stcIMU_history[index][2].z_w = stcIMU_history[index][1].z_w;
			stcIMU_history[index][1].z_w = stcIMU_history[index][0].z_w;
			stcIMU_history[index][0].z_w = stcGyro.w[2]/temp2;
			stcIMU[index].z_w = stcIMU_history[index][0].z_w*0.5+stcIMU_history[index][1].z_w*0.3+stcIMU_history[index][2].z_w*0.2;
			
			//ANO printf
			if(swt[8]){
				ano_o_F2[index%2+6] = stcIMU[index].x_w;
			}else{
				ano_o_F2[index%2+6] = 0;
			}
			
		}
		ucRxCnt=0;
	}
}

/*
��������	ModbusRWReg2
��	����	Addr					���ӻ���ַ		
					R_W						��==0x03/0x06		������/д����
					usReg					����/д	�Ĵ�����ַ			
					usRegNumDate	��д������/���ĸ���
����ֵ��	void
��	�ã�	modbus��д��Э��
ʱ	�䣺	2019��1��22��
��	�ߣ�	meetwit
*/
void 	ModbusRWReg2(unsigned short Addr, unsigned short R_W, unsigned short usReg, unsigned short usRegNumDate){
			char i=0,j=0;
      char cIndex=0;
	    unsigned char record[8];
	    short checkCRC;
			for(j=0;j<imuNum;j++){
				if(allowAddeId[j]==Addr){
					stcIMU[j].addrId = Addr;
					stcIMU[j].addrReg = usReg;
					break;
				}
			}
	    record[cIndex++]=Addr;
	    record[cIndex++]=R_W;
	    record[cIndex++]=usReg<<8;
	    record[cIndex++]=usReg&0xff;
	    record[cIndex++]=usRegNumDate<<8;
	    record[cIndex++]=usRegNumDate&0xff;
	    checkCRC=CRC16(record,cIndex);
	    record[cIndex++]=checkCRC&0xff; 
	    record[cIndex++]=(checkCRC>>8)&0xff;
		  for(i=0;i<=cIndex;i++)
			send485(record[i]);
}



void read_Imu(void){
	
		if(swt[7]){
			ModbusRWReg(allowAddeId[3],imuRead,Roll,3);
			ModbusRWReg2(allowAddeId[4],imuRead,Roll,3);
			delay_ms(3);
		}
		
		if(swt[8]){
			ModbusRWReg(allowAddeId[3],imuRead,GX,3);
			ModbusRWReg2(allowAddeId[4],imuRead,GX,3);
			delay_ms(3);
		}
		
		if(swt[9]){
			ModbusRWReg(allowAddeId[3],imuRead,AX,3);
			ModbusRWReg(allowAddeId[4],imuRead,AX,3);
			delay_ms(3);
		}
	
		
}


void imu_find_point(void){
	cal_k3=stcIMU[3].x_Angle+cal;
	ano_o_F3[6] = cal_k3;
	static u16 assi_time[5]={0,0,0,0,0};
	if(cal_k3 > stcIMU[4].x_Angle){
		if(stcIMU[3].x_w>20){
			assi_time[0]++;
		}else if(stcIMU[3].x_w<-20){
			if(assi_time[0]>10){//����-20
				assi_time[0]=11;
				state_F=1;
			assi_time[4] = 0;
			}
			assi_time[0] = 0;
		}else{
			assi_time[4]++;
		}
		if(assi_time[3]>10){//����
			assi_time[3]=11;
				state_F=4;
			}
		assi_time[3] = 0;
		assi_time[1] ++;
	}else{
		if(stcIMU[3].x_w>20){
			if(assi_time[2]>10){//����20
				assi_time[2]=11;
				state_F=3;
			assi_time[4] = 0;
			}
			assi_time[2] = 0;
		}else if(stcIMU[3].x_w<-20){
			assi_time[2]++;
		}else{
			assi_time[4]++;
		}
		if(assi_time[1]>10){//����
			assi_time[1]=11;
				state_F=2;
			}
			assi_time[1] = 0;
			assi_time[3] ++;
	}
	if(assi_time[4]>30){	//����
		assi_time[4]=0;
		state_F = 5;
	}
}


void imu_find_point2(void){
	static u16 assi_time[5]={0,0,0,0,0};
	if(stcIMU[4].x_Angle > cal_k3){
		if(stcIMU[4].x_w>20){
			assi_time[0]++;
		}else if(stcIMU[4].x_w<-20){
			if(assi_time[0]>10){//����
				assi_time[0]=11;
				state_S=1;
			assi_time[4] = 0;
			}
			assi_time[0] = 0;
		}else{
			assi_time[4]++;
		}
		if(assi_time[3]>20){//����
			assi_time[3]=11;
				state_S=4;
			}
		assi_time[3] = 0;
		assi_time[1] ++;
	}else{
		if(stcIMU[4].x_w>20){
			if(assi_time[2]>10){//����
				assi_time[2]=11;
				state_S=3;
			assi_time[4] = 0;
			}
			assi_time[2] = 0;
		}else if(stcIMU[4].x_w<-20){
			assi_time[2]++;
		}else{
			assi_time[4]++;
		}
		if(assi_time[1]>10){//����
			assi_time[1]=11;
				state_S=2;
			}
			assi_time[1] = 0;
			assi_time[3] ++;
	}
	if(assi_time[4]>30){	//����
		assi_time[4]=0;
		state_S = 5;
	}
}
