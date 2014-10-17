#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/motor.h>

int main(void){
	float t1; float t2; float t3; float t4;
	initMotors();
	InitializeSystemTime();
	Wait(10); // Motor off time
	motorDemo();
	t1 = GetTime();
	t2 = GetTime();
/*	while( (t2-t1)<2){
		t2 = GetTime();
	}*/
	Wait(4); // Motor on time
	motorStop();
}
