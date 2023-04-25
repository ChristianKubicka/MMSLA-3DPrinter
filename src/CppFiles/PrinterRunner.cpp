#include <Printer.hpp>
#include <iostream>

using namespace std;

// Main Method
int main(int argc,char* argv[])
{
	// Checking if the correct number of arguments is provided
	if(argc < 2)
	{
		cerr << "Usage: ./printer command [File]";
		return 1;
	}

	// Creating Printer Object
	string argument = argv[1];
	Printer printer("Config.txt");

	// Taking appropriate action
	if(argument == "zero")
	{
		printer.UI_ZeroingRoutine();
	}
	else
	{
		cerr << "Command Not Recognized.\n";
		return 1;
	}
	return 0;
}
