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

msc6でコンパイル


・済み

Identifyerの大文字、_から始まるもの
改行CR
/ * %
比較　<
後置++ --
//コメント


・ToDo
int,char*の16ビット化
QV8コード出力

エラーメッセージとエラー行
<= < 以外の比較
0xffff
globalの初期化

mainの引数

オペレータの最長一致
charをunsignedにする
int char*以外の型(特にshort,longとそれへのポインタ)
	あと、main()の引数に使うchar**
数値リテラルオプティマイズ

bit-xor

前置演算子
++ --
- !

int *

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
範囲演算子　..

loop(i=0..20){

}

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
	if(argc==2){
		infile=fopen(argv[1], "rb") ;
		if(infile==NULL){
			putstring("Can't open file[") ;
			putstring(argv[1]) ;
			putstring("]\x0a") ;
			exit(0) ;
		}
	}
	if(argc!=3){
		outfilename=malloc(strlen(argv[1])+2) ;
		copystr_until_dot(outfilename, argv[1]) ;
		outfile=fopen(outfilename, "wb") ;
		if(outfile==NULL){
			fclose(infile) ;
			putstring("Can't open file[") ;
			putstring(outfilename) ;
			putstring("]\x0a") ;
			exit(0) ;
		}
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
	while (i <= oldlen - 1) {
		newptr[i] = old[i];
		i++;
	}
	return newptr;
}

char *token;
int token_size;
int line ;	//	エラー行表示用

int i;

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

