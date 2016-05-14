/* ************************************************************************

 							CH453 接口子程序

***************************************************************************
 名称：CH453 接口子程序
 时间：2014.2.16
 内容：本程序包括：
	   CH453數碼管显示与按键扫描函数
 说明：CH453的2线接口，只需要2个I/O引脚，兼容I2C/IIC时序，为了节约传输时间，可以适当减少SCL/SDA之间的延时
       如果希望SCL与其它电路共用，那么只要保持SDA不变化，SCL就可以用于任何用途
 	   如果希望SCL和SDA都与其它电路共用，那么关键是确保SDA只在SCL为低电平期间发生变化，所以SCL和SDA适合作为输出引脚使用
          1、对于SCL引脚，直接输出
          2、对于SDA引脚，输出时：
             如果SCL为低电平，那么直接输出，
             如果SCL为高电平，那么先将SCL输出低电平，再将SDA输出，最后将SCL恢复为高电平
 做過的修改： 删除数据映象缓存区  2014 4-30  11:22
*************************************************************************** */
#include	"ch453.h"
#include    "delay.h"

const unsigned char xdata BCD_decode_tab[0x10] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x58, 0x5E, 0x79, 0x71 };		
void CH453_I2c_Start(unsigned char ch453_num)  // 操作起始
{
	char  SFRPAGE_SAVE  = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;
	if(ch453_num==CH453_F)
	{
		CH453_SDA_SET_F;   /*发送起始条件的数据信号*/
		CH453_SDA_D_OUT_F;   /* 设置SDA为输出方向 */
		CH453_SCL_SET_F;
		CH453_SCL_D_OUT_F;   /* 设置SCL为输出方向 */
		DELAY_0_1US;  delay_us(1);	
	
		CH453_SDA_CLR_F;   /*发送起始信号*/
		DELAY_0_1US; delay_us(1);  
	
		CH453_SCL_CLR_F;   /*钳住I2C总线，准备发送或接收数据 */
	}
	if(ch453_num==CH453_S)
	{
		CH453_SDA_SET_S;   /*发送起始条件的数据信号*/
		CH453_SDA_D_OUT_S;   /* 设置SDA为输出方向 */
		CH453_SCL_SET_S;
		CH453_SCL_D_OUT_S;   /* 设置SCL为输出方向 */
		DELAY_0_1US; delay_us(1); 
	
		CH453_SDA_CLR_S;   /*发送起始信号*/
		DELAY_0_1US;  delay_us(1);
	
		CH453_SCL_CLR_S;   /*钳住I2C总线，准备发送或接收数据 */
	}

	SFRPAGE = SFRPAGE_SAVE;
}

void CH453_I2c_Stop(unsigned char ch453_num)  // 操作结束
{
	char  SFRPAGE_SAVE  = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;
	
	if(ch453_num==CH453_F)
	{
		CH453_SDA_CLR_F;
		DELAY_0_1US;delay_us(1);
	
		CH453_SCL_SET_F;
		DELAY_0_1US;delay_us(1);
	
		CH453_SDA_SET_F;  /*发送I2C总线结束信号*/
		DELAY_0_1US;delay_us(1);
	}
	if(ch453_num==CH453_S)
	{
		CH453_SDA_CLR_S;
		DELAY_0_1US;delay_us(1);
	
		CH453_SCL_SET_S;
		DELAY_0_1US;delay_us(1);
	
		CH453_SDA_SET_S;  /*发送I2C总线结束信号*/
		DELAY_0_1US;delay_us(1);
	}

	SFRPAGE = SFRPAGE_SAVE;
}

