#include "Draw.h"
#include "Location.h"
#include "Timer.h"
#include <p33CH64MP206.h>

//Different shapes it can draw
#define CIRCLE                      0
#define SQUARE                      1
#define ZIGZAG                      2
#define TRIANGLE                    3

//Elementary movement function states
#define STATE_DEFINE_DUTY           0
#define STATE_START_ACTIVATIONS     1
#define STATE_START_PWM             2
#define STATE_FINISH                3

//Shape drawing function states
#define DRAW_INIT                   0
#define DRAW_MOVE_START             1
#define DRAW_MOVE                   2
#define DRAW_FINISH                 3

//Different movements
#define AXIS                        0
#define DIAGONAL                    1
#define MOVE_ROOK_DIAGONALLY        2
#define MOVE_DIAGONALLY_CURVED      3
#define MOVE_ROOK_DIAGONALLY_CURVED 4



//Along axis directions
#define UP                          0
#define RIGHT                       1
#define DOWN                        2
#define LEFT                        3

//45 degree movements
#define NE                          4
#define SE                          5
#define SW                          6
#define NW                          7

//Rook movements
#define R0                          0
#define R1                          1
#define R2                          2
#define R3                          3 
#define R4                          4
#define R5                          5
#define R6                          6 
#define R7                          7

#define ROW                         1
#define COL                         0

#define HIGH                        0
#define LOW                         1

//Global variables
int currentX, currentY,grid_x,grid_y;


static volatile int shape = 0;

extern const int _MAXTIME;

extern  volatile int _PWM1ONTIME;
extern  volatile int _PWM2ONTIME;
extern  volatile int _PWM3ONTIME;
extern  volatile int _PWM4ONTIME;

extern  volatile uint16_t PWM1LATAPORT;
extern  volatile uint16_t PWM1LATCPORT;

extern  volatile uint16_t PWM2LATAPORT;
extern  volatile uint16_t PWM2LATCPORT;

extern  volatile uint16_t PWM3LATBPORT;
extern  volatile uint16_t PWM3LATCPORT;
extern  volatile uint16_t PWM3LATDPORT;

extern  volatile uint16_t PWM4LATBPORT;
extern  volatile uint16_t PWM4LATCPORT;
extern  volatile uint16_t PWM4LATDPORT;


int circleNM[8][8];
int circleNT[8][8];
int squareNM[5][6];
int squareNT[5][6];

int movements[13][64][2];
int starting[13][2];
int start[13][2];
int movelen[13];
int turns[13];
int num_turns = 3;
int startingX = 2;
int startingY = 2;

static volatile int pwm1Duty;
static volatile int staticAxis;
static volatile int pwm1;
static volatile int pwm2;
static volatile int pwm3;
static volatile int pwm4;
static volatile int pwm5Duty;


static volatile int pwmMul;
static volatile int time;

static volatile int row_col;


//State machine states
static volatile int draw_state      = DRAW_INIT ;
static volatile int   board_state   = STATE_DEFINE_DUTY;


