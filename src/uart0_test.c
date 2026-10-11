#include <LPC21xx.h>
#include "project_pg.h"
#include "all_defines.h"

s8 buff[200]="hello";
s8 dummy;
volatile u8 i=0;
u8 i=0,ch,r_flag;

void UART0_isr(void) __irq
{
  if((U0IIR & 0x04)) //check if receive interrupt
  {
                ch = U0RBR;     /* Read to Clear Receive Interrupt */
                if(i<199)
		{
                        buff[i++] = ch;
			buff[i]='\0';
		}
  }
  else
  {
      dummy=U0IIR; //Read to Clear transmit interrupt

  }
   VICVectAddr = 0; /* dummy write */
}

void InitUART0 (void) /* Initialize Serial Interface       */
{

  PINSEL0 |= 0x00000005; /* Enable RxD0 and TxD0              */
  U0LCR = 0x83;         /* 8 bits, no Parity, 1 Stop bit     */
  U0DLL = DIVISOR;           /* 9600 Baud Rate @ CCLK/4 VPB Clock  */
        U0DLM = DIVISOR>>8;
  U0LCR = 0x03;         /* DLAB = 0  */

  #if UART_INT_ENABLE > 0

  VICIntSelect = 0x00000000; // IRQ
  VICVectAddr0 = (unsigned)UART0_isr;
  VICVectCntl0 = 0x20 | 6; /* UART0 Interrupt */
  VICIntEnable = 1 << 6;   /* Enable UART0 Interrupt */ U0IER = 0x03;       /* Enable UART0 RX and THRE Interrupts */

  #endif

}

void UART0_Tx(u8 ch)  /* Write character to Serial Port    */
{
  while (!(U0LSR & 0x20));
  U0THR = ch;
}

u8 UART0_Rx(void)    /* Read character from Serial Port   */
{
  while (!(U0LSR & 0x01));
  return (U0RBR);
}

void UART0_Str(s8 *s)
{
   while(*s)
       UART0_Tx(*s++);
}

void UART0_Int(u32 n)
{
  u8 a[10]={0,0,0,0,0,0,0,0,0,0};
  int i=0;
  if(n==0)
  {
    UART0_Tx('0');
                return;
  }
  else
  {
     while(n>0)
         {
           a[i++]=(n%10)+48;
           n=n/10;
         } --i;
         for(;i>=0;i--)
         {
           UART0_Tx(a[i]);
         }
   }
}

void UART0_Float(f32 f)
{
  int x;
  float temp;
  x=f;
  UART0_Int(x);
  UART0_Tx('.');
  temp=(f-x)*100;
  x=temp;
  UART0_Int(x);
}
