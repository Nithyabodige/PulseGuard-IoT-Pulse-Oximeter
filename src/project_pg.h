/*TYPES.h*/
typedef unsigned char       u8;
typedef signed char         s8;
typedef unsigned short int  u16;
typedef signed short int    s16;
typedef unsigned int        u32;
typedef signed int          s32;
typedef float               f32;

/*DELAY.h*/
void delay_us(u32);
void delay_ms(u32);
void delay_s(u32);

/*INTERRUPT.h*/
void eint0_isr(void)__irq;
void enable_Eint0(void);

/*LCD.h*/
void writelcd(u8);
void cmdlcd(u8);
void lcd_init(void);
void charlcd(u8);
void strlcd(s8 *);
void u32lcd(u32);
void s32lcd(s32);
void f32lcd(f32,u32);
void display(void);

/*KPM.h*/
void init_kpm(void);
u32 colscan(void);
u32 rowcheck(void);
u32 colcheck(void);
u32 keyscan(void);
u32 ReadNum(void);

/*I2C_EEPROM.h*/
void i2c_eeprom_bytewrite(u8,u8,u8);
u8 i2c_eeprom_randomread(u8,u8);
void i2c_eeprom_pagewrite(u8,u8,u8 *,u8);
void i2c_eeprom_seqread(u8,u8,u8 *,u8);

/*I2C.h*/
void init_i2c(void);
void i2c_start(void);
void i2c_restart(void);
void i2c_write(u8);
void i2c_stop(void);
u8 i2c_mack(void);
u8 i2c_nack(void);
u8 i2c_read(u8);

/*MENU.h*/
void settings_menu(void);
void set_hr_lim(void);
void set_spo2_lim(void);
void set_time_int(void);
void maxsensorread(void);

/*SPI*/
void Init_SPI0(void);
u8 SPI0(u8);

/*SPI_EEPROM*/
void Cmd_25LC512(u8 cmd);
void ByteWrite_25LC512(u16 wBuffAddr,u8 dat);
u8   ByteRead_25LC512(u16 rBuffAddr);
void PageWrite_25LC512(u16 pageStartAddr,u8 *ptr128Bytes,u8 n);
void EEPROM_WriteEnable(void);
void EEPROM_Wait(void);

/*MAX30102*/
void max30102_write(u8 reg, u8 value);
void max30102_init(void);
void read_fifo(u32 *red, u32 *ir);
unsigned int calculate_heart_rate(u32 *ir_data, int length);
unsigned int calculate_spo2(u32 *red_data, u32 *ir_data, int length); 
int finger_detected(u32 *ir_data, int length);

#ifndef _UART0_H_
#define _UART0_H_

#define UART_INT_ENABLE 1

//BAUD RATE CALCULATION & DIVISOR
#define FOSC      12000000   //Hz
#define CCLK  	  5*FOSC
#define PCLK  	  CCLK/4
#define BAUD  	  9600
#define DIVISOR   (PCLK/(16 * BAUD))


void InitUART0(void); /* Initialize Serial Interface       */ 
void UART0_Tx(u8 ch);  
u8 UART0_Rx(void); 
void UART0_Str(s8 *);
void UART0_Int(u32);
void UART0_Float(f32);
#endif

#ifndef _ESP01_H_
#define _ESP01_H_

void esp01_connectAP(void);
void esp01_sendToThingspeak(u16,u16);

#endif
/*void timer1_init(void);
void timer1_isr(void)__irq;*/
//TIMER0
void timer0_init(void);
void timer0_isr(void)__irq;
