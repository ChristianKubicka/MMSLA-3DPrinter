#ifndef PRINTER_H
#define PRINTER_H

#include <string>
#include <ConfigurationParser.hpp>
#include <StepperMotor.hpp>
#include <BeamBreakSensor.hpp>
#include <UVLamp.hpp>
#include <Display.hpp>

class Printer
{
	// Class Data
	private:
		StepperMotor* STEPPER_MOTOR = NULL;
		BeamBreakSensor* SENSOR = NULL;
		UVLamp* UV_LAMP = NULL;
		ConfigurationParser* CONFIG = NULL;
		Display* DISPLAY = NULL;

	// Constructors
	public:
		Printer(std::string ConfigurationFile);

	// UI Functions
	public:
		void UI_ZeroingRoutine();
		void UI_PrintRoutine();
		void UI_ManualControl(int step);

	// Helper Functions
	private:
		void Zero();
		void GoTo(int step);
		void GoToStart();
};
#endif
