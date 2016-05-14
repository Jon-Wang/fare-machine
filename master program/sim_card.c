#include "sim_card.h"
#include "delay.h"
#include "uart.h"
#include "intrins.h"

uchar idata tmp[13]={0};    //���������ʱ����
uchar idata tnp[45]={0};    //���������ʱ����

//****************************************************************************
// ��ʱ1��ETU������ʱ�䵥λ
// ��SIM��ʱ��Ƶ��Ϊ4MHz����ETUΪ372/4M=93us
// 12Mʱһ��nop��ʱʱ��1us
//****************************************************************************

//****************************************************************************
// ��SIM����ȡһ���ֽ�
//****************************************************************************
uchar read_SIM_byte()
{

	uchar loop;
	uchar recvdata = 0;
		
	bit Parity = 0; //��żУ��λ

	char  SFRPAGE_SAVE  = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;

	while(SIM_IO); //�ȴ���ʼλ
	delay_us(104); // 93us
	delay_us(104); // 93us
	//delay_1_ETU(); //��ʱһ��etu

	delay_us(52);
	delay_us(52);
	//delay_half_ETU(); //��ʱ���etu
	
	for(loop=0;loop<8;loop++)
	{
		recvdata >>= 1;
		if (SIM_IO)
		{
			recvdata |= 0x80;
		}
		Parity ^= SIM_IO;
		delay_us(104);// 93us
		delay_us(104);// 93us
		//delay_1_ETU(); //��ʱһ��etu
	}
	Parity ^= SIM_IO;
		delay_us(104);// 93us
		delay_us(104);// 93us
	//delay_1_ETU(); //��ʱһ��etu
	
	if(!Parity) //���ֽ���ͨ����żУ����ԭ����ʾ��ͨ��������ʾ0x99
	{
		return recvdata;
	}
	else
	{
		return 0xff;
			  
	}

	SFRPAGE = SFRPAGE_SAVE;
}
//****************************************************************************
// ��SIM������һ���ֽ�
//****************************************************************************
void write_SIM_byte(uchar dataToSend)
{
	uchar loop;
	bit Parity = 0; //��żУ��λ

	char  SFRPAGE_SAVE  = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;


	SIM_IO = 0; //������ʼλ
	delay_us(104);
	delay_us(104);
//	delay_1_ETU(); //��ʱһ��etu
	
	for(loop=0;loop<8;loop++)
	{
		SIM_IO = dataToSend & 0x01;
		Parity ^= dataToSend & 0x01;
		dataToSend >>= 1;
		delay_us(104);
		delay_us(104);
//		delay_1_ETU(); //��ʱһ��etu
	}
	SIM_IO = Parity; //���ǰ8λ���Ľ����1����У��λ��1����������0����0
	delay_us(104);
	delay_us(104);
//	delay_1_ETU(); //��ʱһ��etu
	
	SIM_IO = 1; //��IO����
   	delay_us(104);
   	delay_us(104);
//	delay_1_ETU();
	delay_us(104);
	delay_us(104);
//	delay_1_ETU();

	SFRPAGE = SFRPAGE_SAVE;
}                                                                    
//****************************************************************************
// SIM����ʼ��
//****************************************************************************
void SIM_Init()
{
//	unsigned char i;
	char  SFRPAGE_SAVE  = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;

	SIM_RST = 0; //RST��λ�ź������ṩCLK�źź�400��ʱ�������ڱ��ֵ͵�ƽ
	SIM_IO = 1; //��I/O�˿���Ϊ���շ�ʽ
	SIM_RST = 0; //RST��λ�ź������ṩCLK�źź�400��ʱ�������ڱ��ֵ͵�ƽ

//	delay_400_ETU();
	delay_ms(854);			 //854=400��etu��37.2ms
	delay_ms(854);			 //854=400��etu��37.2ms


	SIM_RST = 1; //�ṩ��λ�źź�400��40000��ʱ�������ڣ�I/O���и�λӦ��ATR

	tmp[0]=read_SIM_byte(); //ATR�źŲ�ֹ3���ֽڣ��˴�ֻ������ǰ3���������ֽڶ���
	tmp[1]=read_SIM_byte();
	tmp[2]=read_SIM_byte();
//	tmp[3]=read_SIM_byte();
//	tmp[4]=read_SIM_byte();
//	tmp[5]=read_SIM_byte();
//	tmp[6]=read_SIM_byte();
//	tmp[7]=read_SIM_byte();
//	tmp[8]=read_SIM_byte();
//  tmp[9]=read_SIM_byte();
//	tmp[10]=read_SIM_byte();
//	tmp[11]=read_SIM_byte();
//	tmp[12]=read_SIM_byte();

	delay_ms(854); 			//854=400��etu��37.2ms
	delay_ms(854); 			//854=400��etu��37.2ms   
//	delay_400_ETU();

	SFRPAGE = SFRPAGE_SAVE;
}

