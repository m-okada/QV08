/*

	q8用アセンブラ
	単独ファイルでC言語include用カンマ区切り１６進ファイルを出力する


	int=32bitsの環境用



segment ident


reg w0-w3,a0-a3,b0-b3,cp,sp,cr,sr,scp,ssp,scr,ssr,ucp,usp,ucr,usr
	cp,sp,cr,srはモードによって変わる、明示的に指定するなら scp,ucpなどを使う

Formal-instruction

Pseudo-instruction

symbol db,dw,dd binary
	db,dw,dd ; binary列の埋め込み
symbol equ exper	symbolは定数か式、評価が先に行われて定数が割り当てられる



symbol func	# sタックフレーム、開放コード自動生成
endf

[reg]
+-*
'
ロケーション：モジュール先頭からのオフセット

$	;現在のロケーション

label:	;ラベルはロケーションと名前のバインド
label equ $ ; これでも同じ


label $

[reg]
+
'


ld,stは、アドレッシングがオペランド必要なオペランドは自動判定。
それ以外はアドレッシングモードを持たないので、アドレッシングモードの判定は不要


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINEBUFFLEN 1024
#define IDENTLEN 64
#define NUMSYMBOL 1024

#define SYM_UNDEF 1
#define SYM_LABEL 2
#define SYM_VAR 3	//	db,dw ...
#define SYM_CONST 4	//	equ number
#define SYM_ 5

#define IDENT_MNEMONIC 10
#define IDENT_REGISTER 11
#define IDENT_MNEMONIC_NOOP 12	//	no opeland

#define REG_BYTE 0x100
#define REG_WORD 0x110
#define REG_ADDR 0x114

#define IDENT_LABEL	20	//	address
#define IDENT_VAR_BYTE 21	//	address & type
#define IDENT_VAR_WORD 22	//	address & type
#define IDENT_VAR_DWORD 23	//	address & type
#define IDENT_GVAR 24	//	global variable (externed)
#define IDENT_SEGMENT 25
#define IDENT_PROC 26	//	procedure
#define IDENT_FUNC 27	//	function

#define IDENT_PSEUDO_INSTRUCTION 40

#define IDENT_ 99


#define TOKEN_NULL 0
#define TOKEN_EOF -1
#define TOKEN_WHITE_SPACE 1
#define TOKEN_ASCII 2
#define TOKEN_IDENT 3
#define TOKEN_EXPER 4


typedef struct tag_opcord{
	char *name ; // opcode mnemonic
	int type ; // type
	int code ; //	additional code
} opcode ;


opcode opecodeTable[]={
	{"ld", IDENT_MNEMONIC, 0x00},
	{"st", IDENT_MNEMONIC, 0x10},
	{"add", IDENT_MNEMONIC, 0},
	{"adc", IDENT_MNEMONIC, 0},
	{"sub", IDENT_MNEMONIC, 0},
	{"sbb", IDENT_MNEMONIC, 0},
	{"move", IDENT_MNEMONIC, 0},
	{"movei", IDENT_MNEMONIC, 0x28},
	{"move.i", IDENT_MNEMONIC, 0},
	{"move.b", IDENT_MNEMONIC, 0},
	{"move.w", IDENT_MNEMONIC, 0},
	{"moveb", IDENT_MNEMONIC, 0},
	{"movew", IDENT_MNEMONIC, 0},
	{"jmp.s", IDENT_MNEMONIC, 0xcd},
	{"jmp.l", IDENT_MNEMONIC, 0},
	{"unknouwn", IDENT_MNEMONIC, 0},
	{"unknouwn", IDENT_MNEMONIC, 0},
	{"unknouwn", IDENT_MNEMONIC, 0},
	{"unknouwn", IDENT_MNEMONIC, 0},
	{"unknouwn", 0, 0},

	{"b0", IDENT_REGISTER, REG_BYTE+0},
	{"b1", IDENT_REGISTER, REG_BYTE+1},
	{"b2", IDENT_REGISTER, REG_BYTE+2},
	{"b3", IDENT_REGISTER, REG_BYTE+3},
	{"b4", IDENT_REGISTER, REG_BYTE+4},
	{"b5", IDENT_REGISTER, REG_BYTE+5},
	{"b6", IDENT_REGISTER, REG_BYTE+6},
	{"b7", IDENT_REGISTER, REG_BYTE+7},

	{"w0", IDENT_REGISTER, REG_WORD+0},
	{"w1", IDENT_REGISTER, REG_WORD+1},
	{"w2", IDENT_REGISTER, REG_WORD+2},
	{"w3", IDENT_REGISTER, REG_WORD+3},

	{"a0", IDENT_REGISTER, REG_ADDR+0},
	{"a1", IDENT_REGISTER, REG_ADDR+1},
	{"a2", IDENT_REGISTER, REG_ADDR+2},
	{"a3", IDENT_REGISTER, REG_ADDR+3},

	{"cp", IDENT_REGISTER, 0x130},
	{"sp", IDENT_REGISTER, 0x131},
	{"cr", IDENT_REGISTER, 0x132},
	{"sr", IDENT_REGISTER, 0x133},
	{"scp", IDENT_REGISTER, 0x134},
	{"ssp", IDENT_REGISTER, 0x135},
	{"scr", IDENT_REGISTER, 0x136},
	{"ssr", IDENT_REGISTER, 0x137},

	{"unknouwn", 0, 0},
	{"unknouwn", 0, 0},
	{"equ", IDENT_PSEUDO_INSTRUCTION, 0x200},
	{"func", IDENT_PSEUDO_INSTRUCTION, 0x200},
	{"db", IDENT_PSEUDO_INSTRUCTION, 0x200},
	{"dw", IDENT_PSEUDO_INSTRUCTION, 0x200},
	{"dd", IDENT_PSEUDO_INSTRUCTION, 0x200},
	{"org", IDENT_PSEUDO_INSTRUCTION, 0x200},
	{"unknouwn", -1, 0},
} ;

typedef struct tag_symbol{
	char name[IDENTLEN] ;
	int type ;
	int value ;	//	address or decrare pos
} symbol ;

symbol symbolTable[NUMSYMBOL] ;


char linebuff[LINEBUFFLEN] ;
int poslinebuff=0 ;
char token[LINEBUFFLEN] ;
unsigned char nextc=0 ;

int sourceline=0 ;

unsigned char outbuff[64*1024] ;
int outbuffpos=0 ;
int code_location=0 ;

void init(void){
	int i ;

	poslinebuff=0 ;
	nextc=0 ;
	linebuff[0]=0 ;
	symbolTable[0].name[0]=0 ;

	for(i=0 ; i<NUMSYMBOL ; i++){
		symbolTable[i].type=0 ;
	}
}

int getOpecode(char *token){
	int i=0 ;
	while(opecodeTable[i].type!=-1){
		if(strcmpi(opecodeTable[i].name, token)==0){
			return i ;
		}
		i++;
	}
	return -1 ;
}

void get_char(void){
	if(poslinebuff < LINEBUFFLEN){
		nextc=linebuff[poslinebuff] ;
		if(nextc!=0){
			poslinebuff++ ;
		}
	}
	else{
		nextc=0 ;
	}
//	printf("[%c]\n", nextc) ;
}

int takechar(int i){
	if(nextc!=0){
		token[i]=nextc;
		i++;
		get_char();
	}

	return i ;
}

int get_token(void){
	int i ;

	i=0 ;

	while((nextc == ' ') || (nextc == 9)){
		get_char() ;
	}

	while((('a' <= nextc) && (nextc <= 'z')) ||(('A' <= nextc) && (nextc <= 'Z')) ||
		(('0' <= nextc) && (nextc <= '9')) || (nextc == '_') || (nextc == '$')){
		i=takechar(i);
	}
	if(i==0){	/* 2文字以上のオペレータはここで食う */
		while((nextc == '<') || (nextc == '=') || (nextc == '>') || (nextc == '|') ||
			 (nextc == '&') || (nextc == '!') || (nextc == '+') || (nextc == '-')){
			i=takechar(i);
		}
	}

	if(i==0){
		if(nextc=='#'){	//	コメントは行末まで読み飛ばし
			while(nextc!=0) takechar(0) ;
			i=0 ;
		}
		else if(nextc=='"'){
			i=takechar(i);
			while (nextc!='"'){
				i=takechar(i);
			}
			i=takechar(i);
		}
		else if (nextc != 0){	/* 一文字オペレータはここで取れる */
			i=takechar(i);
		}
	}
	token[i] = 0;	/* 終端 */

