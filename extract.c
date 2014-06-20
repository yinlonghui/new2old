

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define  S_IDLE     0x10
#define  S_FOUND    0x2
#define  S_SAM_NEXT 0X4
#define  S_NEXT     0x8

int main( int argc , char *argv[])
{
	int state =  S_IDLE ;
	FILE *fp_name , *fp_sam , *fp_out ;

	if( argc != 4){
		fprintf(stderr,"extract <name> <in.sam> <out.sam> \n");
		return -1 ;
	}

	fp_name = fopen(argv[1],"r");
	if(!fp_name){
		fprintf(stderr,"can't open the file : %s",argv[1]);
		return 1 ;
	}

	fp_sam  = fopen(argv[2],"r");
	if(!fp_sam){
		fprintf(stderr,"can't open the file : %s",argv[2]);
		fclose(fp_name);
		return 1;
	}

	fp_out = fopen(argv[3],"w");
	if(!fp_out){
		fprintf(stderr,"can't open the file : %s",argv[3]);
		fclose(fp_name);
		fclose(fp_sam);
		return 1;

	}
	char *name_buffer , *sam_buffer ;
	size_t name_len , sam_len ;
	name_buffer = sam_buffer = NULL ;
	name_len = sam_len = 0 ;
	while(!feof(fp_sam)){
		if(state&S_IDLE) {
			int i = 0 ;
			getline(&name_buffer, &name_len , fp_name);
			for( i = 0 ;  i < name_len ; i++)  
				if(name_buffer[i] == '\n'){
					name_buffer[i] = 0 ;
					break;
				}
			getline(&sam_buffer , &sam_len , fp_sam);
			state &= (~S_IDLE);
		}
		if(state&S_NEXT) {
			int i = 0 ;
			free(name_buffer);
			name_buffer = NULL;
			name_len = 0 ;
			getline(&name_buffer, &name_len , fp_name);
			for( i = 0 ;  i < name_len ; i++)  
				if(name_buffer[i] == '\n'){
					name_buffer[i] = 0 ;
					break;
				}
			if(feof(fp_name)) break ;
			state &= (~S_NEXT);
		}
		else if(state & S_SAM_NEXT ) {
			free(sam_buffer);
			sam_buffer =NULL;
			sam_len = 0 ;
			getline(&sam_buffer , &sam_len , fp_sam);
			state &= (~S_SAM_NEXT);
		}

		if(strstr(sam_buffer,name_buffer)){
			state |= S_FOUND ;
			state |= S_SAM_NEXT;
			fprintf(fp_out,"%s",sam_buffer);
		}else if( state & S_FOUND){
			state &=(~S_FOUND);
			state |= S_NEXT ;
		}else {
			state |= S_SAM_NEXT ;
		}

	}
	free(sam_buffer);
	free(name_buffer);

	

	return 0 ;
}


