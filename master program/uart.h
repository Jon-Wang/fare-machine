#ifndef _UART_H_
#define _UART_H_

extern unsigned char Uart1_Buffer_Flag;		//���buffer��������
extern unsigned char Uart1_Received_Flag;	//buffer���������ϱ�־
//-----------------------------------------
extern unsigned char buffer[65];			//uart�жϽ�������
//-----------------------------------------
extern void Uart1_SendByte(unsigned char value);
extern void Uart1_Judge_received();			//�жϴ���1�Ƿ�������

#endif