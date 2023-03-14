#pragma once
#include "STL_Components.h"
#include "SDL.h"
class Logger
{
private:
	static Logger* instance;
	bool allowConsoleLog;
public:
	Logger():allowConsoleLog(false){}
	inline static Logger* GetInstance()
	{
		if (instance == nullptr) instance = new Logger();
		return instance;
	}
	void ToggleLogger();
	void Log(String message);
	String GetCurrentStringTime();
};

