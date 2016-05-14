/* ************************************************************************

 							CH453 �ӿ��ӳ���

***************************************************************************
 ���ƣ�CH453 �ӿ��ӳ���
 ʱ�䣺2014.2.16
 ���ݣ������������
	   CH453���a����ʾ�밴��ɨ�躯��
 ˵����CH453��2�߽ӿڣ�ֻ��Ҫ2��I/O���ţ�����I2C/IICʱ��Ϊ�˽�Լ����ʱ�䣬�����ʵ�����SCL/SDA֮�����ʱ
       ���ϣ��SCL��������·���ã���ôֻҪ����SDA���仯��SCL�Ϳ��������κ���;
 	   ���ϣ��SCL��SDA����������·���ã���ô�ؼ���ȷ��SDAֻ��SCLΪ�͵�ƽ�ڼ䷢���仯������SCL��SDA�ʺ���Ϊ�������ʹ��
          1������SCL���ţ�ֱ�����
          2������SDA���ţ����ʱ��
             ���SCLΪ�͵�ƽ����ôֱ�������
             ���SCLΪ�ߵ�ƽ����ô�Ƚ�SCL����͵�ƽ���ٽ�SDA��������SCL�ָ�Ϊ�ߵ�ƽ
 ���^���޸ģ� ɾ������ӳ�󻺴���  2014 4-30  11:22
*************************************************************************** */
#include	"ch453.h"
#include    "delay.h"

const unsigned char xdata BCD_decode_tab[0x10] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x58, 0x5E, 0x79, 0x71 };		
void CH453_I2c_Start(unsigned char ch453_num)  // ������ʼ
{
	char  SFRPAGE_SAVE  = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;
	if(ch453_num==CH453_F)
	{
		CH453_SDA_SET_F;   /*������ʼ�����������ź�*/
		CH453_SDA_D_OUT_F;   /* ����SDAΪ������� */
		CH453_SCL_SET_F;
		CH453_SCL_D_OUT_F;   /* ����SCLΪ������� */
		DELAY_0_1US;  delay_us(1);	
	
		CH453_SDA_CLR_F;   /*������ʼ�ź�*/
		DELAY_0_1US; delay_us(1);  
	
		CH453_SCL_CLR_F;   /*ǯסI2C���ߣ�׼�����ͻ�������� */
	}
	if(ch453_num==CH453_S)
	{
		CH453_SDA_SET_S;   /*������ʼ�����������ź�*/
		CH453_SDA_D_OUT_S;   /* ����SDAΪ������� */
		CH453_SCL_SET_S;
		CH453_SCL_D_OUT_S;   /* ����SCLΪ������� */
		DELAY_0_1US; delay_us(1); 
	
		CH453_SDA_CLR_S;   /*������ʼ�ź�*/
		DELAY_0_1US;  delay_us(1);
	
		CH453_SCL_CLR_S;   /*ǯסI2C���ߣ�׼�����ͻ�������� */
	}

	SFRPAGE = SFRPAGE_SAVE;
}

void CH453_I2c_Stop(unsigned char ch453_num)  // ��������
{
	char  SFRPAGE_SAVE  = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;
	
	if(ch453_num==CH453_F)
	{
		CH453_SDA_CLR_F;
		DELAY_0_1US;delay_us(1);
	
		CH453_SCL_SET_F;
		DELAY_0_1US;delay_us(1);
	
		CH453_SDA_SET_F;  /*����I2C���߽����ź�*/
		DELAY_0_1US;delay_us(1);
	}
	if(ch453_num==CH453_S)
	{
		CH453_SDA_CLR_S;
		DELAY_0_1US;delay_us(1);
	
		CH453_SCL_SET_S;
		DELAY_0_1US;delay_us(1);
	
		CH453_SDA_SET_S;  /*����I2C���߽����ź�*/
		DELAY_0_1US;delay_us(1);
	}

	SFRPAGE = SFRPAGE_SAVE;
}

void CH453_I2c_WrByte(unsigned char dat,unsigned char ch453_num)	//дһ���ֽ�����
{

	unsigned char i;

	char  SFRPAGE_SAVE  = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;
	if(ch453_num==CH453_F)
	{
		for(i=0;i!=8;i++)  // ���8λ����
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
		CH453_SCL_SET_F;  // ����Ӧ��
		DELAY_0_1US;delay_us(1);
		CH453_SCL_CLR_F;
	}
	if(ch453_num==CH453_S)
	{
		for(i=0;i!=8;i++)  // ���8λ����
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
		CH453_SCL_SET_S;  										// ����Ӧ��
		DELAY_0_1US;delay_us(1);
		CH453_SCL_CLR_S;
	}
	SFRPAGE = SFRPAGE_SAVE;
}

