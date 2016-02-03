#include <xc.h>
#include "timer2.h"

#define PRESCALAR_1 0

void initTimer2(){
    TMR2 = 0;
    T2CONbits.TCKPS = PRESCALAR_1;
    T2CONbits.TCS = 0;
    IEC0bits.T1IE = 1;
    IPC1bits.T1IP = 7;
    IFS0bits.T1IF = 0;
}

void delayMs(int delay){
    TMR2 = 0;
    PR2 = delay * 9999;
    IFS0bits.T2IF = 0;
    T2CONbits.ON = 1;
    while(IFS0bits.T2IF == 0);
    T2CONbits.ON = 0;
}
