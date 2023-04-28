#include <wiringPi.h>
#include <UVLamp.hpp>

// Constructor
UVLamp::UVLamp(int PWM_Pin)
{
	// Assigning the Global Variable
	UV_PWM_PIN = PWM_Pin;

	// Configuring PWM Pin
	pinMode(UV_PWM_PIN, PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);

	// Configuring the PWM for 5KHz after the clock divisor
	// and 50Hz after the range divisor
	pwmSetClock(3840);
	pwmSetRange(100);

	// Disabline the PWM Output
	this->Disable();
}

// Disabling the PWM Output by setting the duty cycle to zero
void UVLamp::Disable()
{
	pwmWrite(UV_PWM_PIN, 0);
}

// Enabling the PWM Output with the specified intensity
void UVLamp::Enable(int intensity)
{
	pwmWrite(UV_PWM_PIN, intensity);
}
