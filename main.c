/* 
 * File:   main.c
 * Author: gvanhoy
 * Description: lab 0.
 * Created on August 27, 2015, 10:14 AM
 */

#include <xc.h>
#include <sys/attribs.h>
#include "timer.h"
#include "led.h"
#include "interrupt.h"
#include "config.h"
#include "switch.h"
#include "timer2.h"

#define OUTPUT 0
#define INPUT 1

//TODO: Define states of the state machine
//typedef enum stateTypeEnum{
//} stateType;
typedef enum stateTypeEnum{
    led1, led2, led3, pressed, released, ifTimePressedLessThan1Sec, moreThan1Sec
} stateType;

//TODO: Use volatile variables that change within interrupts
 volatile stateType state = led1;

int main() {
    SYSTEMConfigPerformance(10000000);    //Configures low-level system parameters for 10 MHz clock
    enableInterrupts();                   //This function is necessary to use interrupts.

    //TODO: Write each initialization function
    initLEDs();
    initTimer1();
    initSwitch();
    initTimer2();
    stateType nextState;
    int led;
   
    
    while(1){

        switch(state){
            case led1:
                led = 1;
                LATDbits.LATD0 = 1;
                LATDbits.LATD1 = 0;
                LATDbits.LATD2 = 0;
                if(PORTDbits.RD6 == 0) state = pressed;
                break;
            case led2:
                led = 2;
                LATDbits.LATD0 = 0;
                LATDbits.LATD1 = 1;
                LATDbits.LATD2 = 0;
                if(PORTDbits.RD6 == 0) state = pressed;
                break;
            case led3:
                led = 3;
                LATDbits.LATD0 = 0;
                LATDbits.LATD1 = 0;
                LATDbits.LATD2 = 1;
                if(PORTDbits.RD6 == 0) state = pressed;
                break;
            case pressed:
                delayMs(5);
                state = ifTimePressedLessThan1Sec;
                break;
            case released:
                delayMs(5);
                T1CONbits.ON = 0;
                state = nextState;
                break;
            case ifTimePressedLessThan1Sec:
                if(led == 1)nextState = led2;
                else if(led == 2)nextState = led3;
                else nextState = led1;
                if(T1CONbits.ON == 0) TMR1 = 0;
                T1CONbits.ON = 1;
                if(PORTDbits.RD6 == 1) state = released;
                break;
            case moreThan1Sec:
                if(led == 1) nextState = led3;
                else if(led == 2) nextState = led1;
                else nextState = led2;
                if(PORTDbits.RD6 == 1)state = released;
                break;
        }
        
    }
    
    return 0;
}

void __ISR(_TIMER_1_VECTOR, IPL7AUTO) _T1interupt(){
    IFS0bits.T1IF = 0;
    if(state == ifTimePressedLessThan1Sec) state = moreThan1Sec;
}
