#pragma once
#include "String.h"
#include "SDL.h"

//A logger class that can be used to log information to the console for debugging and error checking purposes
class Logger
{
private:
	Logger() {}
	static bool allowConsoleLog;
	static String GetCurrentStringTime();
public:
	static void ToggleLogger();
	static void Log(String message);
};

