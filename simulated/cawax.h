/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>

Many code patterns in this project are thanks to the openmovement project of Newcastle University. 
See: https://github.com/digitalinteraction/openmovement for more details.
*/

#pragma once

#define NOT_IMPLEMENTED 0

#define NOT_IMPLEMENTED_PTR NULL

#define DEFAULT_PARAMETER_VALUE  -160711

#define INVALID_NEGATIVE_PARAM(param) (((param) < 0) && ((param) != DEFAULT_PARAMETER_VALUE))

#define TERMINATION_VALUE  0x1111

#define SAMPLE_BYTES_STRING 45

#define SAMPLE_COMPONENTS_COUNT 5 // time, order, x, y, z

#define SAMPLE_LONGEST_TOKEN_SIZE 11 // longest token in the sample string

/*
TIME def (22 bits) of a sample extracted from the output file including:
 - Minute
 - Second
 - Milisecond
*/
typedef long TIME_MSM;
#define TIME_FROM_MSM(minute, second, milisec) \
		(((TIME_MSM)(minute)   << 16)) \
	   |(((TIME_MSM)(second)   << 10)) \
	   |(((TIME_MSM)(milisec)       )) 
#define TIME_GET_MINUTE(time_msm)		((unsigned int) (((time_msm) >> 16) & 0x1f ))
#define TIME_GET_SECOND(time_msm)		((unsigned int) (((time_msm) >> 10) & 0x1f ))
#define TIME_GET_MILISECOND(time_msm)	((unsigned int) (((time_msm)	  ) & 0x3ff))



