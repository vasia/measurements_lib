/*A post-processing library for merging the generated files
of the profiling library (profiling.h).
Merges files based on timestamps of events and generates
one single file in the appropriate format to be read by Paraver 
(http://www.bsc.es/plantillaA.php?cat_id=485)
Developed for the EMDC 2nd semester course Tools & Measurements Techniques
(http://pcsostres.ac.upc.edu/eitm/doku.php)
by Vasiliki Kalavri (vasilikikalavri@gmail.com)*/

/* Library API
- merge_files()
- generate_trace()
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIRST_LINES 5	//number of header data lines in each file

//merge the files of f_array based on their timestamp
//to create a Paraver-compatible trace file
FILE* merge_files(FILE** f_array, int files){
	
	FILE* trace;	//the file descriptor of the trace file
	long unsigned int cur_time;	//current timestamp
	long unsigned int *buff;	//buffer of current timestamps to be examined
	int i, j, index;
	int event, value;
	char s[100];

	//read first lines of each file
	for(i=0; i<files; i++){
		for(j=0; j<FIRST_LINES; j++)
			fgets(s, 100, f_array[i]);
	}	

	//open trace file
	if((trace = fopen("trace_file", "w")) == NULL){
        	printf("fopen trace file error");
                return NULL;
        }

	//allocate timestamps buffer
	buff = malloc(files*sizeof(long unsigned int));
	
	//load first timestamps
	for(i=0; i<files; i++)
		fscanf(f_array[i], "%lu", &buff[i]);
		

	//find minimun index 
	index = min_index(buff, files);
	printf("minimum index = %d, minimum timestamp = %lu\n\n", index, buff[index]);

	//write record in trace file
	fprintf(trace, "%10lu", buff[index]);
	fscanf(f_array[index], "%d%d", &event, &value);
	fprintf(trace, "%10d\t", event);
	fprintf(trace, "%10d\n", value);

	//read next timestamp of the file
	fscanf(f_array[index], "%lu", &buff[index]);

	//close trace file
	fclose(trace);
	
	return trace;
}

int min_index(int *array, int n){
	int i, index, temp;
	temp = array[0];
	index = 0;	

	for(i=1; i<n; i++){
		if(temp > array[i]){
			index = i;
			temp = array[i];
		}
	}
	
	return index;
}

