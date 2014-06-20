#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "check.h"

pos_t *init_pos_t()
{
	pos_t  *p ;
	p = malloc(sizeof(pos_t));
	p->m = 16 ;
	p->n = 0 ;
	p->pos =  malloc(sizeof(int)*16);
	return p ;
}

#define READ1  64

int push(pos_t *p[2] , int flag , int pos , char cigar[512])
{
	int op = (flag & READ1) ? 0 : 1 ;
	if( p[op]->m == p[op]->n){
		p[op]->m =  p[op]->m << 1 ;
		p[op]->pos =  realloc(p[op]->pos , sizeof(int)*p[op]->m);
		if(p[op]->pos == NULL) {
			fprintf(stderr,"realloc failed\n");
			return  1 ;
		}
	}
	p[op]->pos[p[op]->n] = pos ;
	memcpy(p[op]->cigar,cigar,512*sizeof(char));
	p[op]->n++;
	return  1 ;
}

void  reset(pos_t *pos[2])
{
	pos[0]->n =  pos[1]->n = 0 ;
}

void free_pos(pos_t *p[2])
{

	free(p[0]->pos);
	free(p[1]->pos);
	free(p[0]);
	free(p[1]);
}

void printf_pos(pos_t *p[2])
{
	int i , j  ;
	for( i = 0 ; i < 2  ; i++){
		for( j = 0 ; j < p[i]->n ; j++)
			printf("%d\n",p[i]->pos[j]);
	}
}

stat_t  *init_stat()
{
	stat_t *p = malloc(sizeof(stat_t));
	p->read_num  = p->sam_num = p->bp_no_pos_num = 0 ;
	p->nor_sam_num = 0 ;
	p->bp_num  =  p->one_pos = p->two_pos = 0 ;
	p->unmap_num = p->unmap_bp = p->unmap_nor =  0 ;
	p->one_bp_right = p->one_bp_only_right = 0 ;
	p->two_bp_normal_one_right = p->two_bp_all_right = p->two_bp_one_right = p->two_bp_all_wrong = 0 ;
	p->normal_map_num = p->normal_right_num = 0 ;

	return p ;
}
void  findSSeq2sam(pos_t *p[2] , opt_t *o , sam_t *s)
{

	if( o->out_put && p[0]->n == 1 &&  p[1]->n == 1 && (strstr(p[0]->cigar,"S") || strstr(p[1]->cigar,"S"))){
		fprintf(o->fp_out[3], "%s\n",s->name);
	}

}

