/*
 * sdram.h
 *
 *  Created on: 2016/08/21
 *      Author: nic
 */

#ifndef SOURCES_SDRAM_H_
#define SOURCES_SDRAM_H_

typedef enum _SDR_Command{
	SDR_MRS=0,	//	Set mode register.
	SDR_REF,	//	Auto refresh.
	SDR_SELF,
	SDR_END_SELF,
	SDR_PRE,	//	Precharge
	SDR_PALL,	//	Precharge ALL.
	SDR_ACT,	//	Bank Activation.
	SDR_WRIT,
	SDR_WRITA,
	SDR_READ,
	SDR_READA,
	SDR_BST,	//	Burst stop.
	SDR_NOP,
	SDR_DSEL
} SDR_Command ;


void sdram_init(void);

void sdram_refresh(int start_clmn) ;

int sdram_read_byte(int addr) ;
void sdram_write_byte(int addr, int value) ;

#endif /* SOURCES_SDRAM_H_ */
