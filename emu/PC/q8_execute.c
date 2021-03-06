#include "q8_emu.h"
#include "q8_core.h"
#include "q8_execute.h"

extern BYTE uni ;

void ld_ex1(BYTE op){
	fetch() ;
}

void ld_ex2(BYTE op){
	fetch() ;
}

void st_ex1(BYTE op){
	fetch() ;
}

void st_ex2(BYTE op){
	fetch() ;
}

void execute(BYTE op){
	uni=0 ;
	WORD addr ;

	switch(op){
	case 0x00: set_reg(DREG, get_mem(get_regw(AREG+0))) ; break ;
	case 0x01: set_reg(DREG, get_mem(get_regw(AREG+2))) ; break ;
	case 0x02: set_reg(DREG, get_mem(get_regw(AREG+4))) ; break ;
	case 0x03: set_reg(DREG, get_mem(get_regw(AREG+6))) ; break ;
	case 0x04: set_regw(DREG, get_memw(get_regw(AREG+0))) ; break ;
	case 0x05: set_regw(DREG, get_memw(get_regw(AREG+2))) ; break ;
	case 0x06: set_regw(DREG, get_memw(get_regw(AREG+4))) ; break ;
	case 0x07: set_regw(DREG, get_memw(get_regw(AREG+6))) ; break ;
	case 0x08: ld_ex1(op) ; break ;
	case 0x09: set_reg(DREG+1, get_mem(get_regw(AREG))) ; break ;
	case 0x0a: set_reg(DREG+2, get_mem(get_regw(AREG))) ; break ;
	case 0x0b: set_reg(DREG+3, get_mem(get_regw(AREG))) ; break ;
	case 0x0c: ld_ex2(op) ; break ;
	case 0x0d: addr=get_regw(AREG) ; set_reg(DREG+2, get_mem(addr)) ; set_reg(DREG+3, get_mem(addr+3)) ; break ;
	case 0x0e: addr=get_regw(AREG) ; set_reg(DREG+4, get_mem(addr)) ; set_reg(DREG+5, get_mem(addr+3)) ; break ;
	case 0x0f: addr=get_regw(AREG) ; set_reg(DREG+6, get_mem(addr)) ; set_reg(DREG+7, get_mem(addr+3)) ; break ;

	case 0x10: set_mem(get_regw(AREG), get_reg(DREG+1)) ; break ;
	case 0x11: break ;
	case 0x12: break ;
	case 0x13: break ;
	case 0x14: break ;
	case 0x15: break ;
	case 0x16: break ;
	case 0x17: break ;
	case 0x18: st_ex1(op) ; break ;
	case 0x19: set_mem(get_regw(AREG), get_reg(DREG+1)) ; break ;
	case 0x1a: break ;
	case 0x1b: break ;
	case 0x1c: st_ex2(op) ; break ;
	case 0x1d: break ;
	case 0x1e: break ;
	case 0x1f: break ;

	case 0x20: break ;
	case 0x21: break ;
	case 0x22: break ;
	case 0x23: break ;
	case 0x24: break ;
	case 0x25: break ;
	case 0x26: break ;
	case 0x27: break ;
	case 0x28: break ;
	case 0x29: break ;
	case 0x2a: break ;
	case 0x2b: break ;
	case 0x2c: break ;
	case 0x2d: break ;
	case 0x2e: break ;
	case 0x2f: break ;

	case 0x30: break ;
	case 0x31: break ;
	case 0x32: break ;
	case 0x33: break ;
	case 0x34: break ;
	case 0x35: break ;
	case 0x36: break ;
	case 0x37: break ;
	case 0x38: break ;
	case 0x39: break ;
	case 0x3a: break ;
	case 0x3b: break ;
	case 0x3c: break ;
	case 0x3d: break ;
	case 0x3e: break ;
	case 0x3f: break ;

	case 0x40: uni=1 ; break ;
	case 0x41: uni=1 ; break ;
	case 0x42: uni=1 ; break ;
	case 0x43: uni=1 ; break ;
	case 0x44: uni=1 ; break ;
	case 0x45: uni=1 ; break ;
	case 0x46: uni=1 ; break ;
	case 0x47: uni=1 ; break ;
	case 0x48: uni=1 ; break ;
	case 0x49: uni=1 ; break ;
	case 0x4a: uni=1 ; break ;
	case 0x4b: uni=1 ; break ;
	case 0x4c: uni=1 ; break ;
	case 0x4d: uni=1 ; break ;
	case 0x4e: uni=1 ; break ;
	case 0x4f: uni=1 ; break ;

	case 0x50: break ;
	case 0x51: break ;
	case 0x52: break ;
	case 0x53: break ;
	case 0x54: break ;
	case 0x55: break ;
	case 0x56: break ;
	case 0x57: break ;
	case 0x58: break ;
	case 0x59: break ;
	case 0x5a: break ;
	case 0x5b: break ;
	case 0x5c: break ;
	case 0x5d: break ;
	case 0x5e: break ;
	case 0x5f: break ;

	case 0x60: break ;
	case 0x61: break ;
	case 0x62: break ;
	case 0x63: break ;
	case 0x64: break ;
	case 0x65: break ;
	case 0x66: break ;
	case 0x67: break ;
	case 0x68: break ;
	case 0x69: break ;
	case 0x6a: break ;
	case 0x6b: break ;
	case 0x6c: break ;
	case 0x6d: break ;
	case 0x6e: break ;
	case 0x6f: break ;

	case 0x70: break ;
	case 0x71: break ;
	case 0x72: break ;
	case 0x73: break ;
	case 0x74: break ;
	case 0x75: break ;
	case 0x76: break ;
	case 0x77: break ;
	case 0x78: break ;
	case 0x79: break ;
	case 0x7a: break ;
	case 0x7b: break ;
	case 0x7c: break ;
	case 0x7d: break ;
	case 0x7e: break ;
	case 0x7f: break ;

	case 0x80: break ;
	case 0x81: break ;
	case 0x82: break ;
	case 0x83: break ;
	case 0x84: break ;
	case 0x85: break ;
	case 0x86: break ;
	case 0x87: break ;
	case 0x88: break ;
	case 0x89: break ;
	case 0x8a: break ;
	case 0x8b: break ;
	case 0x8c: break ;
	case 0x8d: break ;
	case 0x8e: break ;
	case 0x8f: break ;

	case 0x90: break ;
	case 0x91: uni=1; break ;
	case 0x92: break ;
	case 0x93: uni=1; break ;
	case 0x94: break ;
	case 0x95: uni=1; break ;
	case 0x96: uni=1; break ;
	case 0x97: uni=1; break ;
	case 0x98: break ;
	case 0x99: break ;
	case 0x9a: break ;
	case 0x9b: break ;
	case 0x9c: break ;
	case 0x9d: break ;
	case 0x9e: break ;
	case 0x9f: break ;

	case 0xa0: break ;
	case 0xa1: break ;
	case 0xa2: break ;
	case 0xa3: break ;
	case 0xa4: break ;
	case 0xa5: break ;
	case 0xa6: break ;
	case 0xa7: break ;
	case 0xa8: break ;
	case 0xa9: break ;
	case 0xaa: break ;
	case 0xab: break ;
	case 0xac: break ;
	case 0xad: break ;
	case 0xae: break ;
	case 0xaf: break ;

	case 0xb0: break ;
	case 0xb1: break ;
	case 0xb2: break ;
	case 0xb3: break ;
	case 0xb4: break ;
	case 0xb5: break ;
	case 0xb6: break ;
	case 0xb7: break ;
	case 0xb8: break ;
	case 0xb9: break ;
	case 0xba: break ;
	case 0xbb: break ;
	case 0xbc: break ;
	case 0xbd: break ;
	case 0xbe: break ;
	case 0xbf: break ;

	case 0xc0: break ;
	case 0xc1: break ;
	case 0xc2: break ;
	case 0xc3: break ;
	case 0xc4: break ;
	case 0xc5: break ;
	case 0xc6: break ;
	case 0xc7: break ;
	case 0xc8: break ;
	case 0xc9: break ;
	case 0xca: break ;
	case 0xcb: break ;
	case 0xcc: break ;
	case 0xcd: break ;
	case 0xce: uni=1; break ;
	case 0xcf: break ;

	case 0xd0: break ;
	case 0xd1: break ;
	case 0xd2: break ;
	case 0xd3: break ;
	case 0xd4: break ;
	case 0xd5: break ;
	case 0xd6: break ;
	case 0xd7: break ;
	case 0xd8: break ;
	case 0xd9: break ;
	case 0xda: break ;
	case 0xdb: break ;
	case 0xdc: break ;
	case 0xdd: break ;
	case 0xde: break ;
	case 0xdf: break ;

	case 0xe0: break ;
	case 0xe1: break ;
	case 0xe2: break ;
	case 0xe3: break ;
	case 0xe4: break ;
	case 0xe5: break ;
	case 0xe6: break ;
	case 0xe7: break ;
	case 0xe8: break ;
	case 0xe9: break ;
	case 0xea: break ;
	case 0xeb: break ;
	case 0xec: break ;
	case 0xed: break ;
	case 0xee: break ;
	case 0xef: break ;

	case 0xf0: uni=1; break ;
	case 0xf1: uni=1; break ;
	case 0xf2: uni=1; break ;
	case 0xf3: uni=1; break ;
	case 0xf4: uni=1; break ;
	case 0xf5: uni=1; break ;
	case 0xf6: uni=1; break ;
	case 0xf7: uni=1; break ;
	case 0xf8: uni=1; break ;
	case 0xf9: uni=1; break ;
	case 0xfa: uni=1; break ;
	case 0xfb: uni=1; break ;
	case 0xfc: uni=1; break ;
	case 0xfd: uni=1; break ;
	case 0xfe: uni=1; break ;
	case 0xff: uni=1; break ;
	}

	if(uni==1){
		//	unimplemented
	}
	int_detect() ;
	inc_pc() ;
}

/*
int main(void){
	BYTE op=0 ;
	execute(op) ;

	return 0 ;
}
*/