//	0xffffffff=4294967295
void putnumber(unsigned int n){
	unsigned int z=0 ;	//	先頭のゼロを抑制
	unsigned int p=10000 ;
	unsigned int w=0 ;

	if(n==0){
		putchar('0') ;
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


void putstring(char *s){
	int i=0 ;
	while(s[i]){
		putchar(s[i]) ;
		i++ ;
	}
}


void asm_out(char *p){
	putchar(10) ;
	putstring(p) ;
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
//	p[2] = n >> 16;
//	p[3] = n >> 24;
}

int load_int(char *p)
{
	int k ;
//	return ((p[0] & 255) + ((p[1] & 255) << 8) + ((p[2] & 255) << 16) + ((p[3] & 255) << 24));
	k=((p[0] & 255) + ((p[1] & 255) << 8));
//	printf("(%d)", k) ;

	return k;
}


void emit(int n, char *s)
{
	i = 0;
	if (code_size <= codepos + n) {
		int x = (codepos + n) << 1;
		code = my_realloc(code, code_size, x);
		code_size = x;
	}
	while (i <= n - 1) {
		code[codepos] = s[i];
		codepos++;
		i++;
	}
}

void be_push()
{
	emit(1, "\x50"); /* push w0 */
	asm_out("push w0") ;
}

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
*/
char *table;
int table_size;
int table_pos;
int stack_pos;

int sym_lookup(char *s)
{
	int t = 0;
	int current_symbol = 0;
	while (t <= table_pos - 1) {
		i = 0;
		while ((s[i] == table[t]) & (s[i] != 0)) {
			i++ ; t++ ;
		}
		if (s[i] == table[t]){
			current_symbol = t;
		}
		while (table[t] != 0){
			t++;
		}
		t = t + 6;
	}
	return current_symbol;
}

void sym_declare(char *s, int type, int value)
{
	int t = table_pos;
	i = 0;
	while (s[i] != 0) {
		if (table_size <= t + 10) {
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
	シンボルへの参照をeaxに入れる
*/
void sym_get_value(char *s)
{
	int t;
	int n;

	if ((t = sym_lookup(s)) == 0){
		error(3);	//	symbol not defined
	}

	emit(3, "\xb8.."); /* mov $n,%eax */
	n=load_int(table + t + 2) ;
	asm_out("mov w0, ") ;
	asm_outn(n) ;
	save_int(code + codepos - 2, n);

	if (table[t + 1] == 'D') { /* defined global */
	}
	else if (table[t + 1] == 'U') { /* undefined global */
		save_int(table + t + 2, codepos + code_offset - 2);
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
		error(4);
	}
}


void be_start()
{
	emit(16, "\x7f\x45\x4c\x46\x01\x01\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00");
	sym_define_global(sym_declare_global("exit"));
	emit(16, "\x00\x00\x00\x00\x00\x00\x00\x00\x34\x00\x20\x00\x01\x00\x00\x00");
	sym_define_global(sym_declare_global("getchar"));
	emit(16, "\x00\x00\x00\x00\x00\x00\x00\x00\x34\x00\x20\x00\x01\x00\x00\x00");
	sym_define_global(sym_declare_global("malloc"));
	emit(16, "\x00\x00\x00\x00\x00\x00\x00\x00\x34\x00\x20\x00\x01\x00\x00\x00");
	sym_define_global(sym_declare_global("putchar"));
	emit(16, "\x00\x00\x00\x00\x00\x00\x00\x00\x34\x00\x20\x00\x01\x00\x00\x00");
}

void be_finish()
{
	i = 0;
	while (i <= codepos - 1) {
		//	putchar(code[i]);
		fputc(code[i], outfile) ;
		i++;
	}
}

void promote(int type)
{
	/* 1 = char lval, 2 = int lval, 3 = other */
	if (type == 1){
		emit(3, "\x0f\xbe\x00"); /* movsbl (%eax),%eax */
		/* charのデフォルトをunsignedにしたので、0拡張に変更 -> だめだったので再度調査してから */
/*		emit(3, "\x0f\xb3\x00"); /* movzbl (%eax),%eax */
	}
	else if (type == 2){
		emit(2, "\x8b\x00"); /* mov (%eax),%eax */
	}
}

int expression();

/*
　以下、再帰下降
	primary-exprが一番底になるので、ここでeaxに値(参照)を入れる。

*/

int imm ;	/* イミディエート値 */

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
			i = i + 1;
		}
		emit(5, "\xb8...."); /* mov $x,%eax */
		save_int(code + codepos - 4, n);
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
		emit(5, "\xb8...."); /* mov $x,%eax */
		save_int(code + codepos - 4, token[1]);
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
				token[i] = token[j];	/* 必ず短くなるので、バッファはtokenを使う */
				j++;
			}
			i++;
		}
		token[i] = 0;
		/* call ... ; the string ; pop %eax */
		emit(5, "\xe8....");
		save_int(code + codepos - 4, i + 1); /* pop eaxの位置をcallしてip(stringの先頭)->eax */
		emit(i + 1, token);
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
	stack_pos = stack_pos + 1;
}

int binary2(int type, int n, char *s)
{
	promote(type);
	emit(n, s);
	stack_pos = stack_pos - 1;
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
		expect("]");
		type = 1;
	}
	else if (accept("(")) {
		int s = stack_pos;
		be_push();
		stack_pos = stack_pos + 1;
		if (accept(")") == 0) {
			promote(expression());
			be_push();
			stack_pos = stack_pos + 1;
			while (accept(",")) {
				promote(expression());
				be_push();
				stack_pos = stack_pos + 1;
			}
			expect(")");
		}
		emit(7, "\x8b\x84\x24...."); /* mov (n * 4)(%esp),%eax */
		save_int(code + codepos - 4, (stack_pos - s - 1) << 2);
		emit(2, "\xff\xd0"); /* call *%eax */
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
		}
		else if (type == 2){
			emit(2, "\xff\x00") ;	/* inc dword ptr(%eax) */
		}
	}
	else if (accept("--")) {
		if (type == 1){
			emit(2, "\xfe\x08") ;	/* dec byte ptr(%eax) */
		}
		else if (type == 2){
			emit(2, "\xff\x08") ;	/* dec dword ptr(%eax) */
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
		}
		else if (accept("/")) {
			binary1(type); /* pop %ebx ; xor %edx,%edx ; xchg %eax,%ebx ; idiv %ebx */
			type = binary2(post_increment_expr(), 7, "\x5b\x31\xd2\x87\xc3\xf7\xfb");
		}
		else if (accept("*")) {
			binary1(type); /* pop %ebx ; imul %ebx */
			type = binary2(post_increment_expr(), 3, "\x5b\xf7\xeb");
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
		}
		else if (accept("-")) {
			binary1(type); /* pop %ebx ; sub %eax,%ebx ; mov %ebx,%eax */
			type = binary2(multiplication_expr(), 5, "\x5b\x29\xc3\x89\xd8");
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
		}
		else if (accept(">>")) {
			binary1(type); /* mov %eax,%ecx ; pop %eax ; sar %cl,%eax */
			type = binary2(additive_expr(), 5, "\x89\xc1\x58\xd3\xf8");
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
		}
		else if(accept("<")){
			binary1(type);
			/* pop %ebx ; cmp %eax,%ebx ; setl %al ; movzbl %al,%eax */
			type = binary2(shift_expr(), 9, "\x5b\x39\xc3\x0f\x9c\xc0\x0f\xb6\xc0");
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
		}
		else if (accept("!=")) {
			binary1(type);
			/* pop %ebx ; cmp %eax,%ebx ; setne %al ; movzbl %al,%eax */
			type = binary2(relational_expr(), 9, "\x5b\x39\xc3\x0f\x95\xc0\x0f\xb6\xc0");
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
	}
	return type;
}


