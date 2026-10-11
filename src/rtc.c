#include<LPC21xx.h>
#include "project_pg.h"
#include "all_defines.h"

/*#define FOSC 12000000
#define CCLK (5*FOSC)
#define PCLK (CCLK/4) */
#define PREINT_VAL ((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK-((PREINT_VAL+1)*32768))

void rtc_init(void)
{
	CCR = 0x00;
    PREINT=PREINT_VAL;
    PREFRAC=PREFRAC_VAL;
   
    SEC = 0;
    MIN = 0;
    HOUR = 12;

// Start RTC using external 32.768 kHz clock 
    CCR = 0x11;
}
// GET RTC TIME IN TOTAL SECONDS
u32 rtc_get_seconds(void)
{
    u32 total_seconds;
    total_seconds = ((u32)HOUR * 3600UL);
    total_seconds += ((u32)MIN * 60UL);
    total_seconds += SEC;
    return total_seconds;
}
void rtc_display_time(void)
{
    cmdlcd(0xC0);
    strlcd("TIME ");
    if(HOUR < 10)
        charlcd('0');
    u32lcd(HOUR);
    charlcd(':');
    if(MIN < 10)
        charlcd('0');
    u32lcd(MIN);
    charlcd(':');
    if(SEC < 10)
        charlcd('0');
    u32lcd(SEC);
}
void rtc_display_monitor(u16 hr,u16 spo2_value)
{
	cmdlcd(0x01);
	cmdlcd(0x80);
	strlcd("HR:");
	u32lcd(hr);
	strlcd("spo2:");
	u32lcd(spo2_value);
	rtc_display_time();
}
/*--------------------------------------------------
       START UPLOAD TIMER
       This stores the current RTC time.
       Example:
       RTC = 10:20:47
       last_upload_time = 10*3600 + 20*60 + 47
--------------------------------------------------*/
u32 rtc_start_upload_timer(void)
{
    return rtc_get_seconds();
}
/*--------------------------------------------------
       CHECK UPLOAD INTERVAL
       time_int = seconds
       Returns:
          1 -> upload
          0 -> wait
--------------------------------------------------*/
u8 rtc_upload_time_reached(u32 *last_upload_time,u16 time_int)
{
    u32 current_time;
    u32 elapsed_time;
    if(time_int == 0)
        return 0;
    current_time = rtc_get_seconds();
/* Normal case */
    if(current_time >= *last_upload_time)
    {
        elapsed_time = current_time - *last_upload_time;
    }
/* Midnight rollover */
    else
    {
        elapsed_time=(86400UL - *last_upload_time)+ current_time;
    }
/*Upload when configured interval has elapsed.*/
    if(elapsed_time >= time_int)
    {
        *last_upload_time = current_time;
        return 1;
    }
    return 0;
}

