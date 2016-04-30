/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>

Many code patterns in this project are thanks to the openmovement project of Newcastle University.
See: https://github.com/digitalinteraction/openmovement for more details.
*/
#pragma once

/** Include all headers to be used within the project here**/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#ifdef __linux__
	#pragma message ("Linux system detected. Some libraries will be added.")
	#include <stdint.h>
#endif
#include "cawax_utils.h"

#define _CRT_SECURE_NO_WARNINGS

// a return value indicating that the function which is expected to return a value has yet to be implemented or has not been implemented completely.
#define NOT_IMPLEMENTED 0

// a return value indicating that the function which is expected to return a pointer has yet to be implemented or has not been implemented completely.
#define NOT_IMPLEMENTED_PTR NULL

#define DEFAULT_PARAMETER_VALUE  -160711

#define INVALID_NEGATIVE_PARAM(param) (((param) < 0) && ((param) != DEFAULT_PARAMETER_VALUE))

#define TERMINATION_VALUE  0x1111

#define SAMPLE_BYTES_STRING 45

#define SAMPLE_COMPONENTS_COUNT 5 // time, order, x, y, z

#define SAMPLE_COMPONENT_SIZE 15 // number of bytes needed to store each token (in string format)

#define SAMPLE_LONGEST_TOKEN_SIZE 11 // longest token in the sample string

// a return value indicating that the function which is expected to return a non-negative value has failed.
#define ERROR_POSITVE_OUTPUT -1

// a return value indicating that the function which is expected to return a pointer has failed.
#define ERROR_PTR_OUTPUT NULL

// define all types of component indices of a sample that might be fed into some particular series processing procedure.
#define CINDEX_X	 0x1
#define CINDEX_Y	 0x2
#define CINDEX_Z	 0x4
#define CINDEX_RMQ	 0x8
#define CINDEX_MEAN	 0x10
#define CINDEX_ORDER 0x20
#define CINDEX_TIME  0x40
// ... subject to be expanded.

/*
TIME def (the time the sample is received) (22 bits) of a sample extracted from the output file including:
- Minute
- Second
- Milisecond
*/
typedef long CAWAX_TIME_MSM;
#define CAWAX_TIME_FROM_MSM(minute, second, millisec) \
		(((CAWAX_TIME_MSM)(minute)   << 16)) \
	   |(((CAWAX_TIME_MSM)(second)   << 10)) \
	   |(((CAWAX_TIME_MSM)(millisec)       ))
#define CAWAX_TIME_GET_MINUTE(time_msm)		((unsigned int) (((time_msm) >> 16) & 0x3f ))
#define CAWAX_TIME_GET_SECOND(time_msm)		((unsigned int) (((time_msm) >> 10) & 0x3f ))
#define CAWAX_TIME_GET_MILLISECOND(time_msm)	((unsigned int) (((time_msm)	  ) & 0x3ff))

/*
TIME def (the time of the internal clock at which the sample is recorded) extracted from the signal output file including
- Second
- Microsecond
*/
typedef unsigned long INTERNAL_TIME;
#define INTERNAL_TIME_FROM_S(second, microsec) \
		(((INTERNAL_TIME)(second) << 20)) \
	|	(((INTERNAL_TIME)(microsec) ))
#define INTERNAL_TIME_GET_SECOND(time_msm)		((unsigned long) (((time_msm) >> 20) & 0x1fffffffffff ))
#define INTERNAL_TIME_GET_MICRO(time_msm)		((unsigned int)  (((time_msm)      ) & 0xfffff ))
#define INTERNAL_TIME_GET_MILLI(time_msm)			((unsigned int)  (INTERNAL_TIME_GET_MICRO(time_msm) + 500) / 1000)

#define INTERNAL_TIME_ASCII_BYTES 15

#define UNIT_SECOND_TO_MICRO 1000000
#define UNIT_MILLISEC_TO_MICRO 1000