//Enables
static volatile int en              = 0;
static volatile int enable          = 0;
void init_movements(void){
    starting[0][0]     =   startingX;
    starting[0][1]     =   startingY;
    //start[0][0]     =   2;
    //start[0][1]     =   3;
    turns[0] = 3;
    movelen[0]         =   8;
    movements[0][0][0] =   AXIS;   movements[0][0][1] = UP;
    movements[0][1][0] =   AXIS;   movements[0][1][1] = UP;
    movements[0][2][0] =   AXIS;   movements[0][2][1] = DOWN;
    movements[0][3][0] =   AXIS;   movements[0][3][1] = DOWN;
    movements[0][4][0] =   AXIS;   movements[0][4][1] = DOWN;
    movements[0][5][0] =   AXIS;   movements[0][5][1] = DOWN;
    movements[0][6][0] =   AXIS;   movements[0][6][1] = UP;
    movements[0][7][0] =   AXIS;   movements[0][7][1] = UP;
    movements[0][8][0] =   AXIS;   movements[0][8][1] = DOWN;  
    
    
    starting[1][0]     =   startingX;
    starting[1][1]     =   startingY;
  //  start[1][0]     =   0;
   // start[1][1]     =   2;
    movelen[1]         =   8;
    turns[1] = 3;
    movements[1][0][0] =   AXIS;   movements[1][0][1] = LEFT;
    movements[1][1][0] =   AXIS;   movements[1][1][1] = LEFT;
    movements[1][2][0] =   AXIS;   movements[1][2][1] = RIGHT;
    movements[1][3][0] =   AXIS;   movements[1][3][1] = RIGHT;
    movements[1][4][0] =   AXIS;   movements[1][4][1] = RIGHT;
    movements[1][5][0] =   AXIS;   movements[1][5][1] = RIGHT;
    movements[1][6][0] =   AXIS;   movements[1][6][1] = LEFT;
    movements[1][7][0] =   AXIS;   movements[1][7][1] = LEFT;
//    movements[1][8][0] =   AXIS;   movements[1][8][1] = LEFT;  
    
    
    starting[2][0]      =   startingX;
    starting[2][1]      =   startingY;
//    start[2][0]     =   0;
//    start[2][1]     =   4;
    movelen[2]          =   12;
    turns[2] = 2;
    movements[2][0][0]  =   DIAGONAL;    movements[2][0][1]     =   NW;
    movements[2][1][0]  =   DIAGONAL;    movements[2][1][1]     =   NW;
    movements[2][2][0]  =   AXIS;    movements[2][2][1]     =   DOWN;
    movements[2][3][0] =    AXIS;    movements[2][3][1]    =    DOWN;
    movements[2][4][0] =    AXIS;    movements[2][4][1]    =    DOWN;  
    movements[2][5][0]  =   AXIS;    movements[2][5][1]=    DOWN;
    movements[2][6][0]  =   AXIS;    movements[2][6][1]     =   RIGHT;
    movements[2][7][0]  =   AXIS;    movements[2][7][1]     =   RIGHT;
    movements[2][8][0]  =   AXIS;    movements[2][8][1]     =   RIGHT;
    movements[2][9][0]  =   AXIS;    movements[2][9][1]     =   RIGHT;
    movements[2][10][0]  =  DIAGONAL;movements[2][10][1]     =   NW;
    movements[2][11][0]  =  DIAGONAL;movements[2][11][1]     =  NW;
//    movements[2][12][0]  =  DIAGONAL;movements[2][12][1]     =  NW;  
    
    starting[3][0]      =   startingX;
    starting[3][1]      =   startingY;
//    start[3][0]     =   4;
//    start[3][1]     =   0;
    movelen[3]          =   12;
    turns[3] = 2;
    movements[3][0][0]  =   DIAGONAL;    movements[3][0][1] =   SE;
    movements[3][1][0]  =   DIAGONAL;    movements[3][1][1]     =   SE;
    movements[3][2][0]  =   AXIS;    movements[3][2][1]     =   UP;
    movements[3][3][0] =    AXIS;    movements[3][3][1]    =    UP;
    movements[3][4][0] =    AXIS;    movements[3][4][1]    =    UP;  
    movements[3][5][0]  =   AXIS;    movements[3][5][1]=    UP;
    movements[3][6][0]  =   AXIS;    movements[3][6][1]     =   LEFT;
    movements[3][7][0]  =   AXIS;    movements[3][7][1]     =   LEFT;
    movements[3][8][0]  =   AXIS;    movements[3][8][1]     =   LEFT;  
    movements[3][9][0]  =   AXIS;    movements[3][9][1]     =   LEFT;
    movements[3][10][0]  =   DIAGONAL;movements[3][10][1]   =   SE;
    movements[3][11][0]  =  DIAGONAL;movements[3][11][1]     =  SE;
    //movements[3][12][0]  =  DIAGONAL; movements[3][12][1]     = SE;
    
    
    starting[4][0]      =   startingX;
    starting[4][1]      =   startingY;
//    start[4][0]     =   0;
//    start[4][1]     =   0;
    movelen[4]          =   20;
    turns[4] = 1;
    movements[4][0][0]  =   AXIS;        movements[4][0][1]     =   UP;
    movements[4][1][0]  =   AXIS;        movements[4][1][1]     =   UP;
    movements[4][2][0]  =   AXIS;        movements[4][2][1]     =   LEFT;
    movements[4][3][0]  =   AXIS;        movements[4][3][1]     =   LEFT;
    movements[4][4][0]  =   AXIS;        movements[4][4][1]     =   DOWN;
    movements[4][5][0]  =   AXIS;        movements[4][5][1]     =   DOWN;
    movements[4][6][0]  =   AXIS;        movements[4][6][1]     =   DOWN;
    movements[4][7][0]  =   AXIS;        movements[4][7][1]     =   DOWN;
    movements[4][8][0]  =   AXIS;        movements[4][8][1]     =   RIGHT; 
    movements[4][9][0]  =   AXIS;        movements[4][9][1]     =   RIGHT;
    movements[4][10][0]  =   AXIS;        movements[4][10][1]     = RIGHT;
    movements[4][11][0]  =   AXIS;        movements[4][11][1]     = RIGHT;
    movements[4][12][0]  =   AXIS;        movements[4][12][1]     = UP;
    movements[4][13][0]  =   AXIS;        movements[4][13][1]     = UP;
    movements[4][14][0]  =   AXIS;        movements[4][14][1]     = UP;
    movements[4][15][0]  =   AXIS;        movements[4][15][1]     = UP;
    movements[4][16][0]  =   AXIS;        movements[4][16][1]     = LEFT;
    movements[4][17][0]  =   AXIS;        movements[4][17][1]     = LEFT;
    movements[4][18][0]  =   AXIS;        movements[4][18][1]     = DOWN;
    movements[4][19][0]  =   AXIS;        movements[4][19][1]     = DOWN;
    
    starting[5][0]      =   startingX;
    starting[5][1]      =   startingY;
//    start[5][0]     =   4;
//    start[5][1]     =   0;
    movelen[5]          =   20;
    turns[5] = 1;
    movements[5][0][0]  =   AXIS;        movements[5][0][1]     =   UP;
    movements[5][1][0]  =   AXIS;        movements[5][1][1]     =   UP;
    movements[5][2][0]  =   AXIS;        movements[5][2][1]     =   RIGHT;
    movements[5][3][0]  =   AXIS;        movements[5][3][1]     =   RIGHT;
    movements[5][4][0]  =   AXIS;        movements[5][4][1]     =   DOWN;
    movements[5][5][0]  =   AXIS;        movements[5][5][1]     =   DOWN;
    movements[5][6][0]  =   AXIS;        movements[5][6][1]     =   DOWN;
    movements[5][7][0]  =   AXIS;        movements[5][7][1]     =   DOWN;
    movements[5][8][0]  =   AXIS;        movements[5][8][1]     =   LEFT; 
    movements[5][9][0]  =   AXIS;        movements[5][9][1]     =   LEFT;
    movements[5][10][0]  =   AXIS;        movements[5][10][1]     = LEFT;
    movements[5][11][0] =   AXIS;        movements[5][11][1]     =  LEFT;
    movements[5][12][0] =   AXIS;        movements[5][12][1]     =  UP;
    movements[5][13][0] =   AXIS;        movements[5][13][1]     =  UP;
    movements[5][14][0] =   AXIS;        movements[5][14][1]     =  UP;
    movements[5][15][0] =   AXIS;        movements[5][15][1]     =  UP;
    movements[5][16][0] =   AXIS;        movements[5][16][1]     =  RIGHT;
    movements[5][17][0] =   AXIS;        movements[5][17][1]     =  RIGHT;
    movements[5][18][0] =   AXIS;        movements[5][18][1]     =  DOWN;
    movements[5][19][0] =   AXIS;        movements[5][19][1]     =  DOWN;
}
void set_en_high(){
    en = 1;
}
void set_en_low(){
    en = 0;
}
int get_loc_en(){
    return en;
}
void portall_off(){
    LATA |= 0x003F;
    LATC |= 0x0001;
    LATB &= 0xAFFF;
    LATC &= 0xFFDF;
    LATD &= 0xFF3F;
}
static volatile int port1 ;
static volatile int row_col1;
static volatile int port2 ;
static volatile int row_col2;


