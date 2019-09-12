#include "Timer.h"
#include "Draw.h"
#include <p33CH64MP206.h>
volatile static unsigned long _systemMicroseconds = 0;
volatile static unsigned long _sysMicro = 0;
static volatile int testing = 0 ;
static volatile int poll=0;
void enableInterrupts(void)
{
/* Enable level 1-7 interrupts */
/* No restoring of previous CPU IPL state performed here */
 INTCON2bits.GIE = 1;
return;
}
void disableInterrupts(void)
{
/* Disable level 1-7 interrupts */
/* No saving of current CPU IPL setting performed here */
 INTCON2bits.GIE = 0;
return;
}
void initInterrupts(void)
{
/* Interrupt nesting enabled here */
INTCON1bits.NSTDIS  = 0;

IPC0bits.T1IP       = 6;
/* Reset Timer1 interrupt flag */
IFS0bits.T1IF       = 0;
/* Enable Timer1 interrupt */
IEC0bits.T1IE       = 1;

}
void Ini_timer(void){
    T1CON           = 0x0;
    TMR1            = 0x0;
    PR1             = 50;
    T1CONbits.TCKPS = 0;
    T1CONbits.TECS  = 3;
    T1CONbits.TON   = 1;
    
}
void timer_off(void){
    T1CONbits.TON = 0;
}
void timer_on(void){
    T1CONbits.TON = 1;
}
void Led_init(void){
    return;
}
const int _MAXTIME = 50;  //
static volatile  int _PWM1Time = 0;
static volatile  int _PWMTime = 50;
volatile int _PWM1ONTIME;
volatile int _PWM2ONTIME;
volatile int _PWM3ONTIME;
volatile int _PWM4ONTIME;

volatile uint16_t PWM1LATAPORT;
volatile uint16_t PWM1LATCPORT;

volatile uint16_t PWM2LATAPORT;
volatile uint16_t PWM2LATCPORT;

volatile uint16_t PWM3LATBPORT;
volatile uint16_t PWM3LATCPORT;
volatile uint16_t PWM3LATDPORT;

volatile uint16_t PWM4LATBPORT;
volatile uint16_t PWM4LATCPORT;
volatile uint16_t PWM4LATDPORT;



void __attribute__((__interrupt__,no_auto_psv)) _T1Interrupt(void)
{
    _systemMicroseconds += 1;    
    _sysMicro           +=1;
    if(_PWMTime <_PWM1ONTIME){
         
            LATA |= (PWM1LATAPORT);
            LATC |= (PWM1LATCPORT);
       // _LATD11 = 1;
    }
    if(_PWMTime < (_PWM2ONTIME)){
        LATA |= (PWM2LATAPORT);
        LATC |= (PWM2LATCPORT);
        //_LATD11 = 1;
    }
    if(_PWMTime <_PWM3ONTIME){
        LATB &= (PWM3LATBPORT);
        LATC &= (PWM3LATCPORT);
        LATD &= (PWM3LATDPORT);
        //_LATD11 = 0;
        //_LATD11 = 1;
    }
    if(_PWMTime < _PWM4ONTIME){
        LATB &= (PWM4LATBPORT);
        LATC &= (PWM4LATCPORT);
        LATD &= (PWM4LATDPORT);
       // _LATC7 = 1;
    }
    if(_PWMTime ==  0){
            LATA &= 0xFFE0;
            LATC &= 0xFFFE;
            LATB |= 0x5000;
            LATC |= 0x0020;
            LATD |= 0x00C0;
            
              
            
            _PWMTime= _MAXTIME+1 ;
    }
    
     //   _LATB1 = 1;
    
    //_LATD11^=1;
    _PWMTime--;
     IFS0bits.T1IF = 0;
}

unsigned long   Timer_getSystemMicroSeconds (void) {
         
        return _systemMicroseconds;
}
unsigned long   Timer_getSystemMiliSeconds (void) {
         
        return _systemMicroseconds/1000;
}
unsigned long   Timer_getSystemMicro (void) {
         
        return _sysMicro;
}
unsigned long   Timer_setSystemMicro (void) {
         
         _sysMicro = 0;
}
unsigned long   Timer_setSystemMicroSeconds (void) {
         
        _systemMicroseconds = 0 ;
}