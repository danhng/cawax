/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>

Many code patterns in this project are thanks to the openmovement project of Newcastle University.
See: https://github.com/digitalinteraction/openmovement for more details.
*/

#include "cawax_datastructs.h"
#include "cawax_maths.h"
#include "cawax_datareader.h"
#include <stdlib.h>
#include <stdio.h>

int testDatastruct();
int testMem();
int testRootMeanSquare();
int testMean();
int testStandardDeviation();
int testIntegral();
int testTimeFormat();
int testCountLines(const char * filename);
int testReadFile(const char * filename);
int testTimeDiff(CAWAX_TIME_MSM arg1, CAWAX_TIME_MSM arg2);
int testTrapezoid(acc sample1, acc sample2, CAWAX_TIME_MSM time1, CAWAX_TIME_MSM time2, sample_th order1, sample_th order2);

int main(void) {
	//testMem();
	//testRootMeanSquare();
	//testMean();
	//testStandardDeviation();
	//testTimeFormat();
	//testCountLines("saw10.csv");
	//testReadFile("saw10.csv");
	//testTimeDiff(1959312, 1981640);
	 
	testTrapezoid(0.494282, -0.48618, 1959312, 1959312 + 500, 1, 2);
	getchar();
}

int testTrapezoid(acc sample1, acc sample2, CAWAX_TIME_MSM time1, CAWAX_TIME_MSM time2, sample_th order1, sample_th order2) {
	printf("Testing trapezoid \n");
	printf("Sample 1: %.6f / %d / %d\n", sample1, time1, order1);
	printf("Sample 2: %.6f / %d / %d\n", sample2, time2, order2);

	printf("Trapezoid = %.6f\n", trapezoid(sample1, sample2, time1, time2, order1, order2));
}

int testTimeDiff(CAWAX_TIME_MSM arg1, CAWAX_TIME_MSM arg2) {
	printf("time1: %d\ntime2: %d\ndiff in mili: %d\n", arg1, arg2, cawaxTimeDiff(arg1, arg2));
}

int testReadFile(const char * filename) {
	int linesRead = 0;
	LinkedList * signal = readFile(filename, 10, &linesRead);
	toStringList(signal);
}

int testCountLines(const char * filename) {
	countLines(filename);
}

int testTimeFormat() {
	CAWAX_TIME_MSM time = CAWAX_TIME_FROM_MSM(29, 30, 100);
	printf("time 29:30.100 is: %d\n", time);
	printf("min is: %d\n", CAWAX_TIME_GET_MINUTE(time));
	printf("sec is: %d\n", CAWAX_TIME_GET_SECOND(time));
	printf("msec is: %d\n", CAWAX_TIME_GET_MILISECOND(time));
}

int testIntegral() {
	acc input[] = { 0.993441002 ,0.995749657,0.991568706,0.999612456,0.996470243 ,0.99319127
		,0.997306302
		,0.995463142
		,1.000453386
		,0.994225109
		,0.999750937
		,0.999913291
		,0.997179535
		,0.99589942
		,0.997915245
		,0.995205669
		,1.002247088
		,0.99526916
		,0.994763131
		,0.996424306
		,0.993584601
		,0.997457621
		,0.993317457
		,0.991386469
		,0.993776789
		, TERMINATION_VALUE
	};
		//printf("Integral of input: %p: %.3f", input, simpson(input, 1, ))
		
	return;
}

int testStandardDeviation() {
	acc vals[] = { 3, 4, 7, 10, TERMINATION_VALUE };
	printf("StdDev: %.3f", standardDeviation(vals, -1));
}

int testMean() {
	acc vals[] = {1, 2, 3, 4, TERMINATION_VALUE};
	printf("Mean: %.3f", mean(vals, -1));

}

int testRootMeanSquare() {
	Sample * sample = (Sample *)malloc(sizeof(Sample));
	sample->x = 0.1;
	sample->y = 0.2;
	sample->z = 0.3;
	toStringSample(sample);
	printf("Root mean square: %.3f", rootMeanSquare(sample));
	free(sample);
}

int testMem() {
	int i = 3;
	int * i_ptr = &i;
	uintptr_t a = (uintptr_t)i_ptr;
	int * b = (int *)a;

	printf("b is: %p, i_ptr is: %p", b, i_ptr);
	*b = 4;
	printf("using b to change i: %d", i == 4);
	getchar();
}

int testDatastruct() {

	Sample * sample = (Sample *)malloc(sizeof(Sample));
	sample->x = 0.1;
	sample->y = 0.2;
	sample->z = 0.3;
	LinkedList * list = makeList();
	printf("APPENDING TEST...\n");
	for (int i = 0; i < 100; i++) {
		add(*sample, list);
	}
	toStringList(list);
	printf("\n");
	printf("SANDWICHING TEST...\n");
	for (int i = 0; i < 100; i++) {
		addI(i, *sample, list);
	}
	toStringList(list);

	printf("Getting x series...\n");

	acc * xs = getSubSeries(X, 0, list->count - 1, list);
	acc * current = xs;
	while ((long)(*(current++)) != TERMINATION_VALUE) {
		printf("next: %2.3f\n", *current);
	}

	puts("Finished.");
	getchar();

}