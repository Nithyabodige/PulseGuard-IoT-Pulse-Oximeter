/*i2c_eeprom_defines*/
#define I2C_EEPROM_SA1 0x50
#define I2C_EEPROM_SA2 0x51
#define I2C_EEPROM_SA3 0x52
#define I2C_EEPROM_SA4 0x53
#define I2C_EEPROM_SA5 0x54
#define I2C_EEPROM_SA6 0x55
#define I2C_EEPROM_SA7 0x56
#define I2C_EEPROM_SA8 0x57
//locations
#define HR_MIN_ADDR     0
#define HR_MAX_ADDR     1
#define SPO2_MIN_ADDR   2
#define TIME_LSB_ADDR   3
#define TIME_MSB_ADDR   4

/*lcd defines*/
#define CLEAR_LCD          0x01
#define MODE_8BIT_2LINE    0x38
#define DSP_ON_CUR_BLK     0x0F
#define SHIFT_CUR_RIGHT    0x14
/*i2c_eeprom_defines*/
#define I2C_EEPROM_SA1 0x50
#define I2C_EEPROM_SA2 0x51
#define I2C_EEPROM_SA3 0x52
#define I2C_EEPROM_SA4 0x53
#define I2C_EEPROM_SA5 0x54
#define I2C_EEPROM_SA6 0x55
#define I2C_EEPROM_SA7 0x56
#define I2C_EEPROM_SA8 0x57
//locations
#define HR_MIN_ADDR     0
#define HR_MAX_ADDR     1
#define SPO2_MIN_ADDR   2
#define TIME_LSB_ADDR   3
#define TIME_MSB_ADDR   4

/*lcd defines*/
#define CLEAR_LCD          0x01
#define MODE_8BIT_2LINE    0x38
#define DSP_ON_CUR_BLK     0x0F
#define SHIFT_CUR_RIGHT    0x14
#define GOTO_CGRAM_START   0x40

#define GOTO_LINE1_POS0    0x80
#define GOTO_LINE1_POS1    0x81
#define GOTO_LINE1_POS2    0x82
#define GOTO_LINE2_POS0    0xC0
#define GOTO_LINE2_POS1    0xC1
#define GOTO_LINE2_POS2    0xC2       

#define GOTO_CGRAM_START   0x40

#define GOTO_LINE1_POS0    0x80
#define GOTO_LINE1_POS1    0x81
#define GOTO_LINE1_POS2    0x82
#define GOTO_LINE2_POS0    0xC0
#define GOTO_LINE2_POS1    0xC1
#define GOTO_LINE2_POS2    0xC2

/*kpm defines*/
#define ROW0 16
#define ROW1 17
#define ROW2 18
#define ROW3 19
#define COL0 20
#define COL1 21
#define COL2 22
#define COL3 23

/*i2c defines*/
#define SCL_FUNC_BIT   (1<<4)	  //0.2
#define SDA_FUNC_BIT   (1<<6)     //0.3
#define LOADVAL    75

#define AA_BIT     2
#define SI_BIT     3
#define STO_BIT    4
#define STA_BIT    5
#define I2EN_BIT   6

#define AAC_BIT    2
#define SIC_BIT    3
#define STAC_BIT   5

/*interrupt*/
#define EINT0_CH   14

/*spi_defines*/
//defines for SPI0 Pin Function Select
#define SCK0     0x00000100  						//P0.4 
#define MISO0    0x00000400  						//P0.5
#define MOSI0    0x00001000  						//P0.6  
#define CS       (1<<7)      						//p0.7 
// Control Register Bits Setting 
#define Mode_0  0x00  									// CPOL 0 CPHA 0
#define Mode_1  0x08  									// CPOL 0 CPHA 1
#define Mode_2  0x10  									// CPOL 1 CPHA 0
#define Mode_3  0x18  									// CPOL 1 CPHA 1
#define MSTR   (1<<5) 									// SPI0 as Master 
#define LSBF   (1<<6) 									// default MSB first,if set LSB first
// Status Register bits
#define SPIF (1<<7) 										// Data Transfer Completion Flag

/*spi_eeprom_defines*/
//Instruction Set 
#define READ   0x03 
#define WRITE  0x02 
#define WRDI   0x04 
#define WREN   0x06 

#define RDSR   0x05
#define WIP    0x01

                   /* defines.h */
									 
#ifndef __DEFINES_H__
#define __DEFINES_H__

#define SETBIT(WORD,BITPOS)            (WORD|=(1<<BITPOS))
#define CLRBIT(WORD,BITPOS)            (WORD&=~(1<<BITPOS))
#define CPLBIT(WORD,BITPOS)            (WORD^=(1<<BITPOS))
#define WRITEBIT(WORD,BITPOS,BIT)      (BIT?SETBIT(WORD,BITPOS):CLRBIT(WORD,BITPOS))
#define READBIT(WORD,BITPOS)           ((WORD>>BITPOS)&1)
#define READWRITEBIT(WORD,WBIT,RBIT)   (WORD=((WORD&~(1<<WBIT))|(((WORD>>RBIT)&1)<<WBIT)))
#define WRITEUNIBBLE(WORD,BITPOS,BYTE) (WORD=(WORD&~(0x0000000F<<BITPOS))|((BYTE>>4)<<BITPOS))
#define WRITELNIBBLE(WORD,BITPOS,BYTE) (WORD=(WORD&~(0x0000000F<<BITPOS))|((BYTE&0X0F)<<BITPOS))
#define READNIBBLE(WORD,BITPOS)        ((WORD>>BITPOS)&0x0000000F)
#define WRITEBYTE(WORD,BITPOS,BYTE)    (WORD=(WORD&~(0x000000FF<<BITPOS))|(BYTE<<BITPOS))      
#define READBYTE(WORD,BITPOS)          ((WORD>>BITPOS)&0x000000FF)

#endif

#define BUZZ 20

