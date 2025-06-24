#include "led.h"
#include "TIMER.h"

int main(void)
{
    LedInit();
    InitTimer0Match0(100000); 

    while (1)
    {
        WaitOnTimer0Match0(); 
        LedStepLeft();     
    }
}
