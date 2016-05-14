/* ************************************************************************

 							SYSTEM 系统初始化子程序

***************************************************************************
 名称：SYSTEM 系统初始化子程序
 时间：2014.04.08
 内容：本程序包括：
 	   单片机时钟设置、I/O端口配置、交叉开关配置
 说明：系统初始化的必要配置
*************************************************************************** */
#include <c8051f130.h>
#include "system.h"
#include "sim_card.h"
#include "ch453.h"
#include "ds1302.h"
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//注意：请使用9600波特率，115200波特率有时会出现错误
///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#define UART1_9600   0x02   			/* 定义串口1通讯波特率 */
#define UART1_115200 0x01    		   	/* 定义串口1通讯波特率 */ 
//------------------------------
void Oscillator_Init()
{
    char i = 0;
	WDTCN=0X07;
	WDTCN=0XDE;
	WDTCN=0XAD;
    SFRPAGE   = CONFIG_PAGE;
    OSCICN    = 0x83;         			//使能内部振荡器，内部振荡器不分频
    CCH0CN    &= ~0x20;       			//高速缓存(Cache)控制寄存器。高速缓存预取禁止
    SFRPAGE   = LEGACY_PAGE;
    FLSCL     = 0xB0;
    SFRPAGE   = CONFIG_PAGE;
    CCH0CN    |= 0x20;
    PLL0CN    |= 0x01;
    PLL0DIV   = 0x01;
    PLL0FLT   = 0x01;
    PLL0MUL   = 0x04;         			 //内部时钟 4倍频 100MHZ
    for (i = 0; i < 15; i++);  			 // Wait 5us for initialization
    PLL0CN    |= 0x02;
    while ((PLL0CN & 0x10) == 0);
    CLKSEL    = 0x32;         			 //SYSCLK PLL/8  
}
//---------------------------------------
void Port_IO_Init()
{
    SFRPAGE   = CONFIG_PAGE;
	P0MDOUT  |= 0x10;                   // Set TX pin to push-pull
	P1MDOUT   = 0Xff;
	P2MDOUT   = 0xfd;					//sim_data引腳配置成开漏
//	P2 |=0x55;
	P3MDOUT |= 0x0c;
//	P3MDOUT   = 0xf7; //U19
//	P3 |=0x08;	
/*
	P3MDOUT   = 0xdf; //U20
	P3 |=0x20;
*/
	P4MDOUT   = 0xf5;					//(开漏输出) 
	P7MDOUT   = 0xff;					//(开漏输出)
    XBR0      = 0x07;					//串口0 ,1
    XBR2      = 0x44;					//交叉使能	，使能若上拉						   
//  XBR1      = 0x80;  					//使能SYSCLK输出
}
//-------------------------------------------
void UART1_Init()                    //串口1初始化設置
{
	SFRPAGE   = UART1_PAGE;
    SCON1     = 0x10; 			     // 8位 ，自动重载，允许接收
    EIE2|=0X40;	 					 // 允许串口1中断
}
//--------------------------------------------
void Interrupts_Init()   
{
  IE= 0x80; 							//开总中断
}
//-------------------------------------------
void Timer1_Init()                      //定时器1初始化，提供UART1的波特率
{
    unsigned char tmp_pag; 
	tmp_pag=SFRPAGE;
	SFRPAGE = TIMER01_PAGE;
	TMOD|=0x20;                      	// TMOD: timer 1, mode 2, 8-bit reload
	CKCON|=UART1_9600;               	// 定時器0/1預分頻时钟输入设置，可以确定UART0/1的波特率
	TH1=0X96;
	TL1=TH1;                        	// 11520BPS初值
	TR1 = 1;                         	// 启动定时器1
	SFRPAGE =tmp_pag;                	// 启动定时器1
}
//--------------------------------------------
void Init_Device(void)
{	 
	 Oscillator_Init(); 				//锁相环设置
     Port_IO_Init();					//端口设置

     UART1_Init();
     Interrupts_Init();
	 Timer1_Init();

	 CH453_INIT();
	 SIM_Init(); 						//SIM卡初始化

//   set_rtc(); 						//1302初始化
}