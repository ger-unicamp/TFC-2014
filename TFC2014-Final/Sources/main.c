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
unsigned char buffer0[12];
unsigned char esp[2]=" ";
unsigned char nLine[3] = "\r\n";
uint16_t vetorCamera[128]={};
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
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
	setMotor(0,1,0);
	setMotor(1,1,0);
	servoPWM_Enable(servoPWM_DeviceData);
	countServo = 19000;
	servoPWM_SetDutyUS(servoPWM_DeviceData,countServo);
	cameraCLK_Enable();
	cameraCLK_EnableEvent();
	cameraCLKBit_ClrVal();
	while(1){
			for (i=0; i < 128; i++){
				vetorCamera[i] = getCamPixel(i);
			}
			for (i=0; i < 128; i++){
				itoa(vetorCamera[i], buffer0);
				Send_String(buffer0);
				Send_String(" ");
				}
			Send_String("\r\n");
  }
  
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
