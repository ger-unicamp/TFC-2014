/*
 * Motor.h
 *
 *  Created on: Oct 5, 2013
 *      Author: guilherme
 */

#ifndef MOTOR_H_
#define MOTOR_H_

void enableMotor();
void disableMotor();
void setMotor(bool channel, bool direction, byte power);
bool motorGetState();

#endif /* MOTOR_H_ */
