/* ************************************************************************

 							DS1302 �r��ӳ���

***************************************************************************
 ���ƣ�DS1302 �r��ӳ���
 ʱ�䣺2014.2.16
 ���ݣ������������
 	   DS1302 �r��Ӻ���
 ˵����DS1302���򣬌��F ���ʱ��������
*************************************************************************** */
#include "delay.h"
#include "ds1302.h"
#include "ch453.h"

unsigned char  l_tmpdate[7]={0,26,11,19,5,1,14};//��/��/ʱ/��/��/��/��/

/*----------------ds1302�˿ڶ���----------------*/

//sbit DS1302_CLK = P2^1;			//ds1302_clk   
//sbit DS1302_DAT = P2^0;			//ds1302_dat
//sbit DS1302_RST = P2^2;			//ds1302_Rst
sbit DS1302_CLK = P3^6;			//ds1302_clk   
sbit DS1302_DAT = P3^5;			//ds1302_dat
sbit DS1302_RST = P3^7;			//ds1302_Rst

/*----------------ds1302д���ֽ������ӳ���----------------*/

void DS1302_WRITE_BYTE(unsigned char dat) 
{
	unsigned char i;
	
	DS1302_CLK = 0;			//��ʼ��ʱ������Ϊ0
    delay_us(5);
	for (i=0;i<8;i++)     	//ѭ��8�� д������
    { 
		DS1302_DAT = dat & 0x01;
		delay_us(5);
		DS1302_CLK = 1;
		delay_us(5);
		DS1302_CLK = 0;
		delay_us(5);
		dat>>=1;  			//����һλ
   }
} 

/*----------------ds1302�����ֽ������ӳ���----------------*/
unsigned char DS1302_READ_BYTE(void) 
{
	unsigned char i,dat;

    delay_us(5);
	for (i=0;i<8;i++) 		//ѭ��8�� ��ȡ����
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

//----------------ds1302д��ַ�������ӳ���----------------

void DS1302_WRITE( unsigned char cmd,unsigned char dat )     
{
	DS1302_RST = 0;				//��ʼ��CE����Ϊ0
	DS1302_CLK = 0;				//��ʼ��ʱ������Ϊ0
	delay_us(5);
	DS1302_RST = 1;				//��ʼ��CE����Ϊ1�����俪ʼ
	delay_us(5);
	DS1302_WRITE_BYTE(cmd);		//���������֣�Ҫд���ʱ��/���ڵ�ַ
	DS1302_WRITE_BYTE(dat);		//д��Ҫ�޸ĵ�ʱ��/����
	DS1302_CLK = 1;				//ʱ��������
	DS1302_RST = 0;				//��ȡ������CE��Ϊ0���������ݵĴ���
}
//---------------ds1302�������ӳ���----------------

unsigned char DS1302_READ(unsigned char cmd)
{
 	unsigned char dat;

 	DS1302_RST = 0;//��ʼ��CE����Ϊ0
	DS1302_CLK = 0;//��ʼ��ʱ������Ϊ0
	delay_us(5);//_nop_();
 	DS1302_RST = 1;//RST=1;
	delay_us(5);//_nop_();
 	DS1302_WRITE_BYTE(cmd);
 	dat=DS1302_READ_BYTE();
	DS1302_CLK = 1;//��ʼ��ʱ������Ϊ0
	DS1302_RST = 0;
	delay_us(5);
	return dat;			//����
}

//----------------ʱ�ӳ�ʼ��--------------------------------
void set_rtc(void)		
{
	unsigned char i,p,tmp;   

	for(i=0;i<7;i++)//BCD����
	{        
		tmp=l_tmpdate[i]/10;
		l_tmpdate[i]=l_tmpdate[i]%10;
		l_tmpdate[i]=l_tmpdate[i]+tmp*16;
	}

 	DS1302_WRITE(0x8e,0X00);
 	p=0x80;	//����ַ	
 	for(i=0;i<7;i++)		//7��д�� ���ʱ��������
 	{
		DS1302_WRITE(p,l_tmpdate[i]);
 		p+=2;  
	}
	DS1302_WRITE(0x8E,0x80);
}
//------------------------------------------------------------------
void DS1302_PLAY()					 //1302��ȡʱ��
{
	unsigned int i,p;
	p=0x81;							//��ַ����

	for(i=0;i<7;i++)				//��7�ζ�ȡ ���ʱ��������
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

	for(i=0;i<7;i++)			//BCD����
	{        
		l_tmpdate[i]=(l_tmpdate[i]/16)*10+(l_tmpdate[i]%16);
	}	
}


