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

#include "src-cycle.h"


/*event struct*/
struct event {
	//pthread_t threadID; 	//the threadID
	ticks timestamp;	//the timestamp of the event
	int event_type;		//event type: application specific
	int event_value;	//event value: event specific
	};

FILE* f;		//the thread's file
struct event *buff;	//the thread's buffer
int cur_pos;		//current position in the buffer
int ext_buff_size;	//global variable for buffer size


/*Library initialization*/
/*Parameters:
	int buff_size: 	number of events that fit in each thread's buffer
	file_prefix: 	to distinguish between threads
*/
	
int init_profiling(int buff_size, char* file_prefix){
	
	struct event e;
	//allocate buffer
	if((buff = malloc(buff_size*sizeof(e))) == NULL){
		printf("out of memory");
		return -1;
	}
	
	//initialize buffer pointer
	cur_pos = 0;
	ext_buff_size = buff_size;
	

	//open file
	if((f = fopen(file_prefix, "w"))== NULL){
		printf("fopen error");
		return -1;
	}
	else{
		fprintf(f, "Initializing [%lu]\n", getticks());
		//fprintf(f, "ThreadID\tTime\t\tType\t\tValue\n");
		return 0;
	}
}


/*flush the buffer*/
void flush(pthread_t threadID){
	int i;
	for(i=0; i<ext_buff_size; i++){
		fprintf(f, "%10u\t", buff[i].timestamp);
		fprintf(f, "%10d\t%10d\n" , buff[i].event_type, buff[i].event_value); 
		}

	//initialize buffer & buffer pointer
	cur_pos = 0;
	for(i=0; i<ext_buff_size; i++){
		buff[i].timestamp = 0;
		buff[i].event_type = 0;
		buff[i].event_value = 0;
		}
}

/* record an event*/
void record_event(int type, int value){

	//check if buffer is full -> how???
	if(cur_pos == ext_buff_size){	
		//if yes, flush the buffer
		flush(pthread_self());
	}
		//if not, save event in the buffer
		buff[cur_pos].timestamp = getticks();
		buff[cur_pos].event_type = type;
		buff[cur_pos].event_value = value;

		//increase buffer pointer
		cur_pos++;
}

/*Library finalization*/
int finalize_profiling(){
	//close file
	if( (fclose(f))!= 0){
		printf("fclose error");
		return -1;
	}
	//free buffer
	free(buff);
	return 0;
}
