#pragma once
#include "String.h"
#include "SDL.h"

//A logger class that can be used to log information to the console for debugging and error checking purposes
class Logger
{
private:
	bool allowConsoleLog;
public:
	Logger():allowConsoleLog(false){}
	void ToggleLogger();
	void Log(String message);
	String GetCurrentStringTime();
};