printf("(t:%s)\n", token) ;

	return i ;
}

void error(int err){
	fprintf(stderr, "%d Error (%d)\n", sourceline, err) ;
	exit(2) ;
}

//	token が strと一致するか？
int peek(char *str){
	if(strcmp(token, str)==0){
		return 1 ;
	}
	return 0 ;
}

int accept(char *s){
	if (peek(s)) {
		get_token();
		return 1;
	}
	else{
		return 0;
	}
}

void expect(char *s, int errn){
	if (accept(s) == 0){
		error(errn);
	}
}


void save_int(char *p, int n){
	p[0] = n & 0xff ;
	p[1] = n >> 8;
}

int load_int(char *p){
	return ((p[0] & 0xff) + ((p[1] & 0xff) << 8));
}


void emit(int n, char *s){
	int i = 0;
	while (i < n) {
		outbuff[outbuffpos] = s[i];
		outbuffpos++;
		i++;
	}
	code_location=outbuffpos ;
}

int sym_lookup(char * str){
	int i ;
	for(i=0 ; i<NUMSYMBOL ; i++){
		if(strcmp(symbolTable[i].name, str)==0){
			return i ;
		}
	}

	return -1 ;
}

/*
	a-z A-Z $ _ で始まって、＋0-9だけで構成されたtoken
*/
int is_symbol(char * str){
	int symbol=1 ;
	unsigned char cc=*str ;

	if((('a' <= cc) && (cc <= 'z')) || (('A' <= cc) && (cc <= 'Z')) || (cc == '_') || (cc == '$')){
		str++ ;
		cc=*str ;

		while(*str){
			if((('a' <= cc) && (cc <= 'z')) || (('A' <= cc) && (cc <= 'Z')) ||
				(('0' <= cc) && (cc <= '9')) || (cc == '_') || (cc == '$')){
				str++ ;
				cc=*str ;
			}
			else{
				symbol=0 ;
				break ;
			}
		}
	}
	else{
		symbol=0 ;
	}
	return symbol ;
}

