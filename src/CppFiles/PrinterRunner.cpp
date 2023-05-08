#include <Printer.hpp>
#include <iostream>
#include <unistd.h>
#include <stdio.h>

using namespace std;

// Main Method
int main(int argc,char* argv[])
{
	// Checking if the correct number of arguments is provided
	if(argc < 2)
	{
		cerr << "Usage: ./printer command\n";
		return 1;
	}

	// Finding the argument
	string argument = argv[1];
	Printer printer("Config.txt");

	// Taking appropriate action
	if(argument == "zero")
	{
		printer.UI_ZeroingRoutine();
	}
	else if(argument == "print")
	{
		printer.UI_PrintRoutine();
	}
	else if(argument == "estimate")
	{
		printer.UI_Estimate();
	}
	else if(argument == "test")
	{
		printer.UI_TestRoutine();
	}
	else if(argument == "manual")
	{
		string arg3 = argv[2];
		printer.UI_ManualControl(stoi(arg3));
	}
	else
	{
		cerr << "Command Not Recognized.\n";
		return 1;
	}
	return 0;
}
