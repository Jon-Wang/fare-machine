#ifndef _DS1302_H_
#define _DS1302_H_

extern unsigned char l_tmpdate[7];								//��/��/ʱ/��/��/��/��/
//------------------------------------------------------------
extern void DS1302_WRITE( unsigned char cmd,unsigned char dat );//ds1302д��ַ�������ӳ���     
extern unsigned char DS1302_READ(unsigned char cmd);			//ds1302�������ӳ���
extern void set_rtc(void);										//ʱ�ӳ�ʼ��
extern void DS1302_PLAY();										//1302��ȡʱ��
//------------------------------------------------------------

#endif