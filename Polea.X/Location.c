#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Location.h"
#include <math.h>
#include <p33CH64MP206.h>
#include "clock1.h"


static volatile int onned_x1;
static volatile int onned_y1;
static volatile int onned_x2;
static volatile int onned_y2;
static volatile int onned_pwm_x1;
static volatile int onned_pwm_y1;
static volatile int onned_pwm_x2;
static volatile int onned_pwm_y2;
unsigned long mTime1;
unsigned long mTime2;




static volatile double current_x                = 1;
static volatile double current_y                = 1;
//static volatile int   board_state               = STATE_INIT;

const static double magnetic_constant           = 1;

static volatile int en1                         = 0;
static volatile double sensor_positions[16][2]  ={{2,2},{2,6},{2,10},{2,14},{6,2},{6,6},{6,10},{6,14},{10,2},{10,6},{10,10},{10,14},{14,2},{14,6},{14,10},{14,14}};

int location_grid[8][8];

int current_location(){
    int row;
    int column;
    int keypressed;
    TRISB = 0b0000000011111111;
    LATB  = 0b1111111100000000;
    //add a delay if nessessarry
    keypressed= PORTB;
    TRISB = 0b1111111100000000;
    LATB  = 0b0000000011111111;
    //add a delay if nessessarry
   //needed TRISB = 0b1111111100000000;
    keypressed ^= PORTB;
    int  bitsX, bitsY;
    for(bitsX =0 ; bitsX<8; bitsX++){
        //needed LATB        = (1<<bitsX);
        keypressed  = PORTB;
        keypressed  <<= 8;
        for(bitsY = 0 ; bitsY <8;bitsY++){
              location_grid[bitsX][bitsY] = (keypressed%2);
              keypressed<<=1;
        }
    }
    for(bitsX =0 ; bitsX<8; bitsX++){
        for(bitsY = 0 ; bitsY <8;bitsY++){
            if(location_grid[bitsX][bitsY] && bitsX<7 && bitsY<7 && bitsX>0 && bitsY>0){
                if(location_grid[bitsX-1][bitsY] && location_grid[bitsX+1][bitsY] && location_grid[bitsX][bitsY-1] && location_grid[bitsX][bitsY+1]){
                    return (bitsX*10)+bitsY;
                }
                
            }  
        }
    }
    for (bitsX =1 ; bitsX<7; bitsX++){
        if(location_grid[bitsX][0]){
                if(location_grid[bitsX-1][0] && location_grid[bitsX+1][0] && location_grid[bitsX][1]){
                    return (bitsX*10);
                }
        }  
    }
    for (bitsX =1 ; bitsX<7; bitsX++){
        if(location_grid[bitsX][7]){
                if(location_grid[bitsX-1][7] && location_grid[bitsX+1][7] && location_grid[bitsX][6]){
                    return (bitsX*10)+7;
                }
        }  
    }
    for (bitsY =1 ; bitsY<7; bitsY++){
        if(location_grid[0][bitsY]){
                if(location_grid[0][bitsY-1] && location_grid[0][bitsY+1] && location_grid[1][bitsY]){
                    return (bitsY);
                }
        }  
    }
    for (bitsY =1 ; bitsY<7; bitsY++){
        if(location_grid[7][bitsY]){
                if(location_grid[7][bitsY-1] && location_grid[7][bitsY+1] && location_grid[6][bitsY]){
                    return 70+(bitsY);
                }
        }  
    }
    if(location_grid[0][0] && location_grid[1][0] && location_grid[0][1]){
        return 0;
    }
    if(location_grid[7][7] && location_grid[7][6] && location_grid[6][7]){
        return 77;
    }
    if(location_grid[0][7] && location_grid[1][7] && location_grid[0][6]){
        return 7;
    }
    if(location_grid[7][0] && location_grid[7][1] && location_grid[6][0]){
        return 70;
    }
    for(bitsX =0 ; bitsX<8; bitsX++){
        for(bitsY = 0 ; bitsY <8;bitsY++){
            if(location_grid[bitsX][bitsY]){
                return (bitsX*10)+bitsY;
            }
        }
    }
    return 33;
}

