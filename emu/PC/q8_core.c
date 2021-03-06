/*

q8_core.c

レジスタ
インストラクションの実行
*/
#include "q8_emu.h"
#include "q8_mmu.h"
#include "q8_core.h"

union tag_internal_register internal_register ;

int system_mode=0 ;	//	Super user mode when reset.
BYTE uni=0 ;
static BYTE int_state[256] ;

void core_init(void){
	int i ;
	for(i=0 ; i<32 ; i++){
		set_regw(i, 0) ;
	}
	set_regw(SR, 0x00) ;
	set_regw(SSR, 0x00) ;

	for(i=0 ; i<256 ; i++){
		int_state[i]=0 ;
	}
	system_mode=1 ;
}

void core_execute(void){
	while(1){
		BYTE op=fetch() ;
		execute(op) ;
		int_detect() ;
	}
}

void reset_sequence(){
	core_init() ;

	//	全てハイインピーダンス

	//	データバス入力
//	DATA_IN() ;

	//	リセットベクタフェッチ
//	ADDR_REGISTER(0xfffe) ;

}

void int_set(BYTE intno){
}

void int_reset(BYTE intno){
}

BYTE int_detect(void){
}

/*
register
*/
/*
void set_reg(BYTE reg, BYTE value){
	internal_register.breg[reg]=value ;
}
BYTE get_reg(BYTE reg){
	return internal_register.breg[reg] ;
}

void set_regw(BYTE reg, WORD value){
	internal_register.wreg[reg]=value ;
}

WORD get_regw(BYTE reg){
	return internal_register.wreg[reg] ;
}
*/

//	Update Code Pointer.
void inc_pc(void){
	internal_register.wreg[CP]++ ;
}

void update_pc(void){
	internal_register.wreg[CP]=internal_register.wreg[WCP] ;
}

void jump(BYTE offset){
	int addr=(int)offset-128 ;
	internal_register.wreg[CP]+=addr ;
}

void jump_l(WORD offset){
	int addr=(int)offset-32768 ;
	internal_register.wreg[CP]+=addr ;
}

void jump_abs(WORD addr){
	internal_register.wreg[CP]=addr ;
}
/*

CPのnバイト先のフェッチ

*/
BYTE fetch(void){
	return get_mem_x(get_regw(CP)) ;
}

/*
	memory
*/
/*
// define as macro.

void set_mem(WORD addr, BYTE data){
}
void set_memw(WORD addr, WORD data){
}

BYTE get_mem(WORD addr){
	return mmu_get_mem(addr) ;
}

BYTE get_mem(WORD addr){
	return mmu_get_mem(addr) ;
}
*/
/*
BUS
*/

/*
int
*/

void hw_int(BYTE intno){
}

void sw_int(BYTE intno){
}

void halt(void){
}

