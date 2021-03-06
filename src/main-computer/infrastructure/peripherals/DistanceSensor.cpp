/*
 * DistanceSensor.cpp
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#include "DistanceSensor.h"

#include "microcontroller/Communicator.h"
using infrastructure::microcontroller::Communicator;

namespace infrastructure {

float DistanceSensor::get_distance() {
	return Communicator::request_distance() * CONVERSION_FACTOR;
}

}  /* namespace infrastructure */
