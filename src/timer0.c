/* TIMER0 */
#include <LPC21xx.h>
#include "project_pg.h"
#include "all_defines.h"

volatile u32 elapsed_time = 0;

void timer0_isr(void)__irq
{
    /* Clear Timer0 MR0 interrupt */
    T0IR = 0x01;

    /* One second has elapsed */
    elapsed_time++;

    /* End of interrupt */
    VICVectAddr = 0;
}

void timer0_init(void)
{
    /*
     * PCLK = CCLK/4
     * CCLK = 5 x 12MHz = 60MHz
     * PCLK = 15MHz
     *
     * MR0 = 15,000,000 counts = 1 second
     */

    T0TCR = 0x02;              /* Reset Timer0 */

    T0PR = 0;                  /* No prescaler */
    T0MR0 = 15000000;          /* 1 second */

    /*
     * Interrupt on MR0
     * Reset Timer0 when MR0 is reached
     */
    T0MCR = 0x03;

    /*
     * Timer0 interrupt channel = 4
     */
    VICIntSelect &= ~(1 << 4);

    /*
     * Use VIC slot 2
     * Slot 0 -> UART0
     * Slot 1 -> EINT0
     */
    VICVectAddr2 = (u32)timer0_isr;
    VICVectCntl2 = 0x20 | 4;

    /* Enable Timer0 interrupt */
    VICIntEnable |= (1 << 4);

    /* Start Timer0 */
    T0TCR = 0x01;
}
