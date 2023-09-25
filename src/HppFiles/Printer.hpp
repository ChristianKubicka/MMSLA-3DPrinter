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
		StepperMotor* VAT_STEPPER_MOTOR = NULL;
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
		void UI_PrintRoutineMM();
		void UI_ManualControl(int step);
		void UI_VatManualControl(int step);
		void UI_TestRoutine();
		void UI_Estimate();

	// Helper Functions
	private:
		void Zero();
		void GoTo(int step);
		void GoToStart();
		void VatGoTo(int step);
		void ExposeLayer(int layer, int numBaseLayers);
		int EstimateTimeMs();
		void PrintTime(int ms);
};
#endif