void resetPWM(){
              _PWM4ONTIME=0;
              _PWM3ONTIME=0;
              _PWM2ONTIME=0;
              _PWM1ONTIME=0;
}
void activate(int rowCol, int type,int pwmNum, int pwmValue){
    switch(type){
        case(COL):
            switch(pwmNum){
                case 3:
                    switch(rowCol){
                        case(0):
                            //B14
                            _PWM3ONTIME  = pwmValue;
                            PWM3LATBPORT = 0xBFFF;
                            PWM3LATCPORT = 0xFFFF;
                            PWM3LATDPORT = 0xFFFF;
                            break;
                        case(1):
                            //B12
                            _PWM3ONTIME  = pwmValue;
                            PWM3LATBPORT = 0xEFFF;
                            PWM3LATCPORT = 0xFFFF;
                            PWM3LATDPORT = 0xFFFF;
                            break;
                        case(2):
                            //C5
                            _PWM3ONTIME  = pwmValue;
                            PWM3LATCPORT = 0xFFDF;
                            PWM3LATBPORT = 0xFFFF;
                            PWM3LATDPORT = 0xFFFF;
                            break;
                        case(3):
                            //D6
                            _PWM3ONTIME  = pwmValue;
                            PWM3LATDPORT = 0xFFBF;
                            PWM3LATCPORT = 0xFFFF;
                            PWM3LATBPORT = 0xFFFF;
                            break;
                        case(4):
                            //D7
                            _PWM3ONTIME  = pwmValue;
                            PWM3LATDPORT = 0xFF7F;
                            PWM3LATCPORT = 0xFFFF;
                            PWM3LATBPORT = 0xFFFF;
                            break;
                    }
                    break;
                case 4:
                     switch(rowCol){
                        case(0):
                            //B14
                            _PWM4ONTIME=pwmValue;
                            PWM4LATBPORT = 0xBFFF;
                            PWM4LATCPORT = 0xFFFF;
                            PWM4LATDPORT = 0xFFFF;
                            break;
                        case(1):
                            //B12
                            _PWM4ONTIME=pwmValue;
                            PWM4LATBPORT = 0xEFFF;
                            PWM4LATCPORT = 0xFFFF;
                            PWM4LATDPORT = 0xFFFF;
                            break;
                        case(2):
                            //C5
                            _PWM4ONTIME=pwmValue;
                            PWM4LATCPORT = 0xFFDF;
                            PWM4LATBPORT = 0xFFFF;
                            PWM4LATDPORT = 0xFFFF;
                            
                            break;
                        case(3):
                            //D6
                            _PWM4ONTIME=pwmValue;
                            PWM4LATDPORT = 0xFFBF;
                            PWM4LATCPORT = 0xFFFF;
                            PWM4LATBPORT = 0xFFFF;
                            break;
                        case(4):
                            //D7
                            _PWM4ONTIME=pwmValue;
                            PWM4LATDPORT = 0xFF7F;
                            PWM4LATCPORT = 0xFFFF;
                            PWM4LATBPORT = 0xFFFF;
                            break;
                    }
                    break;
            }
            
        case(ROW):
            switch(pwmNum){
                case 1:
                    switch(rowCol){
                        case(0):
                            //A4
                            _PWM1ONTIME=pwmValue;
                            PWM1LATAPORT = 0x10;
                            PWM1LATCPORT = 0;
                            break;
                        case(1):
                            //A3
                            _PWM1ONTIME=pwmValue;
                            PWM1LATAPORT = 0x8;
                            PWM1LATCPORT = 0;
                            break;
                        case(2):
                            //A2
                            _PWM1ONTIME=pwmValue;
                            PWM1LATAPORT = 0x4;
                            PWM1LATCPORT = 0;
                            break;
                        case(3):
                            //A1
                            _PWM1ONTIME=pwmValue;
                            PWM1LATAPORT = 0x2;
                            PWM1LATCPORT = 0;
                            break;
                        case(4):
                            //A0
                            _PWM1ONTIME=pwmValue;
                            PWM1LATAPORT = 0x1;
                            PWM1LATCPORT = 0;
                            break;
                        case(5):
                            //C0
                            _PWM1ONTIME=pwmValue;
                            PWM1LATCPORT = 0x1;
                            PWM1LATAPORT = 0;
                            break;
                    }
                    break;
                case 2:
                     switch(rowCol){
                        case(0):
                            //A4
                            _PWM2ONTIME=pwmValue;
                            PWM2LATAPORT = 0x10;
                            PWM2LATCPORT = 0;
                            break;
                        case(1):
                            //A3
                             _PWM2ONTIME=pwmValue;
                            PWM2LATAPORT = 0x8;
                            PWM2LATCPORT = 0;
                            break;
                        case(2):
                            //A2
                            _PWM2ONTIME=pwmValue;
                            PWM2LATAPORT = 0x4;
                            PWM2LATCPORT = 0;
                            break;
                        case(3):
                            //A1
                            _PWM2ONTIME=pwmValue;
                            PWM2LATAPORT = 0x2;
                            PWM2LATCPORT = 0;
                            break;
                        case(4):
                            //A0
                            _PWM2ONTIME=pwmValue;
                            PWM2LATAPORT = 0x1;
                            PWM2LATCPORT = 0;
                            break;
                        case(5):
                            //C0
                            _PWM2ONTIME=pwmValue;
                            PWM2LATAPORT = 0;
                            PWM2LATCPORT = 0x1;
                            break;
                    }
                    break;
            }
            
    }
    
}
void reset_all_pwm(void){
    PWM2LATAPORT = 0;
    PWM2LATCPORT = 0;
    
    PWM1LATCPORT = 0;
    PWM1LATAPORT = 0;
    
    PWM4LATDPORT = 0xFFFF;
    PWM4LATCPORT = 0xFFFF;
    PWM4LATBPORT = 0xFFFF;
    
    PWM3LATDPORT = 0xFFFF;
    PWM3LATCPORT = 0xFFFF;
    PWM3LATBPORT = 0xFFFF;
}
static volatile int var1,var2;
static volatile int PWMTIME = 0;
static volatile int PWMProp = 1;
static volatile int r = 1;
static volatile int pwmDuty = 50;



