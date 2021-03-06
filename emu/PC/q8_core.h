#ifndef Q8_CORE_H
#define Q8_CORE_H

#define INTERNAL_MMU 1

union tag_internal_register{
	BYTE breg[16] ;
	WORD wreg[32] ;
} ;
extern union tag_internal_register internal_register ;

extern int system_mode ;

#define DREG 0
#define AREG 4

#define CP 8+system_mode
#define SP 10+system_mode
#define FR 12+system_mode
#define SR 14+system_mode

#define SCP 9
#define SSP 11
#define SFR 13
#define SSR 15

#define WCP 16


//	アドレスレジスタ　内部使用
#define ADDR_S 16
#define ADDR_D 16

void core_execute(void) ;
void core_init(void) ;

BYTE fetch(void) ;
void inc_pc(void) ;
void jump(BYTE offset) ;
void jump_l(WORD offset) ;
void jump_abs(WORD addr) ;

/*
	get_mem,set_memはマクロにしてMMUあり/なしの切り替えをする@@@
*/
/*
void set_reg(BYTE reg, BYTE value) ;
BYTE get_reg(BYTE reg) ;
void set_regw(BYTE reg, WORD value) ;
WORD get_regw(BYTE reg) ;
*/

#define set_reg(reg, value) internal_register.breg[reg]=value
#define get_reg(reg) internal_register.breg[reg]
#define set_regw(reg, value) internal_register.wreg[reg]=value
#define get_regw(reg) internal_register.wreg[reg]

/*
void set_mem(WORD addr, BYTE data) ;
BYTE get_mem(WORD addr) ;
*/

#define set_mem(addr, value) mmu_mem_write(addr, value)
#define set_memw(addr, value) mmu_mem_writew(addr, value)

#define get_mem_x(addr) mmu_mem_fetch(addr)
#define get_mem(addr) mmu_mem_read(addr)
#define get_memw(addr) mmu_mem_readw(addr)


// reg
void hw_int(BYTE intno) ;
void sw_int(BYTE intno) ;
void halt(void) ;

void int_set(BYTE intno) ;
void int_reset(BYTE intno) ;
BYTE int_detect(void) ;


#define INT_RESET 0
#define INT_PAGE_FAULT 0x10


#endif