#include "cawax_maths.h"
#include <math.h>

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

vel_g integral(acc * input, acc base, size_t count)
{
	/*acc integral = 0;
	acc current = 0;
	long i = 0;
	while ((current = input[i]) != TERMINATION_VALUE && i != count) {
		integral = fabs(current - base);
		++i;
	}*/
	return NOT_IMPLEMENTED;
}


