#ifndef __SV__H
#define __SV__H
#include "parse.h"

typedef struct{
	char type[16];
	int pos ;
	int len ;
	int order_num;
	int tandem;
	int strand;
} ss_v ;

typedef struct{
	int  m , n ;
	ss_v *a;
} sv_t ;

int open_sv(char *mod , sv_t   **sv);
int free_sv(sv_t  *sv);



#endif 
