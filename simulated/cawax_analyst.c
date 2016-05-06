#include "cawax.h"
void exportToC4_5Data_Inner(const char* filename, FeaturedWindow * buf, BOOLEAN comparedAgainstParticularClass, int classOrder);
void produceNamestem(const char * namestem);

void produceCawaxNamestem() ;

/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>
Many code patterns in this project are thanks to the openmovement project of Newcastle University.
See: https://github.com/digitalinteraction/openmovement for more details.

cawax_analyst.c
*/

long action_start_sample(int action_order) {
	return (((action_order) - 1) * TOTAL_TIME_FOR_ACTION + CALIBRATION_TIME_SECOND + NOISE_SECOND) * F_HZ;
}
long action_end_sample(int action_order) {
	return ((action_order) * TOTAL_TIME_FOR_ACTION - NOISE_SECOND) * F_HZ;
}

FeaturedWindow *toFeaturedWindow(Signal *signal, node_index start, node_index end, FeaturedWindow *buf) {

	buf->samples = signal;
	buf->windowStart = start;
	buf->windowEnd = end;

	//printf("buf of feature is: %p, buf of lis is %p\n", buf->samples, signal);
	buf->stdDev_Y = standardDeviation(getSubSeries(Y, start, end, signal), start - end + 1);
	buf->stdDev_RMQ_XYZ = standardDeviation(getSubSeries(RMQ, start, end, signal), start - end + 1);
	buf->mean_Y = mean(getSubSeries(Y, start, end, signal), start - end + 1);
	buf->mean_RMQ = mean(getSubSeries(RMQ, start, end, signal), start - end + 1);
	vel_g * integralBuf = (vel_g *) calloc(2, sizeof(vel_g));
	simpson(buf->samples, (sample_th) start, (sample_th) (end - start), STEP_SIZE(1), BASE_0, DO_RECOVER, UNIT_METER, UNIT_MILLISEC_TO_MICRO, CINDEX_Y | CINDEX_RMQ, 2, integralBuf);
	buf->integral_Y = integralBuf[0];
	buf->integral_RMQ_XYZ = integralBuf[1];
	buf->class = ORDER_N_A;
	//printf("subsignal buf is: %p\n", buf->samples);
	free(integralBuf);
	return buf;
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
	printf("start sample index: %li\n", featuredWindow->windowStart);
	printf("end sample index: %li\n", featuredWindow->windowEnd);
	printf("mean y: %.3f\n", featuredWindow->mean_Y);
	printf("standard dev y: %.3f\n", featuredWindow->stdDev_Y);
	printf("standard dev rmq xyz: %.3f\n", featuredWindow->stdDev_RMQ_XYZ);
	printf("integration y: %.3f\n", featuredWindow->integral_Y);
	printf("integration rmq xyz: %.3f\n", featuredWindow->integral_RMQ_XYZ);
	printf("class: %s\n", CLASS_ORDER_TO_STRING(featuredWindow->class));
	printf("****************************************************************************\n");
	return featuredWindow;
}

posture analyseFeaturedWindow(FeaturedWindow* featuredWindow)
{
	printFeaturedWindow(featuredWindow);
	//TODO CLASSIFIER

	return NOT_IMPLEMENTED;
}


FeaturedWindow * processSignal(Signal *signal) {

	if (tail(signal)->sample.order < action_end_sample(ORDER_LIE)) {
		printf(DEBUG_HEADER_WARN"Insufficient data set to learn all required classes.\n");
	}

	Node * head = signal->head;
	Node * tail = jump(head, (signal->count) - 1);

	// initialise indices
	sample_th windowStartIndex = 0;
	sample_th windowEndIndex = WINDOW_SIZE_SAMPLES;
	if (windowEndIndex > signal->count - 1)
	{
		printf(DEBUG_HEADER_WARN"Insufficient window. Aborting...\n");
		//todo wait for buf (threading)
		exit(0);
	}

	FeaturedWindow *headBuf = NULL;
	FeaturedWindow *currentBuf = headBuf;

	while (windowEndIndex - windowStartIndex == WINDOW_SIZE_SAMPLES) {

		FeaturedWindow * featuredWindow = (FeaturedWindow *) malloc(sizeof(FeaturedWindow));

		printf(DEBUG_HEADER_WARN"windowsStartIndex: %li, windowEndIndex: %li\n", windowStartIndex, windowEndIndex);
		// analyse window
		FeaturedWindow * temp = currentBuf;
		currentBuf = toFeaturedWindow(signal, windowStartIndex, windowEndIndex, featuredWindow);
		analyseFeaturedWindow(featuredWindow);

		if (windowStartIndex == 0) {
			headBuf = featuredWindow;
		}
		else
			temp->next =currentBuf;

		//update window indices
		windowStartIndex += NEXT_WINDOW_SAMPLES;
		windowEndIndex = windowStartIndex + WINDOW_SIZE_SAMPLES;
		if (windowEndIndex > signal->count - 1 || windowStartIndex > signal->count - 1)
		{
			printf(DEBUG_HEADER_WARN"Insufficient window. Aborting...\n");
			//todo wait for buf (threading)
			currentBuf->next = NULL;
			break;
		}
	}
	//free(featuredWindow);
	return headBuf;
};

