#include <LPC21xx.H>
#include "servo.h"
#include "led.h"
#include "keyboard.h"
#include "timer_interrupts.h"
#define DETECTOR_bm (1 << 10)

enum ServoState {CALLIB, IDLE, IN_PROGRESS};

struct Servo {
    enum ServoState eState;
    unsigned int uiCurrentPosition;
    unsigned int uiDesiredPosition;
};

static struct Servo sServo;

static enum DetectorState eReadDetector(void){
    if ((IO0PIN & DETECTOR_bm) == 0){
        return INACTIVE;
    } else {
        return ACTIVE;
    }
}

static void DetectorInit(void){
    IO0DIR &= ~DETECTOR_bm;
}

static void Automat(void){
    switch(sServo.eState){
        case CALLIB:
            if (eReadDetector() == INACTIVE) {
                LedStepLeft();
            } else {
                sServo.uiCurrentPosition = 0;
                sServo.uiDesiredPosition = 0;
                sServo.eState = IDLE;
            }
        break;

        case IDLE:
            if (sServo.uiCurrentPosition != sServo.uiDesiredPosition) {
                sServo.eState = IN_PROGRESS;
            }
        break;

        case IN_PROGRESS:
            if (sServo.uiCurrentPosition < sServo.uiDesiredPosition) {
                LedStepRight();
                sServo.uiCurrentPosition++;
            } else if (sServo.uiCurrentPosition > sServo.uiDesiredPosition) {
                LedStepLeft();
                sServo.uiCurrentPosition--;
            } else {
                sServo.eState = IDLE;
            }
        break;
    }
}

void ServoInit(unsigned int uiServoFrequency){
    LedInit();
    DetectorInit();
    KeyboardInit();
    Timer0Interrupts_Init(1000000 / uiServoFrequency, &Automat);
    ServoCallib();
}

void ServoCallib(void){
    sServo.eState = CALLIB;
}

void ServoGoTo(unsigned int uiPosition){
    sServo.uiDesiredPosition = uiPosition;
}
