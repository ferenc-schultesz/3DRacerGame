#pragma once
#include "Common.h"

class Logger
{
private:
	string logFile;
	string tempLog;
	string errorLog;
	ofstream tempF ("tempLog.txt");

public:
	Logger();
	~Logger();

	void write(string type,  string log);
	void write(string type, const glm::vec3 &v);
	void write(string type, const glm::vec2 &v);
	void write(string type, const glm::vec4 &v);



};