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
    bus = InitializeI2C(PIN_B3, PIN_B2);
    
    // intialize a line sensor to be on the I2C bus at address 0
    ls = InitializeI2CLineSensor(bus, 0);
}

//pos is the position of the sensor that you want to read. pos goes from 0 ~ 7
float SensorRead (int pos){
	float line[8];
	LineSensorReadArray(ls, line);
	return line[pos];
}

void LineFollow (void){ //assuming anything bigger is white and lesser is black
	float left = SensorRead(1); //change pos according to design
	float right = SensorRead(2); //change pos according to design
	
	//ADD: stop motors before movement change??
	if ((left >= threshhold)&&(right >= threshhold)){ //both white
		Forward();
	}
	else if ((left >= threshhold)&&(right < threshhold)){ //left white, right black
		swivelRight();
	}
	else if ((left < threshhold)&&(right >= threshhold)){ //left black, right white
		swivelLeft();
	}
	else if ((left < threshhold)&&(right < threshhold)){ //both black
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

