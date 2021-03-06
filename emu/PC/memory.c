#include "q8_emu.h"
#include "q8_core.h"
#include "q8_mmu.h"
#include "memory.h"

//	メインメモリ
static BYTE mem[PMEM_SIZE] ;
static BYTE boot_rom[4*1024] ;

void mem_init(void){

}

/*
	実装メモリ範囲外の判定はMMUで行う。
*/
BYTE mem_read(DWORD addr){
	return mem[addr & PMEM_MASK] ;
}

void mem_write(DWORD addr, BYTE data){
	mem[addr & PMEM_MASK]=data ;
}


