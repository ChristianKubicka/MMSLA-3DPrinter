#include <wiringPi.h>
#include <StepperMotor.hpp>

using namespace Direction_Constants;

// Constructor
StepperMotor::StepperMotor(int Step_Pin, int Dir_Pin, int Current_Step)
{
	// Initializing Data
	curr_step = Current_Step;
	STEP_PIN = Step_Pin;
	DIR_PIN = Dir_Pin;

	// Initalizing Pins
	pinMode(STEP_PIN, OUTPUT);
	pinMode(DIR_PIN, OUTPUT);
}

// Function for Stepping the Motor at a Given Step Rate
void StepperMotor::Step(int num, int direction, int delayUs)
{
	// Variable Declarations
	int i = 0;

	// Setting Direction Pin
	if(direction == FORWARD) digitalWrite(DIR_PIN, HIGH);
	else			 digitalWrite(DIR_PIN, LOW);

	// Stepping as Required
	for(i = 0;i < num;i++)
	{
		//Cycling Step Pin
		digitalWrite(STEP_PIN, HIGH);
		delayMicroseconds(30);
		digitalWrite(STEP_PIN, LOW);
		delayMicroseconds(30);

		// Updating Step Count
		curr_step += direction;
		delayMicroseconds(delayUs);
	}
}

void StepperMotor::GoToStep(int num, int delayUs)
{
	// Creating a Variable to Hold the Direction
	int dir;

	// Determining the Direction
	if((num - curr_step) > 0) dir = BACKWARD;
	else			  dir = FORWARD;

	// Stepping
	this->Step(num - curr_step, dir, delayUs);
}

// Sets the Step Count to 0
void StepperMotor::Zero()
{
	curr_step = 0;
}


