/*
 * Copyright (C) 2006 Edmund GRIMLEY EVANS <edmundo@rano.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
QV8対応

・済み

Identifyerの大文字、_から始まるもの
改行CR
/ * %
比較　<
後置++ --
//コメント
エラー行（未検証動作あり）
asm出力（debug用）

・ToDo
QV8コード出力

signed intをunsigned intで出力

エラーメッセージ
<= < 以外の比較
0xffff
globalの初期化

mainの引数
	char**への対応

オペレータの最長一致
charをunsignedにする
int char*以外の型(特にshort,longとそれへのポインタ)
	あと、main()の引数に使うchar**
数値リテラルオプティマイズ

bit-xor

前置演算子
++ --
- !

op=

&& ||
評価中断

for
　forは色々行ったり来たりするので・・・
break
continue
switch case default
struct

break,continueはブロック先頭、脱出先のスタックを作る

独自拡張
数値リテラル　0b…
範囲演算子　.. a..b|a以上b未満

loop(int i=0..20){	//	ブロック内スコープの変数

}




a0 lvalue

a2 global pointer
a3 stack frame

memo:
AT&Tフォーマット opcode src,dist
*/


/*
 * A self-compiling compiler for a small subset of C.
 */

/* Our library functions. */

#if (_MSC_VER >= 600)

#include <stdio.h>
#include <malloc.h>

FILE *infile ;
FILE *outfile ;

char* outfilename ;

#else

void exit(int);
int getchar(void);
void *malloc(int);
int putchar(int);

#endif


void putstring(char *s);
void copystr_until_dot(char *s, char *d);
int nextc;


/* The first thing defined must be main(). */
int main1();

#if (_MSC_VER >= 600)
int main(int argc, char*argv[])
{
	if(argc<=1){
		putstring("Usage : ccq8 inputFileName [outputFileName]\x0a") ;
		exit(0) ;
	}
	if(argc>=2){
		infile=fopen(argv[1], "rb") ;
		if(infile==NULL){
			putstring("Can't open file[") ;
			putstring(argv[1]) ;
			putstring("]\x0a") ;
			exit(0) ;
		}
		outfilename=malloc(strlen(argv[1])+2) ;

		if(argc==3){
			copystr_until_dot(outfilename, argv[2]) ;
		}
		else{
			copystr_until_dot(outfilename, argv[1]) ;
		}
	}

	outfile=fopen(outfilename, "wb") ;
	if(outfile==NULL){
		fclose(infile) ;
		putstring("Can't open file[") ;
		putstring(outfilename) ;
		putstring("]\x0a") ;
		exit(0) ;
	}
	return main1();
}

void _getc(void){
	nextc = fgetc(infile);
}

#else
int main(){
	return main1();
}

void _getc(void){
	nextc = getchar();
}
#endif

char *my_realloc(char *old, int oldlen, int newlen)
{
	char *newptr = malloc(newlen);
	int i = 0;
	while (i < oldlen) {
		newptr[i] = old[i];
		i++;
	}
	return newptr;
}

/*
	intへのポインタを実装したら、これを使う
*/
/*
char* grow_memblock(char*ptr, int len, int require_len, int* new_len){

}
*/

char *token;
int token_size;
int line ;	//	エラー行表示用

int i;


void putstring(char *s){
	int i=0 ;
	while(s[i]){
		putchar(s[i]) ;
		i++ ;
	}
}

void putnumber(int n){
	int z=0 ;	//	先頭のゼロを抑制
	int p=10000 ;
	int w=0 ;

	if(n==0){
		putchar('0') ;
		return ;
	}

	if(n<0){
		return ;
	}

	while(1){
		w=n / p ;
		if(w!=0 | z==1){
			putchar('0'+w) ;
		}
		if(w!=0){
			z=1 ;
		}
		if(p==1){
			return ;
		}

		n=n-w*p ;
		p=p/10 ;
	}
}

