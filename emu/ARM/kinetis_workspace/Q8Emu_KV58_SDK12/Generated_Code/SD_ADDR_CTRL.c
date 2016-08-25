/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : SD_ADDR_CTRL.c
**     Project     : Q8Emu_KV58_SDK12
**     Processor   : MKV58F1M0VLQ22
**     Component   : fsl_gpio
**     Version     : Component 1.2.0, Driver 01.00, CPU db: 3.50.001
**     Repository  : KSDK 1.2.0 KV5XF 1.0.0
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-08-25, 04:48, # CodeGen: 66
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file SD_ADDR_CTRL.c
** @version 01.00
*/         
/*!
**  @addtogroup SD_ADDR_CTRL_module SD_ADDR_CTRL module documentation
**  @{
*/         

/* MODULE SD_ADDR_CTRL. */

#include "SD_ADDR_CTRL.h"
#include <stdbool.h>
const gpio_output_pin_user_config_t SD_ADDR_CTRL_OutConfig0[] = {
  {
    .pinName = ADDR00,
    .config.outputLogic = 0,
    .config.slewRate = kPortFastSlewRate,
    .config.isOpenDrainEnabled = false,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = ADDR01,
    .config.outputLogic = 0,
    .config.slewRate = kPortFastSlewRate,
    .config.isOpenDrainEnabled = false,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = ADDR02,
    .config.outputLogic = 0,
    .config.slewRate = kPortFastSlewRate,
    .config.isOpenDrainEnabled = false,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = ADDR03,
    .config.outputLogic = 0,
    .config.slewRate = kPortFastSlewRate,
    .config.isOpenDrainEnabled = false,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = ADDR04,
    .config.outputLogic = 0,
    .config.slewRate = kPortFastSlewRate,
    .config.isOpenDrainEnabled = false,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = ADDR05,
    .config.outputLogic = 0,
    .config.slewRate = kPortFastSlewRate,
    .config.isOpenDrainEnabled = false,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = ADDR06,
    .config.outputLogic = 0,
    .config.slewRate = kPortFastSlewRate,
    .config.isOpenDrainEnabled = false,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = ADDR07,
    .config.outputLogic = 0,
    .config.slewRate = kPortFastSlewRate,
    .config.isOpenDrainEnabled = false,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = ADDR08,
    .config.outputLogic = 0,
    .config.slewRate = kPortFastSlewRate,
    .config.isOpenDrainEnabled = false,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = ADDR09,
    .config.outputLogic = 0,
    .config.slewRate = kPortFastSlewRate,
    .config.isOpenDrainEnabled = false,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = ADDR10,
    .config.outputLogic = 0,
    .config.slewRate = kPortFastSlewRate,
    .config.isOpenDrainEnabled = false,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = ADDR11,
    .config.outputLogic = 0,
    .config.slewRate = kPortFastSlewRate,
    .config.isOpenDrainEnabled = false,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = SD_CS,
    .config.outputLogic = 0,
    .config.slewRate = kPortSlowSlewRate,
    .config.isOpenDrainEnabled = true,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = SD_WE,
    .config.outputLogic = 0,
    .config.slewRate = kPortSlowSlewRate,
    .config.isOpenDrainEnabled = true,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = SD_CAS,
    .config.outputLogic = 0,
    .config.slewRate = kPortSlowSlewRate,
    .config.isOpenDrainEnabled = true,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = SD_RAS,
    .config.outputLogic = 0,
    .config.slewRate = kPortSlowSlewRate,
    .config.isOpenDrainEnabled = true,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = SD_BA0,
    .config.outputLogic = 0,
    .config.slewRate = kPortSlowSlewRate,
    .config.isOpenDrainEnabled = true,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = SD_BA1,
    .config.outputLogic = 0,
    .config.slewRate = kPortSlowSlewRate,
    .config.isOpenDrainEnabled = true,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = SD_LDQM,
    .config.outputLogic = 0,
    .config.slewRate = kPortSlowSlewRate,
    .config.isOpenDrainEnabled = true,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = SD_UDQM,
    .config.outputLogic = 0,
    .config.slewRate = kPortSlowSlewRate,
    .config.isOpenDrainEnabled = true,
    .config.driveStrength = kPortLowDriveStrength,
  },
  {
    .pinName = GPIO_PINS_OUT_OF_RANGE,
  }
};
      


/* END SD_ADDR_CTRL. */
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