void mover(int type ,int direction){
    switch(type){
        case AXIS:
            switch(board_state){
                case STATE_DEFINE_DUTY:
                        grid_x = currentX;
                        grid_y = currentY;
                        switch(direction){
                            case UP:
                                if(grid_y<5){
                                    pwm1        = grid_y;
                                    pwm2        = grid_y+1;
                                    staticAxis  = grid_x;
                                    row_col     = COL;
                                }
                                else{
                                    en = 0;
                                }
                                break;
                            case DOWN:
                                if(grid_y>0){
                                    pwm1        = grid_y;
                                    pwm2        = grid_y-1;
                                    staticAxis  = grid_x;
                                    row_col     = COL;
                                }
                                else{
                                    en = 0;
                                }
                                break;
                            case RIGHT:
                                if(grid_x<4){
                                    pwm1        = grid_x;
                                    pwm2        = grid_x+1;
                                    staticAxis  = grid_y;
                                    row_col     = ROW;
                                }
                                else{
                                    en = 0;
                                }
                                break;
                            case LEFT:
                                if(grid_x>0){
                                    pwm1        = grid_x;
                                    pwm2        = grid_x-1;
                                    staticAxis  = grid_y;
                                    row_col     = ROW;
                                }
                                else{
                                    en = 0;
                                }
                                break;
                        }
                        PWMTIME = 0;
                        board_state = STATE_START_ACTIVATIONS;
                        break;
                case STATE_START_ACTIVATIONS:
                    reset_all_pwm();
                    if(row_col == ROW){
                        activate(staticAxis,ROW,1,pwmDuty);
                        
                            activate(pwm1,COL,3,pwmDuty);
                            //_PWM1ONTIME =  _MAXTIME;
                            activate(pwm2,COL,4,0);
                            //_PWM2ONTIME =  0;
                        
                    }
                    else{
                         activate(staticAxis,COL,3,pwmDuty);
                            //_PWM1ONTIME =  _MAXTIME;
                       
                            activate(pwm1,ROW,1,pwmDuty);
                           
                           // _PWM3ONTIME =  _MAXTIME;
                            activate(pwm2,ROW,2,0);
                           // _PWM4ONTIME =  0;   
                      
                    }
                    Timer_setSystemMicroSeconds ();
                    board_state = STATE_START_PWM;
                    break;
                case STATE_START_PWM:
                        r=1;
                        //10000 roger likes it
                        if(Timer_getSystemMicroSeconds()>5000){
                            if(PWMTIME < pwmDuty){
                                PWMProp=1;
                                
                                if(row_col == ROW){
                                    
                                        //_PWM3ONTIME=_PWM3ONTIME-PWMProp;
                                        //_PWM3ONTIME=_PWM3ONTIME-PWMProp;
                                         _PWM4ONTIME=_PWM4ONTIME+PWMProp;
                                }
                                else{
                                  
                                         //_PWM1ONTIME=_PWM1ONTIME-PWMProp;
                                         _PWM2ONTIME=_PWM2ONTIME+PWMProp;
                                        // _PWM1ONTIME=_PWM1ONTIME-PWMProp;
                                   
                                }
                                PWMTIME=PWMTIME+PWMProp;
                            }
                            else if(PWMTIME<2*pwmDuty){
                                if(row_col == ROW){
                                    
                                       
                                       _PWM3ONTIME=_PWM3ONTIME-PWMProp;
                                }
                                else{
                                  
                                         
                                         _PWM1ONTIME=_PWM1ONTIME-PWMProp;
                                   
                                }
                                PWMTIME=PWMTIME+PWMProp;
                            }
                            else
                               board_state= STATE_FINISH;
                            Timer_setSystemMicroSeconds();
                            PWMProp=1;
                        }
                        break;
                    case STATE_FINISH:
                        //deactivate(staticAxis,row_col);
                        //_LATB10 ^= 1;
                        en = 0;
                        board_state = STATE_DEFINE_DUTY;
                        break;
            }
            break;
        case DIAGONAL:
                switch(board_state){
                    case STATE_DEFINE_DUTY:
                        grid_x = currentX;
                        grid_y = currentY;
                        switch(direction){
                            case NE:
                                if(grid_x<5 && grid_y<4){
                                    pwm1 = grid_x;
                                    pwm2 = grid_y;
                                    pwm3 = grid_x+1;
                                    pwm4 = grid_y+1;
                                }
                                else{
                                    en = 0;
                                }
                                break;
                            case SE:
                                if(grid_x<5 && grid_y>0){
                                    pwm1 = grid_x;
                                    pwm2 = grid_y;
                                    pwm3 = grid_x+1;
                                    pwm4 = grid_y-1;
                                }
                                else{
                                    en = 0;
                                }
                                break;
                            case SW:
                                if(grid_x>0 && grid_y>0){
                                    pwm1 = grid_x;
                                    pwm2 = grid_y;
                                    pwm3 = grid_x-1;
                                    pwm4 = grid_y-1;
                                }
                                else{
                                    en = 0;
                                }
                                break;
                            case NW:
                                if(grid_x>0 && grid_y<4){
                                    pwm1 = grid_x;
                                    pwm2 = grid_y;
                                    pwm3 = grid_x-1;
                                    pwm4 = grid_y+1;
                                }
                                else{
                                    en = 0;
                                }
                                break;
                        }
                        

                        Timer_setSystemMicroSeconds ();
                        PWMTIME = 0;
                        board_state = STATE_START_ACTIVATIONS;
                        break;
                    case STATE_START_ACTIVATIONS:
                        reset_all_pwm();
                        activate(pwm1,COL,3,_MAXTIME);

                        activate(pwm2,ROW,1,_MAXTIME);
                                //_PWM1ONTIME =  _MAXTIME;
                        activate(pwm3,COL,4,0);
                            
                        activate(pwm4,ROW,2,0);
                            
                        //_PWM2ONTIME =  0;

                        Timer_setSystemMicroSeconds ();
                        board_state = STATE_START_PWM;
                        break;
                    case STATE_START_PWM:
                        r=1;
                        if(Timer_getSystemMicroSeconds()>20000){
                            if(PWMTIME<(_MAXTIME)){
                                PWMProp=1;
                                _PWM3ONTIME=_PWM3ONTIME-PWMProp;
                                _PWM4ONTIME=_PWM4ONTIME+PWMProp;
                                _PWM1ONTIME=_PWM1ONTIME-PWMProp;
                                _PWM2ONTIME=_PWM2ONTIME+PWMProp;
                                PWMTIME=PWMTIME+PWMProp;
                            }
                            else
                                board_state= STATE_FINISH;
                            Timer_setSystemMicroSeconds ();
                            PWMProp=1;
                        }
                        break;
                    case STATE_FINISH:
                        en =0;
                        board_state = STATE_DEFINE_DUTY;
                        break;
                }
                break;
                 
       
        }
     }

