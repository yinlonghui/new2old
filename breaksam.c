/*
 * =====================================================================================
 *
 *       Filename:  breaksam.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年06月18日 13时58分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "check.h"
#include "sam.h"
#include "parse.h"

static int  usage()
{
	fprintf(stderr,"breaksam  <in.sam> \n");
	fprintf(stderr,"output:   in.sam.1  S  \n");
	fprintf(stderr,"          in.sam.2  unproper  \n");
	return    -1 ;
}
typedef struct {
	char  name [1024] ;
	char  buffer[2048] ;
	char  cigar[512];
	int   flag ;
} name_e ;

typedef struct {
	name_e    *e ;
	int       m,n ;
} name_t;

void push_n(name_t  *nt[2] , sam_t *s)
{
	int  op = (s->flag & 64) ? 0 : 1 ;
	int  n  =  nt[op]->n ;

	if(n == nt[op]->m){
		nt[op]->m = nt[op]->m << 1;
		nt[op]->e = realloc(nt[op]->e ,sizeof(name_e)* nt[op]->m);
		if( nt[op]->e == NULL){
			fprintf(stderr,"alloc too much memory\n");
			exit(1);
		}
	}
	memcpy(nt[op]->e[n].name, s->name  , 1024*sizeof(char));
	if(s->len > 2048){
		fprintf(stderr,"too much size\n");
		exit(1);
	}
	memcpy(nt[op]->e[n].cigar, s->cigar , 512*sizeof(char));
	memcpy(nt[op]->e[n].buffer, s->buffer , s->len*sizeof(char));
	nt[op]->e[n].flag = s->flag ; 
	nt[op]->n ++ ;
}

int main( int argc , char *argv[])
{
	if(argc != 2)  return usage();
	int  i ;
	char *output = NULL ;
	FILE *fp_out[2];
	name_t  *nt[2] ;
	nt[0] = nt[1] = NULL ;
	opt_t  *o =  malloc(sizeof(opt_t));
	sam_t  *tmp = NULL ;
	o->str_sam =  argv[1];
	if(open_sam(o) == -1){
		free(o);
		return -1 ;
	}
	
	
	output  = calloc(strlen(argv[1])+ 10, sizeof(char));
	for( i = 0 ;  i < 2 ; i++ ){
		sprintf(output,"%s.%d",argv[1],i);
		fp_out[i] =  fopen( output ,"w");
	}
	nt[0] = calloc(1,sizeof(name_t));
	nt[1] = calloc(1,sizeof(name_t));
	nt[0]->m =  1024 ;
	nt[1]->m =  1024 ;
	nt[0]->e =  calloc(nt[0]->m , sizeof(name_e));
	nt[1]->e =  calloc(nt[1]->m , sizeof(name_e));
	while(1){
		sam_t  *s = read_sam(o,0) ;
		if(s == NULL) break ;
		if(tmp == NULL){
			tmp = s ;
			push_n(nt , s);
			int  i ;
			for( i  = 0 ; i < s->n ; i++){

				fprintf(fp_out[0],"%s",s->ht[i].buffer);
				fprintf(fp_out[1],"%s",s->ht[i].buffer);
				free(s->ht[i].buffer);
			}
			free(s->ht);
			//  push ...
		}else if (!strcmp(s->name , tmp->name)){
			free(tmp->buffer);
			free(tmp);
			tmp = s ;
			push_n(nt , s);
		}else{
			if(nt[0]->n%2 == 1  && nt[1]->n%2 == 1 &&  nt[0]->e[0].flag&2 && (strstr(nt[0]->e[0].cigar , "S") || strstr(nt[1]->e[0].cigar , "S") )){
				fprintf(fp_out[0] , "%s" , nt[0]->e[0].buffer);
				fprintf(fp_out[0] , "%s" , nt[1]->e[0].buffer);
			}else if(  nt[0]->n%2 == 1  &&  nt[1]->n%2==1 &&!(nt[0]->e[0].flag&4 || nt[1]->e[0].flag&4)){
				fprintf(fp_out[1] , "%s" , nt[0]->e[0].buffer);
				fprintf(fp_out[1] , "%s" , nt[1]->e[0].buffer);
			}
			free(tmp->buffer);
			free(tmp);
			tmp = s ;
			nt[0]->n =  nt[1]->n =  0 ;
			push_n(nt , s);
		}
	}
	if(nt[0]->n%2 == 1  && nt[1]->n%2 == 1 &&  nt[0]->e[0].flag&2  && (strstr(nt[0]->e[0].cigar , "S") || strstr(nt[1]->e[0].cigar , "S") )){
		fprintf(fp_out[0] , "%s" , nt[1]->e[0].buffer);
		fprintf(fp_out[0] , "%s" , nt[1]->e[0].buffer);
	}else if(  nt[0]->n%2 == 1  &&  nt[1]->n%2==1  &&!(nt[0]->e[0].flag&4 ||  nt[1]->e[0].flag&4)){
		fprintf(fp_out[1] , "%s" , nt[1]->e[0].buffer);
		fprintf(fp_out[1] , "%s" , nt[1]->e[0].buffer);
	}

	free(tmp->buffer);
	free(tmp);
	free(nt[0]->e);
	free(nt[1]->e);
	free(nt[0]);
	free(nt[1]);
	free(output);
	free(o->fp_sam);
	free(o);
	return 0 ;
}