/*
Return a string representation of the numeric time in format mm:ss:lll
*/
char * cawaxTimeToString(const CAWAX_TIME_MSM numericTime, char * buf);

/*
Return the difference (in miliseconds) of time arg2 to arg1 (how can arg1 get to arg2)
- If arg1 < arg2 then return a positive value
- Else return a negative value
*/
long cawaxTimeDiff(CAWAX_TIME_MSM arg1, CAWAX_TIME_MSM arg2);

/*
Return a string representation of the numeric time in format ss:mmmmmm
*/
char * cawaxInternalTimeToString(const INTERNAL_TIME numericTime, char * buf);

/*
Return the difference (in unit of choice) of time arg2 to arg1 (how can arg1 get to arg2)
*/
double cawaxInternalTimeDiff(INTERNAL_TIME arg1, INTERNAL_TIME arg2, int unitToMicro);

/*
Define how the IMU will be attached to the users.
This plays an important role in how sensor data will be interpreted and processed.
See WAX9 Datasheet for more details.
*/
// X towards earth.
#define TOWARDS_EARTH_COMPONENT 0x1

// Y through lateral.
#define TOWARDS_LATERAL_COMPONENT 0x2

// Z toward body.
#define TOWARDS_BODY_COMPONENT 0x4

/*
Define the default bases against which signal components are processed.
The default base is the average value at which the current state of movement is static (no significant movement)
These default bases could be obtained by putting the IMU upright on a flat ground or letting the bearing user standing still.
*/
#define DEF_BASE_X 1 // TODO 
#define DEF_BASE_Y 0 //TODO
#define DEF_BASE_Z 0.02 //TODO


/** Section */
/********************************************** CAWAX DATASTRUCTURES *********************************************************/
#define _CRT_SECURE_NO_WARNINGS

typedef double acc;
typedef unsigned long sample_th;
typedef unsigned long node_index;

// velocity delta based upon g (not m/s^2 but gm/s^2)
typedef long vel_g;

typedef enum {X, Y, Z, RMQ} dataDimension;

/**
 * Represent a sample obtained from the device
 */
typedef struct sample {
	INTERNAL_TIME time; // internal time of the device
	sample_th order; // order as in time domain
	acc x;
    acc y;
    acc z;
	acc rmq;
} Sample;


/* 
Get the address of the queried component

Parameters:
 - Sample * sample: the ptr to the sample from which component is retrieved.
 - int componentIndex: the index of the component as defined in 
*/
void * getComponent(Sample * sample, int componentIndex);

typedef struct node {
    Sample sample;
    struct node *next;
    struct node *prev;
} Node;

typedef struct linkedList {
    long count;
    Node *head;
} LinkedList;
// operations on linked list

/*
Jump from one node to another node.

PARAMS:
 - Node * depart: the node from which the jump is performed.
 - int step: how far should the jump be. (Note: if step < 0 then we jump backward). 
RETURN:
 - Node *: The pointer of the destination node.
EXAMPLE:
 - A step of 1 will return the next node or NULL if there's no next node (tail).
 - A step of -1 will return the prev node or NULL if there's no prev node (head).

*/
Node * jump(const Node * depart, int step);

/*
Make a new Node for a sample.
RETURN: Pointer to the newly created node
*/
Node * makeNode(Sample sample);

/*
Make a new list with NULL head and 0 count
Return: Pointer to the newly created list
*/
LinkedList * makeList();

/**
 * Get the node pointer at the specified index in the list
 *  - Return a null pointer if the index arg exceeds the size of the list.
 */
Node *get(node_index index, LinkedList * list);

/**
 * Add a sample to the end a signal list
 *  - Update pre tail node
 *  - Create new node for the new sample
 *  - Link new node to the list
 */
LinkedList *add(Sample sample, LinkedList *list);

/**
 * Add a sample to the specified index of the list
 *  - Create the node for the new sample
 *  - Update the neighbor nodes as to the newly created node
 */
