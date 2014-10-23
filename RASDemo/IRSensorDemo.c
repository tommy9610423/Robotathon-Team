#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/adc.h>

static tADC *adc[4];
static tBoolean initialized = false;

// function : y = 21.1x + .211111 .  y = output. x = inverted distance (cm)
/*
                y = 21618x + 216;
                y>>10;
                
               input * 3.3 = 21.1x + .21111
               
            21.1111 /  ( input  * 3.3 - .2111 )  = distance
            
                  ( 21618  /  ( input * 3379 -216)) >> 10 = distance
           */     


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


float readRight(void){
    float input = ADCRead(adc[2]);
    if(input < .15){return 1000;} // .15 = 15% of 3.3V = approx. out of bounds distance (80cm)
    float cmdist = (( 21618  /  ( input * 3379 -216)) >> 10);
    cmdist -= 10; // if sensors are placed 10cm inward, offset 10cm
    return cmdist;
}

float readLeft(void){
    float input = ADCRead(adc[0]);
    if(input < .15){return 1000;} // .15 = 15% of 3.3V = approx. out of bounds distance (80cm)
    float cmdist = (( 21618  /  ( input * 3379 -216)) >> 10);
    cmdist -= 10; // if sensors are placed 10cm inward, offset 10cm
    return cmdist;
}

float readFront(void){
    float input = ADCRead(adc[1]);
    if(input < .15){return 1000;} // .15 = 15% of 3.3V = approx. out of bounds distance (80cm)
    float cmdist = (( 21618  /  ( input * 3379 -216)) >> 10);
    cmdist -= 10; // if sensors are placed 10cm inward, offset 10cm
    return cmdist;
    
}


void wallFollow(char x[], float dist){
    if(x[0] == 'r'){
        while(readRight()!= 1000 && readFront() >=15){ // and while no line is detected
            current = readRight();
            if(readRight()-dist >=2){ // drifted to the left
                swivelRight();
                while(readRight()-dist >=2){}
                Forward();
            }
                                                             
            if(dist-readRight() >=2){ // drifted to the right
                swivelLeft();
                while(dist-readRight() >=2){}
                Forward();
            }
                                                
        }
        
                                // insert 90 degree left turn later
       /* if(line detected){return;}
        if(readFront() <= 15){
            float t1 = GetTime();
            float t2 = GetTime();
            swivelRight();
            while(t2-t1 <= 1){    // 
                t2 = GetTime();
            }
        } */
    }
                
        else if(x[0] == 'l'){
            while(readLeft() != 1000 && readFront()>=15){ // and while no line is detected
                if(readLeft()-dist >=2){ // drifted to the left
                    swivelRight();
                    while(readRight()-dist >=2){}
                    Forward();
                }
                                                             
                if(dist-readRight() >=2){ // drifted to the right
                    swivelLeft();
                    while(dist-readRight() >=2){}
                    Forward();
                }
            }
             /* if(line detected){return;}
            if(readFront() <= 15){
                float t1 = GetTime();
                float t2 = GetTime();
                swivelLeft();
                while(t2-t1 <= 1){    // 
                    t2 = GetTime();
                }
            } */
        }
    
        // insert 90 degree right turn later
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
