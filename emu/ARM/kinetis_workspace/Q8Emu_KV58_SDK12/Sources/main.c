/* ###################################################################
**     Filename    : main.c
**     Project     : Q8Emu_KV58_SDK12
**     Processor   : MKV58F1M0VLQ22
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-08-12, 09:23, # CodeGen: 0
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
#if CPU_INIT_CONFIG
  #include "Init_Config.h"
#endif
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "sdram.h"

extern int clock_count ;
extern int reflesh_req ;

volatile uint32_t d_clk ;
uint32_t clka[128] ;

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */
	uint32_t clk1, clk2 ;
	uint32_t volatile * p=(uint32_t *)(0x4004000cUL) ;
	int i ;

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/
	sdram_init() ;
	GPIOD_PDDR=1 ;

	/* Write your code here */
	/*lptmr_status_t r=*/ (void)LPTMR_DRV_Start(0) ;
	/* For example: for(;;) { } */

	for(i=0 ; i<4 ; i++){
		GPIOD_PSOR=1 ;
		GPIOD_PCOR=1 ;
		GPIOD_PSOR=1 ;
		GPIOD_PCOR=1 ;
		GPIOD_PSOR=1 ;
		GPIOD_PCOR=1 ;
		__asm__("nop") ;
		GPIOD_PSOR=1 ;
		__asm__("nop") ;
		__asm__("nop") ;
		GPIOD_PCOR=1 ;
	}

	for(;;){
		//clk1=LPTMR_DRV_GetCurrentPulseCount(0);
		//clk2=LPTMR_DRV_GetCurrentPulseCount(0);
		//d_clk=clk2-clk1 ;
		*p=0UL ;
		clk1=*p ;
		for(i=0 ; i<100 ; i++){
//			clka[i]=GPIOA_PDIR ;
			if(GPIOA_PDIR & 0x01000000) break ;
		}
		*p=0UL ;
		clk2=*p ;
		d_clk=clk2-clk1 ;
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
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
