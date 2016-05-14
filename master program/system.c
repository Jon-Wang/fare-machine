/* ************************************************************************

 							SYSTEM ϵͳ��ʼ���ӳ���

***************************************************************************
 ���ƣ�SYSTEM ϵͳ��ʼ���ӳ���
 ʱ�䣺2014.04.08
 ���ݣ������������
 	   ��Ƭ��ʱ�����á�I/O�˿����á����濪������
 ˵����ϵͳ��ʼ���ı�Ҫ����
*************************************************************************** */
#include <c8051f130.h>
#include "system.h"
#include "sim_card.h"
#include "ch453.h"
#include "ds1302.h"
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//ע�⣺��ʹ��9600�����ʣ�115200��������ʱ����ִ���
///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#define UART1_9600   0x02   			/* ���崮��1ͨѶ������ */
#define UART1_115200 0x01    		   	/* ���崮��1ͨѶ������ */ 
//------------------------------
void Oscillator_Init()
{
    char i = 0;
	WDTCN=0X07;
	WDTCN=0XDE;
	WDTCN=0XAD;
    SFRPAGE   = CONFIG_PAGE;
    OSCICN    = 0x83;         			//ʹ���ڲ��������ڲ���������Ƶ
    CCH0CN    &= ~0x20;       			//���ٻ���(Cache)���ƼĴ��������ٻ���Ԥȡ��ֹ
    SFRPAGE   = LEGACY_PAGE;
    FLSCL     = 0xB0;
    SFRPAGE   = CONFIG_PAGE;
    CCH0CN    |= 0x20;
    PLL0CN    |= 0x01;
    PLL0DIV   = 0x01;
    PLL0FLT   = 0x01;
    PLL0MUL   = 0x04;         			 //�ڲ�ʱ�� 4��Ƶ 100MHZ
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
	P2MDOUT   = 0xfd;					//sim_data���_���óɿ�©
//	P2 |=0x55;
	P3MDOUT |= 0x0c;
//	P3MDOUT   = 0xf7; //U19
//	P3 |=0x08;	
/*
	P3MDOUT   = 0xdf; //U20
	P3 |=0x20;
*/
	P4MDOUT   = 0xf5;					//(��©���) 
	P7MDOUT   = 0xff;					//(��©���)
    XBR0      = 0x07;					//����0 ,1
    XBR2      = 0x44;					//����ʹ��	��ʹ��������						   
//  XBR1      = 0x80;  					//ʹ��SYSCLK���
}
//-------------------------------------------
void UART1_Init()                    //����1��ʼ���O��
{
	SFRPAGE   = UART1_PAGE;
    SCON1     = 0x10; 			     // 8λ ���Զ����أ��������
    EIE2|=0X40;	 					 // ������1�ж�
}
//--------------------------------------------
void Interrupts_Init()   
{
  IE= 0x80; 							//�����ж�
}
//-------------------------------------------
void Timer1_Init()                      //��ʱ��1��ʼ�����ṩUART1�Ĳ�����
{
    unsigned char tmp_pag; 
	tmp_pag=SFRPAGE;
	SFRPAGE = TIMER01_PAGE;
	TMOD|=0x20;                      	// TMOD: timer 1, mode 2, 8-bit reload
	CKCON|=UART1_9600;               	// ���r��0/1�A���lʱ���������ã�����ȷ��UART0/1�Ĳ�����
	TH1=0X96;
	TL1=TH1;                        	// 11520BPS��ֵ
	TR1 = 1;                         	// ������ʱ��1
	SFRPAGE =tmp_pag;                	// ������ʱ��1
}
//--------------------------------------------
void Init_Device(void)
{	 
	 Oscillator_Init(); 				//���໷����
     Port_IO_Init();					//�˿�����

     UART1_Init();
     Interrupts_Init();
	 Timer1_Init();

	 CH453_INIT();
	 SIM_Init(); 						//SIM����ʼ��

//   set_rtc(); 						//1302��ʼ��
}