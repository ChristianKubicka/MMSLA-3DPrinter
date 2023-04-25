#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

namespace Direction_Constants
{
	static const int FORWARD  = -1;
	static const int BACKWARD =  1;
}

class StepperMotor
{
	private:
		int curr_step;
		int STEP_PIN;
		int DIR_PIN;

	public:
		StepperMotor(int Step_Pin, int Dir_Pin, int Current_Step);
		void Step(int num, int direction, int delayUs);
		void GoToStep(int num, int delayUs);
		void Zero();
};

#endif