/*
	式の評価はここが開始
*/

/*
 * expression:
 *         bitwise-or-expr
 *         bitwise-or-expr = expression
 */
int expression()
{
	int type = bitwise_or_expr();
	if (accept("=")) {
		be_push();
		stack_pos++ ;
		promote(expression());
		if(type==1){
			emit(3, "\x5b\x88\x03"); /* pop %ebx ; mov %al,(%ebx) */
		}
		else if(type==2){
			emit(3, "\x5b\x89\x03"); /* pop %ebx ; mov %eax,(%ebx) */
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
		stack_pos = stack_pos + 1;
	}
	else if (accept("if")) {
		expect("(");
		promote(expression());
		emit(8, "\x85\xc0\x0f\x84...."); /* test %eax,%eax ; je ... */
		p1 = codepos;
		expect(")");
		statement();
		emit(5, "\xe9...."); /* jmp ... */
		p2 = codepos;
		save_int(code + p1 - 4, codepos - p1);
		if (accept("else")){
			statement();
		}

		if(p2==codepos){	/* elseが無かったらしい */
			codepos=codepos-5 ;
			save_int(code + p1 - 4, codepos - p1);
		}
		else{
			save_int(code + p2 - 4, codepos - p2);
		}
	}
	else if (accept("while")) {
		expect("(");
		p1 = codepos;
		promote(expression());
		emit(8, "\x85\xc0\x0f\x84...."); /* test %eax,%eax ; je ... */
		p2 = codepos;
		expect(")");
		statement();
		emit(5, "\xe9...."); /* jmp ... */
		save_int(code + codepos - 4, p1 - codepos);
		save_int(code + p2 - 4, codepos - p2);
	}
	else if (accept("return")) {
		if (peek(";") == 0){
			promote(expression());
		}
		expect(";");
		be_pop(stack_pos);
		emit(1, "\xc3"); /* ret */
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
		if (accept(";")) {
			sym_define_global(current_symbol);
			emit(4, "\x00\x00\x00\x00");	/* globalは常に０に初期化 */
		}
		else if (accept("(")) {
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
			if (accept(";") == 0) {
				sym_define_global(current_symbol);
				statement();
				emit(1, "\xc3"); /* ret */
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
	code_offset = 256;
	be_start();
	_getc();
	get_token();
	program();
	be_finish();
	return 0;
}
