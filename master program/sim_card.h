#ifndef _SIM_CARD_H_
#define _SIM_CARD_H_

#include <c8051f130.h>
typedef unsigned char uchar;
typedef unsigned int uint;

/*
sbit SIM_IO = P2^4;  //��SIM����I/O����   U16
sbit SIM_RST = P2^5; //��SIM����RST����
*/
/*
sbit SIM_IO = P2^6;  //��SIM����I/O����   U17
sbit SIM_RST = P2^7; //��SIM����RST����					//�ڶ���,�˿ڶ��x
*/

sbit SIM_IO = P2^1;  //��SIM����I/O����   U19
sbit SIM_RST = P2^0; //��SIM����RST����
/*
sbit SIM_IO = P2^0;  //��SIM����I/O����   U20
sbit SIM_RST = P2^1; //��SIM����RST����
*/
/////////////////////////////////////////////////////////////////////////

extern uchar idata tmp[13];    //���������ʱ����
extern uchar idata tnp[45];    //���������ʱ����

extern void SIM_Init();
extern void SIM_select_file(uint FileID_H,uint FileID_L );
extern void SIM_get_response(res_num);		//ȡ��Ӧ����
extern void SIM_read_0016();		//���������ļ�0016

extern void SIM_get_MAC1();		//MAC1Ӌ��
extern void SIM_check_MAC2();		//У��MAC2
extern void SIM_auth();		//��֤��ȫ��֤��
#endif