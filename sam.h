
#ifndef __SAM__H
#define  __SAM__H
#include "parse.h"

/* sam header */
typedef struct {
	char   *buffer ;
	size_t  len ;
} head_t;

typedef struct {
	int     pos[2] ,map_pos ,flag ;
	char   name[1024];
	char   *buffer;
	head_t  *ht ;
	int     n ;
	size_t  len ;
	char   cigar[512];
} sam_t ;

int open_sam(opt_t* o);
/*  catch sam info , return sam structrue
 *  @simu  > 0  , return DNAA pos  , == 0 , return NULL  
 */
sam_t  *read_sam(opt_t *o ,int simu);

#endif
