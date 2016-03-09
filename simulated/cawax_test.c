/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>

Many code patterns in this project are thanks to the openmovement project of Newcastle University.
See: https://github.com/digitalinteraction/openmovement for more details.
*/

#include "cawax_include.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

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
int testGetComponent();
int testJump();
int testSimpsonSingle();
int testSimpson();
int testTimeInternal();
int test_toFeature();
int test_processSignal();

int main(void) {
	//testMem();
	//testRootMeanSquare();
	//testMean();
	//testStandardDeviation();
	//testTimeFormat();
	//testCountLines("saw10.csv");
	//testReadFile("saw10.csv");
	//testTimeDiff(1959312, 1981640);
	//testTrapezoid(0.494282, -0.48618, INTERNAL_TIME_FROM_S(314,568726), INTERNAL_TIME_FROM_S(314, 668732), 1, 2);
	//testGetComponent();
	//testJump();

	//testSimpsonSingle();
	//testSimpson();
	//testTimeInternal();

	//test_toFeature();
	test_processSignal();
	getchar();
}

int test_processSignal() {
	int linesRead = 0;
	LinkedList * signal = readFile("saw10.csv", 10, &linesRead);
	processSignal(signal);
	//freeLinkedList(signal);
}

int test_toFeature() {
	int linesRead = 0;
	LinkedList * signal = readFile("saw10.csv", 10, &linesRead);
	//toStringList(signal);
	FeaturedWindow * f = (FeaturedWindow *) malloc(sizeof(FeaturedWindow));
	toFeaturedWindow(signal, 0, 5, f);
	printFeaturedWindow(f);

}


int testSimpson() {
	int linesRead = 0;
	LinkedList * signal = readFile("saw10.csv", 10, &linesRead);
	//toStringList(signal);
	vel_g * buf = (vel_g *) malloc(sizeof(vel_g) * 2);
	simpson(signal, 0, signal->count - 1,1, 0, 0, UNIT_METER, UNIT_SECOND_TO_MICRO, CINDEX_X | CINDEX_RMQ, 2, buf);
	/*printf("After being processed signal: \n");
	toStringList(signal);*/
}

int testSimpsonSingle() {
	int linesRead = 0;
	LinkedList * signal = readFile("saw10.csv", 10, &linesRead);
	//toStringList(signal);
	vel_g * buf = malloc(sizeof(vel_g));
	simpsonSingle(signal, 0, signal->count - 1, 1, 0,  0, UNIT_METER, UNIT_SECOND_TO_MICRO, CINDEX_RMQ, buf);
	/*printf("After being processed signal: \n");
	toStringList(signal);*/
}

int testJump() {
	int linesRead = 0;
	LinkedList * signal = readFile("saw10.csv", 10, &linesRead);
	toStringList(signal);

	int jumpVar = 177;
	Node * jumpedForward = jump(signal->head, jumpVar);
	Node * jumpedPrev = jump(jumpedForward, jumpVar * -1);
	printf("start is: %p\njumpedForward by %d is: %p\njumpedPrev by %d is (should get back to start): %p\nVerification: %d\n", 
		signal->head, jumpVar, jumpedForward, jumpVar, jumpedPrev, jumpedPrev == signal->head);
}


int testGetComponent() {
	Sample sample = { 0, 0, 1, 2, 3, 4 };
	sample.x = 1;
	printf("rmq address is: %p\nrmq address from getComponent is: %p\n", &sample.x, getComponent(&sample, CINDEX_RMQ));
	acc * rmq_ptr = (acc *)getComponent(&sample, CINDEX_RMQ);
	printf("x val is: %.3f\n", *rmq_ptr);
	*rmq_ptr = 1;
	printf("new rmq changed using getComponent is (should be 1): %.3f\n", *rmq_ptr);
}

int testTrapezoid(acc sample1, acc sample2, CAWAX_TIME_MSM time1, CAWAX_TIME_MSM time2, sample_th order1, sample_th order2) {
	printf("Testing trapezoid \n");
	printf("Sample 1: %.6f / %li / %li\n", sample1, time1, order1);
	printf("Sample 2: %.6f / %li / %li\n", sample2, time2, order2);

	printf("Trapezoid = %.6f\n", trapezoid(sample1, sample2, time1, time2, order1, order2, UNIT_MILLISEC_TO_MICRO));
}

int testTimeDiff(CAWAX_TIME_MSM arg1, CAWAX_TIME_MSM arg2) {
	printf("time1: %li\ntime2: %li\ndiff in mili: %li\n", arg1, arg2, cawaxTimeDiff(arg1, arg2));
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
	printf("time 29:30.100 is: %li\n", time);
	printf("min is: %d\n", CAWAX_TIME_GET_MINUTE(time));
	printf("sec is: %d\n", CAWAX_TIME_GET_SECOND(time));
	printf("msec is: %d\n", CAWAX_TIME_GET_MILLISECOND(time));

}

int testTimeInternal() {
	INTERNAL_TIME time = INTERNAL_TIME_FROM_S(316, 68726);
	printf("time 316.068726 is: %li\n", time);
	printf("second is: %li\n", INTERNAL_TIME_GET_SECOND(time));
	printf("mili is: %d\n", INTERNAL_TIME_GET_MILLI(time));
	printf("micro is: %d\n", INTERNAL_TIME_GET_MICRO(time));
	INTERNAL_TIME time2 = INTERNAL_TIME_FROM_S(316, 168732);
	printf("time 316.168732 is: %li\n", time2);
	printf("diff is: %.3f\n", cawaxInternalTimeDiff(time, time2, UNIT_MILLISEC_TO_MICRO));
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
		
	return 0;
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