void CH453_I2c_WrByte(unsigned char dat,unsigned char ch453_num)	//写一个字节数据
{

	unsigned char i;

	char  SFRPAGE_SAVE  = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;
	if(ch453_num==CH453_F)
	{
		for(i=0;i!=8;i++)  // 输出8位数据
		{
			if(dat&0x80) {CH453_SDA_SET_F;}
			else {CH453_SDA_CLR_F;}
			DELAY_0_1US;delay_us(1);
			CH453_SCL_SET_F;
			dat<<=1;
			DELAY_0_1US; delay_us(1);
			CH453_SCL_CLR_F;
		}
		CH453_SDA_SET_F;
		DELAY_0_1US;delay_us(1);
		CH453_SCL_SET_F;  // 接收应答
		DELAY_0_1US;delay_us(1);
		CH453_SCL_CLR_F;
	}
	if(ch453_num==CH453_S)
	{
		for(i=0;i!=8;i++)  // 输出8位数据
		{
			if(dat&0x80) {CH453_SDA_SET_S;}
			else {CH453_SDA_CLR_S;}
			DELAY_0_1US;delay_us(1);
			CH453_SCL_SET_S;
			dat<<=1;
			DELAY_0_1US; delay_us(1); 
			CH453_SCL_CLR_S;
		}
		CH453_SDA_SET_S;
		DELAY_0_1US;delay_us(1);
		CH453_SCL_SET_S;  										// 接收应答
		DELAY_0_1US;delay_us(1);
		CH453_SCL_CLR_S;
	}
	SFRPAGE = SFRPAGE_SAVE;
}

void CH453_Write(unsigned short cmd,unsigned char ch453_num)	//写命令
{
	CH453_I2c_Start(ch453_num);               	/*启动总线*/
   	CH453_I2c_WrByte(((unsigned char)(cmd>>7)&CH453_I2C_MASK)|CH453_I2C_ADDR1,ch453_num);
   	CH453_I2c_WrByte((unsigned char)cmd,ch453_num);               /*发送数据*/
  	CH453_I2c_Stop(ch453_num);                 /*结束总线*/ 
}
unsigned char  CH453_I2c_RdByte( unsigned char ch453_num )
{
	unsigned char dat;
	unsigned char i;

	char  SFRPAGE_SAVE  = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;

	if(ch453_num==CH453_F)
	{
	    CH453_SDA_SET_F;
		DELAY_0_1US;delay_us(1);
		dat = 0;
		for( i = 0; i != 8; i++ )
		{
			CH453_SCL_SET_F;
		DELAY_0_1US;delay_us(1);
	
			dat <<= 1;
			if( CH453_SDA_F) dat++;
	
			CH453_SCL_CLR_F;
		DELAY_0_1US;delay_us(1);
		}  
		CH453_SDA_SET_F;		 
		DELAY_0_1US;delay_us(1);	
		CH453_SCL_SET_F;  	
		DELAY_0_1US;delay_us(1);
		CH453_SCL_CLR_F;
		DELAY_0_1US;delay_us(1);
	}
	if(ch453_num==CH453_S)
	{
	    CH453_SDA_SET_S;
		DELAY_0_1US;delay_us(1);
		dat = 0;
		for( i = 0; i != 8; i++ )
		{
			CH453_SCL_SET_S;
		DELAY_0_1US;delay_us(1);
	
			dat <<= 1;
			if( CH453_SDA_S) dat++;
	
			CH453_SCL_CLR_S;
		DELAY_0_1US;delay_us(1);
		}  
		CH453_SDA_SET_S;		 
		DELAY_0_1US;delay_us(1);	
		CH453_SCL_SET_S;  	
		DELAY_0_1US;delay_us(1);
		CH453_SCL_CLR_S;
		DELAY_0_1US;delay_us(1);
	}
	SFRPAGE = SFRPAGE_SAVE;
	return dat;
}

unsigned char CH453_Read(unsigned char ch453_num )		
{
	unsigned char keycode;
   	CH453_I2c_Start(ch453_num);        
   	CH453_I2c_WrByte((unsigned char)(0x4700>>7)&CH453_I2C_MASK|0x01|CH453_I2C_ADDR1,ch453_num);    
   	keycode=CH453_I2c_RdByte(ch453_num);      
	CH453_I2c_Stop(ch453_num);                
	return keycode;
}

