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
	
	FILE *trace;	//the file descriptor of the trace file
	long unsigned int cur_time;	//current timestamp;
	int i, j;
	char s[100];

	for(i=0; i<files; i++){
		for(j=0; j<FIRST_LINES; j++){
			fgets(s, 100, f_array[i]);
	}	
	

	return trace;
}



