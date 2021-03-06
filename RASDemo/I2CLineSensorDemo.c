#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/linesensor.h>

static tLineSensor *ls;
static tBoolean initialized = false;

void initI2CLineSensor(void) {
    tI2C *bus;
    
    // don't initialize this if we've already done so
    if (initialized) {
        return;
    } 
    
    initialized = true;

    // initialize an I2C bus with a couple of pins
 //   bus = InitializeI2C(PIN_B3, PIN_B2);
    
    // intialize a line sensor to be on the I2C bus at address 0
    ls = InitializeGPIOLineSensor(PIN_E1,PIN_E0, PIN_E3, PIN_B4, PIN_B5, PIN_D0, PIN_D1, PIN_E2);
}

float SensorRead (int pos){
	float line[8];
	LineSensorReadArray(ls, line);
	return line[pos];
}
#define threshold 1
void LineFollow (void){ //assuming anything bigger is white and lesser is black
	float left = SensorRead(0); //change pos according to design
	float right = SensorRead(1); //change pos according to design
	
	//ADD: stop motors before movement change??
	if ((left <= threshold)&&(right <= threshold)){ //both white
		Forward();
		return;
	}
	else if ((left <= threshold && left>=0)&&(!(right <= threshold && right>=0))){ //left white, right black
		rotateLeft();
	}
	else if ((!(left <= threshold && left>=0))&&(right <= threshold && right>=0)){ //left black, right white
		rotateRight();
	}
	else if (((!(left <= threshold && left>=0))&&(!(right <= threshold && right>=0)))){ //both black
		Forward();
		//doubleblack is finish??????
	}
}
void i2cLineSensorDemo(void) {
    Printf("Press any key to quit\n");
 
    // loop as long as the user doesn't press a key 
    while (!KeyWasPressed()) {
        int i;
        float line[8];
    
        // put the values of the line sensor into the 'line' array 
        LineSensorReadArray(ls, line);
        Printf("Line Sensor: [");
    
        for (i = 0; i < 8; i++) {
            Printf("%01.4f ", line[i]);
        }
    
        Printf("\b]        \r");
    }
  
    Printf("\n"); 
}



