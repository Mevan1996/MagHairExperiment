
#include "xc.h"
#include "clock1.h"
//MCC Generated 

void clock_conf(void) {
      CLKDIVbits.PLLPRE   = 1;    // N1=1
      PLLFBDbits.PLLFBDIV = 100;  // M = 125
      PLLDIVbits.POST1DIV = 2;    // N2=5
      PLLDIVbits.POST2DIV = 1;    // N3=1
        // Initiate Clock Switch to FRC with PLL (NOSC=0b001)
      __builtin_write_OSCCONH(0x01);
      __builtin_write_OSCCONL(OSCCON | 0x01);
      // Wait for Clock switch to occur
      while (OSCCONbits.OSWEN!= 0);  
}