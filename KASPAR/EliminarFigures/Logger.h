#pragma once
#include <vector>
#include <string>
#include <iostream>

class Logger
{
public:
	Logger(const std::vector<int>& objectSlots);
	void log(const char* data, int xPosCursor, int yPosCursor);
	void newLog(const std::vector<int>& objectSlots);


private:
	const std::string filePath = "log/";
	std::string currentFile;

};

