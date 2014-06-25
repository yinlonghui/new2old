#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sam.h"
#include "parse.h"

int open_sam(opt_t* o)
{
	o->fp_sam = fopen(o->str_sam, "r");
	if(!o->fp_sam){
		fprintf(stderr,"can't open %s\n",o->str_sam);
		return  -1 ;
	}
	return 0 ;
}

sam_t  *read_sam(opt_t *o , int simu)
{
	sam_t  *s =  NULL;

	if(feof(o->fp_sam) != 0 )  
		return s ;
	static int is_head = 0 ;
	
	s = calloc(1,sizeof(sam_t));
	do{
		int  a = getc(o->fp_sam);
		if( a != '@'){
			ungetc(a,o->fp_sam);
			break;
		}else {
			ungetc(a,o->fp_sam);
		}
		if(is_head == 2){
			char *buff = NULL ; 
			size_t  len =  0  ;
			getline(&buff , &len , o->fp_sam);
			free(buff);
			continue ;
		}
		if(is_head == 0)
		s->ht = calloc(30,sizeof(head_t));
		is_head =  1;
		getline( &s->ht[s->n].buffer , &s->ht[s->n].len , o->fp_sam ); 
		s->n++;
	}while(1);
	is_head = 2 ;
	long offset ;
	offset =  ftell(o->fp_sam);
	getline(&s->buffer, &s->len , o->fp_sam);
	fseek(o->fp_sam,offset,SEEK_SET);



	fscanf(o->fp_sam ,"%s%d\t%*[^\t]\t%d\t%*[^\t]\t%s",s->name,&s->flag,&s->map_pos,s->cigar);
	fscanf(o->fp_sam , "%*[^\n]\n");
	if(simu){
		char  *buff  =  strstr(s->name, "_");
		buff++ ;
		s->pos[0] = atoi(buff);
		buff =  strstr(buff,"_");
		buff ++ ;
		s->pos[1] = atoi(buff);
	}
	return  s ;
}
