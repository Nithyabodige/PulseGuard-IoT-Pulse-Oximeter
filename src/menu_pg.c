#include <LPC21xx.h>
#include "project_pg.h"
#include "all_defines.h"

#define BUFFER_SIZE 500

extern u32 red_buffer[BUFFER_SIZE];
extern u32 ir_buffer[BUFFER_SIZE];
extern u32 elapsed_time;

u8 hr_min;
u8 hr_max;
u8 spo2_min;
extern u16 time_int;
u16 stable_hr=0;
u16 stable_spo2=0;
u8 upload_done=0;
void settings_menu()
{
	u32 key;
	
	page1:
		cmdlcd(CLEAR_LCD);
		strlcd(" 1.HR 2.SPO2");
		cmdlcd(GOTO_LINE2_POS0);
		strlcd("A.NXT");
		key=keyscan();
		switch(key)
		{
			case '1':
				set_hr_lim();
				goto page1;
			case '2':
				set_spo2_lim();
				goto page1;
			case 'A':
				goto page2;
		}
	page2:
		cmdlcd(CLEAR_LCD);
		cmdlcd(GOTO_LINE1_POS0);
		strlcd("3.TIME");
		cmdlcd(GOTO_LINE2_POS0);
		strlcd("D.EXIT");
		key=keyscan();
		switch(key)
		{
			case '3':
				set_time_int();
				goto page2;
			case 'D':
				return;
		}
}
void set_hr_lim()
{
	u8 temp;
	//hr_min_lim
	while(1)
	{
		cmdlcd(CLEAR_LCD);
		cmdlcd(GOTO_LINE1_POS0);
		strlcd("HR Min:");
		cmdlcd(GOTO_LINE2_POS0);
		
		temp=ReadNum();
		if(temp>=30 && temp<=200)
		{
			hr_min=temp;
			ByteWrite_25LC512(HR_MIN_ADDR, hr_min);
			break;
		}
		cmdlcd(CLEAR_LCD);
    strlcd("Invalid Value");
    delay_ms(1000);
	}
	//hr_max_lim
	while(1)
	{
		cmdlcd(CLEAR_LCD);
		cmdlcd(GOTO_LINE1_POS0);
		strlcd("HR Max:");
		cmdlcd(GOTO_LINE2_POS0);
		
		temp=ReadNum();
		if(temp>hr_min && temp<=220)
		{
			hr_max=(u8)temp;
			ByteWrite_25LC512(HR_MAX_ADDR, hr_max);
		//	hr_max=ByteRead_25LC512(HR_MAX_ADDR);
			break;
		}
		cmdlcd(CLEAR_LCD);
    	strlcd("Invalid Value");
    	delay_ms(1000);
	}
	cmdlcd(CLEAR_LCD);
	cmdlcd(GOTO_LINE1_POS0);
  	strlcd("HR Saved");
  	cmdlcd(GOTO_LINE2_POS0);
	u32lcd(hr_min);
  	charlcd('-');
  	u32lcd(hr_max);
  	delay_ms(1500);
}
void set_spo2_lim()
{
	u16 temp;
	while(1)
	{
		cmdlcd(CLEAR_LCD);
		cmdlcd(GOTO_LINE1_POS0);
		strlcd("SPO2 MIN:");
		cmdlcd(GOTO_LINE2_POS0);
		temp=ReadNum();
		if(temp>=70 && temp<=100)
		{
			spo2_min=(u8)temp;
			ByteWrite_25LC512(SPO2_MIN_ADDR, spo2_min);
			break;
		}
		cmdlcd(CLEAR_LCD);
    	cmdlcd(GOTO_LINE1_POS0);
    	strlcd("Invalid SPO2");
    	delay_ms(1000);
	}
	cmdlcd(CLEAR_LCD);
    cmdlcd(GOTO_LINE1_POS0);
    strlcd("SPO2 Saved");
    cmdlcd(GOTO_LINE2_POS0);
    u32lcd(spo2_min);
    charlcd('%');
    delay_ms(1500);
}
void set_time_int()
{
    u16 temp;
    while(1)
    {
        cmdlcd(CLEAR_LCD);
        cmdlcd(GOTO_LINE1_POS0);
        strlcd("Time Interval:");
        cmdlcd(GOTO_LINE2_POS0);
        temp = ReadNum();
        if(temp >= 1 && temp <= 3600)
        {
            time_int = temp;
            ByteWrite_25LC512(TIME_LSB_ADDR,(u8)(time_int&0xFF));
			ByteWrite_25LC512(TIME_MSB_ADDR,(u8)((time_int>>8)&0xFF));
            break;
        }
        cmdlcd(CLEAR_LCD);
        cmdlcd(GOTO_LINE1_POS0);
        strlcd("Invalid Time");
        delay_ms(1000);
    }
    cmdlcd(CLEAR_LCD);
    cmdlcd(GOTO_LINE1_POS0);
    strlcd("Time Saved");
    cmdlcd(GOTO_LINE2_POS0);
    u32lcd(time_int);
    strlcd(" sec");
    delay_ms(1500);
}
/*void maxsensorread(void)
{
		u16 hr,spo2;
		//u16 stable_hr=0;
		int i;
		for(i=0;i<BUFFER_SIZE;i++)
		{
			read_fifo(&red_buffer[i],&ir_buffer[i]);
			delay_ms(10);
		}
		//finger detection
	if(finger_detected(ir_buffer, BUFFER_SIZE))
    {
		
        hr = calculate_heart_rate(ir_buffer, BUFFER_SIZE);
        spo2 = calculate_spo2(red_buffer, ir_buffer, BUFFER_SIZE);

        if(hr > 0)
        {
			stable_hr=hr;
			stable_spo2=spo2;
			cmdlcd(0x01);
			strlcd(" Finger Placed");
			delay_ms(2000);
            if(stable_hr == 0)
                stable_hr = hr;
            else if(!(hr > stable_hr + 10 || hr + 15 < stable_hr))
                stable_hr = (stable_hr * 4 + hr) / 5;
        }
	if(spo2>=70&&spo2<=100)
	{
		if(stable_spo2==0)
			stable_spo2=spo2;
		else
			stable_spo2=(stable_spo2*4+spo2)/5;
    }
    else
    {
        stable_hr = 0;
		stable_spo2=0;
    }

    cmdlcd(0x01);

    if(stable_hr == 0)
    {
        cmdlcd(0x80);
        strlcd("Place Finger");
    }
    else
    {
        cmdlcd(0x80);
        strlcd("HR:");
        u32lcd(stable_hr);
        strlcd(" BPM");

        cmdlcd(0xC0);
        strlcd("SpO2:");
        u32lcd(stable_spo2);
        strlcd("%");
		delay_ms(1000);
		esp01_sendToThingspeak(stable_hr,stable_spo2);
    }
    delay_ms(500);
}
}*/
void maxsensorread(void)
{
    u16 hr;
    u16 spo2;
    int i;

    /* -----------------------------------------
       Collect MAX30102 samples
       ----------------------------------------- */
    for(i = 0; i < BUFFER_SIZE; i++)
    {
        read_fifo(&red_buffer[i], &ir_buffer[i]);
        delay_ms(10);
    }

    /* -----------------------------------------
       Check whether finger is present
       ----------------------------------------- */
    if(finger_detected(ir_buffer, BUFFER_SIZE))
    {
        /* Calculate HR */
        hr = calculate_heart_rate(ir_buffer, BUFFER_SIZE);

        /* Calculate SpO2 */
        spo2 = calculate_spo2(red_buffer,
                                    ir_buffer,
                                    BUFFER_SIZE);

        /* -------------------------------------
           Valid HR obtained
           ------------------------------------- */
        if(hr > 0)
        {
            stable_hr = hr;
            stable_spo2 = spo2;

            /* ---------------------------------
               Display HR and SpO2
               --------------------------------- */
            cmdlcd(CLEAR_LCD);

            cmdlcd(GOTO_LINE1_POS0);
            strlcd("BP:");
            u32lcd(stable_hr);

            strlcd(" SpO2:");
            u32lcd(stable_spo2);

            cmdlcd(GOTO_LINE2_POS0);
            strlcd("Finger Detected");

            /* ---------------------------------
               Upload ONLY ONCE
               --------------------------------- */
			elapsed_time=0;
			while(elapsed_time!=time_int);
			if((stable_hr<hr_min || stable_hr>hr_max)	|| (stable_spo2<spo2_min))
			 {
			 	cmdlcd(0x01);
			 	 cmdlcd(GOTO_LINE1_POS0);
            	      strlcd("ABNORMAL");
				 IOSET0=(1<<BUZZ);
			}
			else
			{
			IOCLR0=1<<BUZZ;
			}
            if(upload_done == 0)
            {
                upload_done = 1;
			 esp01_sendToThingspeak(stable_hr,stable_spo2);
			//	elapsed_time=0;

            }
        }
    }
    else
    {
        stable_hr = 0;
        stable_spo2 = 0;

        /*
         * Reset upload flag.
         * This allows another upload when
         * the next finger is placed.
         */
        upload_done = 0;

        cmdlcd(CLEAR_LCD);

        cmdlcd(GOTO_LINE1_POS0);
        strlcd("Place Finger");

        cmdlcd(GOTO_LINE2_POS0);
        strlcd("                ");
    }
}
void display()
{
	cmdlcd(0x01);
	cmdlcd(0x80);
	strlcd("hmin:");
	u32lcd(hr_min);
	cmdlcd(0x88);
	strlcd("hmax:");
	u32lcd(hr_max);
	cmdlcd(0xc0);
	strlcd("spo2:");
	u32lcd(spo2_min);
	cmdlcd(0xc8);
	strlcd("TI:");
	u32lcd(time_int);
    delay_ms(1000);
}

