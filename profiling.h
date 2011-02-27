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
	unsigned int threadID; 	//the threadID
	ticks timestamp;	//the timestamp of the event
	int event_type;		//event type: application specific
	int event_value;	//event value: event specific
	};

FILE* f;	//the thread's file
struct event *buff;	//the thread's buffer
int cur_pos;	//current position in the buffer


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

	//open file
	if((f = fopen(file_prefix, "w"))== NULL){
		printf("fopen error");
		return -1;
	}
	else{
		fprintf(f, "Initializing [%lu]\n", getticks());
		return 0;
	}
}


/* record an event*/
void record_event(int type, int value){
	struct event e;

	e.threadID = pthread_self();
	e.timestamp = getticks();
	e.event_type = type;
	e.event_value = value;

	//check if buffer is full

	//if yes, flush the buffer

	//initialize buffer pointer

	//if not, save event in the buffer
	buff[cur_pos] = e;
}

/*Library finalization*/
int finalize_profiling(){
	if( (fclose(f))!= 0){
		printf("fclose error");
		return -1;
	}
	else
		return 0;
}
