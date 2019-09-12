


#include <p33CH64MP202.h>

#include "uart1.h"

#include <string.h>

int uart_rx_flag=0;   //This is for a handshake between MATLAB and M-Skin device

int UART_RX_flag(void)
{
    return uart_rx_flag;
}

void UART_init(void)
{
    
    U1MODEbits.BRGH=1;
    U1MODEH=0x0;
    //Uncomment
    //U1MODEHbits.BCLKSEL1=1; //BCLKSEL---10-FOSC  /// 00-FOSC/2 or Fp
    U1STA = 0x22; //URXBE & UTXBE status bit enabled 
    U1BRG=0x67; // or 0x97 in hex
    U1BRGH=0x00;
    U1P1 = 0x00;
    // P2 0; 
    U1P2 = 0x00;
    // P3 0; 
    U1P3 = 0x00;
    // P3H 0; 
    U1P3H = 0x00;
    // TXCHK 0; 
    U1TXCHK = 0x00;
    // RXCHK 0; 
    U1RXCHK = 0x00;
    // T0PD 1 ETU; PTRCL disabled; TXRPT Retransmit the error byte once; CONV Direct logic; 
    U1SCCON = 0x00;
    // TXRPTIF disabled; TXRPTIE disabled; WTCIF disabled; WTCIE disabled; BTCIE disabled; BTCIF disabled; GTCIF disabled; GTCIE disabled; RXRPTIE disabled; RXRPTIF disabled; 
    U1SCINT = 0x00;
    // ABDIF disabled; WUIF disabled; ABDIE disabled; 
    U1INT = 0x00;
    //U1MODEHBITS
    
    U1MODEbits.UARTEN = 1;
    U1MODEbits.UTXEN = 1; 
    U1MODEbits.URXEN = 1;
    
    
    TRISBbits.TRISB5 = 0;//B5 output TX //Setting RX pin I/P and TX pin O/P
    TRISBbits.TRISB6 = 1;//B6  RX input

  
    //Assign U1Rx To Pin RP38-RB5
    //***************************
    _U1RXR = 38; //RPINR18bits.U1RXR = 14;
    //Assign U1Tx To Pin RP37-RB6
    //***************************
    // _RP38 = 1;
    NVMKEY = 0x55;
    NVMKEY = 0xAA;
    RPCONbits.IOLOCK =0;
    RPOR2bits.RP37R = 1; //May be 3
    IFS0bits.U1RXIF = 0; //Interrupt Flag bits
    IEC0bits.U1RXIE=1;    
    ANSELB=0x00;
}

char UART_get() {
    while (U1STAHbits.URXBE);
    return U1RXREG;

}

char BusyUART1(void) {

    if (!IFS0bits.U1TXIF) {
        return 1;
    } else {
        IFS0bits.U1TXIF = 0;
        return 0;
    }
}
//---------Settings being used now
void UART_WriteCharTo(unsigned char Data) {
    U1TXREG = Data;
    while (BusyUART1());
}
void UART_WriteIntTo(uint8_t Data) {
    U1TXREG = Data;
    while (BusyUART1());
}

void UART_Write_String(char string1[])
{
    int i;
   for(i=0;i<strlen(string1);i++)
   {
       UART_WriteCharTo(string1[i]);
   }
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1RXInterrupt( void )
{
    
   
    IFS0bits.U1RXIF = 0;
    while(!(U1STAHbits.URXBE==1))
    {
         char temp_data =U1RXREG;
        if(temp_data=='S'){     
            _LATB11=1;
            uart_rx_flag=1;
         //   uart_start_flag=1;
         }
         
       
    }   
}