void CH453_buf_write( unsigned short cmd,unsigned char ch453_num)  // 向CH453输出数据或者操作命令,自动建立数据映象
{
	CH453_Write( cmd,ch453_num);	// 发出
}

void CH453_buf_index( unsigned char index, unsigned char dat,unsigned char ch453_num)  // 向CH453指定的数码管输出数据,自动建立数据映象
// index 为数码管序号,有效值为0到15,分别对应DIG0到DIG15
{
	unsigned short cmd;
	if(ch453_num==CH453_F)
	{
		cmd = ( CH453_DIG0 + ( (unsigned short)index << 8 ) ) | dat ;	// 生成操作命令
	}
	if(ch453_num==CH453_S)
	{
		cmd = ( CH453_DIG0 + ( (unsigned short)index << 8 ) ) | dat ;	// 生成操作命令
	}
	CH453_Write( cmd ,ch453_num);	// 发出
}
//-----------------------------------------------------------------
//CH453初始化
//-----------------------------------------------------------------
void CH453_INIT()	//CH453初始化
{
	unsigned char i;
	for (i = 0;i<15;i++ )	CH453_buf_index(i,0,CH453_F);  // 因为CH453复位时不清空显示内容，所以刚开电后必须人为清空，再开显示	
	for (i = 0;i<16;i++ )	CH453_buf_index(i,0,CH453_S);  // 因为CH453复位时不清空显示内容，所以刚开电后必须人为清空，再开显示

	CH453_buf_write(0x040b,CH453_F);						  // 开启显示
	CH453_buf_write(0x040b,CH453_S);						  // 开启显示
											
	CH453_buf_write(CH453_DIG14 | BCD_decode_tab[2],CH453_F);delay_us( 1 );
	CH453_buf_write(CH453_DIG13 | BCD_decode_tab[0],CH453_F);delay_us( 1 );
	CH453_buf_write(CH453_DIG8  | BCD_decode_tab[0],CH453_S);delay_us( 1 );
}
void Display_Remaining_Sum(unsigned int remaining_sum_s)								//显示余额 58672(5位数)
{	
	unsigned char i;
	for ( i = 7; i < 13; i ++ ) CH453_buf_index( i, 0 ,CH453_S);
		CH453_buf_write(CH453_DIG7 | BCD_decode_tab[remaining_sum_s%100%10],CH453_S);delay_us( 1 );	    	//個位
	if((remaining_sum_s<100000)&&(remaining_sum_s>=10))
		CH453_buf_write(CH453_DIG9 | BCD_decode_tab[remaining_sum_s%100/10],CH453_S);delay_us( 1 );	        //十位
	if((remaining_sum_s<100000)&&(remaining_sum_s>=100))
		CH453_buf_write(CH453_DIG10 | BCD_decode_tab[remaining_sum_s/100%10] | 0x80,CH453_S);delay_us( 1 );	//百位
	if((remaining_sum_s<100000)&&(remaining_sum_s>=1000))
		CH453_buf_write(CH453_DIG11 | BCD_decode_tab[remaining_sum_s/1000%10],CH453_S);delay_us( 1 );		//千位	
	if(remaining_sum_s>=10000)
		CH453_buf_write(CH453_DIG12 | BCD_decode_tab[remaining_sum_s/10000],CH453_S);delay_us( 1 );	    	//万位
}
void Display_Consumption_Sum(Consumption_Sum)										//显示消费金额
{			
	CH453_buf_write(CH453_DIG2 | BCD_decode_tab[0] | 0x80,CH453_S);delay_us( 1 );	//显示交易金额
	CH453_buf_write(CH453_DIG1 | BCD_decode_tab[Consumption_Sum],CH453_S);delay_us( 1 );
	CH453_buf_write(CH453_DIG0 | BCD_decode_tab[0],CH453_S);delay_us( 1 );
}
