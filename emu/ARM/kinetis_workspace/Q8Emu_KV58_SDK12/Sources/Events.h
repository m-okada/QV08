/* ###################################################################
**     Filename    : Events.h
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
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "fsl_device_registers.h"
#include "clockMan1.h"
#include "pin_mux.h"
#include "osa1.h"
#include "uartCom1.h"
#include "pitTimer1.h"
#include "LED.h"
#include "SD_DQ.h"
#include "SD_ADDR_CTRL.h"
#include "SD_CLK.h"
#include "lpTmr1.h"
#include "CLK12M_IN.h"
#include "CrossBar1.h"

#ifdef __cplusplus
extern "C" {
#endif 


void uartCom1_RxCallback(uint32_t instance, void * uartState);

void uartCom1_TxCallback(uint32_t instance, void * uartState);

void PIT0_IRQHandler(void);

void PIT1_IRQHandler(void);

void FTM0_IRQHandler(void);

void lpTmr1_OnTimerCompare(void);

void CrossBar1_Callback(void * param);

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
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
