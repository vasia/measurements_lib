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
- min_index()
- is_eof()
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FIRST_LINES 5	//number of header data lines in each file

//merge the files of f_array based on their timestamp
//to create a Paraver-compatible trace file
FILE* merge_files(FILE** f_array, int files, long unsigned int last_tstamp){
	
	FILE* trace;	//the file descriptor of the trace file
	long unsigned int cur_time;	//current timestamp
	long unsigned int *buff;	//buffer of current timestamps to be examined
	int i, j, index;
	int threadID, event, value;
	char s[100];

 	time_t tim=time(NULL);	

	//read first lines of each file
	for(i=0; i<files; i++){
		for(j=0; j<FIRST_LINES; j++)
			fgets(s, 100, f_array[i]);
	}	

	//open trace file
	if((trace = fopen("trace_file.prv", "w")) == NULL){
        	printf("fopen trace file error");
                return NULL;
        }

	//allocate timestamps buffer
	buff = malloc(files*sizeof(long unsigned int));

	//write first record of the Paraver trace
	fprintf(trace, "#Paraver ");

	//write current date and time
	struct tm *now=localtime(&tim);
	fprintf(trace, "(%02d/%02d/%d at ", now->tm_mday, now->tm_mon+1, now->tm_year-100);
	fprintf(trace, "%02d:%02d):", now->tm_hour, now->tm_min);
	fprintf(trace, "%lu:", last_tstamp);
	fprintf(trace, "1(2):1:1(%d:1),1\n", files);

	//dump communicator line
	fprintf(trace, "c:1:1:1:1:1\n");

	//load first timestamps
	for(i=0; i<files; i++)
		fscanf(f_array[i], "%lu", &buff[i]);
		
	while(!(is_eof(buff, files))){
		//find minimun index 
		index = min_index(buff, files);

		//write event record in trace file
		fprintf(trace, "2:1:1:1:");
		fscanf(f_array[index], "%d%d%d", &threadID, &event, &value);
		fprintf(trace, "%d:", threadID+1);	//threadID+1 because paraver doesn't accept 0 as a valid threadID
		fprintf(trace, "%lu:", buff[index]);	//time
		fprintf(trace, "%d:", event);		//event_type
		fprintf(trace, "%d\n", value);		//event_value

		//read next timestamp of the file
		fscanf(f_array[index], "%lu", &buff[index]);

	}

	//close trace file
	fclose(trace);
	
	return trace;
}

int min_index(int *array, int n){
	int i, index, temp;

	//initialization
	for(i=0; i<n; i++){
		if(array[i] != -1){
			temp = array[i];
			index = i;
			break;
		}
	}

	for(i=0; i<n; i++){
		if(temp > array[i] && array[i] != -1){
			temp = array[i];
			index = i;
		}
	}
	
	return index;
}

int is_eof(int *array, int n){
	int i;
	
	for(i=0; i<n; i++){
		if(array[i]!=-1)
			return 0;
	}
	
	return 1;
}
