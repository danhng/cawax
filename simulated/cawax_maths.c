/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>

Many code patterns in this project are thanks to the openmovement project of Newcastle University.
See: https://github.com/digitalinteraction/openmovement for more details.
*/

#include "cawax_maths.h"
#include <math.h>
#include <assert.h>
#include <stdlib.h>

/*
Calculate the trapezoidal area given sign identical sample values.
Return a negative value on failure.
*/
acc trapezoidPolarIdentical(acc h1, acc h2, double delta);

/*
Calculate the trapezoidal area given sign opposite sample values.
Return a negative value on failure.
*/
acc trapezoidPolarOpposite(acc h1, acc h2, double delta);

//TODO free
acc rootMeanSquare(Sample * sample)
{
	if (!sample) {
		printf("err: NULL ptr on sample. Return 0.");
		return 0;
	}
	return sqrt(sample->x * sample->x + sample->y * sample->y + sample->z * sample->z);
}

//TODO free
acc meanSample(Sample * sample)
{
	return (sample->x + sample->y + sample->z) / 3;
}

//TODO free
acc mean(acc * input, int count)
{
	printf("Calling mean at input: %p, count: %d\n", input, count);
	long i = 0;
	acc sum = 0;
	acc current = 0;
	while ((current = input[i]) != TERMINATION_VALUE && i != count) {
		sum += current;
		++i;
	}
	printf("Calling mean at input: %p, count: %d, final i: %d, final sum: %.3f\n", input, count, i, sum);
	return sum / i;
}

//TODO free
acc standardDeviation(acc * input, size_t count)
{
	printf("Calling StdDev at input: %p, count: %d\n", input, count);
	acc m = mean(input, count);
	
	long i = 0;
	acc sqrSum = 0;
	acc current = 0;
	while ((current = input[i]) != TERMINATION_VALUE && i != count) {
		sqrSum += pow(current - m, 2);
		++i;
	}
	return  sqrt(sqrSum / i);
}

/*
If both are negative
	sample1 = abs sample1;
	sample2 = abs sample2;
Order sample1 < sample2 based on time;
If both are positive
	return (sample2->time - sample1->time) * (sample2->val + sample1->val) / 2;
If one is negative and one is positive
	// see: https://github.com/danhng/cawax/support/trapezoid_diff_signs.JPG for simple proof
	return (a*a + b*b) * (sample2->time - sample1->time) / ( 2 * (abs sample1->val + abs sample2->val)) 
*/
acc trapezoid(acc sample1, acc sample2, CAWAX_TIME_MSM time1, CAWAX_TIME_MSM time2, sample_th order1, sample_th order2) {
	if (order1 == order2) {
		char time1String[10];
		char time2String[10];
		printf("Error while working out trapezoid: duplicated orders of %d with time values: %s, %s\n", order1, cawaxTimeToString(time1, time1String), cawaxTimeToString(time2, time2String));
		return -1;
	}

	// if both below the x axis
	if (sample1 < 0 && sample2 < 0) {
		sample1 = abs(sample1);
		sample2 = abs(sample2);
	}
	// time sort
	// since the numeric values can also be used to compare the times. We could use them directly.
	// since sample orders reflect chronological order too we could therefore use them.
	if (order2 < order1) {
		acc sample1_tmp = 0;
		CAWAX_TIME_MSM sample1Time_tmp = 0;
		sample1_tmp = sample1;
		sample1Time_tmp = time1;
		sample1 = sample2;
		time1 = time2;
		sample2 = sample1_tmp;
		time2 = sample1Time_tmp;
	}

	// if the code reaches this point then we have sample1 -> sample2 (based on sample order and time)
	assert((time1 < time2) && (order1 < order2));
	/*printf("Swapping done.\n");
	printf("sample 1: %.6f  /  %d  /%d\n", sample1, time1, order1 );
	printf("sample 2: %.6f  /  %d  /%d\n", sample2, time2, order2 );*/
	double delta = cawaxTimeDiff(time1, time2);
	if (sample1 >= 0 && sample2 >= 0)
		return trapezoidPolarIdentical(sample1, sample2, delta);
	else
		return trapezoidPolarOpposite(sample1, sample2, cawaxTimeDiff(time1, time2));
}

/* ===================================================================================
Supporting functions for calculating trapezoidal value
=====================================================================================*/
acc trapezoidPolarIdentical(acc h1, acc h2, double delta)
{
	if (h1 * h2 < 0) {
		printf("Calling trapezoidPolarIdentical error: h1 and h2's signs must be similar. %d and %d given.\n", h1, h2);
		return -1;
	}
	if (delta < 0) {
		printf("Calling trapezoidPolarIdentical warning: delta should be positive. %d given.\n", delta);
		delta = fabs(delta);
	}
	return (delta * (fabs(h1) + fabs(h2)) / 2);
}

acc trapezoidPolarOpposite(acc h1, acc h2, double delta)
{
	if (h1 * h2 > 0) {
		fprintf(stderr, "Calling trapezoidPolarOpposite error: h1 and h2 must be sign opposite. %d and %d given.\n", h1, h2);
		return -1;
	}
	if (delta < 0) {
		fprintf(stderr, "Calling trapezoidPolarOpposite warning: delta should be positive. %d given.\n", delta);
		delta = fabs(delta);
	}
	return (h1*h1 + h2*h2) * delta / (2 * (fabs(h1) + fabs(h2)));
}

/*
if base is negative 
	return;
//reprocess signal based on base
while (loop through all nodes)
	sample->target_value += base;
simpson = 0;
current signal = head of signal;
next signal = head of signal + step;
tail;
// TODO handle last pair (head signal in bound, next signal could be out of bound -> limit to the tail)
while (next signal <> tail) 
	tmp = current;
	while (next->time = current->time) // handle time duplicated samples
		tmp = (current + next) / 2;
		next = next + step;
	calculate the trapezoid (tmp, next signal, base);
	add the trapezoid to the simpson;
	current signal = next signal;
	if (tail - current signal) < step
		next signal = tail;
	else 
		next signal = current signal + step;
return simpson;
*/
vel_g * simpson(LinkedList * signal, size_t step, acc base, int inputTargets, vel_g * buf)
{
	if (base < 0) {
		fprintf(stderr, "Calling simpson needs non-negative base. %d given.\n", base);
		exit(EXIT_FAILURE);
	}
	
	//while ()



}