//****************************************************************************
void SIM_select_file(uint FileID_H,uint FileID_L )          ///ѡ���ļ�
{						    
	uchar i;

	write_SIM_byte(0x00);	delay_us(104);delay_us(104);
	write_SIM_byte(0xA4);	delay_us(104);delay_us(104);
	write_SIM_byte(0x00);	delay_us(104);delay_us(104);
	write_SIM_byte(0x00);	delay_us(104);delay_us(104);
	write_SIM_byte(0x02);	delay_us(104);delay_us(104);   //�˴�Ϊʲô��02�����й�����PSAM��Ӧ�ù淶��ͬ

	tnp[0]=read_SIM_byte();			//�˾����Ҫ�ӣ���������ֽڲ��ܶ���

	delay_us(104);delay_us(104);

	write_SIM_byte(FileID_H);delay_us(104);delay_us(104);
    write_SIM_byte(FileID_L);delay_us(104);delay_us(104);



	tnp[1]=read_SIM_byte();
	tnp[2]=read_SIM_byte();
//	tnp[3]=read_SIM_byte();

	for(i=0;i<12;i++)
	{
		delay_us(104);delay_us(104);
	}

}

void SIM_get_response(res_num)		//ȡ��Ӧ����
{						    
	uchar i;

	write_SIM_byte(0x00);
	write_SIM_byte(0xc0);
	write_SIM_byte(0x00);
	write_SIM_byte(0x00);
	write_SIM_byte(res_num);
	
	for(i=0;i<res_num+3;i++)
	{
		tnp[i]=read_SIM_byte();	delay_us(104);delay_us(104);
	}

}

void SIM_read_0016()		//���������ļ�0016
{						    
	uchar i;

	write_SIM_byte(0x00);	delay_us(104);delay_us(104);
	write_SIM_byte(0xb0);	delay_us(104);delay_us(104);
	write_SIM_byte(0x96);	delay_us(104);delay_us(104);
	write_SIM_byte(0x00);	delay_us(104);delay_us(104);
	write_SIM_byte(0x06);	delay_us(104);delay_us(104);

	tnp[0]=read_SIM_byte();
	tnp[1]=read_SIM_byte();
	tnp[2]=read_SIM_byte();
	tnp[3]=read_SIM_byte();
	tnp[4]=read_SIM_byte();
	tnp[5]=read_SIM_byte();
	tnp[6]=read_SIM_byte();
	tnp[7]=read_SIM_byte();
	tnp[8]=read_SIM_byte();

	for(i=0;i<12;i++)
	{
		delay_us(104);delay_us(104);
	}

}



