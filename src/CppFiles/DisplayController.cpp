#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

// Constants
#define DISP_WIDTH 1280
#define DISP_HEIGHT 2400
#define CHANNELS 2
#define BUFF_SIZE 6144000

// Type definitions
typedef unsigned char uint8;

// Function Declarations
uint8 getColorLower(uint8 r, uint8 g, uint8 b);
uint8 getColorUpper(uint8 r, uint8 g, uint8 b);
int getDispOff(int x, int y);
int getImgOff(int x, int y, int width, int height);
int getImgOff(int x, int y, int width);
void DisplayImage(int width, int height, uint8* imageBuffer);
void printDisplayBuffer();

// Global Variable Declarations
char displayBuffer[BUFF_SIZE];

int main()
{
	// Creating the image buffer
	int width = 1600; int height = 1000; int channels = 2;
	uint8 imgBuff[width * height * channels];
	uint8 colorLower, colorUpper;
	int sqLen = 150;

	// Iterating over every pixel
	for(int y = 0;y < height;y++)
	{
		for(int x = 0;x < width;x++)
		{
			if((x < (width/2 + sqLen)) && (x > (width/2 - sqLen)) && (y < (height/2 + sqLen)) && (y > (height/2 - sqLen)))
			{
				colorLower = getColorLower(0,0,0);
				colorUpper = getColorUpper(0,0,0);
			}
			else
			{
				colorLower = getColorLower(0xFF,0xFF,0xFF);
				colorUpper = getColorUpper(0xFF,0xFF,0xFF);
			}

			// Writing to Buffer
			int imgOff = getImgOff(x,y,width);
			imgBuff[imgOff]   = colorLower;
			imgBuff[imgOff+1] = colorUpper;
		}
	}

	// Displaying the image
	DisplayImage(width, height, imgBuff);
}

void DisplayImage(int width, int height, uint8* imageBuffer)
{
	// Converting Image to Display
	for(int y = 0;y < DISP_HEIGHT;y++)
	{
		for(int x = 0;x < DISP_WIDTH;x++)
		{
			// Getting necessary offsets
			int dispBuffOff = getDispOff(x,y);
			int imgBuffOff = getImgOff(x,y,width,height);

			// Completing Buffer Assignments 16-bit color
			displayBuffer[dispBuffOff]   = imageBuffer[imgBuffOff];
			displayBuffer[dispBuffOff+1] = imageBuffer[imgBuffOff + 1];
		}
	}

	// Displaying
	int fildes = open("/dev/fb0", O_WRONLY);
	write(fildes, displayBuffer, BUFF_SIZE);
	close(fildes);
}

void printDisplayBuffer()
{
	for(int i = 0;i < BUFF_SIZE;i++)
	{
		int lowerHex = (0x0F & (displayBuffer[i]));
		int upperHex = (0xF0 & (displayBuffer[i])) >> 4;

		// Printing the hexadecimal numbers
		if((i % 16) == 0) printf("0x%.8X ",i);
		printf("%X%X",lowerHex,upperHex);
		if((i % 2) == 1) printf(" ");
		if((i % 16) == 15) printf("\n");
	}
}

int getImgOff(int x, int y, int width, int height)
{
	// Computnig x and y from x' and y'
	x = (x * width)/DISP_WIDTH;
	y = (y * height)/DISP_HEIGHT;

	return 2 * ((y * width) + x);
}

int getDispOff(int x, int y)
{
	return 2 * ((y * DISP_WIDTH) + x);
}

int getImgOff(int x, int y, int width)
{
	return 2 * ((y * width) + x);
}

uint8 getColorLower(uint8 r, uint8 g, uint8 b)
{
	return (r << 3) | (g >> 5);
}

uint8 getColorUpper(uint8 r, uint8 g, uint8 b)
{
	return (g << 5) | (b >> 3);
}
