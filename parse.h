
#ifndef  __PARSE__H
#define  __PARSE__H
#include <stdio.h>
typedef struct {
	int  len;
	int diff;
	char *mod_fn ;
	FILE *fp_mod ;
	FILE *fp_out ;
	FILE *fp_sam ;
	char *str_sam ;
	char *out_put ;
}opt_t;

opt_t *parse_argv(int argc , char *argv[]);
void free_opt(opt_t *o);


#endif