void SIM_auth()		//��֤��ȫ��֤��
{						    
	uchar i;

	write_SIM_byte(0x80);	delay_us(104);delay_us(104);
	write_SIM_byte(0xCA);	delay_us(104);delay_us(104);
	write_SIM_byte(0x00);	delay_us(104);delay_us(104);
	write_SIM_byte(0x00);	delay_us(104);delay_us(104);
	write_SIM_byte(0x09);	delay_us(104);delay_us(104);
	
	tnp[0]=read_SIM_byte();delay_us(104);delay_us(104);
	delay_us(104);delay_us(104);delay_us(104);delay_us(104);
	delay_us(104);delay_us(104);delay_us(104);delay_us(104);
	delay_us(104);

	write_SIM_byte(0x8c);	delay_us(104);delay_us(104);   // 8C 01 30 43 39 5E 82 3E AF, 9 HEX�ķ�ɢ����
	write_SIM_byte(0x01);	delay_us(104);delay_us(104);
	write_SIM_byte(0x30);	delay_us(104);delay_us(104);
	write_SIM_byte(0x43);	delay_us(104);delay_us(104);
	write_SIM_byte(0x39);	delay_us(104);delay_us(104);  
	write_SIM_byte(0x5e);	delay_us(104);delay_us(104);
	write_SIM_byte(0x82);	delay_us(104);delay_us(104);
	write_SIM_byte(0x3e);	delay_us(104);delay_us(104);
	write_SIM_byte(0xaf);	delay_us(104);delay_us(104);

	for(i=0;i<16;i++)			//��ʱ���ӣ��������Ϣ��������ȷ�ģ��������ص�һ�ֽ�84����������ֽ�9000���м��ĸ��ֽڵ������
	{
		delay_us(104);delay_us(104);
	}

	tnp[1]=read_SIM_byte();
	tnp[2]=read_SIM_byte();

	for(i=0;i<12;i++)
	{
		delay_us(104);delay_us(104);
	}

}

void SIM_get_MAC1()		//MAC1Ӌ��
{						    
	uchar i;

	write_SIM_byte(0x80);
	write_SIM_byte(0x70);
	write_SIM_byte(0x00);  
	write_SIM_byte(0x00);  
	write_SIM_byte(0x24);

	tnp[0]=read_SIM_byte();
	delay_us(104);delay_us(104);

//29 6B 93 CB 
	write_SIM_byte(buffer[16]);	//�������Ȧ���ʼ�����ص������
	write_SIM_byte(buffer[17]);
	write_SIM_byte(buffer[18]);
	write_SIM_byte(buffer[19]);


	write_SIM_byte(buffer[9]);  //�û����������,������������ֵ  //�Ƕ�ֵ��ÿ�ν��׺���Ķ�
	write_SIM_byte(buffer[10]);

	write_SIM_byte(0x00);	//���׽��
	write_SIM_byte(0x00);
	write_SIM_byte(0x00);
	write_SIM_byte(0x32);

	write_SIM_byte(0x06);	//�������ͱ�ʶ

	write_SIM_byte(0x01);	//��������
	write_SIM_byte(0x01);
	write_SIM_byte(0x01);
	write_SIM_byte(0x01);


	write_SIM_byte(0x01);	//����ʱ��
	write_SIM_byte(0x01);
	write_SIM_byte(0x02);

	write_SIM_byte(0x01);	//��Կ�汾

	write_SIM_byte(0x00);	//��Կ�㷨��ʶ

	write_SIM_byte(0x15);  //�û������к�
	write_SIM_byte(0x00);
	write_SIM_byte(0x00);
	write_SIM_byte(0x00);
	write_SIM_byte(0x05);
	write_SIM_byte(0xF5);
	write_SIM_byte(0xDC);
	write_SIM_byte(0x2D);

	write_SIM_byte(0x15);	//�Ե���б�ʶ
	write_SIM_byte(0x00);
	write_SIM_byte(0xFF);
	write_SIM_byte(0x00);
	write_SIM_byte(0x00);
	write_SIM_byte(0x00);
	write_SIM_byte(0x00);
	write_SIM_byte(0x00);

	tnp[1]=read_SIM_byte();
	tnp[2]=read_SIM_byte();

	for(i=0;i<12;i++)
	{
		delay_us(104);delay_us(104);
	}

}
void SIM_check_MAC2()		//У��MAC2
{						    
	uchar i;

	write_SIM_byte(0x80);
	write_SIM_byte(0x72);
	write_SIM_byte(0x00);
	write_SIM_byte(0x00);
	write_SIM_byte(0x04);

	tnp[0]=read_SIM_byte();
	delay_us(104);delay_us(104);

//10 AC 96 DA
	write_SIM_byte(buffer[9]); //MAC2��4hex
	write_SIM_byte(buffer[10]); 
	write_SIM_byte(buffer[11]);
	write_SIM_byte(buffer[12]);

	tnp[1]=read_SIM_byte();
	tnp[2]=read_SIM_byte();
	
	for(i=0;i<12;i++)
	{
		delay_us(104);delay_us(104);
	}
}