void set_enable1(){
    enable = 1;
}
void set_enable0(){
    enable = 0;
}
int get_draw_en(){
    return enable;
}
void set_shape(int sha){
    shape = sha;
}
int get_shape(){
    return shape;
}






//void draw_shape(){
//
////PWM3LATCPORT = 0;
////PWM3LATDPORT = 0;
////PWM3LATBPORT = 0x4000;
//
//    //activate(1, ROW,4, 20);
//    activate(4, ROW,2, 20);
//    activate(3, ROW,1, 50);
//}
static volatile int move_num ;
static volatile unsigned long drawTime;
static volatile int turn_num ;
void draw_shape(){
    
    switch(draw_state){
        case DRAW_INIT: 
            move_num = 0;
            turn_num = 0;
            Timer_setSystemMicroSeconds();
            
            shape = get_current_shape();
            //activate(starting[shape][0],COL,3,_MAXTIME);
            //activate(starting[shape][1],ROW,1,_MAXTIME);
            currentX = starting[shape][0];
            currentY = starting[shape][1];
            draw_state = DRAW_MOVE_START    ;
            break;
        case DRAW_MOVE_START:
            //location = current_location();
            //currentX = location%10;
            //currentY = (location /10);
            //drawTime = Timer_getSystemMiliSeconds();
           // if(drawTime > 2000){
               // reset_all_pwm();
                set_en_high();
                draw_state = DRAW_MOVE;
               // Timer_setSystemMicroSeconds();
            //}
            
            break;
        case DRAW_MOVE:
            //move_dir(0,0);
            
          
            if(!get_loc_en()){
                
                draw_state = DRAW_MOVE_START;
                switch(movements[shape][move_num][0]){
                    case DIAGONAL:
                        switch(movements[shape][move_num][1]){
                            case NE:
                                currentY++;
                                currentX++;
                                break;
                            case SE:
                                currentX++;
                                currentY--;
                                break;
                            case SW:
                                currentY--;
                                currentX--;
                                break;
                            case NW:
                                currentX--;
                                currentY++;
                                break;
                        }
                        break;
                    case AXIS:
                        switch(movements[shape][move_num][1]){
                            case UP:
                                currentY++;
                                break;
                            case RIGHT:
                                currentX++;
                                break;
                            case DOWN:
                                currentY--;
                                break;
                            case LEFT:
                                currentX--;
                                break;
                        }
                        break;
                }
               
                move_num = (move_num+1);
                if(move_num == movelen[shape]){
                    if(turn_num == turns[shape]-1){
                        draw_state = DRAW_FINISH;
                    }
                    else{
                        move_num = 0;
                    }
                    turn_num++;
                    
                }
                     
                
                //_LATB11 = 1; 
            }
            else{
                mover(movements[shape][move_num][0],movements[shape][move_num][1]);
            }
            break;   
        case DRAW_FINISH:
             reset_all_pwm();
            activate(starting[shape][0],COL,3,_MAXTIME);
            activate(starting[shape][1],ROW,1,_MAXTIME);
            draw_state = DRAW_INIT;
            enable = 0;
            break;
    }
//    if(get_draw()){
//        draw_state = DRAW_INIT;
//        board_state = STATE_DEFINE_DUTY;
//    }
}