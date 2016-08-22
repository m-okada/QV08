typedef struct foo{
	int var0 ;
	int var1 ;
} Foo ;

const Foo _foo[]={
	{
		.var0=10,
		.var1=203
	}
} ;

void main(void){
	int i=0 ;
	if(1){
		i++ ;
		i+=2,i=0;
	};
	exit(0) ;
}