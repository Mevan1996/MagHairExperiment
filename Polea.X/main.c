/* 
 * File:   main.c
 * Author: Mevan Wijewardena
 *
 * Created on 15 August 2019, 1:59 PM
 */

#pragma config FNOSC = FRC              //Initial Oscillator Source Selection bits
#pragma config IESO = OFF               //Internal External Start-up Option bit
#pragma config FCKSM = CSECMD           //Enable Clock Switching
#pragma config OSCIOFNC = OFF       
#pragma config S1OSCIOFNC = OFF          // Slave OSC2 Pin Function bit (OSC2 is general purpose digital I/O pin)
#pragma config ICS = PGD2               // ICD Communication Channel Select bits (Communicate on PGC2 and PGD2)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)
             // JTAG Enable bit (JTAG is disabled)
#define ROW                         1
#define COL                         0

#include <p33CH64MP206.h>
#include "Timer.h"
#include "Ports.h"
#include "Draw.h"
//#include "PWM.h"
//#include "HBridge.h"
//#include "Timer.h"
#include "Sensors.h"
#include "clock1.h"
//#include "PWM.h"
#include "Location.h"



unsigned long       _timerMain;

static volatile int power = 0;

static volatile int button0Status = 0;
static volatile int button1Status = 0;
static volatile int button2Status = 0;




static volatile int time1;
static volatile int time2;
static volatile int time0;
static volatile int time;

int get_power(){
    return power;
}
 
volatile int draw = 0;



void    __attribute__((interrupt,no_auto_psv)) _INT0Interrupt (void) {
    button0Status = 1;
    
    IFS0bits.INT0IF     = 0;
}
void    __attribute__((interrupt,no_auto_psv)) _INT2Interrupt (void) {
    reset_all_pwm();
    
       
    IFS1bits.INT2IF     = 0;
}
void    __attribute__((interrupt,no_auto_psv)) _INT1Interrupt (void) {
    reset_all_pwm();
   
    IFS0bits.INT1IF     = 0;
}

int get_draw(void){
    return draw;
}
void reset_draw(void){
    draw = 0;
}
void    initINT0 (void) {
       
        IEC0bits.INT0IE     = 0;
        IFS0bits.INT0IF     = 0;
        IPC0bits.INT0IP     = 7;    //Max Priority
        
        INTCON2bits.INT0EP  = 0;    // Interrupt on positive edge
        
        IEC0bits.INT0IE     = 1;
}
void    initINT1 (void) {
       
        IEC0bits.INT1IE     = 0;
        IFS0bits.INT1IF     = 0;
        IPC3bits.INT1IP     = 7;    //Max Priority
        
        INTCON2bits.INT1EP  = 0;    // Interrupt on positive edge
        
        IEC0bits.INT1IE     = 1;
}
void    initINT2 (void) {
       
        IEC1bits.INT2IE     = 0;
        IFS1bits.INT2IF     = 0;
        IPC5bits.INT2IP     = 7;    //Max Priority
        
        INTCON2bits.INT2EP  = 0;    // Interrupt on positive edge
        
        IEC1bits.INT2IE     = 1;
}
static volatile int32_t q0 = 0;
static volatile int32_t q1 = 0;
static volatile int32_t q2 = 0;
extern const int _MAXTIME;



int     main(void) {
        clock_conf();
        SRbits.IPL      =   2;
        CORCONbits.IPL3 =   0;
        Ports_init ();
        activate(2,ROW,1,_MAXTIME);
        activate(2,COL,3,_MAXTIME);
//        NVMKEY = 0x55;
//        NVMKEY = 0xAA;
//        RPCONbits.IOLOCK =0;
//        RPINR0 = 35;
//        RPINR1 = 36; 
//        RPCONbits.IOLOCK =0;
//        RPOR7bits.RP46R = 0;
     
        enableInterrupts();
        initInterrupts();
        
        //CNCONBbits.ON     = 1;
        CNPUBbits.CNPUB2  = 0;
        CNPUBbits.CNPUB3  = 0;
        CNPUBbits.CNPUB4  = 0;
        
        CNPDBbits.CNPDB2  = 1;
        CNPDBbits.CNPDB3  = 1;
        CNPDBbits.CNPDB4  = 1;
        
        CNPUAbits.CNPUA0  = 0;
        CNPUAbits.CNPUA1  = 0;
        CNPUAbits.CNPUA2  = 0;
        CNPUAbits.CNPUA3  = 0;
        CNPUAbits.CNPUA4  = 0;
        CNPUCbits.CNPUC0  = 0;
        
        CNPDAbits.CNPDA0  = 1;
        CNPDAbits.CNPDA1  = 1;
        CNPDAbits.CNPDA2  = 1;
        CNPDAbits.CNPDA3  = 1;
        CNPDAbits.CNPDA4  = 1;
        CNPDCbits.CNPDC0  = 1;
        
        init_gestures();
        //initINT0();
        //initINT2();
        //initINT1();
        Ini_timer();
        Led_init();
        
        //_LATB1 = 1;
        int currentr = 0;
        set_en_high();
       // light();
        init_movements();
        while(1){
            light();
            Butontimer();
            Button0Control();
            Button1Control();
            Button2Control();
            if( get_draw_en()){
                //light();
                draw_shape();
            }
            else{
              LATA &= 0xFFE0;
              LATC &= 0xFFFE;
              LATB |= 0x5000;
              LATC |= 0x0020;
              LATD |= 0x00C0;
              //set_enable1();
              //set_shape1();
              //reset_all_pwm();
            }
        }
        return 0;
}
