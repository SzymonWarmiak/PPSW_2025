#include "servo.h"
#include "uart.h"

extern char cOdebranyZnak;

int main(void) {
    static unsigned int uiTarget = 0;

    ServoInit(50);
    UART_InitWithInt(9600);

    while(1) {
        if (cOdebranyZnak == '1') {
            uiTarget += 24; 
            ServoGoTo(uiTarget);
            cOdebranyZnak = 0;
        }
        if (cOdebranyZnak == 'c' || cOdebranyZnak == 'C') {
            ServoCallib();
            cOdebranyZnak = 0;
						uiTarget = 0;
        }
    }
}
