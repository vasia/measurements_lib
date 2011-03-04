#include <stdio.h>
#include <stdlib.h>
#include "post_processing.h"

int main(int argc, char *argv[])
{
	FILE *input;		//file containing the names of files to be merged
	FILE** f_array;		//array of file descriptors
	int num_files;
	char s[7];		//change that dynamically!!!
	int i;

	if(argc<3){
		printf("Usage: %s input_file num_files\n", argv[0]);
		return -1;
	}
	else{
		if((input = fopen(argv[1], "r")) == NULL){
			printf("fopen error\n");
			return -1;
		}
		else{
			num_files = atoi(argv[2]);

			//allocate file descriptors array
			f_array = malloc(num_files*sizeof(FILE*));
			
			//open all partial files
			for(i=0; i<num_files; i++){
				fgets(s, 7, input);
				
				if((f_array[i] = fopen(s, "r")) == NULL){
					printf("fopen error\n");
					return -1;
				}

				fgets(s, 7, input);
			}

			//call post_processing library
			merge_files(f_array, num_files);

			//close all files
			for(i=0; i<num_files; i++){
				fclose(f_array[i]);
			}

			fclose(input);
			//free memory
			free(f_array);
		}
	}

	return 0;
}
