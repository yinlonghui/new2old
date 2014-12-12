
#ifndef __CHECK_H
#define __CHECK_H
#include "tras.h"
#include "sv.h"
#include "parse.h"
#include "sam.h"

typedef struct {
	int  *pos ;
	char cigar[512];
	int  n ;
	int  m ;
} pos_t ;

typedef struct{
	int64_t   read_num , sam_num  ,  nor_sam_num , bp_no_pos_num;
	int64_t   bp_num ,  one_pos , two_pos ;
	int64_t   unmap_num  ,unmap_bp , unmap_nor;
	int64_t   one_bp_right ,one_bp_only_right  ;
	int64_t   two_bp_normal_one_right ,two_bp_all_right , two_bp_one_right , two_bp_all_wrong;
	int64_t   normal_map_num , normal_right_num ;
} stat_t ;
/*
 *  statistics structure 
 */

#define ACC_READ(x) (x->read_num++)
#define ACC_SAM(x) (x->sam_num++)


#define ACC_BP(x,y) do{ \
		if(x[0].is_bp)  y->bp_num++ ;\
		if(x[0].is_bp && x[0].n == 1)  y->one_pos++ ;\
		if(x[0].is_bp && x[0].n == 2)  y->two_pos++ ;\
		if(x[1].is_bp)  y->bp_num++ ;\
		if(x[1].is_bp && x[1].n == 1)  y->one_pos++ ;\
		if(x[1].is_bp && x[1].n == 2)  y->two_pos++ ;\
}while(0) \


#define ACC_BP_NO_POS(x,y) do{\
		if(x[0].is_no_pos )  y->bp_no_pos_num++ ;\
		if(x[1].is_no_pos )  y->bp_no_pos_num++ ;\
}while(0)\



pos_t *init_pos_t(); 
int push(pos_t *p[2] , int flag , int pos , char cigar[512]);
void  findSSeq2sam(pos_t *p[2] , opt_t *o , sam_t *s);
void  reset(pos_t *pos[2]);
void free_pos(pos_t *p[2]);
void printf_pos(pos_t *p[2]);
stat_t  *init_stat();
void print_stat(stat_t *st);
void  check_pos(stat_t *st , int diff ,pos_t *p[2] ,result_t  ret[2],sv_t *sv, sv_t *s_old ,opt_t *o , sam_t *s);

#endif
