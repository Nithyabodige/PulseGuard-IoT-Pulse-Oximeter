#include "project_pg.h"
#include "all_defines.h"
void i2c_eeprom_bytewrite(u8 slaveaddr,u8 wbuffaddr,u8 dat)
{
	i2c_start();
	i2c_write(slaveaddr<<1);
	i2c_write(wbuffaddr);
	i2c_write(dat);
	i2c_stop();
	delay_ms(10);
}
u8 i2c_eeprom_randomread(u8 slaveaddr,u8 rbuffaddr)
{
	u8 dat;
	i2c_start();
	i2c_write(slaveaddr<<1);
	i2c_write(rbuffaddr);
	i2c_restart();
	i2c_write(slaveaddr<<1|1);
	dat=i2c_nack();
	i2c_stop();
	return dat;
}
void i2c_eeprom_pagewrite(u8 slaveaddr,u8 wbuffstartaddr,u8 *p,u8 nbytes)
{
	u8 i;
	i2c_start();
	i2c_write(slaveaddr<<1);
	i2c_write(wbuffstartaddr);
	for(i=0;i<nbytes;i++)
	{
		i2c_write(p[i]);
	}
	i2c_stop();
	delay_ms(10);
}
void i2c_eeprom_seqread(u8 slaveaddr,u8 rbuffstartaddr,u8 *p,u8 nbytes)
{
	u8 i;
	i2c_start();
	i2c_write(slaveaddr<<1);
	i2c_write(rbuffstartaddr);
	i2c_restart();
	i2c_write(slaveaddr<<1|1);
	for(i=0;i<nbytes-1;i++)
	{
		p[i]=i2c_mack();
	}
	p[i]=i2c_nack();
}

