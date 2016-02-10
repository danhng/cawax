//
// @author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>.
//

#include "cawax_datastructs.h"
#include "cawax.h"


#ifndef SIMULATED_CAWAX_MATHS_H
#define SIMULATED_CAWAX_MATHS_H

// https://en.wikipedia.org/wiki/Gravitational_acceleration
#define GRAVITY_ACC = 9.80665;

// velocity delta based upon g (not m/s^2 but gm/s^2)
typedef long vel_g;


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
 * Calculate the SD of a sub series from the signal
 */
acc standardDeviation(acc * input, size_t  count);

/*
Calculate the integral (vel_g) of a sub time series
 - The {input} is assumed to be distributed evenly. That is, the interval between adjacent values is known and constant.
 - TODO: Wrapper methods needed.
 */
vel_g integral(acc * input, acc base, size_t count);



#endif //SIMULATED_CAWAX_MATHS_H
