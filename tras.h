#ifndef __TRS_H
#define __TRS_H
#include "sv.h"

typedef struct {
	int  is_bp;
	int  is_no_pos ;
	int  n ;
	int  sv_pos ;
	int  pos[2];
} result_t ;

void tras2result(int pos[2] , result_t ret[2] , int len ,sv_t *sv );

#endif
