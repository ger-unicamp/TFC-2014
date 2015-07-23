/*
 * camera.h
 *
 *  Created on: Oct 2, 2014
 *      Author: Augusto
 */

#ifndef CAMERA2014_H_
#define CAMERA2014_H_

void enableCamera();
void disableCamera();
void enableCLK();
void disableCLK();
void sendSI();
void getDataNoClear();
void getData();
int cameraGetState();

#endif /* CAMERA2014_H_ */