void exportToC4_5Data(const char * filename, FeaturedWindow * buf, BOOLEAN isTest, BOOLEAN namestemsReady) {
	assignTrainedClass(buf, TRUE);

	//export .names file
	if (!namestemsReady) {
		produceCawaxNamestem();
		produceNamestem("stand.names");
		produceNamestem("walk.names");
		produceNamestem("lie.names");
	}
	// export .data files
	exportToC4_5Data_Inner(isTest ? "cawax_test.data" : "cawax.data", buf, FALSE, 0);
	exportToC4_5Data_Inner(isTest ? "stand_test.data" : "stand.data", buf, TRUE, ORDER_STAND);
	exportToC4_5Data_Inner(isTest? "lie_test.data" : "lie.data", buf, TRUE, ORDER_LIE);
	exportToC4_5Data_Inner(isTest ? "walk_test.data" : "walk.data", buf, TRUE, ORDER_WALK);
}

void exportToFullC4_5(const char * username) {
	int linesRead = 0;
	char * filename_Train = (char *) malloc(50);
	char * filename_Test = (char *) malloc(50);

	strcpy(filename_Train, username);
	strcpy(filename_Test, username);
	LinkedList * signal = readFile(strcat(filename_Train, "_Train.csv"), 10, &linesRead);
	exportToC4_5Data(TrainDataStemFile, processSignal(signal), FALSE, FALSE);

	LinkedList * testData = readFile(strcat(filename_Train, "_Test.csv"), 10, &linesRead);
	exportToC4_5Data(TestDataStemFile, processSignal(testData), TRUE, TRUE);
}

void exportToC4_5Data_Inner(const char* filename, FeaturedWindow * buf, BOOLEAN comparedAgainstParticularClass, int classOrder) {
	int count = 0;
	//assign Train class, important
	//assignTrainedClass(buf, TRUE);

	printf(DEBUG_HEADER_INFO"Exporting datafilestem to %s... \n", filename);
	FeaturedWindow *current = buf;
	FILE *fd = fopen(filename, "w");
	if (!fd) {
		fprintf(stderr, DEBUG_HEADER_ERR"Cannot open file %s for writing.\n", filename);
		exit(EXIT_FAILURE);
	}

	while (current) {
		count++;
		if (current->class != ORDER_N_A)
			fprintf(fd, "%s\n", produceTrainDataFeatureString(current, comparedAgainstParticularClass, classOrder));
		current = current->next;
	}
	fclose(fd);

}

void writeAttributes(FILE * fd) {
	fprintf(fd, "%s\n", "meanY : continuous.");
//	fprintf(fd, "%s\n", "meanRMQ: continuous.");
	fprintf(fd, "%s\n", "sdY : continuous.");
	fprintf(fd, "%s\n", "sdRMQ : continuous.");
	fprintf(fd, "%s\n", "integralY : continuous.");
	fprintf(fd, "%s\n", "integralRMQ : continuous.");
}

void produceCawaxNamestem() {
	char* name = "cawax.names";
	FILE *fd = fopen(name, "w");
	if (!fd) {
		fprintf(stderr, DEBUG_HEADER_ERR"Cannot open file %s for writing.\n", name);
		exit(EXIT_FAILURE);
	}
	fprintf(fd, "%s\n", "stand, walk, lie.");
	writeAttributes(fd);

	fclose(fd);
}

