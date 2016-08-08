#ifndef Q8_MMU_H
#define Q8_MMU_H
/*

 MMU

 MMU_PORT_BASE0
  +0 Mode Register.
     bit 7 6 5 4 3 2 1 0
         x | x x x x x x
           |
           +------------ MMU enable 1:Enable. r/w

  +1 Current Task Register.
     bit 7 6 5 4 3 2 1 0
         | 0 0 +-------+
         |             +Task no r/w
         +s=0:User mode. 1:System mode. r/w

 MMU_PORT_BASE1
  +0..15 Page register 0-15.

*/
#define MMU_PAGE_NUM 1024
#define MMU_PAGE_BITS 10
#define MMU_PAGE_SIZE 4096

#define MMU_PORT_BASE0 0xff90
#define MMU_PORT_BASE1 0xffa0
#define MMU_MODE_REG (MMU_PORT_BASE0+0x00)
#define MMU_CURRENT_TASK_REG (MMU_PORT_BASE0+0x01)

#define MMU_PAGE_REG(n) (MMU_PORT_BASE1+n)

#define MMU_RWX 3
#define MMU_RX 2
#define MMU_RW 1
#define MMU_R 0
#define MMU_ND 0



void mmu_init(void) ;
BYTE mmu_mem_read(ADDR_T addr) ;
void mmu_mem_write(ADDR_T addr, BYTE data) ;
WORD mmu_mem_readw(ADDR_T addr) ;
void mmu_mem_writew(ADDR_T addr, WORD data) ;
BYTE mmu_mem_fetch(ADDR_T addr) ;
void mmu_set_mem_attr(ADDR_T start, ADDR_T end, BYTE cs, BYTE rw) ;
DWORD physical_address(ADDR_T addr) ;

#endif
