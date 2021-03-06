#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/motor.h>

#define NoWall 1000  // wall read functions return value 1000 if there is no wall or wall is out of range
bool isLine; // indicates whether there is line or not
bool RobotLeft; // indicator for when robot is detected on left
bool RobotRight; // indicator for when robot is detected on Right
char r[1] = {'r'};
char l[1] = {'l'};
int main(void){ int right; int left; int front;
	initMotors();
	InitializeSystemTime();
	initIRSensor();

	while(1){
		right = readRight();//readRight();
		left = readLeft();
		front = readFront();
	// insert linefollow code
	// later version will account for robot detection
	if(right < left && right > 0){
		wallFollow(r,right);
	}
		else if (left < right && left > 0){
			wallFollow(l,left);
		}
	//motorStop();	
		
		
		
	}
	
}	
