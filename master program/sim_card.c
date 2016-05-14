#include "sim_card.h"
#include "delay.h"
#include "uart.h"
#include "intrins.h"

uchar idata tmp[13]={0};    //存放命令临时数据
uchar idata tnp[45]={0};    //存放命令临时数据

//****************************************************************************
// 延时1个ETU（基本时间单位
// 如SIM卡时钟频率为4MHz，则ETU为372/4M=93us
// 12M时一个nop延时时间1us
//****************************************************************************

//****************************************************************************
// 从SIM卡读取一个字节
//****************************************************************************
uchar read_SIM_byte()
{

	uchar loop;
	uchar recvdata = 0;
		
	bit Parity = 0; //奇偶校验位

	char  SFRPAGE_SAVE  = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;

	while(SIM_IO); //等待起始位
	delay_us(104); // 93us
	delay_us(104); // 93us
	//delay_1_ETU(); //延时一个etu

	delay_us(52);
	delay_us(52);
	//delay_half_ETU(); //延时半个etu
	
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
		//delay_1_ETU(); //延时一个etu
	}
	Parity ^= SIM_IO;
		delay_us(104);// 93us
		delay_us(104);// 93us
	//delay_1_ETU(); //延时一个etu
	
	if(!Parity) //此字节如通过奇偶校验则原样显示，通不过则显示0x99
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
// 向SIM卡发送一个字节
//****************************************************************************
void write_SIM_byte(uchar dataToSend)
{
	uchar loop;
	bit Parity = 0; //奇偶校验位

	char  SFRPAGE_SAVE  = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;


	SIM_IO = 0; //发送起始位
	delay_us(104);
	delay_us(104);
//	delay_1_ETU(); //延时一个etu
	
	for(loop=0;loop<8;loop++)
	{
		SIM_IO = dataToSend & 0x01;
		Parity ^= dataToSend & 0x01;
		dataToSend >>= 1;
		delay_us(104);
		delay_us(104);
//		delay_1_ETU(); //延时一个etu
	}
	SIM_IO = Parity; //如果前8位异或的结果是1，则校验位发1，如果结果是0，则发0
	delay_us(104);
	delay_us(104);
//	delay_1_ETU(); //延时一个etu
	
	SIM_IO = 1; //将IO拉高
   	delay_us(104);
   	delay_us(104);
//	delay_1_ETU();
	delay_us(104);
	delay_us(104);
//	delay_1_ETU();

	SFRPAGE = SFRPAGE_SAVE;
}                                                                    
//****************************************************************************
// SIM卡初始化
//****************************************************************************
void SIM_Init()
{
//	unsigned char i;
	char  SFRPAGE_SAVE  = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;

	SIM_RST = 0; //RST复位信号需在提供CLK信号后400个时钟周期内保持低电平
	SIM_IO = 1; //将I/O端口置为接收方式
	SIM_RST = 0; //RST复位信号需在提供CLK信号后400个时钟周期内保持低电平

//	delay_400_ETU();
	delay_ms(854);			 //854=400个etu，37.2ms
	delay_ms(854);			 //854=400个etu，37.2ms


	SIM_RST = 1; //提供复位信号后400～40000个时钟周期内，I/O口有复位应答ATR

	tmp[0]=read_SIM_byte(); //ATR信号不止3个字节，此处只接收了前3个，其他字节丢弃
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

	delay_ms(854); 			//854=400个etu，37.2ms
	delay_ms(854); 			//854=400个etu，37.2ms   
//	delay_400_ETU();

	SFRPAGE = SFRPAGE_SAVE;
}

//****************************************************************************
void SIM_select_file(uint FileID_H,uint FileID_L )          ///选择文件
{						    
	uchar i;

	write_SIM_byte(0x00);	delay_us(104);delay_us(104);
	write_SIM_byte(0xA4);	delay_us(104);delay_us(104);
	write_SIM_byte(0x00);	delay_us(104);delay_us(104);
	write_SIM_byte(0x00);	delay_us(104);delay_us(104);
	write_SIM_byte(0x02);	delay_us(104);delay_us(104);   //此处为什么是02？和中国金融PSAM卡应用规范不同

	tnp[0]=read_SIM_byte();			//此句必须要加，否则后续字节不能读出

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

void SIM_get_response(res_num)		//取响应数据
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

void SIM_read_0016()		//读二进制文件0016
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



void SIM_auth()		//认证安全认证码
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

	write_SIM_byte(0x8c);	delay_us(104);delay_us(104);   // 8C 01 30 43 39 5E 82 3E AF, 9 HEX的分散因子
	write_SIM_byte(0x01);	delay_us(104);delay_us(104);
	write_SIM_byte(0x30);	delay_us(104);delay_us(104);
	write_SIM_byte(0x43);	delay_us(104);delay_us(104);
	write_SIM_byte(0x39);	delay_us(104);delay_us(104);  
	write_SIM_byte(0x5e);	delay_us(104);delay_us(104);
	write_SIM_byte(0x82);	delay_us(104);delay_us(104);
	write_SIM_byte(0x3e);	delay_us(104);delay_us(104);
	write_SIM_byte(0xaf);	delay_us(104);delay_us(104);

	for(i=0;i<16;i++)			//延时不加，则错误信息返回是正确的，正常返回第一字节84，最后两个字节9000，中间四个字节的随机数
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

void SIM_get_MAC1()		//MAC1計算
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
	write_SIM_byte(buffer[16]);	//随机数，圈存初始化返回的随机数
	write_SIM_byte(buffer[17]);
	write_SIM_byte(buffer[18]);
	write_SIM_byte(buffer[19]);


	write_SIM_byte(buffer[9]);  //用户卡交易序号,即卡计数器的值  //非定值，每次交易后需改动
	write_SIM_byte(buffer[10]);

	write_SIM_byte(0x00);	//交易金额
	write_SIM_byte(0x00);
	write_SIM_byte(0x00);
	write_SIM_byte(0x32);

	write_SIM_byte(0x06);	//交易类型标识

	write_SIM_byte(0x01);	//交易日期
	write_SIM_byte(0x01);
	write_SIM_byte(0x01);
	write_SIM_byte(0x01);


	write_SIM_byte(0x01);	//交易时间
	write_SIM_byte(0x01);
	write_SIM_byte(0x02);

	write_SIM_byte(0x01);	//密钥版本

	write_SIM_byte(0x00);	//密钥算法标识

	write_SIM_byte(0x15);  //用户卡序列号
	write_SIM_byte(0x00);
	write_SIM_byte(0x00);
	write_SIM_byte(0x00);
	write_SIM_byte(0x05);
	write_SIM_byte(0xF5);
	write_SIM_byte(0xDC);
	write_SIM_byte(0x2D);

	write_SIM_byte(0x15);	//试点城市标识
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
void SIM_check_MAC2()		//校验MAC2
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
	write_SIM_byte(buffer[9]); //MAC2，4hex
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

