#include <wiringPi.h>
#include <StepperMotor.hpp>
#include <BeamBreakSensor.hpp>
#include <ConfigurationParser.hpp>
#include <string>
#include <Printer.hpp>
#include <iostream>

using namespace Direction_Constants;
using namespace std;

Printer::Printer(string ConfigurationFile)
{
	// Parsing Parameters
	this->CONFIG = new ConfigurationParser(ConfigurationFile);

	// Initializing WiringPi
	wiringPiSetup();

	// Initializing Stepper Motor
	this->STEPPER_MOTOR = new StepperMotor((this->CONFIG)->GetParam("STEP_PIN"), (this->CONFIG)->GetParam("DIR_PIN"), (this->CONFIG)->GetParam("INIT_STEP"));

	// Initializing Beam Break Sensor
	this->SENSOR = new BeamBreakSensor((this->CONFIG)->GetParam("SENSOR_PIN"));
}


// UI Routines for User Interaction
void Printer::UI_ZeroingRoutine()
{
	// Zeroing the Printer
	cout << "Running Zeroing Routine...\n";
	this->Zero();
	cout << "Zeroing Complete. Press Enter to Continue\n";

	// Waiting for User Input
	getchar();

	// Returning the Printer to It's Initial Step Count
	this->GoToStart();
	cout << "Zeroing Routine Complete.\n";
}


// Private Helper Functions
void Printer::Zero()
{
	// Moving Down Until the Sensor is Broken
	while(!(this->SENSOR)->IsBroken())
	{
		(this->STEPPER_MOTOR)->Step(1, FORWARD, (this->CONFIG)->GetParam("ZERO_STEP_DELAY"));
	}

	// Zeroing the Stepper Motor
	(this->STEPPER_MOTOR)->Zero();
}

void Printer::GoTo(int step)
{
	(this->STEPPER_MOTOR)->GoToStep(step, (this->CONFIG)->GetParam("GOTO_STEP_DELAY"));
}

void Printer::GoToStart()
{
	(this->STEPPER_MOTOR)->GoToStep((this->CONFIG)->GetParam("INIT_STEP"), (this->CONFIG)->GetParam("GOTO_STEP_DELAY"));
}
