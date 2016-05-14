/* ************************************************************************

 							YUYIN 语音子程序

***************************************************************************
 名称：YUYIN 语音子程序
 时间：2014.2.16
 内容：本程序包括：
 	   语音播放函數
 说明：语音程序，K1、K2实现串行语音控制，可以播放0x00~0xff地址中的256段语音
*************************************************************************** */
#include "yuyin.h"
#include "delay.h"

void yuyin_set(unsigned char k1_data) 
{  
	unsigned char i; 
	K1 =0; 
	delay(100);		    		
	K1 = 1;
	delay(100);
	K2 = 0;
	for(i=0;i<8;i++) 
	{	
 		if((k1_data&0x01)==1) 
			{ K1 = 1; } 
		else
	 		{ K1 = 0; } 
			delay(120);				 
		K2 = 1;
		 delay(120);			
		k1_data = k1_data>>1; 
		K2 = 0; 
	}
	K1 = 1; 
	K2 = 1;
}
void yuyin_play(unsigned char dat)
{
	unsigned char h;

	for(h=dat;h<=dat;h++)       //収送从0x80地址到0x90地址的声音 
		{
	 		K1 = 1;
			K2 = 1;
 			yuyin_set(h); 
 			delay(100);
			while(O1 == 1);		//判断忙信号
		 	delay_1(250);
  		}
}
