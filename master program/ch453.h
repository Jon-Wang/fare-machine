#ifndef _CH453_H_
#define _CH453_H_

#include <c8051f130.h>
#include <intrins.h>
#pragma NOAREGS		// ���MCS51���жϷ�������е���CH453�ӳ������ʹ�ü����жϹ��ܣ���ô�������˱���ѡ��

/* ��ʱ0.1uS�ӳ���,�뵥Ƭ���ٶ��й�,����˫��I/O���Բ���Ҫ��ʱ,���ڴ�������׼˫��I/O���������ʱ */
#define		DELAY_0_1US		{_nop_();}		// MCS51>10MHz

#define		CH453_F		0x01		// ��һ��ch453
#define		CH453_S		0x02		// �ڶ���ch453

/* 2�߽ӿڵ�����,��ʵ�ʵ�·�й� */
/*
sbit	CH453_SCL=P0^3;
sbit	CH453_SDA=P0^2;
*/
#define	CH453_SCL_F P4&0x01		//P4.0
#define	CH453_SDA_F P4&0x02		//P4.1
#define	CH453_SCL_S P4&0x04		//P4.2
#define	CH453_SDA_S P4&0x08		//P4.3

/* 2�߽ӿڵ�λ����,�뵥Ƭ���й� */
/*
#define		CH453_SCL_SET		{CH453_SCL2=1;}
#define		CH453_SCL_CLR		{CH453_SCL2=0;}
*/
#define		CH453_SCL_SET_F		{P4|=0x01;}
#define		CH453_SCL_CLR_F		{P4&=0xfe;}
#define		CH453_SCL_D_OUT_F		{}				// ����SCLΪ�������,����˫��I/O���л�Ϊ���
#define		CH453_SDA_SET_F		{P4|=0x02;}
#define		CH453_SDA_CLR_F		{P4&=0xfd;}
#define		CH453_SDA_D_OUT_F		{}				// ����SDAΪ�������,����˫��I/O���л�Ϊ���

#define		CH453_SCL_SET_S		{P4|=0x04;}
#define		CH453_SCL_CLR_S		{P4&=0xfb;}
#define		CH453_SCL_D_OUT_S		{}				// ����SCLΪ�������,����˫��I/O���л�Ϊ���
#define		CH453_SDA_SET_S		{P4|=0x08;}
#define		CH453_SDA_CLR_S		{P4&=0xf7;}
#define		CH453_SDA_D_OUT_S		{}				// ����SDAΪ�������,����˫��I/O���л�Ϊ���

/* ********************************************************************************************* */
/* �������� */

/* CH453�ĳ��������룬���������I2C���ݣ���ô��8λӦ������1λ */
#define CH453_SYSOFF    0x0400					// �ر���ʾ
#define CH453_SYSON1    0x040b					// ������ʾ
#define CH453_DIG0      0x1000		            // �����λ0��ʾ,�����8λ����
#define CH453_DIG1      0x1100		            // �����λ1��ʾ,�����8λ����
#define CH453_DIG2      0x1200		            // �����λ2��ʾ,�����8λ����
#define CH453_DIG3      0x1300		            // �����λ3��ʾ,�����8λ����
#define CH453_DIG4      0x1400		            // �����λ4��ʾ,�����8λ����
#define CH453_DIG5      0x1500					// �����λ5��ʾ,�����8λ����
#define CH453_DIG6      0x1600					// �����λ6��ʾ,�����8λ����
#define CH453_DIG7      0x1700		            // �����λ7��ʾ,�����8λ����
#define CH453_DIG8      0x1800					// �����λ8��ʾ,�����8λ����
#define CH453_DIG9      0x1900					// �����λ9��ʾ,�����8λ����
#define CH453_DIG10     0x1A00		            // �����λ10��ʾ,�����8λ����
#define CH453_DIG11     0x1B00		            // �����λ11��ʾ,�����8λ����
#define CH453_DIG12     0x1C00		            // �����λ12��ʾ,�����8λ����
#define CH453_DIG13     0x1D00		            // �����λ13��ʾ,�����8λ����
#define CH453_DIG14     0x1E00		            // �����λ14��ʾ,�����8λ����
#define CH453_DIG15     0x1F00		            // �����λ15��ʾ,�����8λ����

// CH453�ӿڶ���
#define		CH453_I2C_ADDR1		0x40			// CH453�ĵ�ַ
#define		CH453_I2C_MASK		0x3E			// CH453�ĸ��ֽ���������
//-----------------------------------------------------------------------------
extern const unsigned char xdata BCD_decode_tab[0x10];
//-------------------------�����ӳ���----------------------------------------
extern void CH453_Write(unsigned short cmd,unsigned char ch453_num);	//д����
extern unsigned char CH453_Read(unsigned char ch453_num );	
extern void CH453_buf_write( unsigned short cmd,unsigned char ch453_num);  // ��CH453������ݻ��߲�������,�Զ���������ӳ��
extern void CH453_buf_index( unsigned char index, unsigned char dat,unsigned char ch453_num);  // ��CH453ָ����������������,�Զ���������ӳ��
extern void CH453_INIT();//CH453��ʼ��
extern void Display_Remaining_Sum(unsigned int remaining_sum_s);	//��ʾ��� 58672
extern void Display_Consumption_Sum(Consumption_Sum);						//��ʾ���ѽ��

#endif
