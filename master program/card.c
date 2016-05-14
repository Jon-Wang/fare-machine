#include "card.h"
#include "uart.h"
#include "yuyin.h"
#include "sim_card.h"
#include "function.h"

unsigned char CARD_UID_buffer[4]={0};		 //����4�ֽ�UID�洢


//****************************************************************************
//CPU��/M1������
//****************************************************************************
void CARD_CONNECT_Enable()     //02 00 04 A2 31 00 00 93 03    			//���ӿ�Ƭ
{
	Uart1_SendByte(0x02);
	Uart1_SendByte(0x00);Uart1_SendByte(0x04);
	Uart1_SendByte(0xA2);Uart1_SendByte(0x31);Uart1_SendByte(0x00);Uart1_SendByte(0x00);
	Uart1_SendByte(0x93);
	Uart1_SendByte(0x03);
}
//****************************************************************************
//CPU������
//****************************************************************************
void CPU_SELECT_3F01()     //02 00 09 a2 33 00 a4 00 00 02 3F 01 09 03  //ѡ�� 3F01
{
	Uart1_SendByte(0x02);
	Uart1_SendByte(0x00);Uart1_SendByte(0x09);
	Uart1_SendByte(0xA2);Uart1_SendByte(0x33);Uart1_SendByte(0x00);Uart1_SendByte(0xA4);
	Uart1_SendByte(0x00);Uart1_SendByte(0x00);Uart1_SendByte(0x02);Uart1_SendByte(0x3F);Uart1_SendByte(0x01);
	Uart1_SendByte(0x09);
	Uart1_SendByte(0x03);
}
//-----------------------------------------------------------------------------
void CPU_GET_FACTOR()     //02 00 07 a2 33 80 ca 00 00 09 D2 03     //��ȡ��ɢ����
{
	Uart1_SendByte(0x02);
	Uart1_SendByte(0x00);Uart1_SendByte(0x07);
	Uart1_SendByte(0xA2);Uart1_SendByte(0x33);Uart1_SendByte(0x80);
	Uart1_SendByte(0xCA);Uart1_SendByte(0x00);Uart1_SendByte(0x00);Uart1_SendByte(0x09);
	Uart1_SendByte(0xD2);
	Uart1_SendByte(0x03);
}
//-----------------------------------------------------------------------------
void CPU_INIT_LOAD()     //02 00 12 a2 33 80 50 01 02 0b 01 00 00 00 32 00 00 01 00 01 11 58 03   // 	Ȧ���ʼ��,����00 00 01 00 01 12/11ΪPOSID
{
	Uart1_SendByte(0x02);
	Uart1_SendByte(0x00);Uart1_SendByte(0x12);
	Uart1_SendByte(0xA2);Uart1_SendByte(0x33);Uart1_SendByte(0x80);Uart1_SendByte(0x50);Uart1_SendByte(0x01);
	Uart1_SendByte(0x02);Uart1_SendByte(0x0B);Uart1_SendByte(0x01);Uart1_SendByte(0x00);Uart1_SendByte(0x00);
	Uart1_SendByte(0x00);Uart1_SendByte(0x32);Uart1_SendByte(0x00);Uart1_SendByte(0x00);Uart1_SendByte(0x01);
	Uart1_SendByte(0x00);Uart1_SendByte(0x01);Uart1_SendByte(0x11);
	Uart1_SendByte(0x6B); //12-68///11-6b
	Uart1_SendByte(0x03);
}
//-----------------------------------------------------------------------------
void CPU_GOTO_CONSUME()		//02 00 16 a2 33 80 54 01 00 0f 00 00 00 04(4hex�ն���ˮ)01 01 01 01 01 01 02 de 4f 7e 3e(4hexα�����) 9C 03//��������
{
	Uart1_SendByte(0x02);
	Uart1_SendByte(0x00);Uart1_SendByte(0x16);
	Uart1_SendByte(0xA2);Uart1_SendByte(0x33);Uart1_SendByte(0x80);Uart1_SendByte(0x54);Uart1_SendByte(0x01);Uart1_SendByte(0x00);Uart1_SendByte(0x0f);
	Uart1_SendByte(tnp[1]);Uart1_SendByte(tnp[2]);Uart1_SendByte(tnp[3]);Uart1_SendByte(tnp[4]);
	Uart1_SendByte(0x01);Uart1_SendByte(0x01);Uart1_SendByte(0x01);Uart1_SendByte(0x01);Uart1_SendByte(0x01);Uart1_SendByte(0x01);Uart1_SendByte(0x02);
	Uart1_SendByte(tnp[5]);Uart1_SendByte(tnp[6]);Uart1_SendByte(tnp[7]);Uart1_SendByte(tnp[8]);
	Uart1_SendByte(0x4B^tnp[1]^tnp[2]^tnp[3]^tnp[4]^0x02^tnp[5]^tnp[6]^tnp[7]^tnp[8]);
	Uart1_SendByte(0x03);
}
//-----------------------------------------------------------------------------
//void CPU_READ_0015()     //02 00 07 A2 33 00 B0 95 00 1E AA 03    			//������Ӧ�û����ļ�0015
//{
//	Uart1_SendByte(0x02);
//	Uart1_SendByte(0x00);Uart1_SendByte(0x07);
//	Uart1_SendByte(0xA2);Uart1_SendByte(0x33);Uart1_SendByte(0x00);Uart1_SendByte(0xB0);Uart1_SendByte(0x95);Uart1_SendByte(0x00);Uart1_SendByte(0x1E);
//	Uart1_SendByte(0xAA);
//	Uart1_SendByte(0x03);
//}
//-----------------------------------------------------------------------------
//void CPU_GET_RANDOM()     //02 00 07 A2 33 00 84 00 00 04 11 03    			//ȡ�����
//{
//	Uart1_SendByte(0x02);
//	Uart1_SendByte(0x00);Uart1_SendByte(0x07);
//	Uart1_SendByte(0xA2);Uart1_SendByte(0x33);Uart1_SendByte(0x00);Uart1_SendByte(0x84);Uart1_SendByte(0x00);Uart1_SendByte(0x00);Uart1_SendByte(0x04);
//	Uart1_SendByte(0x11);
//	Uart1_SendByte(0x03);
//}
//****************************************************************************
//M1������
//****************************************************************************
void M1_AUTOR_KEYA()     //02 00 0E A2 33 60 18 FF FF FF FF FF FF 7E BF AE 0F 89 03    // AUTOR KEY A
{
	Uart1_SendByte(0x02);
	Uart1_SendByte(0x00);Uart1_SendByte(0x0E);
	Uart1_SendByte(0xA2);Uart1_SendByte(0x33);Uart1_SendByte(0x60);Uart1_SendByte(0x18);Uart1_SendByte(0xFF);Uart1_SendByte(0xFF);Uart1_SendByte(0xFF);
	Uart1_SendByte(0xFF);Uart1_SendByte(0xFF);Uart1_SendByte(0xFF);Uart1_SendByte(CARD_UID_buffer[0]);Uart1_SendByte(CARD_UID_buffer[1]);Uart1_SendByte(CARD_UID_buffer[2]);Uart1_SendByte(CARD_UID_buffer[3]);
	Uart1_SendByte(0xE9^CARD_UID_buffer[0]^CARD_UID_buffer[1]^CARD_UID_buffer[2]^CARD_UID_buffer[3]);
	Uart1_SendByte(0x03);
}
//-----------------------------------------------------------------------------
void M1_READ_BLOCK()     //02 00 04 A2 33 30 18 B9 03	//READ BLOCK
{
	Uart1_SendByte(0x02);
	Uart1_SendByte(0x00);Uart1_SendByte(0x04);
	Uart1_SendByte(0xA2);Uart1_SendByte(0x33);Uart1_SendByte(0x30);Uart1_SendByte(0x18);
	Uart1_SendByte(0xB9);
	Uart1_SendByte(0x03);
}
//-----------------------------------------------------------------------------
void M1_REDUCE_VALUE(unsigned char val)    //02 00 08 A2 33 C0 18 val 00 00 00  4A 03 //�pֵ VAL�����
{
	Uart1_SendByte(0x02);
	Uart1_SendByte(0x00);Uart1_SendByte(0x08);
	Uart1_SendByte(0xA2);Uart1_SendByte(0x33);Uart1_SendByte(0xC0);Uart1_SendByte(0x18);
	Uart1_SendByte(val);Uart1_SendByte(0x00);Uart1_SendByte(0x00);Uart1_SendByte(0x00);
	Uart1_SendByte(0x49^val);
	Uart1_SendByte(0x03);
}
//-----------------------------------------------------------------------------
void M1_Transfer()     //02 00 04 A2 33 B0 18 39 03	//Transfer
{
	Uart1_SendByte(0x02);
	Uart1_SendByte(0x00);Uart1_SendByte(0x04);
	Uart1_SendByte(0xA2);Uart1_SendByte(0x33);Uart1_SendByte(0xB0);Uart1_SendByte(0x18);
	Uart1_SendByte(0x39);
	Uart1_SendByte(0x03);
}
//-----------------------------------------------------------------------------
void CARD_connect_disable()     //02 00 04 A2 32 00 A0 30 03  //�Ͽ�����
{
	Uart1_SendByte(0x02);
	Uart1_SendByte(0x00);Uart1_SendByte(0x04);
	Uart1_SendByte(0xA2);Uart1_SendByte(0x32);Uart1_SendByte(0x00);Uart1_SendByte(0xa0);
	Uart1_SendByte(0x30);
	Uart1_SendByte(0x03);
}
//---------------------------------------------------------------------------------------------------------
//M1����������
//---------------------------------------------------------------------------------------------------------
void M1_Operation_Process()
{
	if((buffer[3]==0xC0)&&(buffer[4]==0x00)&&(buffer[5]==0x00)&&(buffer[6]==0x00)&&(buffer[7]==0x04)&&(buffer[14]==0x03))//�˿���M1��
	{  
		//-----------------------------------------------------------------
		if( Uart1_Received_Flag == 1)  			//��ʾ���ڽ�����ϣ������˳�(��һ��ָ��) 
		{	
			M1_AUTOR_KEYA();					// AUTOR KEY A
			Uart1_Judge_received();
			//-----------------------------
			if( Uart1_Received_Flag == 1)  		//��ʾ���ڽ�����ϣ������˳�(��һ��ָ��) 
			{	
				M1_READ_BLOCK(); 				// READ BLOCK
				Uart1_Judge_received();
				//-------------------------------
				remaining_sum = (buffer[7]/16)*16*16*16*16*16+(buffer[7]%16)*16*16*16*16+(buffer[6]/16)*16*16*16+(buffer[6]%16)*16*16+(buffer[5]/16)*16+(buffer[5]%16);//�洢�������
				if(  Uart1_Received_Flag == 1)  //��ʾ���ڽ�����ϣ������˳�(��һ��ָ��) 
				{
					M1_AUTOR_KEYA();      		 // AUTOR KEY A
				   	Uart1_Judge_received();
					//-------------------------------
					if(  Uart1_Received_Flag == 1)  //��ʾ���ڽ�����ϣ������˳�(��һ��ָ��) 
					{
						switch(CARD_UID_buffer[1])
						{
							case 0xBF:	
								M1_REDUCE_VALUE(0x5A);break;	       // �pֵ VAL����� N1
						  	case 0x16:	
								M1_REDUCE_VALUE(0x50);break;	       // �pֵ VAL����� N2
						  	case 0x53:	
								M1_REDUCE_VALUE(0x46);break;	       // �pֵ VAL����� N3
						  	case 0xD8:	
								M1_REDUCE_VALUE(0x3C);break;	       // �pֵ VAL����� N4
							default:				  break;
						}	
					   	Uart1_Judge_received();
						//-------------------------------
						if(  Uart1_Received_Flag == 1)  //��ʾ���ڽ�����ϣ������˳�(��һ��ָ��) 
						{
							M1_AUTOR_KEYA(); 	           // AUTOR KEY A
						 	Uart1_Judge_received();
							//-------------------------------
							if(  Uart1_Received_Flag == 1 )  //��ʾ���ڽ�����ϣ������˳�(��һ��ָ��)
							{
								M1_Transfer();	               // Transfer
							   	Uart1_Judge_received();
							  	//-------------------------------
								if( (Uart1_Received_Flag == 1)&&(buffer[3]!= 0xA0) )  //��ʾ���ڽ�����ϣ������˳�(��һ��ָ��)
								{
									CARD_connect_disable();	                //�Ͽ�����
									yuyin_play(0x00);
									CARD_UID_Distinction_Play();        //����UID����ʾÿ�ſ������ѽ�����������
								}
							 }
						}
					}
				}
			}
		}
	  }//if();�˿���M1��
}
//---------------------------------------------------------------------------------------------------------
//CPU����������
//---------------------------------------------------------------------------------------------------------
void CPU_Operation_Process()					//CPU����������
{
	if((buffer[3]==0xC0)&&(buffer[4]==0x00)&&(buffer[5]==0x00)&&(buffer[6]==0x00)&&(buffer[7]==0x08)&&(buffer[14]==0x03))//�˿���CPU��
	 {
		//------------------------------------
		if( Uart1_Received_Flag == 1)   	   //��ʾ���ڽ�����ϣ������˳�(��һ��ָ��) 
		{	
			CPU_SELECT_3F01(); 				   //ѡ�� 3F01	
			Uart1_Judge_received();	
			//-----------------------------------
			if( Uart1_Received_Flag == 1)      //��ʾ���ڽ�����ϣ������˳�(��һ��ָ��) 
			{	
				CPU_GET_FACTOR();  			   //��ȡ��ɢ����
				Uart1_Judge_received();		
				//------------------------------------
				if( Uart1_Received_Flag == 1)  //��ʾ���ڽ�����ϣ������˳�(��һ��ָ��) 
				{	
					CPU_INIT_LOAD();  		   //Ȧ���ʼ��
					Uart1_Judge_received();	
					remaining_sum = (buffer[7]/16)*16*16*16+(buffer[7]%16)*16*16+(buffer[8]/16)*16+(buffer[8]%16);//�洢�������
				}
		    }
	   	 }
	//	SIM_select_file(0x3f,0x00);
	//	SIM_get_response(0x04);
	//	SIM_read_0016();				//���������ļ�0016���PSAM��posID
		SIM_select_file(0x10,0x01);
		SIM_get_response(0x28);			//0x28������ֵ
		SIM_auth();						//��֤��ȫ��֤��
		//----------------------------------------------
		if((tnp[1] == 0x90)&&(tnp[2] == 0x00)&&(buffer[2] == 0x13)&&(buffer[20] == 0x90)&&(buffer[21] == 0x00))//ֻ�е�Ȧ���ʼ��ͨ�������Ұ�ȫ��֤ͨ��ʱ���Ž���MAC1�ļ���
		{
			SIM_get_MAC1();					//���MAC1
			SIM_get_response(0x08);			//0x08������ֵ
			//---------------------------------------------
			if((tnp[0] == 0xc0)&&(tnp[9] == 0x90)&&(tnp[10] == 0x00)&&(Uart1_Received_Flag == 1))//ֻ�е�MAC1������ȷʱ���Ž�����������
			{	
				CPU_GOTO_CONSUME();			//��������
				Uart1_Judge_received();	 
				//--------------------------------------------------
				if((buffer[2] == 0x0c)&&(buffer[13] == 0x90)&&(buffer[14] == 0x00))//ֻ�е�������ȷʱ���Ž���MAC2��У��
				{
					SIM_check_MAC2();		//У��MAC2
					//------------------------------------------------------
					if( (Uart1_Received_Flag == 1)&&(buffer[3]!= 0xA0) )  //��ʾ���ڽ�����ϣ������˳�(��һ��ָ��)
					{
						CARD_connect_disable();	                //�Ͽ�����
						yuyin_play(0x00);
						CARD_UID_Distinction_Play();        //����UID����ʾÿ�ſ������ѽ�����������
					}
				}
			}
		}
	 }//if();�˿���CPU��
}
