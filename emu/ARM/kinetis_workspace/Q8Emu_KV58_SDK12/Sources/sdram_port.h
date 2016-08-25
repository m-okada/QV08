/*
 * sdram_port.h
 *
 *  Created on: 2016/08/25
 *      Author: Manabu Okada
 */

#ifndef SOURCES_SDRAM_PORT_H_
#define SOURCES_SDRAM_PORT_H_


void sdramport_init(void) ;

void sdramport_dq_out(void) ;
void sdramport_dq_in(void) ;

void sdrampot_cke_assert(void) ;
void sdrampot_cke_negate(void) ;

void sdrampot_set_bank(int bank) ;
void sdramport_set_addr(int addr) ;

void sdramport_setmode(int mode) ;
void sdramport_setcommand(int bank, int ctrl, int addr) ;

inline void sdramport_clk(void){
	GPIOD_PSOR=1 ;
	GPIOD_PCOR=1 ;
}

#define SDRAMPORT_QD_OUT GPIOC_PDDR=0x0000FFFFUL
#define SDRAMPORT_QD_IN GPIOC_PDDR=0

#endif /* SOURCES_SDRAM_PORT_H_ */
