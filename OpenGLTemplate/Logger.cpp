#include "Logger.h"
#include <iostream>
#include <fstream>

Logger::Logger()
{
	logFile = "log.txt";
	tempLog = "tempLog.txt";
	errorLog = "errorLog.txt";

	tempF = new ofstream ("tempLog.txt");

}

Logger::~Logger()
{
	tempF.close();
}

void Logger::write(string type,  string  log)
{
	if (type == "temp")
	{
		tempF << log << "\n";
	}
}

void Logger::write(string type, const glm::vec3 & v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	if (type == "temp")
	{
		tempF <<  "X: " << std::to_string(x) << ' ' <<  "Y: " << std::to_string(y) << ' ' << "Z: " << std::to_string(z) << "\n";
	}
}

void Logger::write(string type, const glm::vec2 & v)
{
}

void Logger::write(string type, const glm::vec4 & v)
{
}
