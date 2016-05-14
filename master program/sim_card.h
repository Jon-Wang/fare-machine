#ifndef _SIM_CARD_H_
#define _SIM_CARD_H_

#include <c8051f130.h>
typedef unsigned char uchar;
typedef unsigned int uint;

/*
sbit SIM_IO = P2^4;  //接SIM卡的I/O引脚   U16
sbit SIM_RST = P2^5; //接SIM卡的RST引脚
*/
/*
sbit SIM_IO = P2^6;  //接SIM卡的I/O引脚   U17
sbit SIM_RST = P2^7; //接SIM卡的RST引脚					//第二版,端口定義
*/

sbit SIM_IO = P2^1;  //接SIM卡的I/O引脚   U19
sbit SIM_RST = P2^0; //接SIM卡的RST引脚
/*
sbit SIM_IO = P2^0;  //接SIM卡的I/O引脚   U20
sbit SIM_RST = P2^1; //接SIM卡的RST引脚
*/
/////////////////////////////////////////////////////////////////////////

extern uchar idata tmp[13];    //存放命令临时数据
extern uchar idata tnp[45];    //存放命令临时数据

extern void SIM_Init();
extern void SIM_select_file(uint FileID_H,uint FileID_L );
extern void SIM_get_response(res_num);		//取响应数据
extern void SIM_read_0016();		//读二进制文件0016

extern void SIM_get_MAC1();		//MAC1計算
extern void SIM_check_MAC2();		//校验MAC2
extern void SIM_auth();		//认证安全认证码
#endif