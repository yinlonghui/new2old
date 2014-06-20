#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"


void  init_opt(opt_t *o)
{
	o->len =  100;
	o->diff =  5 ;
	o->out_put = NULL ;
	o->mod_fn[0] = o->mod_fn[1] = NULL ;
	o->str_sam = NULL ;
}

opt_t *parse_argv(int argc , char *argv[])
{
	opt_t *o =  malloc(sizeof(opt_t));
	init_opt(o);
	int c  ; 
	char *out_put;
	while((c = getopt(argc , argv ,"l:d:o:")) >= 0 ){
		switch(c){
			case 'l':  o->len = atoi(optarg); break ;
			case 'd':  o->diff = atoi(optarg); break ;
			case 'o':  o->out_put = optarg; break ;
			default:
				   fprintf(stderr,"error input\n");
				   goto ERR1 ;
		}
	}
	if(optind + 3 != argc ){
		fprintf(stderr,"illegal input\n");
		fprintf(stderr,"new2old  modInf2newRef.txt modInf2oldRef.txt  input.sam\n");
		fprintf(stderr,"-l read sequence length  default:%d\n",o->len);
		fprintf(stderr,"-d max diff   defaul:%d\n",o->diff);
		fprintf(stderr,"-o output file:%s\n",o->out_put);
		o->out_put = NULL ;
		goto ERR1 ;

	}
	o->mod_fn[0]  =  argv[optind];
	o->mod_fn[1]  =  argv[optind + 1];
	o->str_sam =  argv[optind +2];
	if(o->out_put){
		int  i = 0 ;
		out_put = calloc((strlen(o->out_put)+32),sizeof(char));
		for( ;  i < 4 ; i++){
			sprintf(out_put,"%s.%d",o->out_put,i);
			o->fp_out[i] = fopen(out_put,"w");
		}
		free(out_put);
	
	}else{
		fprintf(stderr,"warning: no output file\n");
	}


	return  o ; 
ERR1:
	free_opt(o);
	return NULL;

}

void free_opt(opt_t *o)
{
	if(o->out_put){
		int  i  ;
		for( i = 0 ; i < 3 ; i++)
			fclose(o->fp_out[i]);
	}
	free(o);
}