int char2int(char c, int radix){
	char value=-1 ;

	if(c>='a' && c<='f'){
		value=c-'a'+10 ;
	}
	else if(c>='A' && c<='F'){
		value=c-'A'+10 ;
	}
	else if(c>='0' && c<='9'){
		value=c-'0' ;
	}
	if(value<0 || value>radix){
		return -1 ;
	}
	return (int)value ;
}

int int_value=0 ;
int parse_int(char *str){
	int_value=0 ;
	int radix=10 ;

	if(str[0]=='0'){
		if(str[1]==0){
			int_value=0 ;
			return 0 ;
		}
		if(str[1]=='x'){	//	hex
			radix=16 ;
			str+=2 ;
		}
		else if(str[1]=='b'){
			radix=2 ;
			str+=2 ;
		}
		else if(str[1]=='o'){
			radix=8 ;
			str+=2 ;
		}
	}
	while(*str){
		int d ;
		int_value*=radix ;
		d=char2int(*str, radix) ;
		if(d==-1){
			//	error @@@
		}
		else{
			int_value+=d ;
		}
		str++ ;
	}

	return int_value ;
}

int is_number(char *str){
	if(str[0]>='0' && str[0]<='9'){	//	数字として正しいかどうかをチェックする@@@
		return 1 ;
	}
	return 0 ;
}


/*
	シンボル宣言、定義

	シンボルはソース中に出現したタイミングでdeclareする。
	クラスが決まった時にdefineする。


*/
int sym_declare(char * str){
	int i ;

	if(*str==0){
		error(501) ;
		return -2 ;
	}

	for(i=0 ; i<NUMSYMBOL ; i++){
		if(strcmp(symbolTable[i].name, str)==0){
//			if(symbolTable[i].type!=0){
				error(502) ;
				return -1 ;	//	二重定義
//			}
		}
	}

	// @@@
	for(i=0 ; i<NUMSYMBOL ; i++){
		if(symbolTable[i].type==0){
			strcpy(symbolTable[i].name, str) ;
			symbolTable[i].type=SYM_UNDEF ;
			return i ;
		}
	}
	error(503) ;
	return -1 ;
}

int sym_define(int symno, int type, int value){
	if(symbolTable[symno].type!=0){
		symbolTable[symno].type=type ;
		symbolTable[symno].value=value ;
		return 1 ;
	}
	return 0 ;
}

/*
	tokenがレジスタを示していればコードを、レジスタでない場合-1を返す。
*/
int accept_reg(void){
	int idx=getOpecode(token) ;	//	予約語チェック
	if(idx!=-1 && opecodeTable[idx].type==IDENT_REGISTER){
		get_token() ;
		return opecodeTable[idx].code ;
	}
	return -1 ;
}

/*
	-1 error
*/
#define REG_TYPE_ADDR 1
#define REG_TYPE_WORD 2
#define REG_TYPE_BYTE 3
#define REG_TYPE_SREG 4
#define REG_TYPE_SREG_SYS 5

