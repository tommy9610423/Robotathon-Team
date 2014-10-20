#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/adc.h>

static tADC *adc[4];
static tBoolean initialized = false;

// function : y = 21.1x + .211111 .  y = output. x = inverted distance (cm)
/*
                y = 21618x + 216;
                y>>10;
                
               input /4095 * 3.3 = 21.1x + .21111
               
            21.1111 /  ( input / 4096 * 3.3 - .2111 )  = distance
            
                  ( 21618  /  ( input / 4  * 3379 -216)) >> 10 = distance
               
               
               
               
           */     

// 

void initIRSensor(void) {
    // don't initialize this if we've already done so
    if (initialized) {
        return;
    }
    
    initialized = true;

    // initialize 4 pins to be used for ADC input
    adc[0] = InitializeADC(PIN_D0); // Left IR
    adc[1] = InitializeADC(PIN_D1); // Front IR
    adc[2] = InitializeADC(PIN_D2); // Right IR
//    adc[3] = InitializeADC(PIN_D3);
}


void readRight(void){
ADCRead(adc[2]);

    
}

void readLeft(void){
ADCRead(adc[0]);
    
}

void readFront(void){
ADCRead(adc[1]);
    
}
/*
void IRSensorDemo(void) {
    Printf("Press any key to quit\n");
        
    // loop as long as the user doesn't press a key 
    while (!KeyWasPressed()) {
        Printf(
            "IR values:  %1.3f  %1.3f  %1.3f  %1.3f\r",
            ADCRead(adc[0]),
            ADCRead(adc[1]),
            ADCRead(adc[2]),
            ADCRead(adc[3])
            );
    }
  
    Printf("\n");
}
*/
