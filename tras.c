#include <stdlib.h>
#include <string.h>
#include "tras.h"
#include "sv.h"

ss_v *find_other(sv_t *sv , ss_v *s)
{
	int  i ;
	ss_v *mate = NULL;
	for ( i = 0 ; i < sv->n ; i ++){
		ss_v *tmp = sv->a + i ;
		if( !strcmp(tmp->type , s->type) && s->order_num == tmp->order_num && s != tmp  ){
			mate = tmp ;
			return mate ;
		}
	}
	return mate;
}

void tras2result(int pos[2] , result_t ret[2] , int len ,sv_t *sv )
{
	ret[0].n = ret[1].n = 0 ;
	ret[0].is_no_pos = ret[0].is_bp = 0 ;
	ret[1].is_no_pos = ret[1].is_bp = 0 ;
	int  i , j ;
	for(  i = 0 ; i < 2 ; i++){
		for( j = 0 ; j < sv->n ; j++){
			ss_v *s =  sv->a + j ;
			ret[i].sv_pos =  j ;
			if(pos[i] < s->pos && pos[i] + len > s->pos &&  !strcmp(s->type,"DEL")){
				ret[i].is_bp =  1 ;
				ret[i].n = 2 ;
				ret[i].pos[0] = pos[i] ;
				ret[i].pos[1] = s->pos ;
				break;
			}
			if( !strcmp(s->type,"DEL") && pos[i] >= s->pos  && j < sv->n-1 && pos[i] + len <= (sv->a+j+1)->pos ){
				ret[i].n = 1 ;
				ret[i].pos[0] =  pos[i] ;
				break ;
			}

			if( (j == 0 && pos[i] + len <= s->pos )|| (j == sv->n -1 && pos[i] >= s->pos + s->len) ){

				ret[i].n = 1 ;
				ret[i].pos[0] =  pos[i] ;
				break ;
			}
			if(j < sv->n-1 &&  pos[i] >= s->pos + s->len &&  pos[i] + len <= (sv->a+j+1)->pos){
				ret[i].n = 1 ;
				ret[i].pos[0] =  pos[i] ;
				break ;
			}


			if( pos[i] < s->pos && pos[i] + len  > s->pos){
				if(!strcmp(s->type , "INS")){
					ret[i].is_bp =  1 ;
					ret[i].n = 1 ;
					ret[i].pos[0] = pos[i] ;
					break;
				}

				if(!strcmp(s->type ,"INV")){
					ret[i].is_bp =  1 ;
					ret[i].n = 2 ;
					ret[i].pos[0] = pos[i] ;
					ret[i].pos[1] = s->pos + s->len - (pos[i] + len  - s->pos ) ;
					break;
				}

				if( strstr(s->type,"CNV")  && s->strand == 2){
					ret[i].n = 1 ;
					ret[i].pos[0] = pos[i] ;
					break;
				}

				if( !strcmp(s->type,"CNV") && s->strand == 0){
					ss_v   *mate = find_other(sv,s);
					ret[i].is_bp =  1 ;
					ret[i].n = 2 ;
					ret[i].pos[0] = pos[i] ;
					ret[i].pos[1] = mate->pos ;
					break ;
				}

				if( !strcmp(s->type,"CNV") && s->strand == 1){
					ss_v   *mate = find_other(sv,s);
					ret[i].is_bp =  1 ;
					ret[i].n = 2 ;
					ret[i].pos[0] = pos[i] ;
					ret[i].pos[1] = mate->pos  + mate->len -  (pos[i] + len  - s->pos );
					break;
				}

				if (!strcmp(s->type,"tandemCNV1")){
					ret[i].is_bp =  1 ;
					ret[i].n = 2 ;
					ret[i].pos[0] = pos[i];
					ret[i].pos[1] = s->pos - s->len ; 
					break;
				}
				
				if (!strcmp(s->type,"tandemCNV2")){
					ret[i].is_bp =  1 ;
					ret[i].n = 2 ;
					ret[i].pos[0] = pos[i];
					ret[i].pos[1] = s->pos - (pos[i] + len  - s->pos ); 
					break;
				}
				if (!strcmp(s->type,"tandemCNV3")){
					ret[i].is_bp =  1 ;
					ret[i].n = 2 ;
					ret[i].pos[0] = pos[i];
					ret[i].pos[1] = s->pos + 2*s->len -  (pos[i] + len  - s->pos ); 
					break;
				}
				if(strstr(s->type,"TRS")){
					if( s->strand == 1){
						ss_v   *mate = find_other(sv,s);
						ret[i].is_bp =  1 ;
						ret[i].n = 2 ;
						ret[i].pos[0] = pos[i];
						ret[i].pos[1] = mate->pos + mate->len -  (pos[i] + len  - s->pos );
						break;
					}else{
						ss_v   *mate = find_other(sv,s);
						ret[i].is_bp =  1 ;
						ret[i].n = 2 ;
						ret[i].pos[0] = pos[i];
						ret[i].pos[1] = mate->pos ;
						break;
					}
				}
			}

			if( pos[i] >= s->pos &&  pos[i] + len  <=  s->pos + s->len){
				if(!strcmp(s->type,"INS")){
					ret[i].is_no_pos =  1 ;
					break;
				}
				if(!strcmp(s->type ,"INV")){
					ret[i].pos[0] =  s->pos +  s->pos + s->len - pos[i] - len ;
					ret[i].n  = 1 ;
					break ;

				}
				if( strstr(s->type,"CNV") && s->strand == 2){
					ret[i].pos[0] =  pos[i];
					ret[i].n  = 1 ;
					break ;
				}
				

				if (!strcmp(s->type,"CNV") && s->strand == 1 ){
					ss_v   *mate = find_other(sv,s);
					ret[i].n  = 1 ;
					ret[i].pos[0] = mate->pos +  (s->pos + s->len  - pos[i] - len);
					break;
				}
				if(!strcmp(s->type,"CNV") && s->strand == 0 ){
					ss_v   *mate = find_other(sv,s);
					ret[i].n  = 1 ;
					ret[i].pos[0] = mate->pos +   pos[i] - s->pos ;
					break;
				}
				if(!strcmp(s->type,"tandemCNV1")){
					ret[i].n  = 1 ;
					ret[i].pos[0] =  pos[i] - s->len;
					break;
				}
				if(!strcmp(s->type,"tandemCNV2")){
					ret[i].n  = 1 ;
					ret[i].pos[0] =  s->pos -  (pos[i] + len - s->pos) ;
					break;
				}
				if(!strcmp(s->type,"tandemCNV3")){
					ret[i].n  = 1 ;
					ret[i].pos[0] =   s->pos +  ( s->len + s->pos - pos[i] - len ) ;
					break;
				}
				if(strstr(s->type,"TRS")){
					if( s->strand == 1){
						ss_v   *mate = find_other(sv,s);
						ret[i].n = 1 ;
						ret[i].pos[0] = mate->pos +  (s->pos + s->len  - pos[i] - len);
						break;
					}else{
						ss_v   *mate = find_other(sv,s);
						ret[i].n = 1 ;
						ret[i].pos[0] = mate->pos + pos[i] - s->pos;
						break;
					}
				}
			}
			
			if( pos[i] <  s->pos + s->len  && pos[i] +len > s->pos + s->len) {
				if(!strcmp(s->type , "INS")){
					ret[i].is_bp =  1 ;
					ret[i].pos[0] = s->pos ;
					ret[i].n = 1 ;
					break;
				}

				if( !strcmp(s->type,"CNV") && s->strand == 2){
					ret[i].pos[0] =  pos[i];
					ret[i].n  = 1 ;
					break ;
				}
				if(!strcmp( s->type , "tandemCNV") && s->tandem == 3){
					ret[i].pos[0] =  pos[i];
					ret[i].n  = 1 ;
					break ;
				}
				if(!strcmp(s->type ,"INV")){
					ret[i].is_bp =  1 ;
					ret[i].n = 2 ;
					ret[i].pos[0] = s->pos ;
					ret[i].pos[1] = s->pos + s->len ;
					break;
				}
				if( !strcmp(s->type,"CNV") && s->strand == 0){
					ss_v   *mate = find_other(sv,s);
					ret[i].is_bp =  1 ;
					ret[i].n = 2 ;
					ret[i].pos[0] = mate->pos + mate->len - ( s->pos + s->len - pos[i] );
					ret[i].pos[1] = s->pos ;
					break ;
				}
				if( !strcmp(s->type,"CNV") && s->strand == 1){
					ss_v   *mate = find_other(sv,s);
					ret[i].is_bp =  1 ;
					ret[i].n = 2 ;
					ret[i].pos[0] = mate->pos;
					ret[i].pos[1] = s->pos ;
					break ;
				}
				if (!strcmp(s->type,"tandemCNV1")){
					ret[i].n = 1 ;
					ret[i].pos[0] = pos[i];
					break;
				}
				
				if (!strcmp(s->type,"tandemCNV2")){
					ret[i].is_bp =  1 ;
					ret[i].n = 2 ;
					ret[i].pos[0] = s->pos;
					ret[i].pos[1] = s->pos - s->len ; 
					break;
				}
				if (!strcmp(s->type,"tandemCNV3")){
					ret[i].is_bp =  1 ;
					ret[i].n = 2 ;
					ret[i].pos[0] = s->pos + s->len ;
					ret[i].pos[1] = s->pos + s->len ; 
					break;
				}
				if(strstr(s->type,"TRS")){
					if( s->strand == 1){
						ss_v   *mate = find_other(sv,s);
						ret[i].is_bp =  1 ;
						ret[i].n = 2 ;
						ret[i].pos[0] = s->pos + s->len ;
						ret[i].pos[1] = mate->pos ;
						break;
					}else{
						ss_v   *mate = find_other(sv,s);
						ret[i].is_bp =  1 ;
						ret[i].n = 2 ;
						ret[i].pos[0] = s->pos + s->len;
						ret[i].pos[1] = mate->pos  +  mate->len  - ( s->pos + s->len - pos[i]);
						break;
					}
				}

			}
		}
	}

	for( i =  0 ;  i < ret[0].n ;  i++){
		int  acc =  0 ;
		for(  j =  0 ;  j <  sv->n ; j++){
			ss_v *s =  sv->a + j ;
			if( s->pos <  ret[0].pos[i]){
				if(!strcmp(s->type,"DEL"))  acc+= s->len ;
				else  if(  ( strstr(s->type,"CNV") && s->strand !=2) || !strcmp(s->type,"INS")) acc -= s->len ;
			}else if( s->pos ==  ret[0].pos[i] &&!strcmp(s->type,"DEL")){
				 acc+= s->len ;
			}else break ;
		}
		ret[0].pos[i] += acc ;
		
	}

	for( i =  0 ;  i < ret[1].n ;  i++){
		int  acc =  0 ;
		for(  j =  0 ;  j <  sv->n ; j++){
			ss_v *s =  sv->a + j ;
			if( s->pos <  ret[1].pos[i]){
				if(!strcmp(s->type,"DEL"))  acc+= s->len ;
				else  if(  ( strstr(s->type,"CNV") && s->strand !=2) || !strcmp(s->type,"INS")) acc -= s->len ;
			}else if( s->pos == ret[1].pos[i] && !strcmp(s->type,"DEL")){
				 acc+= s->len ;
			}else break ;
		}
		ret[1].pos[i] += acc ;
		
	}

}
