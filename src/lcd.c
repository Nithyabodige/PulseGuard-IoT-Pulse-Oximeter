#include <LPC21xx.h>
#include "project_pg.h"
#include "all_defines.h"
//#define WRITEBYTE(port,bitpos,value) port=(port&~(0xFF<<bitpos))|((value & 0xFF)<<bitpos)
#define DATA 8
#define RS 22
#define RW 18
#define EN 23
void writelcd(u8 byte)
{
	IOCLR0=1<<RW;
	WRITEBYTE(IOPIN0,DATA,byte);
	IOSET0=1<<EN;
	delay_ms(1);
	IOCLR0=1<<EN;
	delay_us(2);
}
void cmdlcd(u8 opcode)
{
	IOCLR0=1<<RS;
	writelcd(opcode);
}
void lcd_init()
{
	IODIR0|=(0xff<<DATA)|(1<<RS)|(1<<EN);
	delay_ms(15);
	cmdlcd(0x30);
	delay_ms(4);
	delay_us(100);
	cmdlcd(0x30);
	delay_us(100);
	cmdlcd(0x30);
	cmdlcd(MODE_8BIT_2LINE);
	cmdlcd(DSP_ON_CUR_BLK);
	cmdlcd(CLEAR_LCD);
	cmdlcd(SHIFT_CUR_RIGHT);
/*	WRITEBYTE(IODIR0,DATA,0xff);
	IODIR0|=(1<<RS)|(1<<EN)|(1<<RW);
	delay_ms(16);
	cmdlcd(0x30);
	delay_ms(6);
	cmdlcd(0x30);
	delay_ms(1);
	cmdlcd(0x30);
	delay_ms(1);
	cmdlcd(0x38);
	cmdlcd(0x10);
	cmdlcd(0x01);
	cmdlcd(0x06);
	cmdlcd(0x0f); */
}

void charlcd(u8 asciival)
{
	IOSET0=1<<RS;
	writelcd(asciival);
}
void strlcd(s8 *str)
{
	while(*str)
		charlcd(*str++);
}
void u32lcd(u32 num)
{
	u8 a[10];
	s32 i=0;
	if(num==0)
	{
		charlcd('0');
		return;
	}
	while(num>0)
	{
		a[i++]=(num%10)+48;
		num=num/10;
		//i++;
	}
	i--;
	while(i>=0)
	{
		charlcd(a[i--]);
	}
}
void s32lcd(s32 num)
{
	if(num<0)
	{
		charlcd('-');
		num=-num;
	}
	u32lcd(num);
}
void f32lcd(f32 fnum,u32 ndp)
{
	u32 num;
	s32 i;
	if(fnum<0.0)
	{
		charlcd('-');
		fnum=-fnum;
	}
	num=fnum;
	u32lcd(num);
	charlcd('.');
	for(i=0;i<ndp;i++)
	{
		fnum=(fnum-num)*10;
		num=fnum;
		charlcd(num+48);
	}
}
void buildcgram(s8 *p,u32 nBytes)
{
	u32 i;
	cmdlcd(GOTO_CGRAM_START);
	IOSET0=1<<RS;
	IOCLR0=1<<RW;
	for(i=0;i < nBytes;i++)
	{
		writelcd(p[i]);
	}
	cmdlcd(GOTO_LINE1_POS0);
}