void put_hex(int n){
	int h ;
	int c=0 ;

	putchar('$') ;
	while(1){
		h=(n & 0xf000) >> 12 ;
		if(h<10){
			putchar(h+'0') ;
		}
		else{
			putchar(h-10+'A') ;
		}
		n=n<<4 ;
		c++ ;
		if(c==4) return ;
	}
}

// no line feed
void asm_outnl(char *p){
	int i=0 ;
	while(p[i]){
		if(p[i]==';'){
			putchar(10) ;
			putchar(9) ;
		}
		else{
			putchar(p[i]) ;
		}
		i++ ;
	}
}

void asm_out(char *p){
	i=0 ;
	putchar(10) ;
	if(p[i]!=':' & p[i]!='#'){
		putchar(9) ;
	}

	asm_outnl(p) ;
}

void asm_outn(int n){
	putnumber(n) ;
}


void error(int n)
{
	putstring("Error(") ;
	putnumber(line) ;
	putstring(") #") ;
	putnumber(n) ;
	exit(1);
}

void copystr_until_dot(char *d, char *s){
	i=0 ;
	while(1){
		if(s[i]==0){
			return ;
		}
		if(s[i]!='.'){
			d[i]=s[i] ;
			i++ ;
			d[i]=0 ;
		}
		else{
			return ;
		}
	}
}


/*
	tokenへ1文字追加
	get_tokenからのみ
*/
void takechar()
{
	if (token_size <= i+1) {
		int x = (i + 10) << 1;
		token = my_realloc(token, token_size, x);
		token_size = x;
	}
	token[i] = nextc;
	i++;
	_getc();
}

/*
@@@オペレータの最長一致対応
*/
void get_token()
{
	int w = 1;
	while (w) {
		w = 0;
		while ((nextc == ' ') | (nextc == 9) | (nextc == 10) | (nextc == 13)){
			if(nextc == 10){	/* エラーメッセージ用行カウント */
				line++ ;
			}
			_getc();
		}
		i = 0;
		while ((('a' <= nextc) & (nextc <= 'z')) |(('A' <= nextc) & (nextc <= 'Z')) |
			(('0' <= nextc) & (nextc <= '9')) | (nextc == '_') | (nextc == '$')){
			takechar();
		}
		if (i == 0){	/* 2文字以上のオペレータはここで食う　Cに無い組み合わせも食う事に注意 */
			while ((nextc == '<') | (nextc == '=') | (nextc == '>') | (nextc == '|') |
				 (nextc == '&') | (nextc == '!') | (nextc == '+') | (nextc == '-')){
				takechar();
			}
		}
		if (i == 0) {	/* 文字列、コメント */
			if (nextc == 39) {
				takechar();
				while (nextc != 39){
					takechar();
				}
				takechar();
			}
			else if (nextc == '"') {
				takechar();
				while (nextc != '"'){
					takechar();
				}
				takechar();
			}
			else if (nextc == '/') {	/* operator / はここで取れる */
				takechar();
				if (nextc == '*') {
					_getc();
					while (nextc != '/') {
						while (nextc != '*'){
							if(nextc == 10){
								line++ ;
							}
							_getc();
						}
						_getc();
					}
					_getc();
					w = 1;	/* コメントは空のトークンになるのでcontinue */
				}
				else if (nextc == '/') {	//	//コメント
					_getc();
					while (nextc != 10){
						_getc();
					}
					line++ ;
					_getc();
					w = 1;	/* continue */
				}
			}
			else if (nextc != 0-1){	/* 一文字オペレータはここで取れる */
				takechar();
			}
		}
		token[i] = 0;	/* 終端 */
	}
}


/*

*/
int peek(char *s)
{
	int i = 0;
	while ((s[i] == token[i]) & (s[i] != 0)){
		i++;
	}
	return s[i] == token[i];
}


int accept(char *s)
{
	if (peek(s)) {
		get_token();
		return 1;
	}
	else{
		return 0;
	}
}


