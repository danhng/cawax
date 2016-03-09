#pragma once
/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>
Many code patterns in this project are thanks to the openmovement project of Newcastle University.
See: https://github.com/digitalinteraction/openmovement for more details.

cawax_analyst.h
contains signal classifier, analyser based on features defined in cawax_maths.h and other relevant places.
*/

#include "cawax_datareader.h"
#include "cawax_datastructs.h"
#include "cawax_maths.h"

/*
*/
typedef struct {
	// const
	Signal * samples;

	// all the below vars get updated as the window slides along the signal
	node_index windowStart;
	node_index windowEnd;

	// features
	acc mean_x;
	acc stdDev_X;
	acc stdDev_RMQ_XYZ;
	acc integral_X;
	acc integral_RMQ_XYZ;
	//todo expand (make use of secondary-dataDimension data - Y, Z) 
} FeaturedWindow;

#define WINDOW_SIZE_SECOND 1

#define WINDOW_SIZE_OVERLAP_SECOND 0.5

#define SAMPLES_TO_JUMP_NEXT_WINDOW (sample_th) ((WINDOW_SIZE_OVERLAP_SECOND) * (double)(SAMPLING_FREQUENCY_HZ))

#define SAMPLES_TO_JUMP_WINDOW_END ((WINDOW_SIZE_SECOND) * (SAMPLING_FREQUENCY_HZ))

/*
bit 0-1:
	00: n/a
	01: stand/sit
		mean_x >>
	10: lying
		mean_x <<
bit 2:
	0: static 
		small stdDev's
		rmqXYZ  ~ 1g
		integral_RMQ_XYZ / integral_1_1 ~ 1 
	1: active (large stdDev)
		large stdDev's
		rmqXYZ	>> 1g
		integral_RMQ_XYZ / integral_1_1 >> 1

bit 3-4-5: decision
	000: n/a
	if stand/sit
		001: standing still/sitting still
		010: walking
		011: running
	if lying
		100: lying still
		101: 'active' lyingtoStringFeature
		*/
typedef int posture;

FeaturedWindow *toFeaturedWindow(Signal *signal, node_index start, node_index end, FeaturedWindow *buf);

FeaturedWindow *printFeaturedWindow(FeaturedWindow *featuredWindow);

/*
Analyse the subsignal windows.
*/
posture analyseFeaturedWindow(FeaturedWindow * feature);

void processSignal(Signal * signal);
/**
 * Act based on the input posture
 */
void action(posture * p);

void turnOffGyro();

void turnOnGyro();