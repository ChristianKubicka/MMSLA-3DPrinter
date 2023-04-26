#ifndef UV_CONTROLLER_H
#define UV_CONTROLLER_H

class UV_Controller
{
	private:
		int UV_PWM_PIN;

	public:
		UV_Controller(int PWM_Pin);
		void Enable(int intensity);
		void Disable();
}

#endif
