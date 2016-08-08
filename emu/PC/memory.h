#ifndef Q8_MEM_H
#define Q8_MEM_H

#define PMEM_SIZE ((DWORD)2048*1024)
#define PMEM_MASK (PMEM_SIZE-1)

BYTE mem_read(DWORD addr) ;
void mem_write(DWORD addr, BYTE data) ;


#endif
