#pragma once
#include "SimpleINI/SimpleIni.h"
#include "String.h"
#include "Containers.h"

using SectionList = std::list<CSimpleIniA::Entry>;

/// <summary>
/// This class is used for handling ini file interactions and for storing the ini file itself.
/// It is mainly a wrapper class for the Simpleini library 
/// </summary>
class IniFile
{
private:
	CSimpleIniA mIniFile;
	int32_t mErrorNumber;
public:
	IniFile();
	/// <summary>
	/// Open an .ini file specified in the parameter
	/// </summary>
	/// <param name="fileName">: the name if the .ini file to load</param>
	/// <returns>True if the file was opened successfully or false if it failed to open</returns>
	bool LoadFile(String fileName, String directory = "");
	/// <summary>
	/// Saves an ini file to the current directory
	/// </summary>
	/// <param name="fileName">: the name of the .ini file to save </param>
	void SaveFile(String fileName, String directory = "");
	void SetNumberValue(String section, String key, int32_t value);
	void SetStringValue(String section, String key, String value);


	SectionList GetAllSections();
	int32_t GetSigned32Value(String section, String key, int32_t defaultVal = 0);
	uint32_t GetUnsigned32Value(String section, String key, uint32_t defaultVal = 0);
	float GetFloatValue(String section, String key, float defaultVal = 0.0f);
	double GetDoubleValue(String section, String key, double defaultVal = 0.0);
	String GetStringValue(String section, String key, String defaultVal = "");
	vector<String> GetMultipleStringValues(String section, String key);
	bool GetBooleanValue(String section, String key, bool defaultVal = false);
	inline int32_t GetCurrentErrorCode() { return mErrorNumber; }
};