int reg_type(char*str){
	int type=0 ;
	int idx=getOpecode(str) ;
	if(idx!=-1 && opecodeTable[idx].type==IDENT_REGISTER){

	}
	else{
		return -1 ;
	}
}

/*
	行末の CR or LF 以降を取る。
*/
int truncate(char *str){
	int i ;
	for(i=0 ; i<LINEBUFFLEN ; i++){
		if(str[i]==0x0d || str[i]==0x0a){
			str[i]=0 ;
			break ;
		}
	}
	str[LINEBUFFLEN-1]=0 ;
	return strlen(str) ;
}

/*
 const-exper
*/
int const_expr(void){
//	if(){	//	number literal
//	}
//	else if(){	//	const ident
//	}
	return 0 ;
}


int reg_exper(void){
	return 0 ;
}


/*
	アドレッシング式チェック

	base_reg a0-a3
	offset_reg b0-b7,w0-w4
	addr_offset const
*/


int base_reg=-1 ;	//	0:a0 - 3:a3
int offset_reg=-1 ;
int addr_offset=0 ;

int reg_const_expr(int sign){

	if(is_addr_reg(token){
	}
	else if(is_reg(token)){
	}
	else if(is_number(token)){
		addr_offset+(1*sign)+parse_int(token) ;
	}

	get_token();
	return 0 ;
}

int addr_add_expr(void){
	reg_const_expr();
	while (1) {
		if (accept("+")) {
			reg_const_expr(1) ;
		}
		else if (accept("-")) {
			reg_const_expr(-1) ;
		}
		else{
			return 0 ;
		}
	}
	return 0 ;
}

int addr_exper(void){
	base_reg=-1 ;
	addr_offset=0 ;
	offset_reg=-1 ;

	int op=addr_add_expr() ;
}

/*
	ld
	op1 reg
	op2 areg,areg+-const,areg+-wreg



*/
void ope_ld(void){
	get_token() ;
	printf("[%s]", token) ;
	int op1=accept_reg() ;
	if(op1==-1){
		error(102) ;	//	op1 must indicates a register.
	}
	expect(",", 203) ;
	int op2 ;

	// get_token() ;
	int type=0 ;
	type=addr_exper() ;	//	areg +- const|wreg|breg
}

void ope_st(void){
}


int current_symbol=0 ;

//	一行分の処理
void codegen(void){
	current_symbol=-1 ;
	token[0]=0 ;
	get_char();

	while(1){
		if(get_token()==0){
			break ;
		}
		int idx=getOpecode(token) ;	//	予約語チェック

		if(idx!=-1){	//	予約語
			if(opecodeTable[idx].type==IDENT_MNEMONIC){
				switch(opecodeTable[idx].code){
				case 0:	//	ld.b
					ope_ld() ;
					break ;
				case 0x10:
					break ;
				}
			}
		}
		else{		//	シンボルかもしれない
			if(is_symbol(token)){	//	symbolである
				//	行頭は宣言、定義
				current_symbol = sym_declare(token);

				int lookup=sym_lookup(token) ;
				if(lookup!=-1){
					current_symbol = lookup;
				}
				else{
					int symno=sym_declare(token) ;
					if(accept(":")){	//	ラベル定義
						sym_define(symno, SYM_LABEL, code_location) ;
					}
					else{
						//	@@@	行の後ろをパースしてから決める
						sym_define(symno, 0, 0) ;	//	@@@
					}
				}
			}
			else{	//	オペレータかもしれない
			}
		}

printf("opcode(%d)\n", idx) ;
	}
}


void dump_symtable(void){
	int i ;

	for(i=0 ; i<NUMSYMBOL ; i++){
		printf("[%s] %d, %d\n", symbolTable[i].name, symbolTable[i].type, symbolTable[i].value) ;
	}
}

FILE *sfp, *dfp ;

int main(int argc, char** argv){
	int llen ;

printf("(%d)\n", parse_int("0b0101")) ;
printf("(%d)\n", parse_int("0xffff")) ;
printf("(%d)\n", parse_int("2048")) ;


	if(argc<2){
		fprintf(stderr, "usage : q8as sourceFile\n") ;
		return 1 ;
	}

	if( (sfp=fopen(argv[1], "rb"))==0 ){
		fprintf(stderr, "Error : (%s) cannot open source file\n", argv[1]) ;
		return 17 ;
	}

	sourceline=0 ;
	while( fgets(linebuff, LINEBUFFLEN-1, sfp)!=NULL){
		poslinebuff=0 ;
		llen=strlen(linebuff) ;
		sourceline++ ;
		printf("len(%d)\n", llen) ;
		truncate(linebuff) ;
		codegen() ;
	}
}

