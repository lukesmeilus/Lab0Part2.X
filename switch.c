#include <xc.h>
#include "switch.h"

void initSwitch(){
    TRISDbits.TRISD6 = 1;
    CNPUDbits.CNPUD6 = 1;
    CNCONDbits.ON = 1;
    CNENDbits.CNIED6 = 1;
    IFS1bits.CNDIF = 0;
    IEC1bits.CNDIE = 1;
}
