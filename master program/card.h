#ifndef _CARD_H_
#define _CARD_H_

extern unsigned char CARD_UID_buffer[4];		 //M1����4�ֽ�UID�洢
//----------------------------------------------------------------------------
extern void CARD_CONNECT_Enable();     //02 00 04 A2 31 00 00 93 03    			//���ӿ�Ƭ
//--------------CPU������-----------------------------------------------------------------------
extern void CPU_SELECT_3F01();     //02 00 09 a2 33 00 a4 00 00 02 3F 01 09 03  //ѡ�� 3F01
extern void CPU_GET_FACTOR();     //02 00 07 a2 33 80 ca 00 00 09 D2 03         //��ȡ��ɢ����
extern void CPU_INIT_LOAD();      //02 00 12 a2 33 80 50 01 02 0b 01 00 00 00 02 00 00 01 00 01 08 42 03  // 	Ȧ���ʼ��
extern void CPU_GOTO_CONSUME();		//02 00 16 a2 33 80 54 01 00 0f 00 00 00 04(4hex�ն���ˮ)01 01 01 01 01 01 02 de 4f 7e 3e(4hexα�����) 9C 03//��������
//extern void CPU_READ_0015();    //02 00 07 A2 33 00 B0 95 00 1E AA 03    			//������Ӧ�û����ļ�0015
//extern void CPU_GET_RANDOM();     //02 00 07 A2 33 00 84 00 00 04 11 03    			//ȡ�����
//---------------M1������--------------------------------------------------------------------
extern void M1_AUTOR_KEYA();     //02 00 0E A2 33 60 18 FF FF FF FF FF FF 7E BF AE 0F 89 03    // AUTOR KEY A
extern void M1_READ_BLOCK();     //02 00 04 A2 33 30 18 B9 03	//READ BLOCK
extern void M1_REDUCE_VALUE(unsigned char val);    //02 00 08 A2 33 C0 18 00 00 00 val 4A 03 //�pֵ VAL�����
extern void M1_Transfer();     //02 00 04 A2 33 B0 18 39 03	//Transfer
//-----------------------------------------------------------------------------------
extern void CARD_connect_disable();     //02 00 04 A2 32 00 A0 30 03  //�Ͽ�����
//-------------------------------------------------------------------------------------
extern void M1_Operation_Process();//M1����������
extern void CPU_Operation_Process();//CPU����������

#endif