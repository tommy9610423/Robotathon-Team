#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/adc.h>
#include <stdbool.h>
static tADC *adc[3];
static tBoolean initialized = false;

static tPin f1 = PIN_F1;
static tPin f2 = PIN_F2;
static tPin f3 = PIN_F3;
static tBoolean high = true;
static tBoolean low = false;
volatile float inputF;
volatile float inputL;
volatile float inputR;
#define offset 10

void initIRSensor(void) {
    // don't initialize this if we've already done so
    if (initialized) {
        return;
    }
    
    initialized = true;

    // initialize 4 pins to be used for ADC input
    adc[0] = InitializeADC(PIN_E4); // Left IR
    adc[1] = InitializeADC(PIN_D3); // Front IR
    adc[2] = InitializeADC(PIN_E5); // Right IR
}


int readRight(void){int cmdist;
    inputR = ADCRead(adc[2]);
	inputR *= 3.3;
	Printf("%fV ", inputR);
	if(inputR > 2.2){return -1;} // too close
	if(inputR >= 1.1){//10-20cm , 1.3+ V  : distance = -10.2 * input voltage + 33.6.  distance = (-2611*input + 8627) >> 8
		cmdist = (int)(-2611*inputR + 8627);
		cmdist = cmdist >> 8;
		if(cmdist<10){return 0;}
			return cmdist-offset; // offset 
	}
		else if(inputR >=.6 && inputR<1.1){//20cm-40cm, [.75V,1.3V) : distance = -54.8 * input voltage + 85.6. distance = (-56013*input + 87654) >> 10;
			cmdist = (int)(-56013*inputR + 87654);
			cmdist = cmdist >> 10;
			return cmdist-offset;
		}
		else if(inputR>=.5 && inputR<.6){//40cm-60cm, [.5V,.75V) : distance = -100 * input voltage + 110.
			cmdist = (int)(-100* inputR + 110);
			return cmdist-offset;
		}
			else {return 1000;} // input < .5 (too distant)
}


int readLeft(void){int cmdist;
    inputL = ADCRead(adc[0]);
    inputL *= 3.3;
    Printf("%fV ", inputL);
    if(inputL > 2.1){return -1;} // too close
    if(inputL >= 1.1){//10-20cm , 1.3+ V  : distance = -10.2 * input voltage + 33.6.  distance = (-2611*input + 8627) >> 8
	cmdist = (int)(-2611*inputL + 8627);
	cmdist = cmdist >> 8;
        if(cmdist<10){return 0;}
	return cmdist-offset; // offset 
    }
	else if(inputL >=.6 && inputL<1.1){//20cm-40cm, [.75V,1.3V) : distance = -54.8 * input voltage + 85.6. distance = (-56013*input + 87654) >> 10;
		cmdist = (int)(-56013*inputL + 87654);
		cmdist = cmdist >> 10;
		return cmdist-offset;
	}
	else if(inputL>=.5 && inputL<.6){//40cm-60cm, [.5V,.75V) : distance = -100 * input voltage + 110.
		cmdist = (int)(-100* inputL + 110);
		return cmdist-offset;
	}
	else {return 1000;} // input < .5 (too distant)
}

int readFront(void){int cmdist;
    inputF = ADCRead(adc[1]);
    inputF *= 3.3;
    Printf("%fV ", inputF);
    if(inputF > 2.2){return -1;} // too close
	if(inputF >= 1.1){//10-20cm , 1.3+ V  : distance = -10.2 * input voltage + 33.6.  distance = (-2611*input + 8627) >> 8
	    cmdist = (int)(-2611*inputF + 8627);
	    cmdist = cmdist >> 8;
	    if(cmdist<10){return 0;}
		 return cmdist-offset; // offset 
        }
		else if(inputF >=.6 && inputF<1.1){//20cm-40cm, [.75V,1.3V) : distance = -54.8 * input voltage + 85.6. distance = (-56013*input + 87654) >> 10;
			cmdist = (int)(-56013*inputF + 87654);
			cmdist = cmdist >> 10;
			return cmdist-offset;
		}
		else if(inputF>=.5 && inputF<.6){//40cm-60cm, [.5V,.75V) : distance = -100 * input voltage + 110.
			cmdist = (int)(-100* inputF + 110);
			return cmdist-offset;
		}
		else {return 1000;} // input < .5 (too distant)
}



void wallFollow(void){bool right = false; bool left = false;
    
	Forward();
	if(readRight()<readLeft()){right = true;}
		else{left = true;}
				
	if(right){
		while(readRight() != 1000 && readFront() >=25){ // and while no line is detected
					
					
			if(readRight() <= 10){ // drifted to the left
              		  swivelLeft();
              	 	  Wait(.6);// while(readRight()-dist >=2){}
              		  Forward();
			}
                                                                              
         	}
		if(readFront() <= 25){
			swivelLeft(); 
			SetPin(PIN_F2, true); // BLUE
			SetPin(PIN_F2, false);
			Wait(1.5); 
			motorStop();
			
		}
	}
								
		else{
			while(readLeft() != 1000 && readFront() >=25){
			    if(readLeft() <= 10){
			    swivelRight();
		       	    Wait(.6);
			    Forward();
			    }
			}
			if(readFront()<=25){
				swivelRight(); 
				SetPin(PIN_F2, true); // BLUE
				Wait(1.5); 
				SetPin(PIN_F2, false);
				motorStop();
				
			}
		}
		/*	
		
				while(1)
				{
					
					
				
				if(readLeft() < 5)
				{
					SetPin(PIN_F1, true);// RED
					swivelRight();
				}
				else if(readFront() < 10)
				{
					SetPin(PIN_F2, true); // BLUE
					swivelRight();
					Wait(3);
				}
				else 
				{
					Forward();
				}
			}
			*/		
					
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
