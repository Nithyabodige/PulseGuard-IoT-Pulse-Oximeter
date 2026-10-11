#include <LPC21xx.h>
#include "project_pg.h"
#include "all_defines.h"

void init_i2c()
{
	PINSEL0=SCL_FUNC_BIT|SDA_FUNC_BIT;
	I2SCLL=LOADVAL;
	I2SCLH=LOADVAL;
	I2CONSET=1<<I2EN_BIT;
}
void i2c_start()
{
	I2CONSET=1<<STA_BIT;
	while(((I2CONSET>>SI_BIT)&1)==0);
	I2CONCLR=1<<STAC_BIT;
}
void i2c_restart()
{
	I2CONSET=1<<STA_BIT;
	I2CONCLR=1<<SIC_BIT;
	while(((I2CONSET>>SI_BIT)&1)==0);
	I2CONCLR=1<<STAC_BIT;
}
void i2c_write(u8 dat)
{
	I2DAT=dat;
	I2CONCLR=1<<SIC_BIT;
	while(((I2CONSET>>SI_BIT)&1)==0);
}
void i2c_stop()
{
	I2CONSET=1<<STO_BIT;
	I2CONCLR=1<<SIC_BIT;
	while((I2CONSET>>STO_BIT)&1);
}
u8 i2c_mack()
{
	I2CONSET=1<<AA_BIT;
	I2CONCLR=1<<SIC_BIT;
	while(((I2CONSET>>SI_BIT)&1)==0);
	I2CONCLR=1<<AAC_BIT;
	return I2DAT;
}
u8 i2c_nack()
{
	I2CONCLR=1<<AAC_BIT;
	I2CONCLR=1<<SIC_BIT;
	while(((I2CONSET>>SI_BIT)&1)==0);
	return I2DAT;
}
u8 i2c_read(u8 ack)
{
	if(ack)
		return i2c_mack();
	else
		return i2c_nack();
}

