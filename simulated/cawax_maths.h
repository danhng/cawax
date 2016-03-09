/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>

Many code patterns in this project are thanks to the openmovement project of Newcastle University.
See: https://github.com/digitalinteraction/openmovement for more details.
*/

#ifndef SIMULATED_CAWAX_MATHS_H
#define SIMULATED_CAWAX_MATHS_H

#include "cawax.h"
#include "cawax_datastructs.h"



// https://en.wikipedia.org/wiki/Gravitational_acceleration
#define GRAVITY_ACC 9.80665

#define TRAPEZOID_POLAR_SAME(h1, h2, delta) ((delta) * ((h1) + (h2)) / 2)

// units of integration

#define UNIT_METER 1

#define UNIT_G 10

#define BASE_0 0

#define BASE_1 1

#define DO_RECOVER 1

#define NO_RECOVER 0

#define STEP_SIZE(x) (x)



/**
 * Calculate the root mean square of a sample based on sqrt(x^2+y^2+z^2)
 */
acc rootMeanSquare(Sample * sample);

/**
 * Calculate the mean of a sample (x + y + z) / 3
 */
acc meanSample(Sample * sample);

/**
 * Calculate the mean of a sub series from the signal
 *  If count = -1 then calculate the mean for the whole array (100 terminated)
 */
acc mean(acc * input, int count);

/**
 Calculate the SD of a sub series from the signal.
 This would help figure out how acceleration values fluctuated over time thus 
 potentially giving us a useful view on the properties of the movement.
 */
acc standardDeviation(acc * input, size_t  count);

/*
Calculate the area of the trapezoidal shape made up from 2 sample points and their corresponding projections
*/
acc trapezoid(acc sample1, acc sample2, INTERNAL_TIME time1, INTERNAL_TIME time2, sample_th order1, sample_th order2, int unitToMicro);

/*
Calculate the simpson (vel_g) of a sub time series using Trapezium rules with optional step value (default is 1) 
controlling the accuracy level of the simpson
 - Integral accuracy <-><-> Computation time  
 - See https://en.wikipedia.org/wiki/Trapezoidal_rule
 - Note: This method does not support negative base at the moment.
 - TODO: Wrapper methods needed.
Note: The simpson is caluclated based on acc values that are optionally substracted by 1g in an attempt to mask out the effect of gravity 
and give the true movement data. 
 
METHOD SIGNATURE
//TODO update
 Parameters:
 - LinkedList * signal: The signal containing samples to be analysed.
 - size_t step: The step size (default is 1) drives how accurate the results would be.
 - base: Optional postive base against which the integration is calculated. (default is 0).
 - int inputTargets: which component(s) of the samples for which the integration is to be calculated. 
 - vel_g * buf: buffer for result(s)
   + result for each input target is put in the order as per the input targets (X -> Y -> Z -> RMQ -> M -> TIME -> ORDER)
 Return:
 - vel_g * : the buf array specified in the parameter;
 */
vel_g * simpson(LinkedList * signal, sample_th start, sample_th count, size_t step, acc base, char recoverSignal, int gOrMeter, int unitToMicro, int inputTargets, size_t buf_Count, vel_g * buf);

vel_g * simpsonSingle(LinkedList * signal, sample_th start, sample_th count, size_t step, acc base, char recoverSignal, int gOrMeter, int unitToMicro, int inputTarget, vel_g * buf);

#endif //SIMULATED_CAWAX_MATHS_H
