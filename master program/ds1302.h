#ifndef _DS1302_H_
#define _DS1302_H_

extern unsigned char l_tmpdate[7];								//秒/分/时/日/月/周/年/
//------------------------------------------------------------
extern void DS1302_WRITE( unsigned char cmd,unsigned char dat );//ds1302写地址、数据子程序     
extern unsigned char DS1302_READ(unsigned char cmd);			//ds1302读数据子程序
extern void set_rtc(void);										//时钟初始化
extern void DS1302_PLAY();										//1302读取时间
//------------------------------------------------------------

#endif