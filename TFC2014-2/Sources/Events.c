/* ###################################################################
**     Filename    : Events.c
**     Project     : TFC2014
**     Processor   : MKL25Z128VLK4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2014-08-19, 15:12, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "Application.h"
//AO é uma variável global criada para armazenar os valores temporários de leitura para cada pixel da câmera.
typedef enum { Inicial=0, SI_ON, SI_OFF_READ_0, SI_OFF_READ_1, SI_OFF_WAIT} estados;
estados mEstados;
int contClocks; //Variável p/ contar a quantidade de clocks passada depois do pulso SI.
int i;
int leituraCamera;
float soma;
extern byte SI;
extern int counter;
extern int periodo;
extern uint16_t vetor_Camera[2][128]={{},{}};
int camClkState = 0;
int togBuffer = 0;


/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MKL25Z128LK4]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMIINT(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI1_OnInterrupt(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  cameraCLK_OnInterrupt (module Events)
**
**     Component   :  cameraCLK [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
int getLeituraCamera(void)
{
	return leituraCamera;
}



void toggleCamera(){
	if (cameraCLKBit_GetVal()){
		cameraCLKBit_NegVal();
		while(cameraCLKBit_GetVal());
	}
	else
		{
			cameraCLKBit_NegVal();
			while(!cameraCLKBit_GetVal());
		}
}

void toggleBuffer(void){
	if (!togBuffer)
		togBuffer=1;
	else
		togBuffer=0;
	
}

void cameraCLK_OnInterrupt(void)
{
	/* Write your code here ... */
	//Lembrando, interrupção CamCLK é de 30us
	toggleCamera();
	switch (mEstados){
		case SI_ON:
			if (!cameraCLKBit_GetVal()){
				cameraSIBit_SetVal();
				while(!cameraSIBit_GetVal());
				mEstados = SI_OFF_READ_0;
				contClocks = 0;
			}
			break;
		case SI_OFF_READ_0:
			if (cameraCLKBit_GetVal()){
				cameraSIBit_ClrVal();
				while(cameraSIBit_GetVal());
				//AD1_MeasureChan(0,0); // Liga a medida do conversor AD
				//contClocks++;
				mEstados = SI_OFF_READ_1;
			}
				break;
		case SI_OFF_READ_1:
			if (!cameraCLKBit_GetVal()){
			//contClocks++;
			AD1_MeasureChan(0,0); // Liga a medida do conversor AD
			if (contClocks==128){
				mEstados = SI_OFF_WAIT;
				toggleBuffer();
				}
			}
			
			break;
		case SI_OFF_WAIT:
			if (cameraCLKBit_GetVal())
				contClocks++;
				if (contClocks==600)
					mEstados = SI_ON;						
			break;
		default:
			break;
		}
	
	
}

/*
** ===================================================================
**     Event       :  AD1_OnEnd (module Events)
**
**     Component   :  AD1 [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AD1_OnEnd(void)
{
  /* Write your code here ... */
	if (contClocks < 128){
		AD1_GetChanValue(0,&(vetor_Camera[togBuffer][contClocks]));
		contClocks++;}
}

/*
** ===================================================================
**     Event       :  AD1_OnCalibrationEnd (module Events)
**
**     Component   :  AD1 [ADC]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AD1_OnCalibrationEnd(void)
{
  /* Write your code here ... */
}

void sMachine_Init(){
	mEstados = SI_ON;
	contClocks = 0;
	leituraCamera = 0;
}

uint16_t getCamPixel(int pixel){
	if (pixel > 128 || pixel < 0)
		return 0;
	return vetor_Camera[!togBuffer][pixel];
	
}
/*
** ===================================================================
**     Event       :  AS1_OnBlockReceived (module Events)
**
**     Component   :  AS1 [Serial_LDD]
*/
/*! 
**     @brief
**         This event is called when the requested number of data is
**         moved to the input buffer.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void AS1_OnBlockReceived(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
	UART_Desc *ptr = (UART_Desc*)UserDataPtr;
	
	(void)ptr->rxPutFct(ptr->rxChar); /* but received character into buffer */
	(void)AS1_ReceiveBlock(ptr->handle, (LDD_TData *)&ptr->rxChar, sizeof(ptr->rxChar));
}

/*
** ===================================================================
**     Event       :  AS1_OnBlockSent (module Events)
**
**     Component   :  AS1 [Serial_LDD]
*/
/*!
**     @brief
**         This event is called after the last character from the
**         output buffer is moved to the transmitter. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void AS1_OnBlockSent(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
	UART_Desc *ptr = (UART_Desc*)UserDataPtr; 
	ptr->isSent = TRUE; /* set flag so sender knows we have finished */
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
