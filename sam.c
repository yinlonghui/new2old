#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sam.h"
#include "parse.h"

int open_sam(opt_t* o)
{
	o->fp_sam = fopen(o->str_sam, "r");
	if(!o->fp_sam){
		fprintf(stderr,"can't open %s",o->str_sam);
		return 0 ;
	}
	return 0 ;
}

sam_t  *read_sam(opt_t *o)
{
	sam_t  *s =  NULL;

	if(feof(o->fp_sam) != 0 )  
		return s ;
	do{
		char *buffer ;
		size_t len = 0 ;
		int  a = getc(o->fp_sam);
		if( a != '@'){
			ungetc(a,o->fp_sam);
			break;
		}else   ungetc(a,o->fp_sam);
		getline( &buffer , &len , o->fp_sam ); 
		free(buffer);
	}while(1);
	s = calloc(1,sizeof(sam_t));
	fscanf(o->fp_sam ,"%s%d\t%*[^\t]\t%d",s->name,&s->flag,&s->map_pos);
	fscanf(o->fp_sam , "%*[^\n]\n");
	char  *buff  =  strstr(s->name, "_");
	buff++ ;
	s->pos[0] = atoi(buff);
	buff =  strstr(buff,"_");
	buff ++ ;
	s->pos[1] = atoi(buff);
	return  s ;
}
