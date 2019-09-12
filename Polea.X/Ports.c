//DSPIC33CH64MP202 Driver
//----------------------------------------------------------------------------------------------------------
//Version 0.0
//----------------------------------------------------------------------------------------------------------
//Microprocessor: DSPIC33CH64MP202
//----------------------------------------------------------------------------------------------------------
//Authors: Juan Pablo Forero (jpabloforcor@gmail.com)
//----------------------------------------------------------------------------------------------------------
//NZ, Auckland, 2018
//----------------------------------------------------------------------------------------------------------
#include <p33CH64MP206.h>
#include "Ports.h"
#include "Draw.h"

#if MAGSKIN == ENABLE
//void switch_off (void){
//    int row = 0;
//    int col = 0;
//    for(row =0 ;row < 6;row++){
//        deactivate(row,1);
//    }
//    for(col =0 ;col < 6;col++){
//        deactivate(col,0);
//    }
//    
//}
void    Ports_init (void) {
        
        _TRISA0             = 0;                    //Pin 4         NC
        _TRISA1             = 0;                    //Pin 5         NC
        _TRISA2             = 0;                    //Pin 6         NC
        _TRISA3             = 0;                    //Pin 7         NC
        _TRISA4             = 0;                    //Pin 8         NC
                
        _ANSELA0            = 0;
        _ANSELA1            = 0;
        _ANSELA2            = 0;
        _ANSELA3            = 0;
        _ANSELA4            = 0;
        
        _LATA0              = 0;
        _LATA1              = 0;        
        _LATA2              = 0;
        _LATA3              = 0;
        _LATA4              = 0;
        _LATC0              = 0;
        
        _TRISB0             = 0;                    //Pin 13        NC
        _TRISB1             = 0;                    //Pin 14        NC
        _TRISB2             = 1;                    //Pin 15        NRF_IRQ
        _TRISB3             = 1;                    //Pin 16        PGD
        _TRISB4             = 1;                    //Pin 17        PGC
        _TRISB5             = 0;                    //Pin 18        RX-USB
        _TRISB6             = 0;                    //Pin 19        TX-USB
        _TRISB7             = 0;                    //Pin 20        RST-USB
        _TRISB8             = 0;                    //Pin 21        NC
        _TRISB9             = 0;                    //Pin 22        MISO
        _TRISB10            = 0;                    //Pin 25        MOSI
        _TRISB11            = 0;                    //Pin 26        SCK
        _TRISB12            = 0;                    //Pin 27        NRF_CS
        _TRISB13            = 0;                    //Pin 28        NRF_CS
        _TRISB14            = 0;                    //Pin 1         NC
        _TRISB15            = 0;                    //Pin 2         NC
            
        _ANSELB0            = 0;
        _ANSELB1            = 0;
        _ANSELB2            = 0;
        _ANSELB3            = 0;
        _ANSELB7            = 0;
        _ANSELB8            = 0;
        _ANSELB9            = 0;
       
        //_LATB4              = 0;
        _LATB5              = 0;
        _LATB6              = 0;
        
        _LATB10             = 0;
        _LATB11             = 0;
        _LATB12             = 1;
        _LATB13             = 0;
        _LATB14             = 1;
        _LATB15             = 0;
        
        _TRISC0             = 0;                    //Pin 13        NC
        _TRISC1             = 0;                    //Pin 14        NC
        _TRISC2             = 0;                    //Pin 15        NRF_IRQ
        _TRISC3             = 0;                    //Pin 16        PGD
        _TRISC4             = 0;                    //Pin 17        PGC
        _TRISC5             = 0;                    //Pin 18        RX-USB
        _TRISC6             = 0;                    //Pin 19        TX-USB
        _TRISC7             = 0;                    //Pin 20        RST-USB
        _TRISC8             = 0;                    //Pin 21        NC
        _TRISC9             = 0;                    //Pin 22        MISO
        _TRISC10            = 0;                    //Pin 25        MOSI
        _TRISC11            = 0;                    //Pin 26        SCK
        _TRISC12            = 0;                    //Pin 27        NRF_CS
        _TRISC13            = 0;                    //Pin 28        NRF_CS
        _TRISC14            = 0;                    //Pin 1         NC
        _TRISC15            = 0;                    //Pin 2         NC
        
        _LATC0 = 0;
        _LATC1 = 0;
        _LATC2 = 0;
        _LATC3 = 0;
        _LATC4 = 0;
        _LATC5 = 1;
        _LATC6 = 0;
        _LATC7 = 0;
        _LATC8 = 0;
        _LATC9 = 0;
        _LATC10 = 0;
        _LATC11 = 0;
        _LATC12 = 0;
        _LATC13 = 0;
        _LATC14 = 0;
        _LATC15 = 0;
        
        _ANSELC0            = 0;
        _ANSELC1            = 0;
        _ANSELC2            = 0;
        _ANSELC7            = 0;
        
        _TRISD0             = 0;                    //Pin 13        NC
        _TRISD1             = 0;                    //Pin 14        NC
        _TRISD2             = 0;                    //Pin 15        NRF_IRQ
        _TRISD3             = 0;                    //Pin 16        PGD
        _TRISD4             = 0;                    //Pin 17        PGC
        _TRISD5             = 0;                    //Pin 18        RX-USB
        _TRISD6             = 0;                    //Pin 19        TX-USB
        _TRISD7             = 0;                    //Pin 20        RST-USB
        _TRISD8             = 0;                    //Pin 21        NC
        _TRISD9             = 0;                    //Pin 22        MISO
        _TRISD10            = 0;                    //Pin 25        MOSI
        _TRISD11            = 0;                    //Pin 26        SCK
        _TRISD12            = 0;                    //Pin 27        NRF_CS
        _TRISD13            = 0;                    //Pin 28        NRF_CS
        _TRISD14            = 0;                    //Pin 1         NC
        _TRISD15            = 0;                    //Pin 2         NC
        
        _LATD0 = 0;
        _LATD1 = 0;
        _LATD2 = 0;
        _LATD3 = 0;
        _LATD4 = 0;
        _LATD5 = 0;
        _LATD6 = 1;
        _LATD7 = 1;
        _LATD8 = 0;
        _LATD9 = 0;
        _LATD10 = 0;
        _LATD11 = 0;
        _LATD12 = 0;
        _LATD13 = 0;
        _LATD14 = 0;
        _LATD15 = 0;

 }

#endif