LinkedList *addI(node_index index, Sample sample, LinkedList *list);

/**
 * Remove (unlink) a sample from the end of the signal list based on the time and the sample order
 *  - Update the node before the last node
 *  - Free the last node
 */
LinkedList * removeI(LinkedList *list);

/*
Get a subList from the original list
This is used for obtaining continous windows from the signal with 0.5second overlapping.

*/
LinkedList * subList(int start, int end, LinkedList * original, LinkedList * sub);

/**
 * Get the time domain series for a specific dataDimension from {start} (inclusive) to (inclusive) {end}
 *  - If {start} = 0 then we start from the beginning
 *  - If {end} out of bound (>= list count) then {end} is tail (count - 1)
Return a sub series array terminated by 100
 *
 */
acc *getSubSeries(dataDimension d, node_index start, node_index end, LinkedList *list);

/*
Free the linked list: free all memory blocks allocated for the list including:
 - All the nodes.
 - The list.
*/
void freeLinkedList(LinkedList * list);

/*
Empty the linked list
 - Free all the nodes but retain the list.
*/
LinkedList * emptyLinkedList(LinkedList * list);

/*
Print out the string representation of the target Sample struct
 */
int toStringSample(Sample *sample);

/*
Print out the string representation of the target Node struct
*/
int toStringNode(Node *node);
/*
Print out the string representation of the target LinkedList struct
*/
int toStringList(LinkedList *list);

/** End Section data structures */


/** Section **/
/********************************************* CAWAX DATAREADER ***************************************************************/
#define SAMPLE_BYTES 181 // default is just 3 acc values of 8 bytes each (ascii)

// default is WAX9 

#define TIME_RECEIVED_INDEX 1 

#define SAMPLE_INDEX 2
#define TIME_INTERNAL_INDEX 3
#define X_INDEX 4
#define Y_INDEX 5 
#define Z_INDEX 6 

#define SAMPLE_INDEX_BUF_INDEX 0
#define TIME_INTERNAL_BUF_INDEX 1
#define X_BUF_INDEX 2
#define Y_BUF_IDNEX 3
#define Z_BUF_INDEX 4



//#ifdef __WAX9 
//	#define TIME_RECEIVED_INDEX 1
//	#define SAMPLE_INDEX 2
//	#define X_INDEX 4
//	#define Y_INDEX 5
//	#define Z_INDEX 6
//	#define SAMPLE_BYTES 121
//
//#else
//	#ifdef __AX3
//		#define TIME_RECEIVED_INDEX 0
//		#define X_INDEX 1
//		#define Y_INDEX 2
//		#define Z_INDEX 3
//		#define SAMPLE_BYTES 33
//	#endif // __AX3
//#endif 

#ifndef  SAMPLE_INDEX
	#define SAMPLE_INDEX -1
#endif			

#define INDEX_A_TARGET_TOKEN(index)		(((index) == TIME_INTERNAL_INDEX)  \
									 ||  ((index) == SAMPLE_INDEX) \
									 ||  ((index) == X_INDEX) \
									 ||  ((index) == Y_INDEX) \
									 ||  ((index) == Z_INDEX)) 


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
LinkedList * readFile(const char * filename, int count,  int * linesRead);

/** End Section Data reader **/


/** Section */
/********************************************** CAWAX MATHS ******************************************************************/
/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>

Many code patterns in this project are thanks to the openmovement project of Newcastle University.
See: https://github.com/digitalinteraction/openmovement for more details.
*/

#ifndef SIMULATED_CAWAX_MATHS_H

// https://en.wikipedia.org/wiki/Gravitational_acceleration
#define GRAVITY_ACC 9.80665

#define TRAPEZOID_POLAR_SAME(h1, h2, delta) ((delta) * ((h1) + (h2)) / 2)

// units of integration

#define UNIT_METER 1

#define UNIT_G 10

#define BASE_0 0

