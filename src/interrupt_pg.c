#include <lpc21xx.h>
#include "project_pg.h"
#include "all_defines.h"
volatile u8 menu_flag = 0 ;
void eint0_isr(void)__irq
{
	menu_flag = 1;
	settings_menu();
	/*if(menu_flag)
		{
			menu_flag =0;
			cmdLcd(CLEAR_LCD);
			strLcd("INT WORKING MENU");
			delay_ms(1000);	
		}*/
	//clear eint1 flag
	EXTINT =(1<<0);
	//DAMY 
	VICVectAddr = 0;
}
void enable_Eint0(void)
{
	
	EXTINT = (1<<0);
	//cfg 3:2 P0.1 -> 11(3) as eint0 pin
	PINSEL1 = ((PINSEL1 & ~(3<<2)) | (1<<0));
	//SELECT EINT0 AS IRQ
	VICIntSelect &=~ (1<<EINT0_CH);
	//ENABLE THE EINT0 SOURCE
	VICIntEnable |= (1<<EINT0_CH);
	//SELECT SLOT0 FOR EINT0
	VICVectCntl1 = (1<<5)|EINT0_CH;
	//LOAD ISR ADDR INTO ADD REG
	VICVectAddr1 = (u32)eint0_isr;
	//EINT0 TRIGGER AT EDGE
	EXTMODE |= 1<<0;
	//FALLING EDGE
	EXTPOLAR &=~ (1<<0);
	
}

