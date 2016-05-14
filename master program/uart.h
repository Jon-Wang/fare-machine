#ifndef _UART_H_
#define _UART_H_

extern unsigned char Uart1_Buffer_Flag;		//标记buffer接收数组
extern unsigned char Uart1_Received_Flag;	//buffer数组接收完毕标志
//-----------------------------------------
extern unsigned char buffer[65];			//uart中断接收数组
//-----------------------------------------
extern void Uart1_SendByte(unsigned char value);
extern void Uart1_Judge_received();			//判断串口1是否接收完成

#endif