#include "Button.h"
#include "Draw.h"
#define STATE_PRESS 0
#define STATE_DELAY 1
#define STATE_END   2
#define STATE_END2  3


static volatile long button1Timer= 0;
static volatile int ledTime      = 0;
static volatile long button2Timer= 0;
static volatile long button0Timer= 0;
static volatile int button0State = 0;
static volatile int button1State = 0;
static volatile int button2State = 0;
static volatile int led = 0;
static volatile int sha = 0;
static volatile int shape ;
int gestures[30];
void light(void){
    int dup =shape+1 ;
    int c = (dup >>4);
   
        if(dup%2 == 1){
            _LATC7 = 1;
        }
        else{
            _LATC7 = 0;
        }
        dup = dup/2;
        if(dup%2 == 1){
            _LATD11 = 1;
        }
        else{
            _LATD11 = 0;
        }
        dup = dup/2;
        if(dup%2 == 1){
            _LATB0 = 1;
        }
        else{
            _LATB0 = 0;
        }
//        dup = dup/2;
//        if(dup%2 == 1){
//            _LATB0 = 1;
//        }
//        else{
//            _LATB0 = 0;
//        }
//        dup = dup/2;
//        if(dup%2 == 1){
//            _LATB1 = 1;
//            led = 1;
//        }
//        else{
//            _LATB1 = 0;
//            led = 0;
//        }
//    }
//    else{
//        if(ledTime < 100){
//            if(dup%2 == 1){
//                _LATC7 = 1;
//            }
//            else{
//                _LATC7 = 0;
//            }
//            dup = dup/2;
//            if(dup%2 == 1){
//                _LATD10 = 1;
//            }
//            else{
//                _LATD10 = 0;
//            }
//            dup = dup/2;
//            if(dup%2 == 1){
//                _LATD11 =1;
//            }
//            else{
//                _LATD11 = 0;
//            }
//            dup = dup/2;
//            if(dup%2 == 1){
//                _LATB0 = 1;
//            }
//            else{
//                _LATB0 = 0;
//            }
//            dup = dup/2;
////            if(dup%2 == 1){
////                _LATB1 = 1;
////                led = 1;
////            }
////            else{
////                _LATB1 = 0;
////                led = 0;
////            }
//            //ledTime = 0;
//        }
//        else if(ledTime < 200){
//            if(dup%2 == 1){
//                _LATC7 = 0;
//            }
//            else{
//                _LATC7 = 0;
//            }
//            dup = dup/2;
//            if(dup%2 == 1){
//                _LATD10 = 0;
//            }
//            else{
//                _LATD10 = 0;
//            }
//            dup = dup/2;
//            if(dup%2 == 1){
//                _LATD11 =0;
//            }
//            else{
//                _LATD11 = 0;
//            }
//            dup = dup/2;
//            if(dup%2 == 1){
//                _LATB0 = 0;
//            }
//            else{
//                _LATB0 = 0;
//            }
//            dup = dup/2;
////            if(dup%2 == 1){
////                _LATB1 = 1;
////                led = 1;
////            }
////            else{
////                _LATB1 = 0;
////                led = 0;
////            }
//            //ledTime = 0;
//        }
//        else{
//            ledTime = 0;
//        }
//    }
    
}
int get_current_shape(void){
    return shape;
}
void init_gestures(){
    gestures[0]=0;
    gestures[1]=0;
    gestures[2]=1;
    gestures[3]=1;
    gestures[4]=2;
    gestures[5]=2;
    gestures[6]=3;
    gestures[7]=3;
    gestures[8]=4;
    gestures[9]=4;
    gestures[10]=5;
    gestures[11]=5;
//    gestures[6]=1;
//    gestures[7]=3;
//    gestures[8]=1;
//    gestures[9]=3;
//    gestures[10]=0;
//    gestures[11]=2;
//    gestures[12]=1;
//    gestures[13]=3;
//    gestures[14]=4;
//    gestures[15]=3;
//    gestures[16]=4;
//    gestures[17]=3;
//    gestures[18]=2;
//    gestures[19]=4;
//    gestures[20]=1;
//    gestures[21]=2;
//    gestures[22]=3;
//    gestures[23]=3;
//    gestures[24]=4;
//    gestures[25]=3;
//    gestures[26]=0;
//    gestures[27]=0;
//    gestures[28]=2;
//    gestures[29]=0;
  
    shape = gestures[0]; 
}
void set_shape1(){
    shape++;
}
void get_led(){
    return led;
}
void Butontimer(){
    if(Timer_getSystemMicro() > 500){
        button0Timer++;
        button1Timer++;
        button2Timer++;
        ledTime++;
        Timer_setSystemMicro();
    }
}
void Button0Control(void){
    switch(button0State){
        case STATE_PRESS:
            if(PORTBbits.RB2){
                
                button0State = STATE_DELAY;
                
             button0Timer=0;
            }
            break;
        case STATE_DELAY:

            if(button0Timer > 100){
                if(PORTBbits.RB2){
                   button0State = STATE_END;
                   Button0action();
                   button0Timer=0;
                }else{
                    button0State=STATE_PRESS;
                }
            }
            
            break;
            
        case STATE_END:
            if(PORTBbits.RB2==0){
                button0State = STATE_END2;
                button0Timer=0;
                
            }
            
        case STATE_END2:
            if(PORTBbits.RB2==0 & button0Timer>1000){
                button0State = STATE_PRESS;
                
                button0Timer=0;
                
            }
            break;
    }
}
static volatile int power = 0;
void Button1action(){
   // if(power == 0){
        set_enable1();
        //power = 1;
   // }
   // else{
     //   reset_all_pwm();
      //  power = 0;
   // }
    
     
}
void Button2action(){
     
    if(sha == 0){
            sha = 5;
    }
    else{
         sha--;
    }
    //light();
    shape = sha;
}
void Button0action(){
    
        if(sha == 5){
                sha = 0;
        }
        else{
             sha++;
        }
       // light();
        shape = sha;
}
void Button1Control(void){
    switch(button1State){
        case STATE_PRESS:
            if(PORTBbits.RB3){
              
                button1State = STATE_DELAY;
                
             button1Timer=0;
            }
            break;
        case STATE_DELAY:

            if(button1Timer > 100){
                if(PORTBbits.RB3){
                   button1State = STATE_END;
                  Button1action();
                   button1Timer=0;
                }else{
                    button1State=STATE_PRESS;
                }
            }
            
            break;
            
        case STATE_END:
            if(PORTBbits.RB3==0){
                button1State = STATE_END2;
                button1Timer=0;
                
            }
            
        case STATE_END2:
            if(PORTBbits.RB3==0 & button1Timer>1000){
                button1State = STATE_PRESS;
                 
                button1Timer=0;
                
            }
            break;
    }
}
void Button2Control(void){
    switch(button2State){
        case STATE_PRESS:
            if(PORTBbits.RB4){
        
                button2State = STATE_DELAY;
                
             button2Timer=0;
            }
            break;
        case STATE_DELAY:

            if(button2Timer > 100){
                if(PORTBbits.RB4){
                   button2State = STATE_END;
                   Button2action();
                   button2Timer=0;
                }else{
                    button2State=STATE_PRESS;
                }
            }
            
            break;
            
        case STATE_END:
            if(PORTBbits.RB4==0){
                button2State = STATE_END2;
                button2Timer=0;
                
            }
            
        case STATE_END2:
            if(PORTBbits.RB4==0 & button2Timer>250){
                button2State = STATE_PRESS;
                button2Timer=0;
                
            }
            break;
    }
}
