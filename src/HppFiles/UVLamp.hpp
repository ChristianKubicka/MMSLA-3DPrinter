#ifndef UV_LAMP_H
#define UV_LAMP_H

class UVLamp
{
	private:
		int UV_PWM_PIN;

	public:
		UVLamp(int PWM_Pin);
		void Enable(int intensity);
		void Disable();
};

#endif
