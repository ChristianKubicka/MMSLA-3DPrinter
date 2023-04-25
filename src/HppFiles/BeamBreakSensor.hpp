#ifndef BEAM_BREAK_SENSOR_H
#define BEAM_BREAK_SENSOR_H

class BeamBreakSensor
{
	private:
		int SENSOR_PIN;

	public:
		BeamBreakSensor(int sensor_pin);
		bool IsBroken();
};

#endif
