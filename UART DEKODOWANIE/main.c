#include "uart.h"
#include "servo.h"
#include "command_decoder.h"
#include "led.h"
#include "TIMER.h"
#include <string.h>

#define RECIEVER_SIZE 20

char cOdebranyLancuch[RECIEVER_SIZE];

int main(void) {
	ServoInit(50);
  UART_InitWithInt(9600);

    while (1) {
        if (eReciever_GetStatus() == READY) {
            memset(cOdebranyLancuch, 0, RECIEVER_SIZE); 
            Reciever_GetStringCopy(cOdebranyLancuch);
            DecodeMsg(cOdebranyLancuch);

            if (ucTokenNr > 0 && asToken[0].eType == KEYWORD) {
                switch (asToken[0].uValue.eKeyword) {
                    case CALLIB:
                        ServoCallib();
                        LedOn(0); 
                        break;
                    case GOTO:
                        if (ucTokenNr >= 2 && asToken[1].eType == NUMBER) {
                            ServoGoTo(asToken[1].uValue.uiValue);
                            LedOn(1); 
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
}
