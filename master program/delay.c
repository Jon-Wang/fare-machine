#include "c8051f130.h"
#include "delay.h"

/*-----------—” ±◊”≥Ã–Ú------------------------*/
void delay_us (unsigned int us)   //Œ¢√Î
{
  while(us--);
}
void delay_ms (unsigned int ms)	//∫¡√Î
{
  unsigned int i,j;
  for(i=0;i<ms;i++)
  for(j=0;j<15;j++)
  delay_us(10);
}
void delay (int z)
{ 
	unsigned char x,y; 
	for(x=z;x>0;x--)
 		for(y=110;y>0;y--);
}
void delay_1 (int c)
{ 
	unsigned char a;
 	for(a=c;a>0;a--) ; 
}
