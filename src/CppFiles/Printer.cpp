#include <wiringPi.h>
#include <StepperMotor.hpp>
#include <BeamBreakSensor.hpp>
#include <ConfigurationParser.hpp>
#include <UVLamp.hpp>
#include <string>
#include <Printer.hpp>
#include <Display.hpp>
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

	// Initializing UV Controller
	this->UV_LAMP = new UVLamp((this->CONFIG)->GetParam("UV_PIN"));
	this->UV_LAMP->Disable();

	// Initializing Display
	this->DISPLAY = new Display((this->CONFIG)->GetParam("BUFF_SIZE"),(this->CONFIG)->GetParam("DISP_HEIGHT"),(this->CONFIG)->GetParam("DISP_WIDTH"),(this->CONFIG)->GetParam("CHANNELS"));
}

void Printer::UI_ManualControl(int step)
{
	// Disabling UV
	this->UV_LAMP->Disable();

	// Setting the direction
	int dir;
	if(step > 0) dir = BACKWARD;
	else
	{
		step = -1 * step;
		dir = FORWARD;
	}

	for(int i = 0;i < step;i++)
	{
		this->STEPPER_MOTOR->Step(1,dir, this->CONFIG->GetParam("ZERO_STEP_DELAY"));
	}

}

void Printer::UI_PrintRoutine()
{
	// Creating a buffer to hold all necessary strings
	char buffer[200];

	// Accessing Necessary Values
	int numBaseLayers = (this->CONFIG)->GetParam("NUM_BASE_LAYERS");
	int numNormLayers = (this->CONFIG)->GetParam("NUM_NORM_LAYERS");
	int totalLayers = numBaseLayers + numNormLayers;

	// Estimating the print time
	int printTimeMs = EstimateTimeMs();

	// Beginning the Print
	for(int i = 0;i < totalLayers;i++)
	{
		// Notifying the User
		printf("Printing Layer %d of %d.            ", i+1, totalLayers);

		// Positioning the Stepper Motor
		this->GoTo(i + 1);

		// Loading the Image
		sprintf(buffer,"IMG_%d.png",i);
		(this->DISPLAY)->DisplayImage(buffer);

		// Exposing the Image
		(this->UV_LAMP)->Enable((this->CONFIG)->GetParam("UV_INTENSITY"));

		// Determining if a layer is a base layer and delaying accordingly
		if(i < numBaseLayers)	delay((this->CONFIG)->GetParam("BASE_LAYER_MS"));
		else			delay((this->CONFIG)->GetParam("NORM_LAYER_MS"));

		// Disabling the image
		(this->UV_LAMP)->Disable();

		// Cycling to introduce new resin
		this->GoTo(i + 1 + ((this->CONFIG)->GetParam("NUM_STEPS_UP")));
	}

	// Returning to start
	this->GoToStart();

	// Notifying the User that the print is complete
	printf("Print Complete.\n");
}

void Printer::UI_TestRoutine()
{
	// Disabling the UV Lamp
	this->UV_LAMP->Disable();

	// Testing Stepper Motor
	cout << "Initiating Testing\n";
	cout << "Testing Stepper Motor\n";
	this->STEPPER_MOTOR->Step(500, FORWARD, (this->CONFIG)->GetParam("ZERO_STEP_DELAY"));
	this->STEPPER_MOTOR->Step(500, BACKWARD,(this->CONFIG)->GetParam("ZERO_STEP_DELAY"));
	cout << "Stepper Motor Testing Complete\n";

	// Testing Beam Break Sensor
	cout << "Testing Beam Break Sensor\n";
	cout << "Please Break the Beam\n";
	while(!(this->SENSOR)->IsBroken()) delay(100);
	cout << "Beam Broken.\nSensor Testing Complete.\n";

	// Testing Display and UV Lamp
	cout << "Testing Display and UV Lamp\n";
	this->DISPLAY->DisplayTestPattern();
	this->UV_LAMP->Enable(this->CONFIG->GetParam("UV_INTENSITY"));
	delay(5000);
	this->DISPLAY->Clear();
	this->UV_LAMP->Disable();
	cout << "UV Lamp Testing Complete\n";
	cout << "Testing Complete\n";
}


// UI Routines for User Interaction
void Printer::UI_ZeroingRoutine()
{
	// Disabling the UV Lamp
	this->UV_LAMP->Disable();

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

int Printer::UI_Estimate()
{
	int estimate = EstimateTimeMs();
	this->PrintTime(estimate);
}

int Printer::EstimateTimeMs()
{
	// Estimating Print Time
	int printMs = 0;

	// Getting Variables
	int init_step = (this->CONFIG)->GetParam("INIT_STEP");
	int goto_step_delay = (this->CONFIG)->GetParam("GOTO_STEP_DELAY");
	int numBaseLayers = (this->CONFIG)->GetParam("NUM_BASE_LAYERS");
	int numNormLayers = (this->CONFIG)->GetParam("NUM_NORM_LAYERS");
	int base_layer_ms = (this->CONFIG)->GetParam("BASE_LAYER_MS");
	int norm_layer_ms = (this->CONFIG)->GetParam("NORM_LAYER_MS");
	int num_steps_up = (this->CONFIG)->GetParam("NUM_STEPS_UP");

	// Adding time to first layer
	printMs = printMs + ((init_step * goto_step_delay)/1000);

	// Adding Base Layer Time
	printMs = printMs + (numBaseLayers * (base_layer_ms + ((goto_step_delay * (num_steps_up + (num_steps_up - 1)))/1000)));

	// Adding Normal Layer Time
	printMs = printMs + (numNormLayers * (norm_layer_ms + ((goto_step_delay * (num_steps_up + (num_steps_up - 1)))/1000)));

	// Adding return to start print time
	printMs = printMs + (((init_step - (numNormLayers + numBaseLayers)) * goto_step_delay)/1000);

	// Returning Time
	return printMs;
}

int Printer::PrintTime(int ms)
{
	// Computing the number of hours in the interval
	int hours = ms / 3600000;
	ms -= ms / 3600000;

	// Computing the number of minutes in the interval
	int minutes = ms / 60000;
	ms -= ms / 60000;

	// Computing the number of seconds in the interval
	int seconds = ms / 1000;
	ms -= ms /1000;

	// Printing the result
	printf("Estimate: %d:%d:%d.%d\n", hours, minutes, seconds, ms);
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
