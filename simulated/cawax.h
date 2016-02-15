/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>

Many code patterns in this project are thanks to the openmovement project of Newcastle University. 
See: https://github.com/digitalinteraction/openmovement for more details.
*/
#pragma once

#include <math.h>

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
TIME def (22 bits) of a sample extracted from the output file including:
 - Minute
 - Second
 - Milisecond
*/
typedef long CAWAX_TIME_MSM;
#define CAWAX_TIME_FROM_MSM(minute, second, milisec) \
		(((CAWAX_TIME_MSM)(minute)   << 16)) \
	   |(((CAWAX_TIME_MSM)(second)   << 10)) \
	   |(((CAWAX_TIME_MSM)(milisec)       )) 
#define CAWAX_TIME_GET_MINUTE(time_msm)		((unsigned int) (((time_msm) >> 16) & 0x3f ))
#define CAWAX_TIME_GET_SECOND(time_msm)		((unsigned int) (((time_msm) >> 10) & 0x3f ))
#define CAWAX_TIME_GET_MILISECOND(time_msm)	((unsigned int) (((time_msm)	  ) & 0x3ff))

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




