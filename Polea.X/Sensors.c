#include "Sensors.h"
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <p33CH64MP206.h>
#include <stdint.h>
#define STATE_START     0
#define STATE_INIT      1
#define STATE_TRIGGER   2
#define STATE_DATA      3
#define STATE_STORE     4
static int sensor_state [12]= {STATE_START};
static int sensor_values[12]= {0};
void EnableAndCalibrate()
{
    // Set initialization time to maximum
    
    // Turn on ADC module
    
    // Turn on analog power for dedicated core 
    // Turn on analog power for shared core
    ADCON3Hbits.SHREN       = 1;
    ADCON5Lbits.SHRPWR      = 1;
    ADCON5Hbits.WARMTIME    = 15;
    ADCON1Lbits.ADON        = 1;
    // Wait when the shared core is ready for operation
    while(ADCON5Lbits.SHRRDY == 0){
        _LATB10             = 1;
    }
    _LATB10 = 0;
    // Turn on digital power to enable triggers to the shared core
    ADCON3Hbits.SHREN       = 1;
    // Enable calibration for the dedicated core 0
    // Enable calibration for the shared core
    ADCAL1Hbits.CSHREN      = 1;
    // Single-ended input calibration
    // Start calibration
    ADCAL1Hbits.CSHRRUN     = 1;
    // Poll for the calibration end
    while(ADCAL1Hbits.CSHRRDY == 0);
    // End the shared core calibration
    ADCAL1Hbits.CSHREN      = 0;
}

void init_ADC( void )
{
        // ADC INITIALIZATION
        // Configure the I/O pins to be used as analog inputs.
        // Configure the common ADC clock.
        ADCON1Lbits.ADON    =   0;
        ADCON3Hbits.CLKSEL = 2; // clock from FRC oscillator
        ADCON3Hbits.CLKDIV = 0; // no clock divider (1:1)
        
        
        // Configure the cores? ADC clock.
        // Configure the ADC reference sources.
        ADCON3Lbits.REFSEL = 0; // AVdd as voltage reference
        // Configure the integer of fractional output format.
        ADCON1Hbits.SHRRES  =   3;
        
        ADCON1Hbits.FORM = 0; // integer format
        // Select single-ended input configuration and unsigned output format.
        ADMOD0Lbits.SIGN0   = 0; // AN0 /RA0
        ADMOD0Lbits.SIGN1   = 0; // AN1 /RA1
        ADMOD0Lbits.SIGN2   = 0; // AN2 /RA2
        ADMOD0Lbits.SIGN3   = 0; // AN3 /RA3
        ADMOD0Lbits.SIGN4   = 0; // AN4 /RA4
        
        
        
        
        ADCON2Hbits.SHRSAMC =   10;
       
       
        EnableAndCalibrate(); // See Example 5-1
        // Enable and calibrate the module.
        
        // Configure and enable ADC interrupts.
        ADIELbits.IE0 = 1; // enable interrupt for AN0
        ADIELbits.IE1 = 1; // enable interrupt for AN1
        ADIELbits.IE2 = 1; // enable interrupt for AN0
        ADIELbits.IE3 = 1; // enable interrupt for AN1
        ADIELbits.IE4 = 1; // enable interrupt for AN0
       
        _ADCAN0IF = 0; // clear interrupt flag for AN0
        _ADCAN0IE = 1; // enable interrupt for AN0
        _ADCAN1IF = 0; // clear interrupt flag for AN1
        _ADCAN1IE = 1; // enable interrupt for AN1
        _ADCAN2IF = 0; // clear interrupt flag for AN0
        _ADCAN2IE = 1; // enable interrupt for AN0
        _ADCAN3IF = 0; // clear interrupt flag for AN1
        _ADCAN3IE = 1; // enable interrupt for AN1
        _ADCAN4IF = 0; // clear interrupt flag for AN0
        _ADCAN4IE = 1; // enable interrupt for AN0
       
        
}