void produceNamestem(const char * namestem) {
	FILE *fd = fopen(namestem, "w");
	if (!fd) {
		fprintf(stderr, DEBUG_HEADER_ERR"Cannot open file %s for writing.\n", namestem);
		exit(EXIT_FAILURE);
	}
	fprintf(fd, "%s\n", "true, false .");
	writeAttributes(fd);
	fclose(fd);

}

void assignTrainedClass(FeaturedWindow * buf, BOOLEAN override){
	printf(DEBUG_HEADER_INFO"Classifying features...\n");
	FeaturedWindow * current = buf;
	while (current) {
		//printFeaturedWindow(current);
		node_index start = current->windowStart;
		node_index end = current->windowEnd;
		int startClass = classifyTarget(current->windowStart);
		int endClass = classifyTarget(current->windowEnd);
		printf("!!!!start %d, end %d\n", startClass, endClass);
		// if window is in to-be-classified interval
		if ( startClass != ORDER_N_A && (startClass == endClass))
		{
			//printf(DEBUG_HEADER_INFO"whoops. inside a class interval: %i\n", startClass);
			// if no class is assigned for this window yet...
			if (current->class == ORDER_N_A)
				current->class = startClass;
				// else override needs considering
			else if (override)
					current->class = startClass;
		}
		current = current->next;
	}
	printf(DEBUG_HEADER_INFO"Finished classifying features.\n");
}

BOOLEAN isSampleToBeClassifiedAsActionX(node_index index, int actionOrder) {
	printf("index: %li, action order start: %li, end: %li\n", index, action_start_sample(actionOrder), action_end_sample(actionOrder));
	if (actionOrder >= ORDER_N_A)
	{
		printf(DEBUG_HEADER_WARN"%i is not a good index to classify.\n", actionOrder);
		return FALSE;
	}
	if (index >= action_start_sample(actionOrder) && index <= action_end_sample(actionOrder))
		return TRUE;
	return FALSE;
}

int classifyTarget(node_index index) {
	if (isSampleToBeClassifiedAsActionX(index, ORDER_STAND))
		return ORDER_STAND;
	if (isSampleToBeClassifiedAsActionX(index, ORDER_WALK))
		return ORDER_WALK;
	if (isSampleToBeClassifiedAsActionX(index, ORDER_LIE))
		return ORDER_LIE;
	return ORDER_N_A;
}



char * produceTrainDataFeatureString(FeaturedWindow *window, BOOLEAN comparedAgainstSomeClass, int classOrder) {
	printFeaturedWindow(window);
	char * string = (char*) malloc(100);
	char * numBuf = (char *) malloc(100);
//	acc mean_x;
//	acc mean_RMQ;
//	acc stdDev_X;
//	acc stdDev_RMQ_XYZ;
//	acc integral_X;
//	acc integral_RMQ_XYZ;

	// get the mean rmq
	sprintf(numBuf, "%.3f", window->mean_Y);
	strcpy(string, numBuf);
	// append the separator
	strcat(string, ", ");

//	sprintf(numBuf, "%.3f", window->mean_RMQ);
//	strcat(string, numBuf);
//	// append the separator
//	strcat(string, ", ");

	sprintf(numBuf, "%.3f", window->stdDev_Y);
	strcat(string, numBuf);
	// append the separator
	strcat(string, ", ");

	sprintf(numBuf, "%.3f", window->stdDev_RMQ_XYZ);
	strcat(string, numBuf);
	// append the separator
	strcat(string, ", ");

	sprintf(numBuf, "%.3f", window->integral_Y);
	strcat(string, numBuf);
	// append the separator
	strcat(string, ", ");

	// get the integration rmq
	sprintf(numBuf, "%.3f", window->integral_RMQ_XYZ);
	strcat(string, numBuf);
	// append the separator
	strcat(string, ", ");

	if (!comparedAgainstSomeClass)
	// append the class into the train data
		strcat(string, CLASS_ORDER_TO_STRING(window->class));
	else
		strcat(string, window->class == classOrder ? "true" : "false");

	printf("string produced for feature: %s\n", string);


	return string;
}


void action(posture *p) {
};

void turnOffGyro() {

};

void turnOnGyro() {

}
