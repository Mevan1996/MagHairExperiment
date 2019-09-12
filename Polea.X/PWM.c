#include "PWM.h"
#include <p33CH64MP206.h>
#include <stdint.h>
void PWMInitialization(void){
    //PWM master period
    PG4PER   = 10;
    //PWM master high period
    PG4DC    =  0;
    //PWM master phase
    PG4PHASE =     0;
    
    PG4DC = 0;
    /* Select PWM Generator Duty Cycle Register as PG4DC */
    /* Select PWM Generator Duty Cycle Register as PG4ER */
    /* Select PWM Generator Duty Cycle Register as PG4PHASE */
    /* PWM Generator broadcasts software set of UPDREQ control */
    /* bit and EOC signal to other PWM Generators. */
    /* PWM Buffer Update Mode is at start of next PWM cycle if UPDREQ = 1 */
    /* PWM generator operates in Single Trigger Mode */
    /* Start of Cycle is local EOC */
    PG4CONH = 0x0800;
    /* PWM Generator is disabled */
    /* PWM Generator uses Master Clock selected by the PCLKCONbits.MCLKSEL bits */
    /* PWM Generator operates in Independent Edge PWM mode */
    PG4CONL = 0x0008;
    /* PWM Generator Output Mode is Complementary Mode */
    /* PWM Generator controls the PWMxH output pin */
    /* PWM Generator controls the PWMxL output pin */
    PG4IOCONH = 0x000C; 
     /* Override is enabled on PWMxH/L with OVRDAT = 0b00, turning OFF PWM
    outputs */
    /* User output overrides are synchronized to next start of cycle */
    //PG1IOCONL = 0x3000;
     /* PGxTRIGA register compare event is enabled as trigger source for ADC
    Trigger 1 */
    /* A write of the PGxDC register automatically sets the UPDREQ bit */
    /* PWM generator trigger output is EOC*/
     /* Select PWM Generator Duty Cycle Register as MDC */
    /* Select PWM Generator Period Register as MPER */
    /* Select PWM Generator Phase Register as MPHASE */
    /* PWM generator does not broadcast UPDATE status bit or EOC signal
    * to other PWM generators */
    /* PWM Buffer Update Mode is slaved immediate*/
    /* PWM generator operates in Single Trigger Mode */
    /* Start of Cycle is PG1 trigger output selected by
    * PG1EVTbits.PGTRGSEL<2:0> bits */
     /* Override is enabled on PWMxH/L with OVRDAT = 0b00, turning OFF PWM
    outputs */
    /* User output overrides are synchronized to next start of cycle */
    //PG2IOCONL = 0x0300;

    PG4EVTL = 0x0008;
    
    PG4CONH = 0x6301;
    /* PWM Generator is enabled */
    /* PWM Generator uses Master Clock selected by the PCLKCONbits.MCLKSEL bits */
    /* PWM Generator operates in Independent Edge PWM mode*/
    PG4CONL = 0x0008;
    /* PWM Generator output operates in Complementary Mode */
    /* PWM Generator controls the PWMxH output pin */
    /* PWM Generator controls the PWMxL output pin */
    PG4IOCONH = 0x000C;
   
    NVMKEY = 0x55;
    NVMKEY = 0xAA;
    RPCONbits.IOLOCK =0;
    //RPOR7bits.RP46R = 0b100010;  //PWM1H
    //RPOR7bits.RP47R = 0b100011;  //PWM1L
    //RPOR6bits.RP44R = 0b100010;  //PWM2H
    //RPOR6bits.RP45R = 0b100011;  //PWM2L
    PG4CONLbits.ON = 1;
}