void expect(char *s)
{
	if (accept(s) == 0){
		error(1);
	}
}

char *code;
int code_size;
int codepos;
int code_offset;

void save_int(char *p, int n)
{
	p[0] = n;
	p[1] = n >> 8;
}

int load_int(char *p)
{
	return ((p[0] & 255) + ((p[1] & 255) << 8));
}


void emit(int n, char *s)
{
	i = 0;
	if (code_size <= codepos + n) {	//	grow
		int x = (codepos + n) << 1;
		code = my_realloc(code, code_size, x);
		code_size = x;
	}
	while (i < n) {
		code[codepos] = s[i];
		codepos++;
		i++;
	}
}

void be_push()
{
	emit(1, "\xb0"); /* push w0 */
	asm_out("push w0") ;
}

//	lvalue又は関数アドレスのpush
void be_push_addr()
{
	emit(1, "\xb4"); /* push a0 */
	asm_out("push a0") ;
}

/*
	ToDo
*/
void be_pop(int n)
{
	if(n!=0){
		emit(4, "\x81\xc4.."); /* add $(n * 4),%esp */
		asm_out("add sp, ") ;
		asm_outn(n*2) ;
		save_int(code + codepos - 2, n*2);
	}
}


/*
	シンボルテーブル

	table先頭からシンボル
		1バイト \0	文字列終端
		'U','D','A','L'のいづれか
		2バイト、値又は定義位置
		2バイト、32bitから移植のため空き。
		次のシンボル

	宣言済み未定義の関数呼び出しは、コード中の前の呼び出し位置をｔableに保存しておく。
	定義されたときにそれを遡ってアドレスを埋め込む。
*/
char *table;

int table_size;
int table_pos;
int stack_pos;

/*
	lookup
*/
int sym_lookup(char *s)
{
	int t = 0;
	int current_symbol = 0;
	while (t < table_pos) {
		i = 0;
		while ((s[i] == table[t]) & (s[i] != 0)) {	//	strcmp
			i++ ; t++ ;
		}
		if (s[i] == table[t]){	//	比較して一致するならそこをとりあえずマーク
			current_symbol = t;
		}
		while (table[t] != 0){
			t++;
		}
		t = t + 6;	//	ローカルはもっと後ろに定義されているので、引き続きサーチ
	}
	return current_symbol;
}

void sym_declare(char *s, int type, int value)
{
	int t = table_pos;
	i = 0;
	while (s[i] != 0) {
		if (table_size <= t + 10) {	//	growup
			int x = (t + 10) << 1;
			table = my_realloc(table, table_size, x);
			table_size = x;
		}
		table[t] = s[i];
		i++;t++;
	}
	table[t] = 0;
	t++ ;
	table[t] = type;
	t++ ;
	save_int(table + t, value);
	table_pos = t + 4;
}

int sym_declare_global(char *s)
{
	int current_symbol = sym_lookup(s);

	asm_out("# declare ") ;
	asm_outnl(s) ;
	if (current_symbol == 0) {
		sym_declare(s, 'U', code_offset);
		current_symbol = table_pos - 6;
	}
	return current_symbol;
}

void sym_define_global(int current_symbol)
{
	int i;
	int j;
	int t = current_symbol;
	int v = codepos + code_offset;
	if (table[t + 1] != 'U'){
		error(2); /* symbol redefined */
	}

	i = load_int(table + t + 2) - code_offset;

	/* 定義されたので、シンボル位置をコード中の参照位置に埋め込む */
	while (i) {
		j = load_int(code + i) - code_offset;
		save_int(code + i, v);
		i = j;
	}
	table[t + 1] = 'D';
	save_int(table + t + 2, v);
}

int number_of_args;

