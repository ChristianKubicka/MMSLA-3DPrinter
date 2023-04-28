#include "ConfigurationParser.hpp"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

ConfigurationParser::ConfigurationParser(string filename)
{
	// Opening the Configuration File
	ifstream file(filename);

	// Getting the contents of the file
	string param_name, param_value;
	if(file.is_open())
	{
		// Reading contents of config file
		while(1)
		{
			if(!getline(file, param_name, ':')) break;
			if(!getline(file, param_value)) break;
			this->config[param_name] = stoi(param_value);
		}
	}
	else
	{
		// Throwing Execption
		throw invalid_argument("File \"" + filename  +  "\" not found.\n");
	}
}

int ConfigurationParser::GetParam(string param)
{
	// Returning the Param if it Exists
	if(config.count(param) != 0)
	{
		return config[param];
	}
	// Throwing an Exception Otherwise
	cout << param << "\n";
	throw invalid_argument("Param is not Defined in Configuration File.\n");
}
