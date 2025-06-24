#include <LPC21xx.H>
#include "timer_interrupts.h"


#define mCOUNTER_ENABLE     0x00000001
#define mCOUNTER_RESET      0x00000002


#define mINTERRUPT_ON_MR0   0x00000001
#define mRESET_ON_MR0       0x00000002
#define mMR0_INTERRUPT      0x00000001


#define VIC_TIMER0_CHANNEL_NR 4
#define mIRQ_SLOT_ENABLE      0x00000020


static void (*ptrTimer0InterruptFunction)(void);


__irq void Timer0IRQHandler(void) {
    T0IR = mMR0_INTERRUPT;                 
    (*ptrTimer0InterruptFunction)();       
    VICVectAddr = 0x00;                    
}


void Timer0Interrupts_Init(unsigned int uiPeriodUs, void (*ptrInterruptFunction)(void)) {
    ptrTimer0InterruptFunction = ptrInterruptFunction;


    VICIntEnable |= (1 << VIC_TIMER0_CHANNEL_NR);
    VICVectCntl0  = mIRQ_SLOT_ENABLE | VIC_TIMER0_CHANNEL_NR;
    VICVectAddr0  = (unsigned long)Timer0IRQHandler;

    T0MR0 = 15 * uiPeriodUs;
    T0MCR = mINTERRUPT_ON_MR0 | mRESET_ON_MR0;
    T0TCR = mCOUNTER_ENABLE;
}
