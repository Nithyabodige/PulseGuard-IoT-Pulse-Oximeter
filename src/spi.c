#include <LPC21xx.h>	  
#include "project_pg.h"
#include "all_defines.h"

// peripheral/hw spi
void Init_SPI0(void)
{
  PINSEL0|=SCK0|MISO0|MOSI0; //select SPI pin functionality  
  S0SPCCR = 14;    // bit clock rate 
  S0SPCR  = (MSTR|Mode_3); //spi module in master mode,
                           //CPOL =1,CCPHA = 1. MSB first
	IOSET0 = CS;
	//IOPIN0 |= CS;
	IODIR0 |= CS; 
	//IOSET0=CS;
}

u8 SPI0(u8 data)
{
   //u8 stat;
   //stat=S0SPSR;    //clear SPIF 
   S0SPDR= data;   // load spi tx reg
   while(((S0SPSR>>7)&1)==0); // wait for transmission to complete
   return S0SPDR;    // read data from SPI data reg, place into buffer 
}


