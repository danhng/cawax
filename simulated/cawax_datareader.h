/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>
Several code patterns in this project are thanks to the openmovement project of Newcastle University.
See: https://github.com/digitalinteraction/openmovement for more details.
*/

/*
Operations used to read csv data files generated from waxrec.
*/
#pragma once

#include "cawax_datastructs.h"

#ifdef __WAX9 
	#define TIME_INDEX 1
	#define SAMPLE_INDEX 2
	#define X_INDEX 4
	#define Y_INDEX 5
	#define Z_INDEX 6
#else
	#ifdef __AX3
		#define TIME_INDEX 0
		#define X_INDEX 1
		#define Y_INDEX 2
		#define Z_INDEX 3
	#endif // __AX3
#endif 

#ifndef  SAMPLE_INDEX
	#define SAMPLE_INDEX -1
#endif


/*
Count how many line (samples) in a file (relative path). 
- If the last line is not terminated by a LF then it is still considered a line and thus added to the line counter.
TODO: open a file twice?
 - Return a non-negative value if the file is read successfully. Otherwise, a negative value is returned.
*/
int countLines(const char * filename);

/*
Read {count} lines from the csv file (starting from the first line) into a LinkedList struct.
 - if {count} = {DEFAULT_PARAMETER_VALUE} then read the whole file.
*/
LinkedList * readFile(const char * filename, size_t count);



