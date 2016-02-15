/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>

Many code patterns in this project are thanks to the openmovement project of Newcastle University. 
See: https://github.com/digitalinteraction/openmovement for more details.
*/
#pragma once

#include <math.h>

#define _CRT_SECURE_NO_WARNINGS

#define NOT_IMPLEMENTED 0

#define NOT_IMPLEMENTED_PTR NULL

#define DEFAULT_PARAMETER_VALUE  -160711

#define INVALID_NEGATIVE_PARAM(param) (((param) < 0) && ((param) != DEFAULT_PARAMETER_VALUE))

#define TERMINATION_VALUE  0x1111

#define SAMPLE_BYTES_STRING 45

#define SAMPLE_COMPONENTS_COUNT 5 // time, order, x, y, z

#define SAMPLE_COMPONENT_SIZE 15 // number of bytes needed to store each token (in string format)

#define SAMPLE_LONGEST_TOKEN_SIZE 11 // longest token in the sample string




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




