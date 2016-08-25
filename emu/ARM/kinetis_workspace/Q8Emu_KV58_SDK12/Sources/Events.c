/* ###################################################################
**     Filename    : Events.c
**     Project     : Q8Emu_KV58_SDK12
**     Processor   : MKV58F1M0VLQ22
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-08-12, 09:23, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
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

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

void uartCom1_RxCallback(uint32_t instance, void * uartState)
{
  /* Write your code here ... */
}

void uartCom1_TxCallback(uint32_t instance, void * uartState)
{
  /* Write your code here ... */
}

int clock_count=0 ;
int reflesh_req=0 ;
static int reflesh_tick=0 ;
void PIT0_IRQHandler(void)
{
	/* Clear interrupt flag.*/
	PIT_HAL_ClearIntFlag(g_pitBase[FSL_PITTIMER1], FSL_PITTIMER1_CHANNEL);
	/* Write your code here ... */
	reflesh_req=1 ;	//	reflesh request flag.
	reflesh_tick+=6 ;
	if(reflesh_tick > 500){
		reflesh_tick=0 ;
		GPIO_DRV_TogglePinOutput(GPIOA6) ;
	}
}



void lpTmr1_OnTimerCompare(void)
{
	/* Write your code here ... */
	clock_count++ ;
}

void CrossBar1_Callback(void * param)
{
    /* Write you code here... */
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
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