void  check_pos(stat_t *st , int diff ,pos_t *p[2] ,result_t  ret[2] , sv_t *sv, sv_t  *s_old ,opt_t *o ,sam_t *s)
{
#define ABS(x,y) (abs(x-y) < diff)
	int  i , j ;
	for( i = 0 ; i < 2 ; i++){
		if(!p[i]->pos[0] && ret[i].is_bp)     st->unmap_bp++;
		if(!p[i]->pos[0] && ret[i].is_no_pos) 	st->unmap_nor++;

		if(!p[i]->pos[0] && p[i]->n == 1)  {
			st->unmap_num++;
			continue ;
		}
		if( ret[i].is_bp && ret[i].n == 1 && p[i]->n == 1  ){
			if(p[i]->pos[0] == ret[i].pos[0]){
				st->one_bp_only_right++ ;
				st->one_bp_right++ ;
			}
		}else if(ret[i].is_bp  && ret[i].n == 1 ){
			for( j = 0 ; j <  p[i]->n ; j++){
				if(p[i]->pos[j] - ret[i].pos[0]){
					st->one_bp_right++;
					break;
				}
			}
		}else if(ret[i].is_bp && ret[i].n == 2){
			if(p[i]->n%2== 1) {
				if( ABS(p[i]->pos[0],ret[i].pos[0])|| ABS(p[i]->pos[0],ret[i].pos[1]))  {
					st->two_bp_normal_one_right++;
				}else {
					int  tmp =  0 ;
					for ( j = 1 ; j < p[i]->n ; j+=2){
						if((ABS(p[i]->pos[j],ret[i].pos[0])&&ABS(p[i]->pos[j+1],ret[i].pos[1]))|| (ABS(p[i]->pos[j+1],ret[i].pos[0])&& ABS(p[i]->pos[j],ret[i].pos[1])))
							tmp |= 2;
						else if( ABS(p[i]->pos[j],ret[i].pos[0]) || ABS(p[i]->pos[j+1],ret[i].pos[1])||ABS(p[i]->pos[j+1],ret[i].pos[0])||ABS(p[i]->pos[j],ret[i].pos[1]))
							tmp |= 1 ;
						 
					}
					if( tmp & 2 ) {
						st->two_bp_all_right++;
						if(o->out_put)
						fprintf(o->fp_out[0],"%s\t%s\t%d\t%d\n",s->name,sv->a[ret[i].sv_pos].type,s_old->a[ret[i].sv_pos].pos,s_old->a[ret[i].sv_pos].pos +s_old->a[ret[i].sv_pos].len);
					}
					else if( tmp == 1){
						st->two_bp_one_right++;
						if(o->out_put)
						fprintf(o->fp_out[1],"%s\n",s->name);
					}
					else    st->two_bp_all_wrong++;
				}
			}else{
				int  tmp =  0 ;
				for ( j = 0 ; j < p[i]->n ; j+=2){
					if((ABS(p[i]->pos[j],ret[i].pos[0])&&ABS(p[i]->pos[j+1],ret[i].pos[1]))|| (ABS(p[i]->pos[j+1],ret[i].pos[0])&& ABS(p[i]->pos[j],ret[i].pos[1])))
						tmp |= 2	;
					else if( ABS(p[i]->pos[j],ret[i].pos[0]) || ABS(p[i]->pos[j+1],ret[i].pos[1])||ABS(p[i]->pos[j+1],ret[i].pos[0])||ABS(p[i]->pos[j],ret[i].pos[1]))
						tmp |= 1 ;
					 
				}
				if( tmp & 2 ){
					st->two_bp_all_right++ ;
					if(o->out_put)
					fprintf(o->fp_out[0],"%s\t%s\t%d\t%d\n",s->name,sv->a[ret[i].sv_pos].type,s_old->a[ret[i].sv_pos].pos,s_old->a[ret[i].sv_pos].pos +s_old->a[ret[i].sv_pos].len);
				}
				else if( tmp == 1) {
					st->two_bp_one_right++;
					if(o->out_put)
					fprintf(o->fp_out[1],"%s\n",s->name);
				}
				else    st->two_bp_all_wrong++;
			}
		}else  if(!ret[i].is_bp){
			st->normal_map_num++ ;
			if(o->out_put)
			if(!ret[1-i].is_bp && o->out_put)
			fprintf(o->fp_out[2],"%s\n",s->name);
			for(  j = 0 ; j < p[i]->n ; j++){
				if( p[i]->pos[j] == ret[i].pos[0]){
					st->normal_right_num++;
					break ;
				}
			}
		}
	}
}

void print_stat(stat_t *st)
{
	fprintf(stderr," read num = %ld , sam num = %ld  nor_sam_num = %ld \n",st->read_num , st->sam_num , st->nor_sam_num);
	fprintf(stderr," bp_no_pos_num  = %ld \n",st->bp_no_pos_num);
	fprintf(stderr," bp num = %ld , one pos = %ld   two pos = %ld\n",st->bp_num , st->one_pos , st->two_pos) ;
	fprintf(stderr," unmap_num = %ld  , unmap_bp = %ld  unmap_no_pos = %ld\n",st->unmap_num , st->unmap_bp , st->unmap_nor);
	fprintf(stderr," one_bp_right = %ld   one_bp_only_right = %ld \n",st->one_bp_right , st->one_bp_only_right);
	fprintf(stderr," two_bp_all_right = %ld , two_bp_normal_one_right =%ld ,  two_bp_one_right = %ld  two_bp_all_wrong = %ld \n",st->two_bp_all_right , st->two_bp_normal_one_right ,st->two_bp_one_right , st->two_bp_all_wrong);
	fprintf(stderr,"normal_map_num = %ld  , normal_right_num  = %ld \n", st->normal_map_num ,st->normal_right_num);

}
