/* ###################################################################
**     Filename    : main.c
**     Project     : TFC2014
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2014-08-19, 15:12, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "enableMotorBit.h"
#include "motor1IN1PWM.h"
#include "motor1IN2PWM.h"
#include "servoPWM.h"
#include "motor2IN2PWM.h"
#include "TU1.h"
#include "TU2.h"
#include "motor2IN1PWM.h"
#include "WAIT1.h"
#include "cameraCLK.h"
#include "TimerIntLdd1.h"
#include "cameraCLKBit.h"
#include "BitIoLdd1.h"
#include "cameraSIBit.h"
#include "BitIoLdd2.h"
#include "TU3.h"
#include "AS1.h"
#include "RxBuf.h"
#include "AD2.h"
#include "AdcLdd2.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "Motor.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "camera.h"
#include "Application.h"
#define LNDERIV 100

unsigned char buffer0[12];
unsigned char esp[2]=" ";
unsigned char nLine[3] = "\r\n";
int vetor_Derivadas[2];
#define LEFT 0
#define RIGHT 1
int Servo = 0;
float P=0, I=0, D=0;
float kP = 1800, kI = 0, kD = 0;
uint16_t vetorCamera[128]={};
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
float constProporc[128]={-0.5121916, -0.5054387, -0.49863485, -0.49178025, -0.48487496, -0.47791907, -0.47091264, -0.46385592, -0.45674902, -0.44959208, -0.44238544, -0.4351292, -0.42782366, -0.4204692, -0.41306597, -0.40561444, -0.3981149, -0.39056775, -0.3829734, -0.3753323, -0.3676449, -0.35991174, -0.35213327, -0.34431013, -0.3364428, -0.32853198, -0.32057822, -0.31258222, -0.30454475, -0.2964664, -0.28834802, -0.28019032, -0.27199417, -0.26376036, -0.2554898, -0.24718332, -0.23884188, -0.23046643, -0.22205792, -0.21361737, -0.20514579, -0.19664425, -0.18811381, -0.17955557, -0.17097066, -0.16236024, -0.15372545, -0.14506748, -0.13638757, -0.12768692, -0.1189668, -0.11022847, -0.1014732, -0.09270228, -0.083917044, -0.0751188, -0.06630889, -0.05748867, -0.048659474, -0.039822686, -0.030979669, -0.022131803, -0.01328047, -0.0044270544, 0.0044270544, 0.01328047, 0.022131803, 0.030979669, 0.039822686, 0.048659474, 0.05748867, 0.06630889, 0.0751188, 0.083917044, 0.09270228, 0.1014732, 0.11022847, 0.1189668, 0.12768692, 0.13638757, 0.14506748, 0.15372545, 0.16236024, 0.17097066, 0.17955557, 0.18811381, 0.19664425, 0.20514579, 0.21361737, 0.22205792, 0.23046643, 0.23884188, 0.24718332, 0.2554898, 0.26376036, 0.27199417, 0.28019032, 0.28834802, 0.2964664, 0.30454475, 0.31258222, 0.32057822, 0.32853198, 0.3364428, 0.34431013, 0.35213327, 0.35991174, 0.3676449, 0.3753323, 0.3829734, 0.39056775, 0.3981149, 0.40561444, 0.41306597, 0.4204692, 0.42782366, 0.4351292, 0.44238544, 0.44959208, 0.45674902, 0.46385592, 0.47091264, 0.47791907, 0.48487496, 0.49178025, 0.49863485, 0.5054387, 0.5121916};
int posBranco = 64;
float find_Const(float pos){

	return constProporc[(unsigned int)pos];
}
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
	{
  /* Write your local variable definition here */
	uint16 countServo;
	int i;
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	ADC0_CFG2 |= ADC_CFG2_MUXSEL_MASK;
  /*** End of Processor Expert internal initialization.                    ***/
	
	
  /* Write your code here */
  /* For example: for(;;) { } */
	
	
	
/*
   * Motores são setados com valores de 0 a 255, 2o argumento resolve o sentido.
   */
	cameraSIBit_ClrVal();
	sMachine_Init();
	Serial_Init();
	enableMotor();

	servoPWM_Enable(servoPWM_DeviceData);
	countServo = 19000;
	servoPWM_SetDutyUS(servoPWM_DeviceData,countServo);
	cameraCLK_Enable();
	cameraCLK_EnableEvent();
	cameraCLKBit_ClrVal();
	
	while(1){
		setMotor(0,1,0);
		setMotor(1,1,0);
			for (i=0; i < 128; i++){
				vetorCamera[i] = getCamPixel(i);
			}
/*
			for (i=0; i < 128; i++){
							vetorCamera[i] = getCamPixel(i);
						}
			for (i=0; i < 128; i++){
							itoa(vetorCamera[i], buffer0);
							Send_String(buffer0);
							Send_String(" ");
							}
			Send_String("\r\n");
			
			
			
			for (i=0; i < 128; i++){
				itoa(vetorCamera[i], buffer0);
				Send_String(buffer0);
				Send_String(" ");
				}
			Send_String("\r\n");
*/
			vetor_Derivadas[0]=-1;
			vetor_Derivadas[1]=-1;
			for (i=posBranco; i >=0;i--)
				if (vetorCamera[i]==255){
					posBranco = i;
					break;
				}
			for (i=posBranco; i < 128;i++)
				if (vetorCamera[i]==255){
					posBranco = i;
					break;
				}
			
			for (i=posBranco; i >= 0;i--)
				if (i+5 > 127)
					continue;
				if (vetorCamera[i]-vetorCamera[i+5] > LNDERIV){
					vetor_Derivadas[LEFT] = i+3;
					break;
				}
			if (vetor_Derivadas[LEFT] == -1)
				vetor_Derivadas[LEFT] = 0;

			
			for (i=posBranco; i < 128;i++)
				if (i-5 < 0)
					continue;
				if (vetorCamera[i] - vetorCamera[i-5] > LNDERIV){
									vetor_Derivadas[RIGHT] = i-3;
									break;
				}
				if (vetor_Derivadas[RIGHT] == -1)
					vetor_Derivadas[RIGHT] = 127;
					
			
			P = (find_Const(vetor_Derivadas[LEFT]+vetor_Derivadas[RIGHT])/2);
			
			Servo = kP * P + kI * I + kD * D;
			servoPWM_SetDutyUS(servoPWM_DeviceData,countServo+Servo);
			itoa((int)Servo, buffer0);
			Send_String(buffer0);
			Send_String(" ");
			itoa((int)vetor_Derivadas[LEFT], buffer0);
			Send_String(buffer0);
			Send_String(" ");
			itoa((int)vetor_Derivadas[RIGHT], buffer0);
			Send_String(buffer0);
			Send_String("\r\n");
			
			}
			
			// Filtragem Básica dos pixels
			// Definir um limite para mínimo
			// Achar mínimos das linhas
			// Casos:
				//2 mínimos
					//achar mediana dos mínimos
				//mínimo único
					//
				// nenhum mínimo
 
   
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
