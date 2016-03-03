#include "cawax.h"
#include <stdio.h>

int CINDICES[] = { CINDEX_X, CINDEX_Y, CINDEX_Z, CINDEX_RMQ, CINDEX_MEAN, CINDEX_ORDER, CINDEX_TIME };
int CINDICES_COUNT = sizeof(CINDICES) / sizeof(CINDICES[0]);

char * cawaxTimeToString(const CAWAX_TIME_MSM numericTime, char * buf)
{
	sprintf(buf, "%d:%d.%d", CAWAX_TIME_GET_MINUTE(numericTime), CAWAX_TIME_GET_SECOND(numericTime), CAWAX_TIME_GET_MILLISECOND(numericTime));
	return buf;
}

long cawaxTimeDiff(CAWAX_TIME_MSM arg1, CAWAX_TIME_MSM arg2)
{
	char time1[10];
	char time2[10];
	//printf("arg1 time: %s, arg2 time: %s\n", cawaxTimeToString(arg1, time1), cawaxTimeToString(arg2, time2));
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
	int miliDif = CAWAX_TIME_GET_MILLISECOND(arg2) - CAWAX_TIME_GET_MILLISECOND(arg1);

	return sign_factor * (
		(minDif) * 60000
		+ (secDif) * 1000
		+ (miliDif));
}

char * cawaxInternalTimeToString(const INTERNAL_TIME numericTime, char * buf) {
	sprintf(buf, "%d:%d", INTERNAL_TIME_GET_SECOND(numericTime), INTERNAL_TIME_GET_MICRO(numericTime));
	return buf;
}

/*
Return the difference (in unit of choice) of time arg2 to arg1 (how can arg1 get to arg2)
Default is microsecond
*/
double cawaxInternalTimeDiff(INTERNAL_TIME arg1, INTERNAL_TIME arg2, int unitToMicro) {
	if (arg1 == arg2)
		return 0;
	int sign_factor = (arg1 < arg2) ? 1 : -1;
	if (arg2 < arg1) {
		INTERNAL_TIME tmp = arg1;
		arg1 = arg2;
		arg2 = tmp;
	}
	int secDiff = INTERNAL_TIME_GET_SECOND(arg2) - INTERNAL_TIME_GET_SECOND(arg1);
	int microDiff = INTERNAL_TIME_GET_MICRO(arg2) - INTERNAL_TIME_GET_MICRO(arg1);

	// todo add 0.5 to get correctly rounded result?
	return (double) sign_factor * (secDiff * 1000000 + microDiff) / unitToMicro ;

}