/*
	シンボルへの参照をa0に入れる
	関数の場合
		D(定義済み)なら
		U(未定義、宣言済み)なら
*/
void sym_get_value(char *s)
{
	int t;
	int n;

	if ((t = sym_lookup(s)) == 0){
		error(3);	//	symbol not defined
	}

	if (table[t + 1] == 'D' | table[t + 1] == 'U') {
		emit(3, "\xb8.."); /* mov $n,%eax */
		n=load_int(table + t + 2) ;
		asm_out("mov a0,") ;
		save_int(code + codepos - 2, n);
	}

	if (table[t + 1] == 'D') { /* defined global */
		asm_outn(n) ;
	}
	else if (table[t + 1] == 'U') { /* undefined global */
		//
		save_int(table + t + 2, codepos + code_offset - 2);
		asm_outn(codepos + code_offset - 2) ;
	}
	else if (table[t + 1] == 'L') { /* local variable */
		int k = (stack_pos - table[t + 2] - 1) << 1;
		emit(5, "\x8d\x84\x24.."); /* lea (n * 4)(%esp),%eax */
		save_int(code + codepos - 2, k);
		asm_out("lea a0, sp+") ;
		asm_outn(k) ;
	}
	else if (table[t + 1] == 'A') { /* argument */
		int k = (stack_pos + number_of_args - table[t + 2] + 1) << 1;
		emit(5, "\x8d\x84\x24.."); /* lea (n * 4)(%esp),%eax */
		save_int(code + codepos - 2, k);
		asm_out("lea a0, sp+") ;
		asm_outn(k) ;
	}
	else{
		error(4);	//	internal error
	}
}


