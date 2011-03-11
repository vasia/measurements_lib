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
	long unsigned int last_tstamp, rduration, tduration;	//timestamp of last operation and durations in ns and ticks

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
		
			//read durations and last timestamp -> merge will do the conversion to ns
			fscanf(input, "%lu%lu%lu", &rduration, &tduration, &last_tstamp);

			//call post_processing library
			merge_files(f_array, num_files, rduration, tduration, last_tstamp);

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
