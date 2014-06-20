#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sv.h"
int cmp_sort(const void *p1 , const void *p2)
{
	const ss_v  *tmp1  =  p1 ;
	const ss_v  *tmp2  =  p2 ;
	return     (tmp1->pos > tmp2->pos ) - (tmp2->pos > tmp1->pos) ;

}
int read_sv(FILE *fp_mod , sv_t *sv)
{
	while(!feof(fp_mod)){
		if(sv->m == sv->n){
			sv->m = sv->m << 1;
			sv->a = realloc( sv->a ,sizeof(ss_v)*sv->m);
		}
		sv->a[sv->n].strand  = sv->a[sv->n].order_num = -1;
		fscanf(fp_mod , "%d%s%d", &sv->a[sv->n].pos ,sv->a[sv->n].type,&sv->a[sv->n].len);
		if( strstr(sv->a[sv->n].type ,"TRS") || strstr(sv->a[sv->n].type ,"CNV")){
			fscanf(fp_mod ,"%d",&sv->a[sv->n].order_num);
		}
		if( strstr(sv->a[sv->n].type,"CNV")){
			char strand;
			fscanf(fp_mod,"\t%*[^\t]\t%*[^\t]\t%c",&strand);
			if(strand == '-') sv->a[sv->n].strand = 2;
			else if(strand == '0') sv->a[sv->n].strand = 0;
			else sv->a[sv->n].strand = 1;
		}
		fscanf(fp_mod, "%*[^\n]\n");
		sv->n++ ;

	} 
	int i  , tmp ;
	for( i = 0 ; i < sv->n ; i += tmp){
		tmp = 1 ;
		if(strstr(sv->a[i].type ,"tandem")){
			tmp ++ ;
			if( sv->a[i].strand == 2){
				if(strstr(sv->a[i+1].type,"1")) sv->a[i].tandem = 1 ;
				else if( strstr(sv->a[i+1].type,"2"))  sv->a[i].tandem = 2 ;
				else    sv->a[i].tandem = 3 ;
				
			}else  if(sv->a[i+1].strand == 2){
				if(strstr(sv->a[i].type,"1")) sv->a[i+1].tandem = 1 ;
				else if( strstr(sv->a[i].type,"2"))  sv->a[i+1].tandem = 2 ;
				else   sv->a[i+1].tandem = 3 ;
				
			}
		}
	}
	for( i = 0 ; i < sv->n ; i += tmp){
		tmp =  1 ;
		if(strstr(sv->a[i].type,"TRS")){
			tmp =  1 ;
		}
		if(!strcmp(sv->a[i].type,"TRS1")){
			tmp++;
			if(sv->a[i].pos < sv->a[i+1].pos) {
				sv->a[i].strand =  2;
				sv->a[i+1].strand =  0;
			}else{
				sv->a[i].strand =  0;
				sv->a[i+1].strand =  2;
			}
		}else   if(!strcmp(sv->a[i].type,"TRS2")){
			tmp++;
			if(sv->a[i].pos < sv->a[i+1].pos) {
				sv->a[i].strand =  2;
				sv->a[i+1].strand =  1;
			}else{
				sv->a[i].strand =  1;
				sv->a[i+1].strand =  2;
			}
		}else   if(!strcmp(sv->a[i].type,"TRS3")){
			tmp++;
			if(sv->a[i].pos < sv->a[i+1].pos) {
				sv->a[i].strand =  1;
				sv->a[i+1].strand =  2;
			}else{
				sv->a[i].strand =  2;
				sv->a[i+1].strand =  1;
			}
		}

	}

	qsort(sv->a ,sv->n ,sizeof(ss_v) , cmp_sort);
#if 0
	for( i = 0 ; i < sv->n ; i++)
		printf("%d\t%s\t%d\t%d\t%d\t%d\n",sv->a[i].pos,sv->a[i].type ,sv->a[i].len , sv->a[i].order_num ,sv->a[i].strand,sv->a[i].tandem );
#endif
	return 0;
}

int open_sv(char *mod , sv_t **sv)
{
	FILE *fp_mod = fopen(mod , "r");
	if(!fp_mod){
		fprintf(stderr,"can't open %s \n", mod);
		return 1 ;
	}
	sv_t  *p =  malloc(sizeof(sv_t));
	p->n = 0 ;
	p->m = 32;
	p->a =  malloc(sizeof(ss_v)*p->m);
	*sv =  p ;
	read_sv(fp_mod,p);

	fclose(fp_mod);
	return 0;
}



int free_sv(sv_t  *sv)
{
	free(sv->a);
	free(sv);
	return 0 ;
}

