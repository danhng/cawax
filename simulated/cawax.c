#include "cawax.h"
#include <stdio.h>

char * cawaxTimeToString(const CAWAX_TIME_MSM numericTime, char * buf)
{
	 sprintf(buf, "%d:%d.%d", CAWAX_TIME_GET_MINUTE(numericTime), CAWAX_TIME_GET_SECOND(numericTime), CAWAX_TIME_GET_MILISECOND(numericTime));
	 return buf;
}

long cawaxTimeDiff(CAWAX_TIME_MSM arg1, CAWAX_TIME_MSM arg2)
{
	char time1[10];
	char time2[10];
	printf("arg1 time: %s, arg2 time: %s\n", cawaxTimeToString(arg1, time1), cawaxTimeToString(arg2, time2));
	if (arg1 == arg2)
		return 0;
	int sign_factor = (arg1 < arg2) ? 1 : -1;
	if (arg2 < arg1) {
		CAWAX_TIME_MSM tmp = arg1;
		arg1 = arg2;
		arg2 = tmp;
	}
	int minDif = CAWAX_TIME_GET_MINUTE(arg2) - CAWAX_TIME_GET_MINUTE(arg1);
	int secDif = CAWAX_TIME_GET_SECOND(arg2) - CAWAX_TIME_GET_SECOND(arg1);
	int miliDif = CAWAX_TIME_GET_MILISECOND(arg2) - CAWAX_TIME_GET_MILISECOND(arg1);

	return sign_factor * (
						  (CAWAX_TIME_GET_MINUTE(arg2)		- CAWAX_TIME_GET_MINUTE(arg1))		* 60000 
						+ (CAWAX_TIME_GET_SECOND(arg2)		- CAWAX_TIME_GET_SECOND(arg1))		* 1000 
						+ (CAWAX_TIME_GET_MILISECOND(arg2)	- CAWAX_TIME_GET_MILISECOND(arg1))		);
}