void be_start()
{
//	実行ファイルヘッダ
	emit(16, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
	emit(16, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
//	必要なファンクションリクエスト
	sym_define_global(sym_declare_global("exit"));
	emit(16, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
	sym_define_global(sym_declare_global("getchar"));
	emit(16, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
	sym_define_global(sym_declare_global("malloc"));
	emit(16, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
	sym_define_global(sym_declare_global("putchar"));
	emit(16, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
//	ここからmain

//	push returnaddrして、main最後のretでexitできるように。
	asm_out("mov a0,pc;add a0,5;push a0;jmp.s main") ;
	asm_out("call exit") ;
	asm_out(":main") ;
}

void be_finish()
{
	i = 0;
	while (i <= codepos - 1) {
		//	putchar(code[i]);
#if (_MSC_VER >= 600)
		fputc(code[i], outfile) ;
#else
		putchar(code[i]) ;
#endif
		i++;
	}
}

void promote(int type)
{
	/* 1 = char lval, 2 = int lval, 3 = other */
	if (type == 1){
		emit(3, "\x0f\xbe\x00"); /* movsbl (%eax),%eax */
		asm_out("ld b0,[a0]") ;
		asm_out("sxt w0") ;
		/* charのデフォルトをunsignedにしたので、0拡張に変更 -> だめだったので再度調査してから */
/*		emit(3, "\x0f\xb3\x00"); /* movzbl (%eax),%eax */
	}
	else if (type == 2){
		emit(2, "\x8b\x00"); /* mov (%eax),%eax */
		asm_out("ld w0,[a0]") ;
	}
}

int expression();

/*
　以下、再帰下降
	primary-exprが一番底になるので、ここでeaxに値(参照)を入れる。

*/

int imm ;	/* イミディエート値 */
int stringn ;
int label ;

/*
 * primary-expr:
 *     identifier
 *     constant
 *     ( expression )
 */
int primary_expr()
{
	int type;

	if (('0' <= token[0]) & (token[0] <= '9')) {
		int n = 0;
		int base=10 ;
		i = 0;

		while (token[i]) {
			n = n*10 + token[i] - '0';
			i++;
		}
		emit(3, "\xb8.."); /* mov $x,%eax */
		save_int(code + codepos - 2, n);
		asm_out("movi w0,") ;
		asm_outn(n) ;
		type = 3;
	}
	else if ((('a' <= token[0]) & (token[0] <= 'z')) | (('A' <= token[0]) & (token[0] <= 'Z')) |
		(token[0] == '_') | (token[0] == '$')){
		sym_get_value(token);
		type = 2;
	}
	else if (accept("(")) {
		type = expression();
		if (peek(")") == 0){
			error(5);
		}
	}
	else if ((token[0] == 39) & (token[1] != 0) & (token[2] == 39) & (token[3] == 0)) {
		emit(5, "\xb8.."); /* mov $x,%eax */
		save_int(code + codepos - 2, token[1]);
		asm_out("movi w0,") ;
		asm_outn(token[1]) ;
		type = 3;
	}
	else if (token[0] == '"') {
		int i = 0;
		int j = 1;
		int k;
		while (token[j] != '"') {
			if ((token[j] == 92) & (token[j + 1] == 'x')) {	/* back slash+x */
				if (token[j + 2] <= '9'){
					k = token[j + 2] - '0';
				}
				else{
					k = token[j + 2] - 'a' + 10;
				}

				k = k << 4;

				if (token[j + 3] <= '9'){
					k = k + token[j + 3] - '0';
				}
				else{
					k = k + token[j + 3] - 'a' + 10;
				}
				token[i] = k;
				j = j + 4;
			}
			else {
				token[i] = token[j];	/* 必ず短くなるので、バッファはtokenを使ってOK */
				j++;
			}
			i++;
		}
		token[i] = 0;
		/* call ... ; the string ; pop %eax */
		emit(3, "\xe8..");
		save_int(code + codepos - 2, i + 1); /* pop eaxの位置をcallしてip(stringの先頭)->eax */
		emit(i + 1, token);
		asm_out("lea a0,String") ;
		asm_outn(stringn) ;
		stringn++ ;
		emit(1, "\x58");
		type = 3;
	}
	else{
		error(6);
	}
	get_token();
	return type;
}

void binary1(int type)
{
	promote(type);
	be_push();
	stack_pos++;
}

int binary2(int type, int n, char *s)
{
	promote(type);
	emit(n, s);
	stack_pos--;
	return 3;
}


/*
 * postfix-expr:
 *         primary-expr
 *         postfix-expr [ expression ]
 *         postfix-expr ( expression-list-opt )
 */
int postfix_expr()
{
	int type = primary_expr();
	if (accept("[")) {
		binary1(type); /* pop %ebx ; add %ebx,%eax */
		binary2(expression(), 3, "\x5b\x01\xd8");
		asm_out("pop w1;add w0,w1") ;
		expect("]");
		type = 1;
	}
	else if (accept("(")) {
		int s = stack_pos;
		be_push_addr();	//	直前のグローバルシンボルの参照（関数のアドレス）がa0に入っているので積む
		stack_pos++;
		if (accept(")") == 0) {
			promote(expression());
			be_push();
			stack_pos++;
			while (accept(",")) {
				promote(expression());
				be_push();
				stack_pos++;
			}
			expect(")");
		}
		emit(7, "\x8b\x84\x24.."); /* mov (n * 4)(%esp),%eax */
		save_int(code + codepos - 2, (stack_pos - s - 1) << 1);
		asm_out("ld a0, [sp+") ;
		asm_outn( (stack_pos - s - 1) << 1) ;	//	primary_exprで関数名＝グローバルシンボルが積まれているアドレス
		asm_outnl("]") ;

		emit(2, "\xff\xd0"); /* call *%eax */
		asm_out("call a0") ;
		be_pop(stack_pos - s);
		stack_pos = s;
		type = 3;
	}
	return type;
}


/*
 * post-increment-expr:
 *         postfix-expr
 *         post-increment-expr ++
 *         post-increment-expr --
 */
int post_increment_expr()
{
	int type = postfix_expr();
	if (accept("++")) {
		if (type == 1){
			emit(2, "\xfe\x00") ;	/* inc byte ptr(%eax) */
			asm_out("ld b0,[a0];inc b0;st [a0],b0") ;
		}
		else if (type == 2){
			emit(2, "\xff\x00") ;	/* inc dword ptr(%eax) */
			asm_out("ld w0,[a0];inc w0;st [a0],w0") ;
		}
	}
	else if (accept("--")) {
		if (type == 1){
			emit(2, "\xfe\x08") ;	/* dec byte ptr(%eax) */
			asm_out("ld b0,[a0];dec b0;st [a0],b0") ;
		}
		else if (type == 2){
			emit(2, "\xff\x08") ;	/* dec dword ptr(%eax) */
			asm_out("ld w0,[a0];dec w0;st [a0],w0") ;
		}
	}
	return type;
}


/*
 * multiplication-expr:
 *         increment-expr
 *         multiplication-expr * post-increment-expr
 *         multiplication-expr % post-increment-expr
 *         multiplication-expr / post-increment-expr
 */
int multiplication_expr()
{
	int type = post_increment_expr();
	while (1) {
		if (accept("%")) {
			binary1(type); /* pop %ebx ; xor %edx,%edx ; xchg %eax,%ebx ; idiv %ebx ; mov %edx,%eax */
			type = binary2(post_increment_expr(), 9, "\x5b\x31\xd2\x87\xc3\xf7\xfb\x89\xd0");
			asm_out("pop w1;div w0;") ;
		}
		else if (accept("/")) {
			binary1(type); /* pop %ebx ; xor %edx,%edx ; xchg %eax,%ebx ; idiv %ebx */
			type = binary2(post_increment_expr(), 7, "\x5b\x31\xd2\x87\xc3\xf7\xfb");
			asm_out("pop w1;div w0;mov w0,w1") ;
		}
		else if (accept("*")) {
			binary1(type); /* pop %ebx ; imul %ebx */
			type = binary2(post_increment_expr(), 3, "\x5b\xf7\xeb");
			asm_out("pop w1;mul w0") ;
		}
		else{
			return type;
		}
	}
}


/*
 * additive-expr:
 *         multiplication-expr
 *         additive-expr + multiplication-expr
 *         additive-expr - multiplication-expr
 */
int additive_expr()
{
	int type = multiplication_expr();
	while (1) {
		if (accept("+")) {
			binary1(type); /* pop %ebx ; add %ebx,%eax */
			type = binary2(multiplication_expr(), 3, "\x5b\x01\xd8");
			asm_out("pop w1;add w0,w1") ;
		}
		else if (accept("-")) {
			binary1(type); /* pop %ebx ; sub %eax,%ebx ; mov %ebx,%eax */
			type = binary2(multiplication_expr(), 5, "\x5b\x29\xc3\x89\xd8");
			asm_out("pop w1;sub w1,w0;mov w0,w1") ;
		}
		else{
			return type;
		}
	}
}

/*
 * shift-expr:
 *         additive-expr
 *         shift-expr << additive-expr
 *         shift-expr >> additive-expr
 */
int shift_expr()
{
	int type = additive_expr();
	while (1) {
		if (accept("<<")) {
			binary1(type); /* mov %eax,%ecx ; pop %eax ; shl %cl,%eax */
			type = binary2(additive_expr(), 5, "\x89\xc1\x58\xd3\xe0");
			asm_out("pop w1;sal w1,b0;mov w0,w1") ;
		}
		else if (accept(">>")) {
			binary1(type); /* mov %eax,%ecx ; pop %eax ; sar %cl,%eax */
			type = binary2(additive_expr(), 5, "\x89\xc1\x58\xd3\xf8");
			asm_out("pop w1;sar w1,b0;mov w0,w1") ;
		}
		else{
			return type;
		}
	}
}

/*
 * relational-expr:
 *         shift-expr
 *         relational-expr <= shift-expr
 */
int relational_expr()
{
	int type = shift_expr();
	while(1){
		if(accept("<=")) {
			binary1(type);
			/* pop %ebx ; cmp %eax,%ebx ; setle %al ; movzbl %al,%eax */
			type = binary2(shift_expr(), 9, "\x5b\x39\xc3\x0f\x9e\xc0\x0f\xb6\xc0");
			asm_out("pop w1;cmp w1,w0;setle w0") ;
		}
		else if(accept("<")){
			binary1(type);
			/* pop %ebx ; cmp %eax,%ebx ; setl %al ; movzbl %al,%eax */
			type = binary2(shift_expr(), 9, "\x5b\x39\xc3\x0f\x9c\xc0\x0f\xb6\xc0");
			asm_out("pop w1;cmp w1,w0;setl w0") ;
		}
		else{
			return type;
		}
	}
}

/*
 * equality-expr:
 *         relational-expr
 *         equality-expr == relational-expr
 *         equality-expr != relational-expr
 */
int equality_expr()
{
	int type = relational_expr();
	while (1) {
		if (accept("==")) {
			binary1(type);
			/* pop %ebx ; cmp %eax,%ebx ; sete %al ; movzbl %al,%eax */
			type = binary2(relational_expr(), 9, "\x5b\x39\xc3\x0f\x94\xc0\x0f\xb6\xc0");
			asm_out("pop w1;cmp w1,w0;sete w0") ;
		}
		else if (accept("!=")) {
			binary1(type);
			/* pop %ebx ; cmp %eax,%ebx ; setne %al ; movzbl %al,%eax */
			type = binary2(relational_expr(), 9, "\x5b\x39\xc3\x0f\x95\xc0\x0f\xb6\xc0");
			asm_out("pop w1;cmp w1,w0;setne w0") ;
		}
		else{
			return type;
		}
	}
}

/*
 * bitwise-and-expr:
 *         equality-expr
 *         bitwise-and-expr & equality-expr
 */
int bitwise_and_expr()
{
	int type = equality_expr();
	while (accept("&")) {
		binary1(type); /* pop %ebx ; and %ebx,%eax */
		type = binary2(equality_expr(), 3, "\x5b\x21\xd8");
		asm_out("pop w1;and w0,w1") ;
	}
	return type;
}

/*
 * bitwise-or-expr:
 *         bitwise-and-expr
 *         bitwise-and-expr | bitwise-or-expr
 */
int bitwise_or_expr()
{
	int type = bitwise_and_expr();
	while (accept("|")) {
		binary1(type); /* pop %ebx ; or %ebx,%eax */
		type = binary2(bitwise_and_expr(), 3, "\x5b\x09\xd8");
		asm_out("pop w1;or w0,w1") ;
	}
	return type;
}


/*
	式の評価はここが開始
*/

int labelno ;

/*
 * expression:
 *         bitwise-or-expr
 *         bitwise-or-expr = expression
 */
int expression()
{
	int type = bitwise_or_expr();
	if (accept("=")) {
		be_push_addr();	//	lvalueのまま
		stack_pos++ ;
		promote(expression());
		if(type==1){
			emit(3, "\x5b\x88\x03"); /* pop %ebx ; mov %al,(%ebx) */
			asm_out("pop a0;st [a0],b0;ext w0") ;
		}
		else if(type==2){
			emit(3, "\x5b\x89\x03"); /* pop %ebx ; mov %eax,(%ebx) */
			asm_out("pop a0;st [a0],w0") ;
		}
		else{
			error(7) ;	/* lvalue以外への代入 */
		}
		stack_pos-- ;
		type = 3;
	}
	return type;
}

/*
 * type-name:
 *     char *
 *     int
 */
void type_name()
{
	get_token();
	while (accept("*")) {
	}
}

/*
 * statement:
 *     { statement-list-opt }
 *     type-name identifier ;
 *     type-name identifier = expression;
 *     if ( expression ) statement
 *     if ( expression ) statement else statement
 *     while ( expression ) statement
 *     return ;
 *     expr ;
 */
void statement()
{
	int p1;
	int p2;
	int llbl ;

	if (accept("{")) {
		int n = table_pos;
		int s = stack_pos;
		while (accept("}") == 0){
			statement();
		}
		table_pos = n;
		be_pop(stack_pos - s);
		stack_pos = s;
	}
	else if (peek("char") | peek("int")) {
		type_name();
		sym_declare(token, 'L', stack_pos);
		get_token();
		if (accept("=")){
			promote(expression());
		}
		expect(";");
		be_push();
		stack_pos++;
	}
	else if (accept("if")) {
		llbl=label;
		label++ ;

		expect("(");
		promote(expression());
		emit(6, "\x85\xc0\x0f\x84.."); /* test %eax,%eax ; je ... */
		p1 = codepos;
		asm_out("jrz w0,label_if_") ;
		asm_outn(llbl) ;

		expect(")");
		statement();

		emit(3, "\xe9.."); /* jmp ... */
		asm_out("jmp label_e_") ;
		asm_outn(llbl) ;

		p2 = codepos;
		save_int(code + p1 - 2, codepos - p1);

		asm_out(":label_if_") ;
		asm_outn(llbl) ;


		if (accept("else")){
			statement();
		}

		if(p2==codepos){	/* elseが無かったらしい */
			codepos=codepos-3 ;
			save_int(code + p1 - 2, codepos - p1);
		}
		else{
			save_int(code + p2 - 2, codepos - p2);
		}
		asm_out(":label_e_") ;
		asm_outn(llbl) ;
	}
	else if (accept("while")) {
		llbl=label;
		label++ ;

		expect("(");
		p1 = codepos;
		asm_out(":label_w_") ;
		asm_outn(llbl) ;

		promote(expression());

		emit(6, "\x85\xc0\x0f\x84.."); /* test %eax,%eax ; je ... */
		asm_out("jrz w0,label_w_e") ;
		asm_outn(llbl) ;

		p2 = codepos;
		expect(")");

		statement();
		emit(3, "\xe9.."); /* jmp ... */
		asm_out("jmp label_w_") ;
		asm_outn(llbl) ;
		asm_out(":label_w_e") ;
		asm_outn(llbl) ;

		save_int(code + codepos - 2, p1 - codepos);
		save_int(code + p2 - 2, codepos - p2);
	}
	else if (accept("return")) {
		if (peek(";") == 0){
			promote(expression());
		}
		expect(";");
		be_pop(stack_pos);
		emit(1, "\xc3"); /* ret */
		asm_out("ret") ;
	}
	else {
		expression();
		expect(";");
	}
}

/*
 * program:
 *     declaration
 *     declaration program
 *
 * declaration:
 *     type-name identifier ;
 *     type-name identifier ( parameter-list ) ;
 *     type-name identifier ( parameter-list ) statement
 *
 * parameter-list:
 *     parameter-declaration
 *     parameter-list, parameter-declaration
 *
 * parameter-declaration:
 *     type-name identifier-opt
 */
void program()
{
	int current_symbol;
	while (token[0]) {
		type_name();
		current_symbol = sym_declare_global(token);
		get_token();
		if (accept(";")) {	//	変数定義
			sym_define_global(current_symbol);
			emit(2, "\x00\x00");	/* globalは常に０に初期化 */
		}
		else if (accept("(")) {	//	関数
			int n = table_pos;
			number_of_args = 0;
			while (accept(")") == 0) {
				number_of_args++ ;
				type_name();
				if (peek(")") == 0) {
					sym_declare(token, 'A', number_of_args);
					get_token();
				}
				accept(","); /* ignore trailing comma */
			}
			if (accept(";") == 0) {	//	関数定義
				sym_define_global(current_symbol);
				statement();
				emit(1, "\xc3"); /* ret */
				asm_out("ret") ;
			}
			table_pos = n;
		}
		else{
			error(8);
		}
	}
}

int main1()
{
	code_offset = 64;
	be_start();
	_getc();
	get_token();
	program();
	be_finish();
	return 0;
}
