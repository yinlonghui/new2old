#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sv.h"
#include "sam.h"
#include "tras.h"
#include "check.h"
#include "parse.h"


int main( int argc , char *argv[])
{
	opt_t *opt ;
	sv_t   *sv ;
	sam_t  *tmp = NULL ;
	stat_t *stat = NULL ; 
	pos_t  *p[2];
	result_t ret[2];
	opt =  parse_argv(argc,argv);
	if(opt == NULL) return 1 ;

	open_sv(opt , &sv);
	open_sam(opt);
	p[0] =  init_pos_t();
	p[1] =  init_pos_t();
	stat = init_stat();

	while(1){
		sam_t *s = read_sam(opt);
		if(!s) break;
		if( tmp == NULL){
			tmp = s ;
			push(p,tmp->flag ,tmp->map_pos);
			tras2result(tmp->pos,ret,opt->len,sv);
		}else if( !strcmp(tmp->name,s->name)){
			free(tmp);
			ACC_SAM(stat);
			tmp = s ;
			push(p,tmp->flag ,tmp->map_pos);
		}else{
			free(tmp);
			tmp = s ;
			check_pos(stat,opt->diff,p,ret,sv);
			ACC_READ(stat);
			ACC_SAM(stat);
			ACC_BP_NO_POS(ret,stat);
			ACC_BP(ret,stat);
			tras2result(tmp->pos,ret,opt->len,sv);
			reset(p);
			push(p,tmp->flag ,tmp->map_pos);
		}
	}
	check_pos(stat,opt->diff,p,ret,sv);
	print_stat(stat);

	//   free all various 
	free(stat); 
	free(tmp);
	free_pos(p);
	free_sv(opt , sv );
	free_opt(opt);
	return  0 ;
}

