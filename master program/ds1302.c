/* ************************************************************************

 							DS1302 時鐘子程序

***************************************************************************
 名称：DS1302 時鐘子程序
 时间：2014.2.16
 内容：本程序包括：
 	   DS1302 時鐘子函數
 说明：DS1302程序，實現 秒分时日月周年
*************************************************************************** */
#include "delay.h"
#include "ds1302.h"
#include "ch453.h"

unsigned char  l_tmpdate[7]={0,26,11,19,5,1,14};//秒/分/时/日/月/周/年/

/*----------------ds1302端口定义----------------*/

//sbit DS1302_CLK = P2^1;			//ds1302_clk   
//sbit DS1302_DAT = P2^0;			//ds1302_dat
//sbit DS1302_RST = P2^2;			//ds1302_Rst
sbit DS1302_CLK = P3^6;			//ds1302_clk   
sbit DS1302_DAT = P3^5;			//ds1302_dat
sbit DS1302_RST = P3^7;			//ds1302_Rst

/*----------------ds1302写单字节数据子程序----------------*/

void DS1302_WRITE_BYTE(unsigned char dat) 
{
	unsigned char i;
	
	DS1302_CLK = 0;			//初始化时钟线置为0
    delay_us(5);
	for (i=0;i<8;i++)     	//循环8次 写入数据
    { 
		DS1302_DAT = dat & 0x01;
		delay_us(5);
		DS1302_CLK = 1;
		delay_us(5);
		DS1302_CLK = 0;
		delay_us(5);
		dat>>=1;  			//右移一位
   }
} 

/*----------------ds1302读单字节数据子程序----------------*/
unsigned char DS1302_READ_BYTE(void) 
{
	unsigned char i,dat;

    delay_us(5);
	for (i=0;i<8;i++) 		//循环8次 读取数据
 	{			
		dat>>=1;
 		if(DS1302_DAT==1)
			dat|=0x80;
		DS1302_CLK = 1;		//SCK=1;
		delay_us(5);		
		DS1302_CLK = 0;		//sck=0
		delay_us(5);
	}
	return dat;
}   

//----------------ds1302写地址、数据子程序----------------

void DS1302_WRITE( unsigned char cmd,unsigned char dat )     
{
	DS1302_RST = 0;				//初始化CE线置为0
	DS1302_CLK = 0;				//初始化时钟线置为0
	delay_us(5);
	DS1302_RST = 1;				//初始化CE线置为1，传输开始
	delay_us(5);
	DS1302_WRITE_BYTE(cmd);		//传输命令字，要写入的时间/日期地址
	DS1302_WRITE_BYTE(dat);		//写入要修改的时间/日期
	DS1302_CLK = 1;				//时钟线拉高
	DS1302_RST = 0;				//读取结束，CE置为0，结束数据的传输
}
//---------------ds1302读数据子程序----------------

unsigned char DS1302_READ(unsigned char cmd)
{
 	unsigned char dat;

 	DS1302_RST = 0;//初始化CE线置为0
	DS1302_CLK = 0;//初始化时钟线置为0
	delay_us(5);//_nop_();
 	DS1302_RST = 1;//RST=1;
	delay_us(5);//_nop_();
 	DS1302_WRITE_BYTE(cmd);
 	dat=DS1302_READ_BYTE();
	DS1302_CLK = 1;//初始化时钟线置为0
	DS1302_RST = 0;
	delay_us(5);
	return dat;			//返回
}

//----------------时钟初始化--------------------------------
void set_rtc(void)		
{
	unsigned char i,p,tmp;   

	for(i=0;i<7;i++)//BCD处理
	{        
		tmp=l_tmpdate[i]/10;
		l_tmpdate[i]=l_tmpdate[i]%10;
		l_tmpdate[i]=l_tmpdate[i]+tmp*16;
	}

 	DS1302_WRITE(0x8e,0X00);
 	p=0x80;	//传地址	
 	for(i=0;i<7;i++)		//7次写入 秒分时日月周年
 	{
		DS1302_WRITE(p,l_tmpdate[i]);
 		p+=2;  
	}
	DS1302_WRITE(0x8E,0x80);
}
//------------------------------------------------------------------
void DS1302_PLAY()					 //1302读取时间
{
	unsigned int i,p;
	p=0x81;							//地址传递

	for(i=0;i<7;i++)				//分7次读取 秒分时日月周年
	{
		l_tmpdate[i]=DS1302_READ(p);
		p+=2;
	}
//	l_tmpdate[0]&=0x7f;
//	l_tmpdate[1]&=0x7f;
////l_tmpdate[2]&=0x1f;
//	l_tmpdate[3]&=0x3f;
//	l_tmpdate[4]&=0x1f;
//	l_tmpdate[5]&=0x07;
//	l_tmpdate[6]&=0xff;	

	for(i=0;i<7;i++)			//BCD处理
	{        
		l_tmpdate[i]=(l_tmpdate[i]/16)*10+(l_tmpdate[i]%16);
	}	
}


