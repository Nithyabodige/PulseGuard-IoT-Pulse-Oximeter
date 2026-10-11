//delay.c
#include "project_pg.h"
void delay_us(u32 dly_us)
{
	dly_us *=12;
	while(dly_us--);
}
void delay_ms(u32 dly_ms)
{
	dly_ms *=12000;
	while(dly_ms--);
}
void delay_s(u32 dly_s)
{
	dly_s *=12000000;
	while(dly_s--);
}


