#include "cawax.h"
#include <stdio.h>

char * cawaxTimeToString(const CAWAX_TIME_MSM numericTime, char * buf)
{
	 sprintf(buf, "%d:%d.%d", CAWAX_TIME_GET_MINUTE(numericTime), CAWAX_TIME_GET_SECOND(numericTime), CAWAX_TIME_GET_MILISECOND(numericTime));
	 return buf;
}