//void get_current_position(){
//    int *p    = get_max_sensors();
//    int *q    = get_sensor_values();
//    double s1 = pow((magnetic_constant/(*(q+*(p))))  , 0.33333);
//    double s2 = pow((magnetic_constant/(*(q+*(p+1)))), 0.33333);
//    double s3 = pow((magnetic_constant/(*(q+*(p+2)))), 0.33333);
//    double y1 = sensor_positions[*(p)][1];
//    double x1 = sensor_positions[*(p)][0];
//    double y2 = sensor_positions[*(p+1)][1];
//    double x2 = sensor_positions[*(p+1)][0];
//    double y3 = sensor_positions[*(p+2)][1];
//    double x3 = sensor_positions[*(p+2)][0];
//    double m1 = s1-s3 + (y1*y1)+(x3*x3)-(x1*x1)-(y3*y3);
//    double m2 = s1-s2 + (y1*y1)+(x2*x2)-(x1*x1)-(y2*y2);
//    double m3 = -(m1*y2)+(m2*y3);
//    double m4 = -(m1*x2)+(m2*x3);
//    double d  = -(x3*y2)+(x2*y3);
//    current_x = m3/d;
//    current_y = m4/d;
//    grid_x = (int) (((current_x-1)/2)+0.5);
//    grid_y = (int) (((current_y-1)/2)+0.5);
//    if(grid_x>7){
//        grid_x = 7;
//    }
//    if(grid_y>7){
//        grid_y = 7;
//    }
//    if(grid_x<0){
//        grid_x = 0;
//    }
//    if(grid_y<0){
//        grid_y = 0;
//    }
//}

