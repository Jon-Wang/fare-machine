/* ************************************************************************

 							UART  C8051F130���ڳ���

***************************************************************************
 ���ƣ�UART  C8051F130���ڳ���
 ʱ�䣺2014.04.08
 ���ݣ������������
 	   C8051F130��UART0��UART1����
 ˵����UART0:��

  	   UART1:��ԃ���ͣ��жϽ���(���յ����ݷ���buffer[]������)��UART1������9600/115200�ɱ䣬
*************************************************************************** */
#include<c8051f130.h>
#include"uart.h"
#include"delay.h"

unsigned char Uart1_Buffer_Flag=0;    //���buffer��������
unsigned char Uart1_Received_Flag=0;    //buffer���������ϱ�־

unsigned char buffer[65]={0};		 //uart�жϽ�������

//--------------------------------------------------------------------

//--------------------------------------------------------------------
void Uart1_Judge_received()			//�жϴ���1�Ƿ�������
{
	unsigned int i;

	if(	Uart1_Received_Flag == 1)//����һ�Mbuffer����ȫ��������ɣ���һ��ָ��ȴ�����ʱ���Ȱ�Uart1_Received_Flag��־��0;
	{
		Uart1_Received_Flag = 0;
	}
//---------------------------------------
	while(Uart1_Received_Flag == 0)
	{
		for(i=0;i<=6000;i++)
		{
			delay_ms(2);
			if(Uart1_Received_Flag == 1)//����1�Ƿ������ϣ������˳���־		 	   
			{ 
				 break;
			}
		}

		break;
	}
//-----------------------------------------
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
void Uart1_SendByte(unsigned char value)
{
	char  SFRPAGE_SAVE  = SFRPAGE;
    SFRPAGE = UART1_PAGE;	

	SBUF1=value;            // �������ݵ�uart1
	while(!TI1);            // �ȴ�����1���ֽ��������
	TI1=0;  				// ����жϱ�־

	SFRPAGE = SFRPAGE_SAVE;
}

void uart1()interrupt 20        //uart�жϣ�ʵ�ֽ��չ��ܣ����յ����ݷ���buffer������
{
	unsigned char SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE=UART1_PAGE;
	
	if(RI1==1)                 //���ձ�־
    {  
	  	Uart1_Buffer_Flag++;
		buffer[Uart1_Buffer_Flag-1] = SBUF1;
	 	RI1 = 0; 
	}
	if((buffer[Uart1_Buffer_Flag-1]==0x03)&&(Uart1_Buffer_Flag>(buffer[2]+4)))//���Ա�ʶ���ж����һ���ַ��Ƿ���ȷ�������
	{
	    Uart1_Buffer_Flag = 0;	// ��־��0���´�buffer�����������ʱ���Դ�buffer[0]��ʼ
		Uart1_Received_Flag = 1; //��־��1����ʾһ�������Ѿ��������
	}

	SFRPAGE = SFRPAGE_SAVE;
}