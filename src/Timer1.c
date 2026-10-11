  /* TIMER1 */
#include <LPC21xx.h>
#include "project_pg.h"
#include "all_defines.h"

volatile u32 elapsed_seconds = 0;

void timer1_isr(void)__irq
{

    /* One second has elapsed */
    elapsed_seconds++;
	 T1IR = 0x01; //clear timer1 MR0 interrupt flag
    /* End of interrupt */
    VICVectAddr = 0;
}

void timer1_init(void)
{
    /*
     * PCLK = CCLK/4
     * CCLK = 5 x 12MHz = 60MHz
     * PCLK = 15MHz
     *
     * MR0 = 15,000,000 counts = 1 second
     */

    T1TCR = 0x02;              /* Reset Timer1 */

    T1PR = 14999;                  /* 1 ms timer tick */
    T1MR0 = 1000;          /* match after 1000 ms*/

    /*
     * Interrupt on MR1
     * Reset Timer1 when MR1 is reached
     */
    T1MCR = 0x03;

    /*
     * Timer1 as irq
     */
    VICIntSelect &= ~(1 << 5);

    VICVectAddr5 = (u32)timer1_isr;
    VICVectCntl5 = 0x20 | 5;

    /* Enable Timer0 interrupt */
    VICIntEnable |= (1 << 5);

    /* Start Timer0 */
    T1TCR = 0x01;
}
