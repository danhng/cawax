/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>

Many code patterns in this project are thanks to the openmovement project of Newcastle University.
See: https://github.com/digitalinteraction/openmovement for more details.
*/

#include "cawax.h"

extern const int CINDICES[];
extern const int CINDICES_COUNT;

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
acc mean(acc * input, node_index count)
{
	//printf("Calling mean at input: %p, count: %d\n", input, count);
	long i = 0;
	acc sum = 0;
	acc current = 0;
	while ((current = input[i]) != TERMINATION_VALUE && i != count) {
		sum += current;
		++i;
	}
	//printf("Calling mean at input: %p, count: %d, final i: %d, final sum: %.3f\n", input, count, i, sum);
	return sum / i;
}

/**
Calculate the SD of a sub series from the signal.
This would help figure out how acceleration values fluctuated over time thus
potentially giving us a useful view on the properties of the movement.
*/
//TODO free
acc standardDeviation(acc * input, size_t count)
{
	//printf("Calling StdDev at input: %p, count: %d\n", input, count);
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
Calculate the area of the trapezoidal shape made up from 2 sample points and their corresponding projections

PSEUDO: 
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
acc trapezoid(acc sample1, acc sample2, INTERNAL_TIME time1, INTERNAL_TIME time2, sample_th order1, sample_th order2, int unitToMicro) {
	if (order1 == order2) {
		char time1String[INTERNAL_TIME_ASCII_BYTES];
		char time2String[INTERNAL_TIME_ASCII_BYTES];
		printf("Error while working out trapezoid: duplicated orders of %d with time values: %s, %s\n", order1, cawaxInternalTimeToString(time1, time1String), cawaxInternalTimeToString(time2, time2String));
		return -1;
	}

	// if both below the x axis
	if (sample1 < 0 && sample2 < 0) {
		sample1 = fabs(sample1);
		sample2 = fabs(sample2);
	}
	// time sort
	// since the numeric values can also be used to compare the times. We could use them directly.
	// since sample orders reflect chronological order too we could therefore use them.
	if (order2 < order1) {
		acc sample1_tmp = 0;
		INTERNAL_TIME sample1Time_tmp = 0;
		sample1_tmp = sample1;
		sample1Time_tmp = time1;
		sample1 = sample2;
		time1 = time2;
		sample2 = sample1_tmp;
		time2 = sample1Time_tmp;
	}

	// if the code reaches this point then we have sample1 -> sample2 (based on sample order and time)
	assert((time1 < time2) && (order1 < order2));
	//printf("Swapping done.\n");
	//printf("sample 1: %.6f  /  %d  /%d\n", sample1, time1, order1 );
	//printf("sample 2: %.6f  /  %d  /%d\n", sample2, time2, order2 );
	double delta = cawaxInternalTimeDiff(time1, time2, unitToMicro);
	//printf("delta in %d is: %.3f\n", unitToMicro, delta);
	if (sample1 >= 0 && sample2 >= 0)
		return trapezoidPolarIdentical(sample1, sample2, delta);
	else
		return trapezoidPolarOpposite(sample1, sample2, delta);
}

/* ===================================================================================
Supporting functions for calculating trapezoidal value
=====================================================================================*/
acc trapezoidPolarIdentical(acc h1, acc h2, double delta)
{
	if (h1 * h2 < 0) {
		printf(DEBUG_HEADER_INFO"Calling trapezoidPolarIdentical error: h1 and h2's signs must be similar. %.6f and %.6f given.\n", h1, h2);
		return -1;
	}
	if (delta < 0) {
		printf(DEBUG_HEADER_INFO"Calling trapezoidPolarIdentical warning: delta should be positive. %.3f given.\n", delta);
		delta = fabs(delta);
	}
	acc r = (delta * (fabs(h1) + fabs(h2)) / 2);
	//printf("trap%.6f\n", r);
	return r;
}

acc trapezoidPolarOpposite(acc h1, acc h2, double delta)
{
	if (h1 * h2 > 0) {
		fprintf(stderr, DEBUG_HEADER_INFO"Calling trapezoidPolarOpposite error: h1 and h2 must be sign opposite. %.6f and %.6f given.\n", h1, h2);
		return -1;
	}
	if (delta < 0) {
		fprintf(stderr, DEBUG_HEADER_INFO"Calling trapezoidPolarOpposite warning: delta should be positive. %.3f given.\n", delta);
		delta = fabs(delta);
	}
	acc r = (h1*h1 + h2*h2) * delta / (2 * (fabs(h1) + fabs(h2)));
	//printf("trap%.6f\n", r);
	return r;
}

/*
DESCRIPTION
Calculate the simpson (vel_g) of a sub time series using Trapezium rules with optional step value (default is 1)
controlling the accuracy level of the simpson
- Integral accuracy <-><-> Computation time
- See https://en.wikipedia.org/wiki/Trapezoidal_rule
- Note: This method does not support negative base at the moment.
- TODO: Wrapper methods needed.
Note: The simpson is caluclated based on acc values that are optionally substracted by 1g in an attempt to mask out the effect of gravity
and give the true movement data.

METHOD SIGNATURE
Parameters:
- LinkedList * signal: The signal containing samples to be analysed.
- size_t step: The step size (default is 1) drives how accurate the results would be.
- base: Optional postive base against which the integration is calculated. (default is 0).
- int inputTargets: which component(s) of the samples for which the integration is to be calculated.
- vel_g * buf: buffer for result(s)
+ result for each input inputTarget is put in the order as per the input targets (X -> Y -> Z -> RMQ -> M -> TIME -> ORDER)
Return:
- vel_g * : the buf array specified in the parameter;

PSEUDO CODE
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
while (current signal <> tail) 
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
vel_g * simpsonSingle(LinkedList * signal, sample_th start, sample_th count, size_t step, acc base, char recoverSignal, int gOrMeter, int unitToMicro, int inputTarget, vel_g * buf)
{
	//printf(DEBUG_HEADER_INFO"Calling simpsonSingle on input %i\n", inputTarget);
	if (base < 0) {
		//printf(DEBUG_HEADER_INFO"Calling simpson requires non-negative base. %d given.\n", base);
		return ERROR_PTR_OUTPUT;
	}

	if (signal->count == 0 || !(signal->head)) {
		printf(DEBUG_HEADER_ERR"Calling simpson requires non-empty signal. however either count is 0: %li or head is null: %p\n", signal->count, signal->head);
		return ERROR_PTR_OUTPUT;
	}

	// work out the starting point (start node)
	Node * current = get(start, signal);
	char changed = 0;

	// process all samples based on {base} parameter
	if (base != 0)
	{
		//printf(DEBUG_HEADER_INFO"Processing signal against base %.6f ...\n", base);
		//TODO: recover the signal later if recover Signal is set.
		changed = 1;
		while (current->next) {
			acc * target_ptr = (acc *)getComponent(&(current->sample), inputTarget);
			//TODO Wrapper function
			*target_ptr -= base;
			current = current->next;
		}
	}

	acc simpson = 0;
	current = get(start, signal);
	Node * next = jump(current, step);
	// work out the end node
	Node *endNode = jump(current, count);
	Node * tmp = NULL;
	//printf("endNode is: %p\n", endNode);

	while (current != endNode) {
		// get the tmp node
		tmp = makeNode(current->sample);
		// next and prev for tmp are not needed.

		//handling duplications (based on sample time)
		// duplicate counter
		int dups = 1;
		// initially dupsSum is the first encountered node
		acc * dupsSum = (acc *)getComponent(&(tmp->sample), inputTarget);
		while (next->sample.time == current->sample.time) {
			// increase the dups counter
			++dups;
			*dupsSum += *((acc *)getComponent(&(next->sample), inputTarget));
			next = jump(next, step);
		}
		*dupsSum /= dups;
		char timeBuf[INTERNAL_TIME_ASCII_BYTES];
		//printf("\nDups counter at time %-15s: %d, avg: %.6f\n", cawaxInternalTimeToString(current->sample.time, timeBuf), dups, *dupsSum);
		// DO CALCULATION STUFF HERE;

		// pass 1 instead of the unitToMicro could save us a lot of arithmetic operations on large diviso(mostly divisions) while calculatio trapezoids. 
		//todo perform the final unit conversion at the end.
		acc trapezoidal =  trapezoid(*dupsSum, *((acc *)getComponent(&(next->sample), inputTarget)), tmp->sample.time, next->sample.time, tmp->sample.order, next->sample.order, unitToMicro );
		char timeNextBuf[INTERNAL_TIME_ASCII_BYTES];
		//printf("Got trapezoid %.6f on [%-15s : %-.6f] to [%-15s : %-.6f]\n", trapezoidal, timeBuf, *dupsSum, cawaxInternalTimeToString(next->sample.time, timeNextBuf), *((acc *)getComponent(&(next->sample), inputTarget)));
		simpson += trapezoidal;
		//printf("simpson till %15s: %.6f\n",timeNextBuf, simpson);
		// next current and next pair
		current = next;
		// if next would be out of bound then assign endNode to next, else assign the jumping result to next
		if (!(next = jump(current, step))) {
			//printf("warn: simpsonSingle:  next would be out of bound on current %p, assigning endNode %p to next.\n", current, endNode);
			next = endNode;
		}
	}

	if (!tmp)
		free(tmp);
	
	char time1[INTERNAL_TIME_ASCII_BYTES];
	char time2[INTERNAL_TIME_ASCII_BYTES];

	// recover (if needs be) all samples based on {base} parameter
	if (changed && recoverSignal)
	{
		//printf(DEBUG_HEADER_INFO"Recovering signal from base %.6f ...\n", base);
		while (current->next) {
			acc * target_ptr = (acc *)getComponent(&(current->sample), inputTarget);
			//TODO Wrapper function
			*target_ptr += base;
			current = current->next;
		}
	}

	// convert to required unit
	if (gOrMeter == UNIT_METER)
		simpson = simpson * GRAVITY_ACC;
	else if (gOrMeter != UNIT_G) {
		//printf(DEBUG_HEADER_INFO"Error on calling simpsonSingle: Invalid unit: %d\n", gOrMeter);
		simpson = 0;
	}
	printf(DEBUG_HEADER_INFO"Simpson Spartial(g/9 or m/1): %d, Time(to micro): %d  calculated for interval %s to %s (%.3f s) is: %.6f\n",
		   gOrMeter, unitToMicro,
		   cawaxInternalTimeToString(signal->head->sample.time, time1),
		   cawaxInternalTimeToString(endNode->sample.time, time2), cawaxInternalTimeDiff(signal->head->sample.time, endNode->sample.time, UNIT_SECOND_TO_MICRO),
		   simpson);

	*buf = (vel_g) simpson;
	return buf;
}

/*
Calculate simpson for multiple input targets
*/
vel_g * simpson(LinkedList * signal, sample_th start, sample_th count, size_t step, acc base, char recoverSignal, int gOrMeter, int unitToMicro, int inputTargets, size_t bufCount, vel_g * buf)
{
	if (signal->count == 0 || !(signal->head)) {
		printf(DEBUG_HEADER_INFO"Calling simpson requires non-empty signal. however either count is 0: %d or head is null: %p\n", signal->count, signal->head);
		return NULL;
	}
	if (!buf) {
		printf(DEBUG_HEADER_INFO"Error while calling simpson. Require non-null buffer. %p given.\n", buf);
		return NULL;
	}
	// mask out trash
	inputTargets &= 0x7f;
	int i = 0;
	int bufI = 0;
	while (i < CINDICES_COUNT && bufI < bufCount) {
		//printf("Current CINDEX is: %d \n", CINDICES[i]);
		if (inputTargets & CINDICES[i]) {
			simpsonSingle(signal, start, count, step, base, recoverSignal, gOrMeter, unitToMicro, CINDICES[i], &buf[bufI]);
			bufI++;
		}
		++i;
	}
	//printf("subsignal address after simpson is: %p\n", signal);
}



