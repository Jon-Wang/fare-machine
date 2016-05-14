#ifndef _YUYIN_H_
#define _YUYIN_H_

#include <c8051f130.h>

sbit Yuyin_Rest = P3^0;
sbit K1 = P3^2;			//yuyin_P10
sbit K2 = P3^3;		 	//yuyin_P8
sbit O1 = P3^1;			//yuyin_busy

extern void yuyin_play(unsigned char dat);

#endif