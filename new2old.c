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
	sv_t   *sv , *s_old;
	sam_t  *tmp = NULL ;
	stat_t *stat = NULL ; 
	pos_t  *p[2];
	result_t ret[2];
	opt =  parse_argv(argc,argv);
	if(opt == NULL) return 1 ;

	open_sv(opt->mod_fn[0] , &sv);
	open_sv(opt->mod_fn[1] , &s_old);
	open_sam(opt);
	p[0] =  init_pos_t();
	p[1] =  init_pos_t();
	stat = init_stat();

	while(1){
		sam_t *s = read_sam(opt,1);
		if(!s) break;
		if( tmp == NULL){
			tmp = s ;
			int i ;
			for( i =  0 ; i < s->n ; i++)
				free(s->ht[i].buffer);
			free(s->ht);
			push(p,tmp->flag ,tmp->map_pos,tmp->cigar);
			tras2result(tmp->pos,ret,opt->len,sv);
		}else if( !strcmp(tmp->name,s->name)){
			free(tmp->buffer);
			free(tmp);
			ACC_SAM(stat);
			tmp = s ;
			push(p,tmp->flag ,tmp->map_pos, tmp->cigar);
		}else{
			check_pos(stat,opt->diff,p,ret,sv,s_old,opt,tmp);
			findSSeq2sam(p,opt,tmp);
			free(tmp->buffer);
			free(tmp);
			tmp = s ;
			ACC_READ(stat);
			ACC_SAM(stat);
			ACC_BP_NO_POS(ret,stat);
			ACC_BP(ret,stat);
			tras2result(tmp->pos,ret,opt->len,sv);
			reset(p);
			push(p,tmp->flag ,tmp->map_pos , tmp->cigar);
		}
	}
	check_pos(stat,opt->diff,p,ret,sv,s_old ,opt,tmp);
	findSSeq2sam(p,opt,tmp);
	print_stat(stat);

	//   free all various 
	free(stat); 
	free(tmp->buffer);
	free(tmp);
	free_pos(p);
	free_sv(sv);
	free_sv(s_old);
	fclose(opt->fp_sam);
	free_opt(opt);
	return  0 ;
}

