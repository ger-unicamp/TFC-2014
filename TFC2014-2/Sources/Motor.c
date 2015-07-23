/*
 * Motor.c
 *
 *  Created on: Oct 5, 2013
 *      Author: guilherme
 */

#include "enableMotorBit.h"
#include "motor1IN1PWM.h"
#include "motor2IN1PWM.h"
#include "motor1IN2PWM.h"
#include "motor2IN2PWM.h"

//LDD_TDeviceData *enableMotorBit_DeviceData;
LDD_TDeviceData *motor1IN1PWM_DeviceData;
LDD_TDeviceData *motor2IN1PWM_DeviceData;
LDD_TDeviceData *motor1IN2PWM_DeviceData;
LDD_TDeviceData *motor2IN2PWM_DeviceData;

bool enabledMotor = 0;

/**
 * ativa H-Bridge enable
 * inicializa PWM power 0
 */
void enableMotor()
{
	//enableMotorBit_DeviceData = enableMotorBit_Init(NULL); set to auto
	enableMotorBit_SetVal(enableMotorBit_DeviceData);
	motor1IN1PWM_DeviceData = motor1IN1PWM_Init(NULL);
	motor2IN1PWM_DeviceData = motor2IN1PWM_Init(NULL);
	motor1IN2PWM_DeviceData = motor1IN2PWM_Init(NULL);
	motor2IN2PWM_DeviceData = motor2IN2PWM_Init(NULL);
	motor1IN1PWM_SetRatio8(motor1IN1PWM_DeviceData,0);
	motor1IN2PWM_SetRatio8(motor1IN2PWM_DeviceData,0);
	motor2IN1PWM_SetRatio8(motor2IN1PWM_DeviceData,0);
	motor2IN2PWM_SetRatio8(motor2IN2PWM_DeviceData,0);
	enabledMotor = 1;
}


void disableMotor()
{
	motor1IN1PWM_SetRatio8(motor1IN1PWM_DeviceData,0);
	motor1IN2PWM_SetRatio8(motor1IN2PWM_DeviceData,0);
	motor2IN1PWM_SetRatio8(motor2IN1PWM_DeviceData,0);
	motor2IN2PWM_SetRatio8(motor2IN2PWM_DeviceData,0);
	enableMotorBit_ClrVal(enableMotorBit_DeviceData);
	enabledMotor = 0;
}

/**
 * channel: 0 (esquerda) or 1 (direita)
 * direction: 0(frente), 1(re)
 * power: 0-255
 */
void setMotor(bool channel, bool direction, byte power)
{
	if(!enabledMotor)return;
	switch(channel){
	case 0:
		if(direction == 0)
			{
				motor1IN1PWM_SetRatio8(motor1IN1PWM_DeviceData,(uint8)power);
				motor1IN2PWM_SetRatio8(motor1IN2PWM_DeviceData,0);
				return;
			}
			motor1IN2PWM_SetRatio8(motor1IN2PWM_DeviceData,(uint8)power);
			motor1IN1PWM_SetRatio8(motor1IN1PWM_DeviceData,0);
			return;
			
	case 1:
		if(direction == 0)
			{
				motor2IN1PWM_SetRatio8(motor2IN1PWM_DeviceData,(uint8)power);
				motor2IN2PWM_SetRatio8(motor2IN2PWM_DeviceData,0);
				return;
			}
			motor2IN2PWM_SetRatio8(motor2IN2PWM_DeviceData,(uint8)power);
			motor2IN1PWM_SetRatio8(motor2IN1PWM_DeviceData,0);
			return;
			
	default:return;
	}
}

bool motorGetState()
{
	return enabledMotor;
}


