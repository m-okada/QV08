/*
 Q8 MMU

 MMU

 page size=4K

 page数は10ビットでアドレス空間22ビット＝4Mバイトまで。

タスク毎のページレジスタ

MMU内部レジスタ
ページ毎にタスク番号8ビット、マッピング位置4ビット、書き込み、実行パーミッション2ビット

*/
#include "q8_emu.h"
#include "q8_core.h"
#include "q8_mmu.h"
#include "q8_mem.h"

WORD mem_state[MMU_PAGE_NUM] ;

void mmu_init(void){
	int i ;
	for(i=0 ; i<MMU_PAGE_NUM ; i++){
		mem_state[i]=MMU_ND ;
	}
}

void mmu_assign(BYTE task, WORD page, PHY_ADDR_T paddr){

}

BYTE mmu_mem_read(ADDR_T addr){
	DWORD p_addr=physical_address(addr) ;

}

WORD mmu_mem_readw(ADDR_T addr){
}

void mmu_mem_write(ADDR_T addr, BYTE data){
}

void mmu_mem_writew(ADDR_T addr, WORD data){
}


BYTE mmu_mem_fetch(ADDR_T addr){
}

void mmu_set_mem_attr(ADDR_T start, ADDR_T end, BYTE cs, BYTE rw){

}

DWORD physical_address(ADDR_T addr){
	PHY_ADDR_T paddr ;

	paddr=addr>>12 ;


	return paddr ;
}
