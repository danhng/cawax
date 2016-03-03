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
	LinkedList * subSignal;
	// features
	acc mean_x;
	acc stdDev_X;
	acc stdDev_RMQ_XYZ;
	acc integral_X;
	acc integral_RMQ_XYZ;
	//todo expand (make use of secondary-dataDimension data - Y, Z) 
} Feature;



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

Feature * toFeature(LinkedList * subSignal, Feature * buf);

Feature * printFeature(Feature * feature);

/*
Analyse the subsignal windows.
*/
posture analyse(Feature * feature);

