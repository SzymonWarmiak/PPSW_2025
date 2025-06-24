#include "led.h"
#include "keyboard.h"



void Delay(unsigned int uiDelayInMS){

int iLoopCounter;

uiDelayInMS = uiDelayInMS*12000;

    for(iLoopCounter=0; iLoopCounter< uiDelayInMS; iLoopCounter++){}
}



enum State { STATE_LEFT, STATE_STOP, STATE_RIGHT, STATE_BLINKING };
			unsigned char ucBlinkCounter = 0;
			unsigned char ucIsLedOn = 0;

int main(void)
{
			
		  enum State eState = STATE_STOP;
	
	    LedOn(0);
			LedInit();
			KeyboardInit();


	
while(1){
        switch(eState){
            case STATE_STOP:
                if(eKeyboardRead() == BUTTON_0){
                    eState = STATE_LEFT;
                }
                else if(eKeyboardRead() == BUTTON_2){
                    eState = STATE_RIGHT;
                }
                else{
                    eState = STATE_STOP;
                }
                break;

            case STATE_LEFT:
                if(eKeyboardRead() == BUTTON_1){
                    eState = STATE_STOP;
                }
                else if(eKeyboardRead() == BUTTON_2){
                    ucBlinkCounter = 0;
                    ucIsLedOn = 0;
                    eState = STATE_BLINKING;
                }
                else{
                    LedStepLeft();
                    eState = STATE_LEFT;
                }
                break;

            case STATE_BLINKING:
                if(ucBlinkCounter < 10){
                    if(ucIsLedOn == 0){
                        LedOn(0);       // LED0 ON
                        ucIsLedOn = 1;
                    }
                    else{
                        LedOn(4);       // ALL OFF
                        ucIsLedOn = 0;
                        ucBlinkCounter++;
                    }
                    eState = STATE_BLINKING;
                }
                else{
                    eState = STATE_RIGHT;
                }
                break;

            case STATE_RIGHT:
                if(eKeyboardRead() == BUTTON_1){
                    eState = STATE_STOP;
                }
                else{
                    LedStepRight();
                    eState = STATE_RIGHT;
                }
                break;
        }

        Delay(100); // 10 Hz
    }
}