#define BASE_1 1

#define DO_RECOVER 1

#define NO_RECOVER 0

#define STEP_SIZE(x) (x)



/**
 * Calculate the root mean square of a sample based on sqrt(x^2+y^2+z^2)
 */
acc rootMeanSquare(Sample * sample);

/**
 * Calculate the mean of a sample (x + y + z) / 3
 */
acc meanSample(Sample * sample);

/**
 * Calculate the mean of a sub series from the signal
 *  If count = -1 then calculate the mean for the whole array (100 terminated)
 */
acc mean(acc * input, int count);

/**
 Calculate the SD of a sub series from the signal.
 This would help figure out how acceleration values fluctuated over time thus 
 potentially giving us a useful view on the properties of the movement.
 */
acc standardDeviation(acc * input, size_t  count);

/*
Calculate the area of the trapezoidal shape made up from 2 sample points and their corresponding projections
*/
acc trapezoid(acc sample1, acc sample2, INTERNAL_TIME time1, INTERNAL_TIME time2, sample_th order1, sample_th order2, int unitToMicro);

/*
Calculate the simpson (vel_g) of a sub time series using Trapezium rules with optional step value (default is 1) 
controlling the accuracy level of the simpson
 - Integral accuracy <-><-> Computation time  
 - See https://en.wikipedia.org/wiki/Trapezoidal_rule
 - Note: This method does not support negative base at the moment.
 - TODO: Wrapper methods needed.
Note: The simpson is caluclated based on acc values that are optionally substracted by 1g in an attempt to mask out the effect of gravity 
and give the true movement data. 
 
METHOD SIGNATURE
//TODO update
 Parameters:
 - LinkedList * signal: The signal containing samples to be analysed.
 - size_t step: The step size (default is 1) drives how accurate the results would be.
 - base: Optional postive base against which the integration is calculated. (default is 0).
 - int inputTargets: which component(s) of the samples for which the integration is to be calculated. 
 - vel_g * buf: buffer for result(s)
   + result for each input target is put in the order as per the input targets (X -> Y -> Z -> RMQ -> M -> TIME -> ORDER)
 Return:
 - vel_g * : the buf array specified in the parameter;
 */
vel_g * simpson(LinkedList * signal, size_t step, acc base, char recoverSignal, int gOrMeter, int unitToMicro, int inputTargets, int count, vel_g * buf);

vel_g * simpsonSingle(LinkedList * signal, size_t step, acc base, char recoverSignal, int gOrMeter, int unitToMicro, int target, vel_g * buf);

#endif //SIMULATED_CAWAX_MATHS_H

/**  End section maths **/


/** Section **/
/**************************************************CAWAX ANALYSIS *******************************************/
/*
*/
typedef struct {
	LinkedList * subSignal;
	// features
	acc mean_x;
	acc stdDev_X;
	acc stdDev_RMQ_XYZ;
	acc integral_X;
	acc integral_RMQ_XYZ;
	//todo expand (make use of secondary-dataDimension data - Y, Z) 
} Feature;



/*
bit 0-1:
	00: n/a
	01: stand/sit
		mean_x >>
	10: lying
		mean_x <<
bit 2:
	0: static 
		small stdDev's
		rmqXYZ  ~ 1g
		integral_RMQ_XYZ / integral_1_1 ~ 1 
	1: active (large stdDev)
		large stdDev's
		rmqXYZ	>> 1g
		integral_RMQ_XYZ / integral_1_1 >> 1

bit 3-4-5: decision
	000: n/a
	if stand/sit
		001: standing still/sitting still
		010: walking
		011: running
	if lying
		100: lying still
		101: 'active' lyingtoStringFeature
		*/
typedef int posture;

Feature * toFeature(LinkedList * subSignal, Feature * buf);

Feature * printFeature(Feature * feature);

/*
Analyse the subsignal windows.
*/
posture analyse(Feature * feature);


/** End Section Analysis **/

