#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/motor.h>

static tMotor *leftMotor;
static tMotor *rightMotor;
static tBoolean initialized = false;

void initMotors(void) {
    // don't initialize this if we've already done so
    if (initialized) {
        // make sure the motors are off
        SetMotor(leftMotor, 0);
        SetMotor(rightMotor, 0);
        return;
    }
    
    initialized = true;

    leftMotor = InitializeServoMotor(PIN_B7,false);
    rightMotor = InitializeServoMotor(PIN_C5,true);
    
    // other pins can also be used for motors on the sacapuntas boosterpack:
    //leftMotor = InitializeMotor(PIN_F1, PIN_F0, true, false);
    //rightMotor = InitializeMotor(PIN_F3, PIN_F2, true, false);

    // make sure the motors are off
    SetMotor(leftMotor, 0);
    SetMotor(rightMotor, 0);
}


void motorStop(void){
		SetMotor(leftMotor,0);
		SetMotor(rightMotor,0);
}

void Forward(void){
	SetMotor(leftMotor,1);
	SetMotor(rightMotor,1);
}

void Backward(void){
	SetMotor(leftMotor,-1);
	SetMotor(rightMotor,-1);
}

void swivelRight(void){
	SetMotor(leftMotor,1);
	SetMotor(rightMotor,0);
}

void swivelLeft(void){
	SetMotor(leftMotor,0);
	SetMotor(rightMotor,1);
}

void rotateRight(void){
	SetMotor(leftMotor,1);
	SetMotor(rightMotor,-1);
}

void rotateLeft(void){
	SetMotor(leftMotor,-1);
	SetMotor(rightMotor,1);
}
