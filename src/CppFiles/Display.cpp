#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <Display.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Display::Display(int Buff_Size, int Disp_Height, int Disp_Width, int Channels)
{
	// Assigning Variables
	this->BUFFER_SIZE = Buff_Size;
	this->DISP_HEIGHT = Disp_Height;
	this->DISP_WIDTH = Disp_Width;
	this->CHANNELS = Channels;

	// Creating Display Buffer
	this->DisplayBuffer = new char[this->BUFFER_SIZE];
}

void Display::Clear()
{
        for(int y = 0;y < this->DISP_HEIGHT;y++)
        {
                for(int x = 0;x < this->DISP_WIDTH;x++)
                {
                        // Getting Necessary Offsets
                        int dispBuffOff = getDispOff(x,y);

			// Setting Display to Black
                        this->DisplayBuffer[dispBuffOff] = this->getColorUpper(0,0,0);
                        this->DisplayBuffer[dispBuffOff + 1] = this->getColorLower(0,0,0);
                }
        }

        // Displaying
        int fildes = open("/dev/fb0", O_WRONLY);
        write(fildes, this->DisplayBuffer, this->BUFFER_SIZE);
        close(fildes);
}

void Display::DisplayTestPattern()
{
	for(int y = 0;y < this->DISP_HEIGHT;y++)
	{
		for(int x = 0;x < this->DISP_WIDTH;x++)
		{
			// Getting Necessary Offsets
			int dispBuffOff = getDispOff(x,y);

			// Completing Buffer Assignments
			if(((x/100)%2) == 0)
			{
				this->DisplayBuffer[dispBuffOff] = this->getColorUpper(0,0,0);
				this->DisplayBuffer[dispBuffOff + 1] = this->getColorLower(0,0,0);
			}
			else
			{
				this->DisplayBuffer[dispBuffOff] = this->getColorUpper(255,255,255);
				this->DisplayBuffer[dispBuffOff + 1] = this->getColorLower(255,255,255);
			}
		}
	}

	// Displaying
	int fildes = open("/dev/fb0", O_WRONLY);
	write(fildes, this->DisplayBuffer, this->BUFFER_SIZE);
	close(fildes);
}

bool Display::DisplayImage(char* filename)
{
	// Getting the image
	int width, height, bpp;
	uint8_t* img = stbi_load(filename, &width, &height, &bpp, 0);
	uint8_t lower, upper;
	bool return_value = false;

	// Converting Image to Display
	for(int y = 0;y < this->DISP_HEIGHT;y++)
	{
		for(int x = 0;x < this->DISP_WIDTH;x++)
		{
			// Getting necessary offsets
			int dispBuffOff = getDispOff(x,y);

			// Completing Buffer Assignments 16-bit color
			lower = this->getLower(img, x, y, width, height, bpp);
			upper = this->getUpper(img, x, y, width, height, bpp);
			this->DisplayBuffer[dispBuffOff]   = upper;
			this->DisplayBuffer[dispBuffOff+1] = lower;

			// Checking if a color exists
			if(return_value || lower != 0 || upper != 0)
			{
				return_value = true;
			}
		}
	}

	// Displaying
	int fildes = open("/dev/fb0", O_WRONLY);
	write(fildes, this->DisplayBuffer, this->BUFFER_SIZE);
	close(fildes);

	// Freeing the image
	stbi_image_free(img);

	// Returning the return value
	return return_value;
}

int Display::getDispOff(int x, int y)
{
	return 2 * ((y * this->DISP_WIDTH) + x);
}

uint8_t Display::getLower(uint8_t* image, int x, int y, int width, int height, int bpp)
{
	// Getting x and y from x' and y'
	x = (x * width)/(this->DISP_WIDTH);
	y = (y * height)/(this->DISP_HEIGHT);

	// Computing the offset
	int offset = bpp * ((y * width) + x);

	// Returning the required color
	return this->getColorLower(image[offset + 0], image[offset + 1], image[offset + 2]);
}

uint8_t Display::getUpper(uint8_t* image, int x, int y, int width, int height, int bpp)
{
	// Computing x and y from x' and y'
	x = (x * width)/(this->DISP_WIDTH);
	y = (y * height)/(this->DISP_HEIGHT);

	// Computing the offset
	int offset = bpp * ((y * width) + x);

	// Returning the required color
	return this->getColorUpper(image[offset + 0], image[offset + 1], image[offset + 2]);

}

uint8_t Display::getColorLower(uint8_t r, uint8_t g, uint8_t b)
{
	return (r << 3) | (g >> 5);
}

uint8_t Display::getColorUpper(uint8_t r, uint8_t g, uint8_t b)
{
	return (g << 5) | (b >> 3);
}
