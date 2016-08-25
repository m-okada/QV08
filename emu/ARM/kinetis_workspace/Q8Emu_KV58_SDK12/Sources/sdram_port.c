/*
 * sdram_port.c
 *
 *  Created on: 2016/08/25
 *      Author: Manabu Okada
 */

#include "cpu.h"
#include "sdram_port.h"

/*

k:\home\projects\qv08\emu\sdram.txt

                       /RAS /CAS /WE
LOAD MODE REGISTER       L   L   L
AUTO REFRESH             L   L   H  (Auto refresh or Self refresh)
PRECHARGE                L   H   L  (Deactivate row)
ACTIVE                   L   H   H  (Select bank / Active row)
WRITE                    H   L   L  (Select bank/column , start write)
READ                     H   L   H  (Select bank/column , start read)
BURST TERMINATE          H   H   L
NOP                      H   H   H

*/

/*
CLK [GPIOD0]
CLKE [GPIOD1]
CS_n [GPIOD2]
BA[2] [GPIOB16,17]
A[12] [GPIOB0-11]
DQ[16] [GPIOC0-15]
command RAS_b,CAS_b,WE_b [GPIOD3-5]
DQM[2] [GPIOD6-7]

 */


/*
enum _SD_ADDR_CTRL_pinNames{
  ADDR00 = GPIO_MAKE_PIN(GPIOB_IDX, 0U),
  ADDR01 = GPIO_MAKE_PIN(GPIOB_IDX, 1U),
  ADDR02 = GPIO_MAKE_PIN(GPIOB_IDX, 2U),
  ADDR03 = GPIO_MAKE_PIN(GPIOB_IDX, 3U),
  ADDR04 = GPIO_MAKE_PIN(GPIOB_IDX, 4U),
  ADDR05 = GPIO_MAKE_PIN(GPIOB_IDX, 5U),
  ADDR06 = GPIO_MAKE_PIN(GPIOB_IDX, 6U),
  ADDR07 = GPIO_MAKE_PIN(GPIOB_IDX, 7U),
  ADDR08 = GPIO_MAKE_PIN(GPIOB_IDX, 8U),
  ADDR09 = GPIO_MAKE_PIN(GPIOB_IDX, 9U),
  ADDR10 = GPIO_MAKE_PIN(GPIOB_IDX, 10U),
  ADDR11 = GPIO_MAKE_PIN(GPIOB_IDX, 11U),
  // 12
  // 13
  // 14
  // 15
  SD_CS = GPIO_MAKE_PIN(GPIOB_IDX, 16U),
  SD_WE = GPIO_MAKE_PIN(GPIOB_IDX, 17U),
  SD_CAS = GPIO_MAKE_PIN(GPIOB_IDX, 18U),
  SD_RAS = GPIO_MAKE_PIN(GPIOB_IDX, 19U),
  SD_BA0 = GPIO_MAKE_PIN(GPIOB_IDX, 20U),
  SD_BA1 = GPIO_MAKE_PIN(GPIOB_IDX, 21U),
  SD_LDQM = GPIO_MAKE_PIN(GPIOB_IDX, 22U),
  SD_UDQM = GPIO_MAKE_PIN(GPIOB_IDX, 23U),
};
*/

unsigned int port_sd_addr=0 ;
void sdramport_init(void){
	SDRAMPORT_QD_IN ;
	__asm__("nop") ;
}

void sdramport_setmode(int mode){
	;
}
void sdramport_setcommand(int bank, int ctrl, int addr){
	;
}



