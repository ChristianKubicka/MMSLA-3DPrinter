#ifndef PRINTER_H
#define PRINTER_H

#include <string>
#include <ConfigurationParser.hpp>
#include <StepperMotor.hpp>
#include <BeamBreakSensor.hpp>

class Printer
{
	// Class Data
	private:
		StepperMotor* STEPPER_MOTOR = NULL;
		BeamBreakSensor* SENSOR = NULL;
		ConfigurationParser* CONFIG = NULL;

	// Constructors
	public:
		Printer(std::string ConfigurationFile);

	// UI Functions
	public:
		void UI_ZeroingRoutine();

	// Helper Functions
	private:
		void Zero();
		void GoTo(int step);
		void GoToStart();
};
#endif
