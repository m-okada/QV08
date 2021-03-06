#include <stdio.h>
#include "q8_emu.h"
#include "q8_mmu.h"
#include "q8_core.h"

int main(int argc, char *argv[]){
	mem_init() ;
	mmu_init() ;
	core_init() ;

	core_execute() ;

	return 0 ;
}

/*

	クロック変更がイベントになっていて、立ち上がりエッジでそれぞれのブロックが出力を確定する。

*/


void mem_config(void){
	mmu_set_mem_attr(0x0000, 0x7fff, 0, MMU_RWX) ;
	mmu_set_mem_attr(0x8000, 0xdfff, 1, MMU_RWX) ;
	mmu_set_mem_attr(0xe000, 0xfeff, 2, MMU_RX) ;	//	ROM1
	mmu_set_mem_attr(0xff00, 0xffff, 3, MMU_RX) ;	//	ROM2

}

