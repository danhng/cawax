#include "cawax_analyst.h"
#include <stdio.h>
#include <stdlib.h>

/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>
Many code patterns in this project are thanks to the openmovement project of Newcastle University.
See: https://github.com/digitalinteraction/openmovement for more details.

cawax_analyst.c
*/

Feature * toFeature(LinkedList * subSignal, Feature * buf)
{

	buf->subSignal = subSignal;
	printf("buf of feature is: %p, buf of lis is %p\n", buf->subSignal, subSignal);
	buf->stdDev_X = standardDeviation(getSubSeries(X, 0, subSignal->count - 1, subSignal), subSignal->count);
	buf->stdDev_RMQ_XYZ = standardDeviation(getSubSeries(RMQ, 0, subSignal->count - 1, subSignal), subSignal->count);
	buf->mean_x = mean(getSubSeries(X, 0, subSignal->count - 1, subSignal), subSignal->count);
	vel_g * integralBuf = (vel_g *) calloc(2, sizeof(vel_g));
	simpson(buf->subSignal, STEP_SIZE(1), BASE_0, DO_RECOVER, UNIT_METER, UNIT_SECOND_TO_MICRO, CINDEX_X | CINDEX_RMQ, 2, integralBuf);
	buf->integral_X = integralBuf[0];
	buf->integral_RMQ_XYZ = integralBuf[1];
	printf("subsignal buf is: %p\n", buf->subSignal);
	//free(integralBuf);
}

Feature * printFeature(Feature * feature)
{
	char time1[INTERNAL_TIME_ASCII_BYTES];
	char time2[INTERNAL_TIME_ASCII_BYTES];
	Node * head = feature->subSignal->head;
	printf("Subsignal from %s to %s...\n", cawaxInternalTimeToString(head->sample.time, time1),
		cawaxInternalTimeToString(jump(head, feature->subSignal->count - 1), time2));
	printf("mean X: %.3f\n", feature->mean_x);
	printf("standard dev x: %.3f\n", feature->stdDev_X);
	printf("standard dev rmq xyz: %.3f\n", feature->stdDev_RMQ_XYZ);
	printf("integration x: %.3f\n", feature->integral_X);
	printf("integration rmq xyz: %.3f\n", feature->integral_RMQ_XYZ);
}

posture analyse(Feature * feature)
{
	printFeature(feature);

	//todo split into windows of 0.5s overlap
}
