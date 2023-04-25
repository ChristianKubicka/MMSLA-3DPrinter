#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H
#include <string>
#include <map>


class ConfigurationParser
{
	private:
		std::map<std::string,int> config = {};

	public:
		ConfigurationParser(std::string filename);
		int GetParam(std::string filename);

};
#endif
