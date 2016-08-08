void putnumber(int);
void putmessage(void);
int main(){
	int i;
	i=20 ;
	i++ ;
	putmessage() ;
	putnumber(i*200) ;
	putchar(10) ;
	putnumber(32767) ;
	return 0 ;
}

char *Msg ;
int _i ;

void putmessage(void){
	int w=0 ;
	_i=0 ;
	Msg="error message." ;

	while(1){
		if(Msg[w]==0){
			return ;
		}
		putchar(Msg[w]) ;
		w=w+1 ;
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