void CH453_Write(unsigned short cmd,unsigned char ch453_num)	//д����
{
	CH453_I2c_Start(ch453_num);               	/*��������*/
   	CH453_I2c_WrByte(((unsigned char)(cmd>>7)&CH453_I2C_MASK)|CH453_I2C_ADDR1,ch453_num);
   	CH453_I2c_WrByte((unsigned char)cmd,ch453_num);               /*��������*/
  	CH453_I2c_Stop(ch453_num);                 /*��������*/ 
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

void CH453_buf_write( unsigned short cmd,unsigned char ch453_num)  // ��CH453������ݻ��߲�������,�Զ���������ӳ��
{
	CH453_Write( cmd,ch453_num);	// ����
}

void CH453_buf_index( unsigned char index, unsigned char dat,unsigned char ch453_num)  // ��CH453ָ����������������,�Զ���������ӳ��
// index Ϊ��������,��ЧֵΪ0��15,�ֱ��ӦDIG0��DIG15
{
	unsigned short cmd;
	if(ch453_num==CH453_F)
	{
		cmd = ( CH453_DIG0 + ( (unsigned short)index << 8 ) ) | dat ;	// ���ɲ�������
	}
	if(ch453_num==CH453_S)
	{
		cmd = ( CH453_DIG0 + ( (unsigned short)index << 8 ) ) | dat ;	// ���ɲ�������
	}
	CH453_Write( cmd ,ch453_num);	// ����
}
//-----------------------------------------------------------------
//CH453��ʼ��
//-----------------------------------------------------------------
void CH453_INIT()	//CH453��ʼ��
{
	unsigned char i;
	for (i = 0;i<15;i++ )	CH453_buf_index(i,0,CH453_F);  // ��ΪCH453��λʱ�������ʾ���ݣ����Ըտ���������Ϊ��գ��ٿ���ʾ	
	for (i = 0;i<16;i++ )	CH453_buf_index(i,0,CH453_S);  // ��ΪCH453��λʱ�������ʾ���ݣ����Ըտ���������Ϊ��գ��ٿ���ʾ

	CH453_buf_write(0x040b,CH453_F);						  // ������ʾ
	CH453_buf_write(0x040b,CH453_S);						  // ������ʾ
											
	CH453_buf_write(CH453_DIG14 | BCD_decode_tab[2],CH453_F);delay_us( 1 );
	CH453_buf_write(CH453_DIG13 | BCD_decode_tab[0],CH453_F);delay_us( 1 );
	CH453_buf_write(CH453_DIG8  | BCD_decode_tab[0],CH453_S);delay_us( 1 );
}
void Display_Remaining_Sum(unsigned int remaining_sum_s)								//��ʾ��� 58672(5λ��)
{	
	unsigned char i;
	for ( i = 7; i < 13; i ++ ) CH453_buf_index( i, 0 ,CH453_S);
		CH453_buf_write(CH453_DIG7 | BCD_decode_tab[remaining_sum_s%100%10],CH453_S);delay_us( 1 );	    	//��λ
	if((remaining_sum_s<100000)&&(remaining_sum_s>=10))
		CH453_buf_write(CH453_DIG9 | BCD_decode_tab[remaining_sum_s%100/10],CH453_S);delay_us( 1 );	        //ʮλ
	if((remaining_sum_s<100000)&&(remaining_sum_s>=100))
		CH453_buf_write(CH453_DIG10 | BCD_decode_tab[remaining_sum_s/100%10] | 0x80,CH453_S);delay_us( 1 );	//��λ
	if((remaining_sum_s<100000)&&(remaining_sum_s>=1000))
		CH453_buf_write(CH453_DIG11 | BCD_decode_tab[remaining_sum_s/1000%10],CH453_S);delay_us( 1 );		//ǧλ	
	if(remaining_sum_s>=10000)
		CH453_buf_write(CH453_DIG12 | BCD_decode_tab[remaining_sum_s/10000],CH453_S);delay_us( 1 );	    	//��λ
}
void Display_Consumption_Sum(Consumption_Sum)										//��ʾ���ѽ��
{			
	CH453_buf_write(CH453_DIG2 | BCD_decode_tab[0] | 0x80,CH453_S);delay_us( 1 );	//��ʾ���׽��
	CH453_buf_write(CH453_DIG1 | BCD_decode_tab[Consumption_Sum],CH453_S);delay_us( 1 );
	CH453_buf_write(CH453_DIG0 | BCD_decode_tab[0],CH453_S);delay_us( 1 );
}
