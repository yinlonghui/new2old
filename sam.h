
#ifndef __SAM__H
#define  __SAM__H
#include "parse.h"
typedef struct {
	int     pos[2] ,map_pos ,flag ;
	char   name[1024];
} sam_t ;
int open_sam(opt_t* o);
sam_t  *read_sam(opt_t *o);

#endif
