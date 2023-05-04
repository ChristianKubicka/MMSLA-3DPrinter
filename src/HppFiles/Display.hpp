#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>

class Display
{
	private:
		// Private constant variables
		int BUFFER_SIZE;
		int DISP_HEIGHT;
		int DISP_WIDTH;
		int CHANNELS;

		// Data for the Display Buffer
		char* DisplayBuffer;

	// Constructor
	public:
		Display(int Buffer_Size, int Disp_Height, int Disp_Width, int Channels);
		void DisplayImage(char* filename);
		void DisplayTestPattern();
		void Clear();

	// Private Helper Function
	private:
		int getDispOff(int x, int y);
		uint8_t getLower(uint8_t* image, int x, int y, int width, int height, int bpp);
		uint8_t getUpper(uint8_t* image, int x, int y, int width, int height, int bpp);
		uint8_t getColorUpper(uint8_t r, uint8_t g, uint8_t b);
		uint8_t getColorLower(uint8_t r, uint8_t g, uint8_t b);
};

#endif
