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
#include "src-cycle.h"


/*event struct*/
struct event {
	//pthread_t threadID; 	//the threadID
	ticks timestamp;	//the timestamp of the event
	int event_type;		//event type: application specific
	int event_value;	//event value: event specific
	};

FILE** f_array;		//array of file descriptors
int* cur_pos_array;	//array of current positions
struct event *buff_array;	//array of the thread's buffers

struct event *buff;	//the thread's buffer
//int cur_pos;		//current position in the buffer
int ext_buff_size;	//global variable for buffer size

/*Library initialization*/
/*Parameters:
	int buff_size: 	number of events that fit in each thread's buffer
	file_prefix: 	to distinguish between threads
*/
	
int init_profiling(int buff_size, char* file_prefix, int num_threads){
	
	struct event e;
	char file_suf[10];	//file suffix
	int i;

	ext_buff_size = buff_size;

	//allocate file descriptors array
	f_array = malloc(num_threads*sizeof(FILE*));

	//allocate cur positions array
	cur_pos_array = malloc(num_threads*sizeof(int));

	//allocate array of buffers
	if((buff_array = malloc(num_threads*buff_size*sizeof(e))) == NULL){
		printf("out of memory");
		return -1;
	}

	//allocate buffer
	/*if((buff = malloc(buff_size*sizeof(e))) == NULL){
		printf("out of memory");
		return -1;
	}*/

	//initialize buffer pointers and open files
	for(i=0; i<num_threads; i++){

		cur_pos_array[i] = 0;

		//file name depending on threadID
		sprintf(file_suf, "%d", i);

		if((f_array[i] = fopen(strcat(file_suf, file_prefix), "w"))== NULL){
			printf("fopen error");
			return -1;
		}
		else{
			fprintf(f_array[i], "Initializing Time: [%lu]\n", getticks());
			fprintf(f_array[i], "ThreadID: [%u]\n", i);
		}
	}



	/*if((f = fopen(strcat(file_prefix, file_suf), "w"))== NULL){
		printf("fopen error");
		return -1;
	}
	else{
		fprintf(f, "Initializing Time: [%lu]\n", getticks());
		fprintf(f, "ThreadID: [%u]\n", pthread_self());
		return 0;
	}*/
	return 0;
}


/*flush the buffer*/
void flush(int i){
	
	int j;

	for(j=0; j<ext_buff_size; j++){
		fprintf(f_array[i], "%10lu\t", buff_array[(i*ext_buff_size) + j].timestamp);
		fprintf(f_array[i], "%10d\t" , buff_array[(i*ext_buff_size) + j].event_type);
		fprintf(f_array[i], "%10d\n" , buff_array[(i*ext_buff_size) + j].event_value);
	}


	/*for(i=0; i<ext_buff_size; i++){
		fprintf(f, "%10lu\t", buff[i].timestamp);
		fprintf(f, "%10d\t%10d\n" , buff[i].event_type, buff[i].event_value); 
		}*/

	//initialize buffer & buffer pointer
	cur_pos_array[i] = 0;
	/*for(j=0; j<ext_buff_size; j++){
		buff_array[(i*ext_buff_size)].timestamp = 0;
		buff_array[(i*ext_buff_size)].event_type = 0;
		buff_array[(i*ext_buff_size)].event_value = 0;
	}*/
}

/* record an event*/
void record_event(int type, int value, int i, int num_threads){

	//check if buffer is full
	if(cur_pos_array[i] == ext_buff_size){	
		//if yes, flush the buffer
		flush(i);
	}

		//if not, save event in the buffer
		buff_array[(i*ext_buff_size) + cur_pos_array[i]].timestamp = getticks();
		buff_array[(i*ext_buff_size) + cur_pos_array[i]].event_type = type;
		buff_array[(i*ext_buff_size) + cur_pos_array[i]].event_value = value;

		//if not, save event in the buffer
		/*buff[cur_pos].timestamp = getticks();
		buff[cur_pos].event_type = type;
		buff[cur_pos].event_value = value;*/

		//increase buffer pointer
		cur_pos_array[i]++;
}

/*Library finalization*/
int finalize_profiling(int num_threads){

	int i;
	//flush buffer
	for(i=0; i<num_threads; i++)
		flush(i);

	//close files
	//open files
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
