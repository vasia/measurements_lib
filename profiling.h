/*A simple library for profiling pThread applications.
Developed for the EMDC 2nd semester course Tools & Measurements Techniques
(http://pcsostres.ac.upc.edu/eitm/doku.php)
by Vasiliki Kalavri (vasilikikalavri@gmail.com)*/

/* Library API

- init_profiling(buff_size, file_prefix)
- record_event(type, value)
- flush(thread)
- finalize()
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
int* buff;	//the thread's buffer


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
	//open file
	if((f = fopen(file_prefix, "w"))== NULL){
		printf("fopen");
		return -1;
	}
	else{
		fprintf(f, "Initializing...\n");
		return 0;
	}
}

int finalize_profiling(){
	fclose(f);
}
