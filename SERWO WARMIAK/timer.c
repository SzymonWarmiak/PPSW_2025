#include <LPC21xx.H>
#include "timer.h"

#define mCOUNTER_ENABLE (1 << 0)
#define mCOUNTER_RESET  (1 << 1)
#define mINTERRUPT_ON_MR0  (1 << 0)
#define mRESET_ON_MR0      (1 << 1)

void InitTimer0(void){
    T0TCR = mCOUNTER_ENABLE; 
}

void WaitOnTimer0(unsigned int uiTimeUs){
    T0TCR |= mCOUNTER_RESET;        
    T0TCR &= ~mCOUNTER_RESET;       

    while (T0TC < (uiTimeUs * 15))  
    {}
}
void InitTimer0Match0(unsigned int uiDelayTime)
{
    T0MR0 = uiDelayTime * 15;

    T0TCR = mCOUNTER_RESET;
    T0TCR = mCOUNTER_ENABLE;

    T0MCR = mINTERRUPT_ON_MR0 | mRESET_ON_MR0;
}

void WaitOnTimer0Match0(void){
    while ((T0IR & mINTERRUPT_ON_MR0) == 0) {}

    T0IR = mINTERRUPT_ON_MR0;
}
