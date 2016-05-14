#include "function.h"
#include "card.h"
#include "ch453.h"
#include "yuyin.h"
#include "delay.h"
#include "ds1302.h"
//------------------------------
unsigned int remaining_sum = 0;			//存储消费余额
//----------------------------------------------------------
void CARD_UID_Distinction_Play()        //根据UID，显示每张卡的消费金额，及语音提醒
{
	unsigned char i;
	switch(CARD_UID_buffer[1])
	{
		case 0xBF:	
			Display_Consumption_Sum(9);			 // 显示消费金额 N1 0.9
			Display_Remaining_Sum(remaining_sum);//显示消费余额
			yuyin_play(0x00);					 //N1 
			break;	       
		case 0x16:	
			Display_Consumption_Sum(8);			 // 显示消费金额 N2 0.8		
			Display_Remaining_Sum(remaining_sum);//显示消费余额
			yuyin_play(0x00);yuyin_play(0x02);	 //N2    愛心卡
			break;	       
		case 0x53:	
			Display_Consumption_Sum(7);			 // 显示消费金额 N3 0.7		
			Display_Remaining_Sum(remaining_sum);//显示消费余额
			yuyin_play(0x00);yuyin_play(0x03);	 //N3    退休卡
			break;	      
		case 0xD8:	
			Display_Consumption_Sum(6);			  // 显示消费金额 N4 0.6
			Display_Remaining_Sum(remaining_sum); //显示消费余额
			yuyin_play(0x00);yuyin_play(0x04);	  //N4    老年卡
			break;	
 		case 0x30:	
			Display_Consumption_Sum(5);			  // 显示消费金额 N4 0.5
			Display_Remaining_Sum(remaining_sum); //显示消费余额
			yuyin_play(0x00);yuyin_play(0x02);	  //CPU   老年卡
			break;	     
		default:				      
	 		break;	
	}
//	delay_ms(10000);
	 //-----------------------------------------------------------------
	for (i = 7;i<13;i++ )
	{	
		CH453_buf_index(i,0,CH453_S);  //读卡结束后，消费余额人为清空
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void DS1302_PLAY_TIME()			 //1302时间显示
{
	DS1302_PLAY();
	CH453_buf_write(CH453_DIG2 | BCD_decode_tab[l_tmpdate[0]/10],CH453_F);delay_us( 1 );	  //秒
	CH453_buf_write(CH453_DIG1 | BCD_decode_tab[l_tmpdate[0]%10],CH453_F);delay_us( 1 );
	CH453_buf_write(CH453_DIG4 | BCD_decode_tab[l_tmpdate[1]/10],CH453_F);delay_us( 1 );	  //分
	CH453_buf_write(CH453_DIG3 | BCD_decode_tab[l_tmpdate[1]%10],CH453_F);delay_us( 1 );

	CH453_buf_write(CH453_DIG5 | BCD_decode_tab[l_tmpdate[2]%10]  | 0x80,CH453_F);delay_us( 1 );
	if(l_tmpdate[2]>9)
	CH453_buf_write(CH453_DIG6 | BCD_decode_tab[l_tmpdate[2]/10],CH453_F);delay_us( 1 );	  //时

	CH453_buf_write(CH453_DIG7 | BCD_decode_tab[l_tmpdate[3]/10],CH453_F);delay_us( 1 );	  //日
	CH453_buf_write(CH453_DIG8 | BCD_decode_tab[l_tmpdate[3]%10],CH453_F);delay_us( 1 );
//	CH453_buf_write(CH453_DIG10 | BCD_decode_tab[l_tmpdate[4]/10],CH453_F);delay_us( 1 );	  //月
	CH453_buf_write(CH453_DIG9 | BCD_decode_tab[l_tmpdate[4]%10],CH453_F);delay_us( 1 );
	CH453_buf_write(CH453_DIG12 | BCD_decode_tab[l_tmpdate[6]/10],CH453_F);delay_us( 1 );	  //年
	CH453_buf_write(CH453_DIG11 | BCD_decode_tab[l_tmpdate[6]%10],CH453_F);delay_us( 1 );

}
