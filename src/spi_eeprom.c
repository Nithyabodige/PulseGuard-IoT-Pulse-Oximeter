#include <LPC21xx.h>
#include "project_pg.h"
#include "all_defines.h"

void Cmd_25LC512(u8 cmd)
{
  IOCLR0=CS;
  SPI0(cmd);//issue WREN/WRDI
  IOSET0=CS;
}
void ByteWrite_25LC512(u16 addr,u8 dat)
{
  Cmd_25LC512(WREN);//activate write enable latch
	
  //EEPROM_WriteEnable(WREN);

  IOCLR0=CS;
  SPI0(WRITE); //issue WRITE instruction
  SPI0(addr>>8);//msbyte of 16-bit address loc to be written into
  SPI0(addr);//lsbyte of 16-bit address loc to be written into
  SPI0(dat);//data for that address loc
  IOSET0=CS;
  //EEPROM_Wait();
  delay_ms(10);
   Cmd_25LC512(WRDI);//disable/deactivate write enable latch
}  
u8 ByteRead_25LC512(u16 addr)
{
  u8 dat;
  IOCLR0=CS;
  SPI0(READ);   //issue READ instruction
  SPI0(addr>>8);//msbyte of 16-bit address loc to be read
  SPI0(addr);   //lsbyte of 16-bit address loc to be read
  dat=SPI0(0x00);//sending garbage & reading data at loc
  IOSET0=CS;
  return dat;   
}  
void PageWrite_25LC512(u16 pageStartAddr,u8 *ptr128Bytes,u8 n)
{
 u8 i;
  Cmd_25LC512(WREN);//activate write enable latch
  /*IOCLR0=CS;
  SPI0(WRITE);//issue WRITE instruction
  SPI0(pageStartAddr>>8);//msbyte of 16-bit start of page address loc to be written into
  SPI0(pageStartAddr);//lsbyte of 16-bit address start of page loc to be written into
  while(*ptr128Bytes)
    SPI0(*ptr128Bytes++);
  IOSET0=CS;   
  delay_ms(10);
  Cmd_25LC512(WRDI);*///disable/deactivate write enable latch
 
  //EEPROM_WriteEnable();
  IOCLR0=CS;
  SPI0(WRITE);
  SPI0((u8)(pageStartAddr>>8));
  SPI0((u8)pageStartAddr);
  for(i=0;i<n;i++)
  {
  	SPI0(ptr128Bytes[i]);
  }
  IOSET0=CS;
  EEPROM_Wait();
  Cmd_25LC512(WRDI);
}
u8 ReadStatus_25LC512(void)
{
	u8 status;
	IOCLR0 = CS;
	SPI0(RDSR);
	status = SPI0(0x00);
	IOSET0 = CS;
	return status;
}
void EEPROM_Wait(void)
{
	while(ReadStatus_25LC512()&WIP);
}
/*void EEPROM_WriteEnable(u8 cmd)
{
	IOCLR0=CS;
	SPI0(cmd);
	IOSET0=CS;
} */
