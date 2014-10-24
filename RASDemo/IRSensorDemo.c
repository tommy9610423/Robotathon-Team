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

Option 2: 2 linear functions, one from distance 10-20cm, the other from distance 20-60cm (no offset)
10-20cm , 1.3+ V  : distance = -10.2 * input voltage + 33.6.  distance = (-2611*input + 8627) >> 8
20cm-40cm, [.75V,1.3V) : distance = -54.8 * input voltage + 85.6. distance = (-56013*input + 87654) >> 10;
40cm-60cm, [.5V,.75V) : distance = -100 * input voltage + 110.
alternate for 2nd (accurate from 20cm-40cm) : distance = -36.5 * input V + 65.2. distance = (-37376*input + 66765) >> 10.
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


int readRight(void){int cmdist;
    float input = ADCRead(adc[2]);
		input *= 3.3;
		if(input >= 1.3){//10-20cm , 1.3+ V  : distance = -10.2 * input voltage + 33.6.  distance = (-2611*input + 8627) >> 8
			cmdist = (int)(-2611*input + 8627);
			cmdist = cmdist >> 8;
			if(cmdist<10){return 0;}
			return cmdist-10; // offset 
		}
			else if(input >=.75 && input<1.3){//20cm-40cm, [.75V,1.3V) : distance = -54.8 * input voltage + 85.6. distance = (-56013*input + 87654) >> 10;
				cmdist = (int)(-56013*input + 87654);
				cmdist = cmdist >> 10;
				return cmdist-10;
			}
			else if(input>=.5 && input<.75){//40cm-60cm, [.5V,.75V) : distance = -100 * input voltage + 110.
				cmdist = (int)(-100* input + 110);
				return cmdist-10;
			}
			else {return 1000;} // input < .5 (too distant)
			
}

int readLeft(void){int cmdist;
    float input = ADCRead(adc[0]);
    input *= 3.3;
		if(input >= 1.1){//10-20cm , 1.3+ V  : distance = -10.2 * input voltage + 33.6.  distance = (-2611*input + 8627) >> 8
			cmdist = (int)(-2611*input + 8627);
			cmdist = cmdist >> 8;
			if(cmdist<10){return 0;}
			return cmdist-10; // offset 
		}
			else if(input >=.6 && input<1.1){//20cm-40cm, [.75V,1.3V) : distance = -54.8 * input voltage + 85.6. distance = (-56013*input + 87654) >> 10;
				cmdist = (int)(-56013*input + 87654);
				cmdist = cmdist >> 10;
				return cmdist-10;
			}
			else if(input>=.5 && input<.6){//40cm-60cm, [.5V,.75V) : distance = -100 * input voltage + 110.
				cmdist = (int)(-100* input + 110);
				return cmdist-10;
			}
			else {return 1000;} // input < .5 (too distant)
}

int readFront(void){int cmdist;
    float input = ADCRead(adc[1]);
    input *= 3.3;
	if(input >= 1.3){//10-20cm , 1.3+ V  : distance = -10.2 * input voltage + 33.6.  distance = (-2611*input + 8627) >> 8
		cmdist = (int)(-2611*input + 8627);
		cmdist = cmdist >> 8;
		if(cmdist<10){return 0;}
	`	     return cmdist-10; // offset 
		}
		else if(input >=.75 && input<1.3){//20cm-40cm, [.75V,1.3V) : distance = -54.8 * input voltage + 85.6. distance = (-56013*input + 87654) >> 10;
			cmdist = (int)(-56013*input + 87654);
			cmdist = cmdist >> 10;
			return cmdist-10;
			}
		else if(input>=.5 && input<.75){//40cm-60cm, [.5V,.75V) : distance = -100 * input voltage + 110.
			cmdist = (int)(-100* input + 110);
			return cmdist-10;
		}
		else {return 1000;} // input < .5 (too distant)
    
}


void wallFollow(char x[], int dist){
    if(x[0] == 'r'){
				Forward();
        while(readRight()!= 1000 && readFront() >=15){ // and while no line is detected
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
            motorStop();                                    
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
						Forward();
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
								motorStop();
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