//void move_dir(int type, int direction){
//    switch(board_state1){
//        case STATE_INIT:
//            if(en){
//                //switch on the grid_x column and the grid_y row
//                mTime1=Timer_getSystemMicroSeconds();
//                board_state1 = STATE_WAIT; 
//                
//                _LATB11 = 1;
//            }
//            break;
//        case STATE_WAIT:
//             mTime2=Timer_getSystemMicroSeconds();
//            if((mTime2-mTime1)>10){
//                //switch off the grid_x column and the grid_y row
//                board_state1 = STATE_DIRECTIONS;
//                _LATB11 = 0;
//            }
//            break;
//        case STATE_DIRECTIONS:
//           switch(type){
//                case MOVE_ALONG_AXIS:
//                        switch(direction){
//                            case UP:
//                                
//                                if(grid_y<7){
//                                    MPER   = 10000;
//                                    //PWM master high period
//                                    MDC    =  0;
//                                    //PWM master phase
//                                    MPHASE =     0;
//                                    PG4CONLbits.ON = 1;
//                                    //switch on grid_x,grid_y+1
//                                    onned_x1 = grid_x;
//                                    onned_y1 = grid_y+1;
//                                    board_state1 = STATE_WAIT_FINISH; 
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                            case RIGHT:
//                                if(grid_x<7){
//                                    //switch on grid_x+1,grid_y
//                                    onned_x1 = grid_x+1;
//                                    onned_y1 = grid_y;
//                                    board_state1 = STATE_WAIT_FINISH;
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                            case DOWN:
//                                if(grid_y>0){
//                                    //switch on grid_x,grid_y-1
//                                    onned_x1 = grid_x;
//                                    onned_y1 = grid_y-1;
//                                    board_state1 = STATE_WAIT_FINISH;
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                            case LEFT:
//                                if(grid_x>0){
//                                    //switch on grid_x-1,grid_y
//                                    onned_x1 = grid_x-1;
//                                    onned_y1 = grid_y;
//                                    board_state1 = STATE_WAIT_FINISH;
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                        }
//                        break;
//                case MOVE_DIAGONALLY:
//                        switch(direction){
//                            case NE:
//                                if(grid_y<7 && grid_x<7){
//                                    //switch on grid_x,grid_y+1 and grid_y , grid_x+1 pwms
////                                    RPOR6bits.RP44R = 0b100010;
////                                    RPOR3bits.RP38R = 0b100011;
////                                    PG4CONLbits.ON = 1;
//                                    
//                                    
//                                    onned_pwm_x1 = grid_x;
//                                    onned_pwm_y1 = grid_y+1;
//                                    onned_pwm_x2 = grid_x+1;
//                                    onned_pwm_y2 = grid_y;
//                                    board_state1 = STATE_WAIT_INTERMEDIATE;
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                            case SE:
//                                if(grid_x<7 && grid_y>0){
//                                    //switch on grid_x,grid_y-1 and grid_y , grid_x+1 pwms
//                                    onned_pwm_x1 = grid_x;
//                                    onned_pwm_y1 = grid_y-1;
//                                    onned_pwm_x2 = grid_x+1;
//                                    onned_pwm_y2 = grid_y;
//                                    board_state1 = STATE_WAIT_INTERMEDIATE;
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                            case SW:
//                                if(grid_x>0 && grid_y>0){
//                                    //switch on grid_x,grid_y-1 and grid_y+1 , grid_x-1 pwms
//                                    onned_pwm_x1 = grid_x;
//                                    onned_pwm_y1 = grid_y-1;
//                                    onned_pwm_x2 = grid_x-1;
//                                    onned_pwm_y2 = grid_y;
//                                    board_state1 = STATE_WAIT_INTERMEDIATE;
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                            case NW:
//                                if(grid_x>0 && grid_y<7){
//                                     //switch on grid_x,grid_y+1 and grid_y , grid_x-1 pwms
//                                    onned_pwm_x1 = grid_x;
//                                    onned_pwm_y1 = grid_y+1;
//                                    onned_pwm_x2 = grid_x-1;
//                                    onned_pwm_y2 = grid_y;
//                                    board_state1 = STATE_WAIT_INTERMEDIATE;
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                        }
//                        break;
//                case MOVE_ROOK_DIAGONALLY:
//                       switch(direction){
//                            case R0:
//                                if(grid_y<6 && grid_x<7){
//                                    //switch on grid_x,grid_y+1 and grid_y+1 , grid_x+1 pwms
////                                    RPOR6bits.RP44R = 0b100010;
////                                    RPOR3bits.RP38R = 0b100011;
////                                    PG4CONLbits.ON = 1;
//                                    onned_x1 = grid_x;
//                                    onned_y1 = grid_y+1;
//                                    onned_x2 = grid_x+1;
//                                    onned_y2 = grid_y+1;
//                                    board_state1 = STATE_WAIT_INTERMEDIATE;
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                            case R1:
//                                if(grid_x<6 && grid_y<7){
//                                    //switch on grid_x+1,grid_y and grid_y+1 , grid_x+1 pwms
//                                    onned_x1 = grid_x+1;
//                                    onned_y1 = grid_y;
//                                    onned_x2 = grid_x+1;
//                                    onned_y2 = grid_y+1;
//                                    board_state1 = STATE_WAIT_INTERMEDIATE;
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                            case R2:
//                                if(grid_x<6 && grid_y>0){
//                                    //switch on grid_x+1,grid_y-1 and grid_y , grid_x+1 pwms
//                                    onned_pwm_x1 = grid_x+1;
//                                    onned_pwm_y1 = grid_y-1;
//                                    onned_pwm_x2 = grid_x+1;
//                                    onned_pwm_y2 = grid_y;
//                                    board_state1 = STATE_WAIT_INTERMEDIATE;
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                            case R3:
//                                if(grid_x<7 && grid_y>1){
//                                     //switch on grid_x,grid_y-1 and grid_y-1 , grid_x+1 pwms
//                                    onned_pwm_x1 = grid_x;
//                                    onned_pwm_y1 = grid_y-1;
//                                    onned_pwm_x2 = grid_x+1;
//                                    onned_pwm_y2 = grid_y-1;
//                                    board_state1 = STATE_WAIT_INTERMEDIATE;
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                            case R4:
//                                if(grid_y>1 && grid_x>0){
//                                    //switch on grid_x-1,grid_y-1 and grid_y-1 , grid_x pwms
////                                    RPOR6bits.RP44R = 0b100010;
////                                    RPOR3bits.RP38R = 0b100011;
////                                    PG4CONLbits.ON = 1;
//                                    onned_x1 = grid_x-1;
//                                    onned_y1 = grid_y-1;
//                                    onned_x2 = grid_x;
//                                    onned_y2 = grid_y-1;
//                                    board_state1 = STATE_WAIT_INTERMEDIATE;
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                            case R5:
//                                if(grid_x>1 && grid_y>0){
//                                    //switch on grid_x-1,grid_y and grid_y-1 , grid_x-1 pwms
//                                    onned_x1 = grid_x-1;
//                                    onned_y1 = grid_y;
//                                    onned_x2 = grid_x-1;
//                                    onned_y2 = grid_y-1;
//                                    board_state1 = STATE_WAIT_INTERMEDIATE;
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                            case R6:
//                                if(grid_x>1 && grid_y<7){
//                                    //switch on grid_x-1,grid_y+1 and grid_y , grid_x-1 pwms
//                                    onned_pwm_x1 = grid_x-1;
//                                    onned_pwm_y1 = grid_y+1;
//                                    onned_pwm_x2 = grid_x-1;
//                                    onned_pwm_y2 = grid_y;
//                                    board_state1 = STATE_WAIT_INTERMEDIATE;
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                            case R7:
//                                if(grid_x>0 && grid_y<6){
//                                     //switch on grid_x-1,grid_y+1 and grid_y+1 , grid_x pwms
//                                    onned_pwm_x1 = grid_x;
//                                    onned_pwm_y1 = grid_y+1;
//                                    onned_pwm_x2 = grid_x-1;
//                                    onned_pwm_y2 = grid_y+1;
//                                    board_state1 = STATE_WAIT_INTERMEDIATE;
//                                }
//                                else{
//                                    board_state1 = STATE_INIT;
//                                    en = 0;
//                                }
//                                break;
//                        }
//                        break;
//                case MOVE_DIAGONALLY_CURVED:
//                        break;
//                case MOVE_ROOK_DIAGONALLY_CURVED:
//                        break;
//            }    
//             
//            Timer_setSystemMicroSeconds();
//            break;
//        case STATE_WAIT_INTERMEDIATE:
//            MDC+=1;
//            if(MDC == 10000){
//               //switch off the pwms
////                RPOR6bits.RP44R = 0;
////                RPOR3bits.RP38R = 0;
////                _LATB12 = 0;
////                _LATB6  = 0;
//                
//                board_state1 = STATE_EXACT;
//            }
//            break;
//        case STATE_EXACT:
//            switch(type){
//                case MOVE_ALONG_AXIS:
//                        
//                        break;
//                case MOVE_DIAGONALLY:
//                        switch(direction){
//                            case NE:
//                               //switch on grid_x+1,grid_y+1 
//                                break;
//                            case SE:
//                               //switch on grid_x+1,grid_y-1 
//                                break;
//                            case SW:
//                                //switch on grid_x-1,grid_y-1 
//                                break;
//                            case NW:
//                                //switch on grid_x-1,grid_y+1 
//                                break;
//                        }
//                        board_state1 = STATE_WAIT_FINISH;
//                        break;
//                case MOVE_ROOK_DIAGONALLY:
//                    switch(direction){
//                        case R0:
//                           //switch off grid_x,grid_y+1 and grid_y+1 , grid_x+1 pwms
//                           break;
//                        case R1:
//                            //switch off grid_x+1,grid_y and grid_y+1 , grid_x+1 pwms
//                            break; 
//                        case R2:
//                             //switch off grid_x+1,grid_y-1 and grid_y , grid_x+1 pwms
//                            break;
//                        case R3:
//                            //switch off grid_x,grid_y-1 and grid_y-1 , grid_x+1 pwms
//                            break;
//                        case R4:
//                             //switch off grid_x-1,grid_y-1 and grid_y-1 , grid_x pwms
//                             break;
//                        case R5:
//                             //switch off grid_x-1,grid_y and grid_y-1 , grid_x-1 pwms
//                             break;
//                        case R6:
//                             //switch off grid_x-1,grid_y+1 and grid_y , grid_x-1 pwms
//                             break;
//                        case R7:
//                              //switch off grid_x-1,grid_y+1 and grid_y+1 , grid_x pwms
//                               break;
//                    }
//                    break;
//                case MOVE_DIAGONALLY_CURVED:
//                        break;
//                case MOVE_ROOK_DIAGONALLY_CURVED:
//                        break;
//            }    
//            Timer_setSystemMicroSeconds(); 
//            break;
//        case  STATE_WAIT_FINISH:
//            MDC+=1;
//            if(MDC == 9999){
//               //switch off the pwms
////                RPOR6bits.RP44R = 0;
////                RPOR3bits.RP38R = 0;
////                _LATB12 = 0;
////                _LATB6  = 0;
//                
//                board_state1 = STATE_EXACT;
//            }
////            if(Timer_getSystemMicroSeconds ()>10){
////                board_state1 = STATE_FINISH;
////            }
//            break;
//        case STATE_FINISH:
//            board_state1 = STATE_INIT;
//            en = 0;
//            break;
//        
//    }
// 
//}
//void move(int x, int y){
//    switch(board_state){
//        case STATE_INIT:
//            if(en){
//                grid_x = (int) (((current_x-1)/2)+0.5);
//                grid_y = (int) (((current_y-1)/2)+0.5);
//                if(grid_x>7){
//                    grid_x = 7;
//                }
//                if(grid_y>7){
//                    grid_y = 7;
//                }
//                if(grid_x<0){
//                    grid_x = 0;
//                }
//                if(grid_y<0){
//                    grid_y = 0;
//                }
//                //switch on the grid_x column and the grid_y row
//                Timer_setSystemMicroSeconds();
//                board_state = STATE_WAIT; 
//            }
//            
//            break;
//        case STATE_WAIT:
//      
//            if(Timer_getSystemMiliSeconds ()>10){
//               //switch off the grid_x column and the grid_y row
//                board_state = STATE_DIRECTIONS;
//            }
//            break;
//        case STATE_DIRECTIONS:
//            if(x>grid_x){
//                if(y>grid_y){
//                    if(x-grid_x > y-grid_y){
//                        //switch on the grid_x+1 column and the grid_y row
//                        onned_x1 = grid_x+1; onned_y1 = grid_y;
//
//                    }
//                    else if(x-grid_x == y-grid_y){
//                        //switch on the grid_x+1 column and the grid_y+1 row
//                        onned_x1 = grid_x+1; onned_y1 = grid_y+1;
//                    }
//                    else{
//                         //switch on the grid_x column and the grid_y+1 row
//                        onned_x1 = grid_x; onned_y1 = grid_y+1;
//                    }
//
//                }
//                else if(y<grid_y){
//                   if(x-grid_x > grid_y-y){
//                        //switch on the grid_x+1 column and the grid_y row
//                       onned_x1 = grid_x+1; onned_y1 = grid_y;
//                    }
//                    else if(x-grid_x == grid_y-y){
//                        //switch on the grid_x+1 column and the grid_y-1 row
//                        onned_x1 = grid_x+1; onned_y1 = grid_y-1;
//                    }
//                    else{
//                         //switch on the grid_x column and the grid_y-1 row
//                        onned_x1 = grid_x; onned_y1 = grid_y-1;
//                    }
//                }
//                else{
//                    //switch on the grid_x+1 column and the grid_y row
//                    onned_x1 = grid_x+1; onned_y1 = grid_y;
//                }
//            }
//            else if(x<grid_x){
//                if(y>grid_y){
//                    if(grid_x-x > y-grid_y){
//                        //switch on the grid_x-1 column and the grid_y row
//                        onned_x1 = grid_x-1; onned_y1 = grid_y;
//                    }
//                    else if(grid_x-x == y-grid_y){
//                        //switch on the grid_x-1 column and the grid_y+1 row
//                        onned_x1 = grid_x-1; onned_y1 = grid_y+1;
//                    }
//                    else{
//                         //switch on the grid_x column and the grid_y+1 row
//                        onned_x1 = grid_x; onned_y1 = grid_y+1;
//                    }
//
//                }
//                else if(y<grid_y){
//                   if(grid_x-x > grid_y-y){
//                        //switch on the grid_x-1 column and the grid_y row
//                       onned_x1 = grid_x-1; onned_y1 = grid_y;
//                    }
//                    else if(grid_x-x == grid_y-y){
//                        //switch on the grid_x-1 column and the grid_y-1 row
//                        onned_x1 = grid_x-1; onned_y1 = grid_y-1;
//                    }
//                    else{
//                         //switch on the grid_x column and the grid_y-1 row
//                        onned_x1 = grid_x; onned_y1 = grid_y-1;
//                    }
//                }
//                else{
//                    //switch on the grid_x-1 column and the grid_y row
//                    onned_x1 = grid_x-1; onned_y1 = grid_y;
//                }
//            }
//            else{
//                if(y>grid_y){
//                   //switch on the grid_x column and the grid_y+1 row
//                    onned_x1 = grid_x; onned_y1 = grid_y+1;
//                }
//                else if(y<grid_y){
//                  //switch on the grid_x column and the grid_y-1 row
//                    onned_x1 = grid_x; onned_y1 = grid_y-1;
//                }
//                else{
//                    //switch on the grid_x column and the grid_y row
//                    onned_x1 = grid_x; onned_y1 = grid_y;
//                }
//            }
//            Timer_setSystemMicroSeconds();
//            board_state = STATE_WAIT_FINISH;
//            break;
//        case STATE_WAIT_FINISH:
//            if(Timer_getSystemMiliSeconds ()>10){
//               //switch off the onned_x1 column and the onned_y1 row
//                board_state = STATE_FINISH;
//            }
//            break;
//        case STATE_FINISH:
//            board_state = STATE_INIT;
//            en = 0;
//            break;
//    }
//}

