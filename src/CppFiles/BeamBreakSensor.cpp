#include <wiringPi.h>
#include <BeamBreakSensor.hpp>

BeamBreakSensor::BeamBreakSensor(int sensor_pin)
{
	// Initializing Pin Number
	SENSOR_PIN = sensor_pin;

	// Pin Mode Setup
	pinMode(SENSOR_PIN, INPUT);
	pullUpDnControl(SENSOR_PIN, PUD_UP);
}

bool BeamBreakSensor::IsBroken()
{
	// Variables and Reading Data
	int sensor_value;
	sensor_value = digitalRead(SENSOR_PIN);

	// Returning a value
	if(sensor_value == 0)	return true;
	else			return false;
}
