/*A simple library for profiling pThread applications.
Developed for the EMDC 2nd semester course Tools & Measurements Techniques
(http://pcsostres.ac.upc.edu/eitm/doku.php)
by Vasiliki Kalavri (vasilikikalavri@gmail.com)*/

/* Library API

- init_profiling(buff_size, file_prefix)
- record_event(type, value)
- flush(thread)
- finalize_profiling()
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "src-cycle.h"

#define END_OF_FILE -1

/*real time variables*/
struct timeval tvs, tve;
double starttime, endtime;		//start and end time in ns 
long unsigned int startticks, endticks;	//and ticks



/*event struct*/
struct event {
	ticks timestamp;	//the timestamp of the event
	int event_type;		//event type: application specific
	int event_value;	//event value: event specific
	};

FILE** f_array;		//array of file descriptors
int* cur_pos_array;	//array of current positions
struct event *buff_array;	//array of the thread's buffers

int ext_buff_size;	//global variable for buffer size
FILE *input;		//file containing the post-processing data

/*Library initialization*/
/*Parameters:
	int buff_size: 	number of events that fit in each thread's buffer
	file_prefix: 	to distinguish between threads
	num_threads:	number of threads (including master thread)
*/
	
int init_profiling(int buff_size, char* file_prefix, int num_threads){
	
	struct event e;
	char file_suf[10];	//file suffix
	char *temp = malloc(10*sizeof(char));	//to temporary store the file name
	int i;

	ext_buff_size = buff_size;

	//get time in ns and ticks
	gettimeofday(&tvs, NULL);
  	starttime = tvs.tv_usec*1000;
	startticks = getticks();
	

	//allocate file descriptors array
	if((f_array = malloc(num_threads*sizeof(FILE*))) == NULL){
		printf("out of memory: f_array\n");
		return -1;
	}

	//allocate cur positions array
	if((cur_pos_array = malloc(num_threads*sizeof(int))) == NULL){
		printf("out of memory: cur_pos_array\n");
		return -1;
	}

	//allocate array of buffers
	if((buff_array = malloc(num_threads*buff_size*sizeof(e))) == NULL){
		printf("out of memory: buff_array");
		return -1;
	}

	//open post-processing file - begin
	if((input = fopen("input", "w")) == NULL){
		printf("fopen error: post processing file\n");
		return -1;
	}

	//initialize buffer pointers and open files
	for(i=0; i<num_threads; i++){

		cur_pos_array[i] = 0;

		//file name depending on threadID
		sprintf(file_suf, "%d", i);
		temp = strcat(file_suf, file_prefix);

		/*write file name in the post-processing file*/
		fprintf(input, "%s\n", temp);
		
		if((f_array[i] = fopen(temp, "w"))== NULL){
			printf("fopen error");
			return -1;
		}
		else{
			fprintf(f_array[i], "Initializing Time: [%lu]\n", getticks());
			fprintf(f_array[i], "ThreadID: [%u]\n\n", i);
			fprintf(f_array[i], "Timestamp\tThreadID\tEvent\tValue\n\n");
		}
	}
	//close post-processing file
	fclose(input);
	return 0;
}


/*flush the buffer*/
/*i is the thread id*/
void flush(int i){
	
	int j;

	for(j=0; j<ext_buff_size; j++){
		if(buff_array[(i*ext_buff_size) + j].timestamp !=0){
			fprintf(f_array[i], "%lu ", buff_array[(i*ext_buff_size) + j].timestamp);	//time
			fprintf(f_array[i], "%d ", i);	//thread id
			fprintf(f_array[i], "%d " , buff_array[(i*ext_buff_size) + j].event_type);	//event
			fprintf(f_array[i], "%d\n" , buff_array[(i*ext_buff_size) + j].event_value);	//value
		}
	}

	//initialize buffer & buffer pointer -- not needed if called from finalize => fix it!
	cur_pos_array[i] = 0;
	for(j=0; j<ext_buff_size; j++){
		buff_array[(i*ext_buff_size) + j].timestamp = 0;
		buff_array[(i*ext_buff_size) + j].event_type = 0;
		buff_array[(i*ext_buff_size) + j].event_value = 0;
	}
}

/* record an event*/
/*i is the thread id*/
void record_event(int type, int value, int i){

	//check if buffer is full
	if(cur_pos_array[i] == ext_buff_size){	
		//if yes, flush the buffer
		flush(i);
	}

		//if not, save event in the buffer
		buff_array[(i*ext_buff_size) + cur_pos_array[i]].timestamp = getticks();
		buff_array[(i*ext_buff_size) + cur_pos_array[i]].event_type = type;
		buff_array[(i*ext_buff_size) + cur_pos_array[i]].event_value = value;

		//increase buffer pointer
		cur_pos_array[i]++;
}

/*Library finalization*/
int finalize_profiling(int num_threads){

	int i;
	char s[50];

	//flush buffer
	for(i=0; i<num_threads; i++)
		flush(i);

	//signal end-of-file
	for(i=0; i<num_threads; i++){
		fprintf(f_array[i], "%d\n", END_OF_FILE);
	}

	//get time in ns and ticks
	gettimeofday(&tve, NULL);
  	endtime = tve.tv_usec*1000;	//I want nanoseconds!
	endticks = getticks();

	//open post-processing file
	if((input = fopen("input", "r+")) == NULL){
			printf("fopen error\n");
			return -1;
		}
	//move to the end of the file
	for(i=0; i<num_threads; i++)
		fgets(s, 50, input);

	//write real and tick duration
	fprintf(input, "%.0lf\t%lu\t", endtime - starttime, elapsed(endticks, startticks));

	//write the last timestamp
	fprintf(input, "%lu\n", getticks());

	//close file containing the post-processing data
	fclose(input);

	//close files
	for(i=0; i<num_threads; i++){
		if( (fclose(f_array[i]))!= 0){
			printf("fclose error");
			return -1;
		}
	}
	//free buffer and current position array
	free(buff_array);
	free(cur_pos_array);
	return 0;
}
