#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "parse.h"


void  init_opt(opt_t *o)
{
	o->len =  100;
	o->diff =  5 ;
	o->out_put = NULL ;
	o->mod_fn = NULL ;
	o->str_sam = NULL ;
}

opt_t *parse_argv(int argc , char *argv[])
{
	opt_t *o =  malloc(sizeof(opt_t));
	init_opt(o);
	int c  ;
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
	if(optind + 2 != argc ){
		fprintf(stderr,"illegal input\n");
		fprintf(stderr,"\n");
		fprintf(stderr,"-l %d\n",o->len);
		fprintf(stderr,"-d %d\n",o->diff);
		fprintf(stderr,"-o %s\n",o->out_put);
		goto ERR1 ;

	}

	o->mod_fn  =  argv[optind];
	o->str_sam =  argv[optind + 1];

	return  o ; 
ERR1:
	free_opt(o);
	return NULL;

}

void free_opt(opt_t *o)
{
	free(o);
}

