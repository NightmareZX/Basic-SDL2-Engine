#include "Logger.h"
#include <iomanip>
#include <iostream>

using std::cout;

void Logger::ToggleLogger(){ allowConsoleLog = !allowConsoleLog; }
void Logger::Log(String message)
{
	if (!allowConsoleLog) return;
	cout << "< " << GetCurrentStringTime() << " > " << message << '\n';
}

String Logger::GetCurrentStringTime()
{
	const time_t now = time(nullptr);

	tm localTime;
	localtime_s(&localTime, &now);

	char buffer[80];
	strftime(buffer, std::size(buffer), "%d-%m-%Y %H:%M:%S", &localTime);

	OutputStringStream stream;
	stream << std::put_time(&localTime, "%d-%m-%Y %H:%M:%S");
	return stream.str();
}