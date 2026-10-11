#include <LPC21xx.h>
#include "project_pg.h"
#include "all_defines.h"


//variables 
extern u8 hr_min;
extern u8 hr_max;
extern u8 spo2_min;
 u16 time_int;
extern u16 stable_hr;
extern u16 stable_spo2;
extern volatile u8 menu_flag;

int main()
{
	//intializations
	IODIR0|=(1<<BUZZ);
	init_i2c();      // MAX30102
	Init_SPI0();  
	lcd_init();   // 25LC512 EEPROM
	init_kpm();
	InitUART0();
	enable_Eint0();
	max30102_init();
	timer0_init();
	
	cmdlcd(CLEAR_LCD);     //clear lcd
	strlcd(" Pulse Gaurd");
	cmdlcd(GOTO_LINE2_POS0);
	strlcd("starting...");
	delay_ms(2000);
	
	hr_min   = ByteRead_25LC512(HR_MIN_ADDR);
	hr_max   = ByteRead_25LC512(HR_MAX_ADDR);
	spo2_min = ByteRead_25LC512(SPO2_MIN_ADDR);
	time_int  = ByteRead_25LC512(TIME_LSB_ADDR);
	time_int |= ((u16)ByteRead_25LC512(TIME_MSB_ADDR) << 8);
	
	cmdlcd(CLEAR_LCD);
	strlcd(" System Ready");
	delay_ms(1000);
	esp01_connectAP();

    display();
//	delay_ms(1000);

	while(1)
	{
		if(menu_flag)
		{
			menu_flag=0;
			settings_menu();
			display();
		}
									                                                                                         
		//delay_ms(1000);
		maxsensorread();
		//delay_ms(2000);
	}
}
