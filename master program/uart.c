/* ************************************************************************

 							UART  C8051F130串口程序

***************************************************************************
 名称：UART  C8051F130串口程序
 时间：2014.04.08
 内容：本程序包括：
 	   C8051F130的UART0、UART1程序
 说明：UART0:待

  	   UART1:查詢发送，中断接收(接收的数据放在buffer[]数组中)，UART1波特率9600/115200可变，
*************************************************************************** */
#include<c8051f130.h>
#include"uart.h"
#include"delay.h"

unsigned char Uart1_Buffer_Flag=0;    //标记buffer接收数组
unsigned char Uart1_Received_Flag=0;    //buffer数组接收完毕标志

unsigned char buffer[65]={0};		 //uart中断接收数组

//--------------------------------------------------------------------

//--------------------------------------------------------------------
void Uart1_Judge_received()			//判断串口1是否接收完成
{
	unsigned int i;

	if(	Uart1_Received_Flag == 1)//若上一組buffer数据全部接收完成，下一条指令等待接收时，先把Uart1_Received_Flag标志清0;
	{
		Uart1_Received_Flag = 0;
	}
//---------------------------------------
	while(Uart1_Received_Flag == 0)
	{
		for(i=0;i<=6000;i++)
		{
			delay_ms(2);
			if(Uart1_Received_Flag == 1)//串口1是否接收完毕，正常退出标志		 	   
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

	SBUF1=value;            // 发送数据到uart1
	while(!TI1);            // 等待发送1个字节数据完成
	TI1=0;  				// 清除中断标志

	SFRPAGE = SFRPAGE_SAVE;
}

void uart1()interrupt 20        //uart中断，实现接收功能，接收的数据放在buffer数组中
{
	unsigned char SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE=UART1_PAGE;
	
	if(RI1==1)                 //接收标志
    {  
	  	Uart1_Buffer_Flag++;
		buffer[Uart1_Buffer_Flag-1] = SBUF1;
	 	RI1 = 0; 
	}
	if((buffer[Uart1_Buffer_Flag-1]==0x03)&&(Uart1_Buffer_Flag>(buffer[2]+4)))//测试标识，判断最后一个字符是否正确接收完毕
	{
	    Uart1_Buffer_Flag = 0;	// 标志清0，下次buffer数组接收数据时可以从buffer[0]开始
		Uart1_Received_Flag = 1; //标志置1，表示一组数据已经接收完毕
	}

	SFRPAGE = SFRPAGE_SAVE;
}