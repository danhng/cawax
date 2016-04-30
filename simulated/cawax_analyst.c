#include "cawax.h"

/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>
Many code patterns in this project are thanks to the openmovement project of Newcastle University.
See: https://github.com/digitalinteraction/openmovement for more details.

cawax_analyst.c
*/

FeaturedWindow *toFeaturedWindow(Signal *signal, node_index start, node_index end, FeaturedWindow *buf) {

	buf->samples = signal;
	buf->windowStart = start;
	buf->windowEnd = end;

	//printf("buf of feature is: %p, buf of lis is %p\n", buf->samples, signal);
	buf->stdDev_X = standardDeviation(getSubSeries(X, start, end, signal), start - end + 1);
	buf->stdDev_RMQ_XYZ = standardDeviation(getSubSeries(RMQ, start, end, signal), start - end + 1);
	buf->mean_x = mean(getSubSeries(X, start, end, signal), start - end + 1);
	vel_g * integralBuf = (vel_g *) calloc(2, sizeof(vel_g));
	simpson(buf->samples, (sample_th) start, (sample_th) (end - start), STEP_SIZE(1), BASE_0, DO_RECOVER, UNIT_METER, UNIT_SECOND_TO_MICRO, CINDEX_X | CINDEX_RMQ, 2, integralBuf);
	buf->integral_X = integralBuf[0];
	buf->integral_RMQ_XYZ = integralBuf[1];
	//printf("subsignal buf is: %p\n", buf->samples);
	free(integralBuf);
}

FeaturedWindow *printFeaturedWindow(FeaturedWindow *featuredWindow)
{
	char time1s[INTERNAL_TIME_ASCII_BYTES];
	char time2s[INTERNAL_TIME_ASCII_BYTES];

	Node * windowStartNode = get(featuredWindow->windowStart, featuredWindow->samples);
	Node * windowEndNode = get(featuredWindow->windowEnd, featuredWindow->samples);
	INTERNAL_TIME time1 = windowStartNode->sample.time;
	INTERNAL_TIME time2 = windowEndNode->sample.time;
	printf("****************************************************************************\n"
				   "Analysing window from %s to %s [%.3f milliseconds]...\n"
				  , cawaxInternalTimeToString(time1, time1s),
		   cawaxInternalTimeToString(time2, time2s),
		   cawaxInternalTimeDiff(time1, time2, UNIT_MILLISEC_TO_MICRO));
	printf("mean X: %.3f\n", featuredWindow->mean_x);
	printf("standard dev x: %.3f\n", featuredWindow->stdDev_X);
	printf("standard dev rmq xyz: %.3f\n", featuredWindow->stdDev_RMQ_XYZ);
	printf("integration x: %.3f\n", featuredWindow->integral_X);
	printf("integration rmq xyz: %.3f\n", featuredWindow->integral_RMQ_XYZ);
	printf("****************************************************************************\n");
}

posture analyseFeaturedWindow(FeaturedWindow* featuredWindow)
{
	printFeaturedWindow(featuredWindow);
	//TODO CLASSIFIER

	return NOT_IMPLEMENTED;
}


void processSignal(Signal *signal) {
	Node * head = signal->head;
	Node * tail = jump(head, (signal->count) - 1);

	// initialise indices
	sample_th windowStartIndex = 0;
	sample_th windowEndIndex = SAMPLES_TO_JUMP_WINDOW_END;
	if (windowEndIndex > signal->count - 1)
	{
		printf(DEBUG_HEADER_WARN"Insufficient window. Aborting...");
		//todo wait for buf (threading)
		exit(0);
	}

	FeaturedWindow * featuredWindow = (FeaturedWindow *) malloc(sizeof(FeaturedWindow));

	while (windowEndIndex - windowStartIndex == SAMPLES_TO_JUMP_WINDOW_END) {
		printf(DEBUG_HEADER_WARN"windowsStartIndex: %li, windowEndIndex: %li\n", windowStartIndex, windowEndIndex);
		// analyse window
		toFeaturedWindow(signal, windowStartIndex, windowEndIndex, featuredWindow);
		analyseFeaturedWindow(featuredWindow);

		//update window indices
		windowStartIndex += SAMPLES_TO_JUMP_NEXT_WINDOW;
		windowEndIndex = windowStartIndex + SAMPLES_TO_JUMP_WINDOW_END;
		if (windowEndIndex > signal->count - 1 || windowStartIndex > signal->count - 1)
		{
			printf(DEBUG_HEADER_WARN"Insufficient window. Aborting...");
			//todo wait for buf (threading)
			break;
		}
	}
	free(featuredWindow);
}

void action(posture *p) {
}

void turnOffGyro() {

}

void turnOnGyro() {

}
