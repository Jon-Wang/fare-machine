#ifndef _CH453_H_
#define _CH453_H_

#include <c8051f130.h>
#include <intrins.h>
#pragma NOAREGS		// 如果MCS51在中断服务程序中调用CH453子程序或者使用键盘中断功能，那么建议加入此编译选项

/* 延时0.1uS子程序,与单片机速度有关,对于双向I/O可以不需要延时,对于带上拉的准双向I/O建议加上延时 */
#define		DELAY_0_1US		{_nop_();}		// MCS51>10MHz

#define		CH453_F		0x01		// 第一個ch453
#define		CH453_S		0x02		// 第二個ch453

/* 2线接口的连接,与实际电路有关 */
/*
sbit	CH453_SCL=P0^3;
sbit	CH453_SDA=P0^2;
*/
#define	CH453_SCL_F P4&0x01		//P4.0
#define	CH453_SDA_F P4&0x02		//P4.1
#define	CH453_SCL_S P4&0x04		//P4.2
#define	CH453_SDA_S P4&0x08		//P4.3

/* 2线接口的位操作,与单片机有关 */
/*
#define		CH453_SCL_SET		{CH453_SCL2=1;}
#define		CH453_SCL_CLR		{CH453_SCL2=0;}
*/
#define		CH453_SCL_SET_F		{P4|=0x01;}
#define		CH453_SCL_CLR_F		{P4&=0xfe;}
#define		CH453_SCL_D_OUT_F		{}				// 设置SCL为输出方向,对于双向I/O需切换为输出
#define		CH453_SDA_SET_F		{P4|=0x02;}
#define		CH453_SDA_CLR_F		{P4&=0xfd;}
#define		CH453_SDA_D_OUT_F		{}				// 设置SDA为输出方向,对于双向I/O需切换为输出

#define		CH453_SCL_SET_S		{P4|=0x04;}
#define		CH453_SCL_CLR_S		{P4&=0xfb;}
#define		CH453_SCL_D_OUT_S		{}				// 设置SCL为输出方向,对于双向I/O需切换为输出
#define		CH453_SDA_SET_S		{P4|=0x08;}
#define		CH453_SDA_CLR_S		{P4&=0xf7;}
#define		CH453_SDA_D_OUT_S		{}				// 设置SDA为输出方向,对于双向I/O需切换为输出

/* ********************************************************************************************* */
/* 常量定义 */

/* CH453的常用命令码，如果考虑与I2C兼容，那么高8位应该右移1位 */
#define CH453_SYSOFF    0x0400					// 关闭显示
#define CH453_SYSON1    0x040b					// 开启显示
#define CH453_DIG0      0x1000		            // 数码管位0显示,需另加8位数据
#define CH453_DIG1      0x1100		            // 数码管位1显示,需另加8位数据
#define CH453_DIG2      0x1200		            // 数码管位2显示,需另加8位数据
#define CH453_DIG3      0x1300		            // 数码管位3显示,需另加8位数据
#define CH453_DIG4      0x1400		            // 数码管位4显示,需另加8位数据
#define CH453_DIG5      0x1500					// 数码管位5显示,需另加8位数据
#define CH453_DIG6      0x1600					// 数码管位6显示,需另加8位数据
#define CH453_DIG7      0x1700		            // 数码管位7显示,需另加8位数据
#define CH453_DIG8      0x1800					// 数码管位8显示,需另加8位数据
#define CH453_DIG9      0x1900					// 数码管位9显示,需另加8位数据
#define CH453_DIG10     0x1A00		            // 数码管位10显示,需另加8位数据
#define CH453_DIG11     0x1B00		            // 数码管位11显示,需另加8位数据
#define CH453_DIG12     0x1C00		            // 数码管位12显示,需另加8位数据
#define CH453_DIG13     0x1D00		            // 数码管位13显示,需另加8位数据
#define CH453_DIG14     0x1E00		            // 数码管位14显示,需另加8位数据
#define CH453_DIG15     0x1F00		            // 数码管位15显示,需另加8位数据

// CH453接口定义
#define		CH453_I2C_ADDR1		0x40			// CH453的地址
#define		CH453_I2C_MASK		0x3E			// CH453的高字节命令掩码
//-----------------------------------------------------------------------------
extern const unsigned char xdata BCD_decode_tab[0x10];
//-------------------------对外子程序----------------------------------------
extern void CH453_Write(unsigned short cmd,unsigned char ch453_num);	//写命令
extern unsigned char CH453_Read(unsigned char ch453_num );	
extern void CH453_buf_write( unsigned short cmd,unsigned char ch453_num);  // 向CH453输出数据或者操作命令,自动建立数据映象
extern void CH453_buf_index( unsigned char index, unsigned char dat,unsigned char ch453_num);  // 向CH453指定的数码管输出数据,自动建立数据映象
extern void CH453_INIT();//CH453初始化
extern void Display_Remaining_Sum(unsigned int remaining_sum_s);	//显示余额 58672
extern void Display_Consumption_Sum(Consumption_Sum);						//显示消费金额

#endif
