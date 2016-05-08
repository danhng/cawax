/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>

Many code patterns in this project are thanks to the openmovement project of Newcastle University.
See: https://github.com/digitalinteraction/openmovement for more details.
*/

#include "cawax.h"

int main(int argc, char *argv[]) {
	char * userName = (char *)malloc(100);
	if (argc == 1) {
		printf(DEBUG_HEADER_WARN"Username not specified. Default to Michael Williamson.\n");
		strcpy(userName, "DF");
	}
	else {
		strcpy(userName, argv[1]);
	}
	int linesRead = 0;
	
	char * trainFile = (char *)malloc(100);
	char * testFile = (char *)malloc(100);
	char * testDataFile = (char *)malloc(100);


	strcat(strcpy(trainFile, userName), "_Train.csv");
	strcat(strcpy(testFile, userName), "_Test.csv");
	strcat(strcpy(testDataFile, userName), "_Test.data");

	LinkedList * signal = readFile(trainFile, 10, &linesRead);
	exportToC4_5Data(TrainDataStemFile, processSignal(signal), FALSE, FALSE);

	LinkedList * testData = readFile(testFile, 10, &linesRead);
	exportToC4_5Data(testDataFile, processSignal(testData), TRUE, TRUE);
	
	freeLinkedList(signal);
	free(testDataFile);
	free(trainFile);
	free(testFile);
	return 0;
}