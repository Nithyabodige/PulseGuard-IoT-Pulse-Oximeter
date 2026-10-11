                        /*  file: mcp3204.c */
#include <LPC21xx.h>
#include "project_pg.h"
#include "all_defines.h"

f32 Read_ADC_MCP3204(u8 channelNo)
{
  u8 hByte,lByte;
  u32 adcVal=0;
   
  //IOCLR0=CS;  //select/activate chip 
	IOPIN0&=~CS;
	
  SPI0(0x06);
  hByte = SPI0(channelNo<<6);
  lByte = SPI0(0x00);
	
	//IOSET0=CS; //de-select/de-activate chp
	IOPIN0|=CS;
	
  adcVal=((hByte&0x0f)<<8)|lByte;
  return ((adcVal*5.0)/4096);
}
                     /* end of mcp3204.c */
