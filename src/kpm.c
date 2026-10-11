#include <LPC21xx.h>
#include "project_pg.h"
#include "all_defines.h"

u32 kpmLUT[4][4]={
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','=','D'}
};
void init_kpm()
{
	IODIR1|=15<<ROW0;
}
u32 colscan()
{
	if(((IOPIN1>>COL0)&15)<15)
	{
		return 0;
	}
	else
		return 1;
}
u32 rowcheck()
{
	u32 rno;
	for(rno=0;rno<4;rno++)
	{
		IOPIN1=(IOPIN1&~(15<<ROW0))|(~(1<<rno)<<ROW0);
		if(colscan()==0)
		{
			break;
		}
	}
		IOCLR1=15<<ROW0;
		return rno;
}
u32 colcheck()
{
	u32 cno;
	for(cno=0;cno<4;cno++)
	{
		if(((IOPIN1>>(COL0+cno))&1)==0)
		{
				break;
		}
	}
		return cno;
}
	
u32 keyscan(void)
{
    u32 rno,cno,key;
    while(colscan());          // Wait for key press
    delay_ms(20);              // Debounce
    rno = rowcheck();
    cno = colcheck();
    key = kpmLUT[rno][cno];
    while(!colscan());         // Wait until key released
    delay_ms(20);            	 // Debounce
    return key;
}
u32 ReadNum(void)
{
    u8 key;
    u32 num = 0;
    //u8 digits = 0;
	 cmdlcd(GOTO_LINE2_POS0);
    while(1)
    {
        key = keyscan(); 
        if(key >= '0' && key <= '9')
        {
            //if(digits < 5)// Maximum 3 digits
            //{
                num = (num * 10) + (key - '0');
              //  digits++;
                charlcd(key);
            //}
        }
        else if(key == '*')//backspace
        {
           // if(digits > 0)
            //{
                num /= 10;
              //  digits--;
                cmdlcd(GOTO_LINE2_POS0);
                strlcd("                ");
                cmdlcd(GOTO_LINE2_POS0);
                u32lcd(num);
           // }
		}
        else if(key == '=') //Save
        {
			cmdlcd(0x01);
			cmdlcd(GOTO_LINE1_POS0);
            return num;
        }
    }
}
