#include "IniFile.h"

#define PARSING_ERROR -1
using StringList = SectionList;

IniFile::IniFile()
{
	mIniFile.SetUnicode();
	mErrorNumber = 0;
}
bool IniFile::LoadFile(String fileName, String directory)
{
	String fileAndPath = directory + fileName;
	SI_Error errorCode = mIniFile.LoadFile(fileAndPath.c_str());
	if (errorCode == 0) return true;
	else if (errorCode < 0)
	{
		return false;
	}
}
void IniFile::SaveFile(String fileName, String directory)
{
	String fileAndPath = directory + fileName;
	SI_Error errorCode = mIniFile.SaveFile(fileAndPath.c_str());
}
void IniFile::SetNumberValue(String section, String key, int32_t value)
{
	mIniFile.SetLongValue(section.c_str(), key.c_str(), value);
}
void IniFile::SetStringValue(String section, String key, String value)
{
	mIniFile.SetValue(section.c_str(), key.c_str(), value.c_str());
}

int32_t IniFile::GetSigned32Value(String section, String key, int32_t defaultVal)
{
	int32_t value = static_cast<int32_t>(mIniFile.GetLongValue(section.c_str(),key.c_str(),defaultVal));
	return value;
}
uint32_t IniFile::GetUnsigned32Value(String section, String key, uint32_t defaultVal)
{
	uint32_t value = static_cast<uint32_t>(mIniFile.GetLongValue(section.c_str(), key.c_str(), defaultVal));
	return value;
}
float IniFile::GetFloatValue(String section, String key, float defaultVal)
{
	float value = static_cast<float>(mIniFile.GetDoubleValue(section.c_str(), key.c_str(), defaultVal));
	return value;
}
double IniFile::GetDoubleValue(String section, String key, double defaultVal)
{
	double value = mIniFile.GetDoubleValue(section.c_str(), key.c_str(), defaultVal);
	return value;
}
String IniFile::GetStringValue(String section, String key, String defaultVal)
{
	String value = mIniFile.GetValue(section.c_str(), key.c_str(), defaultVal.c_str());
	return value;
}
vector<String> IniFile::GetMultipleStringValues(String section, String key)
{
	vector<String> stringList;
	String values = mIniFile.GetValue(section.c_str(), key.c_str());
	StringStream ss(values);
	String singleValue = "";
	while (getline(ss, singleValue, ','))
	{
		singleValue.erase(std::remove_if(singleValue.begin(), singleValue.end(), std::isspace),singleValue.end());
		stringList.push_back(singleValue);
	}
	return stringList;
}
SectionList IniFile::GetAllSections()
{
	SectionList list;
	mIniFile.GetAllSections(list);
	list.sort(CSimpleIniA::Entry::LoadOrder());
	return list;
}
bool IniFile::GetBooleanValue(String section, String key, bool defaultVal)
{
	bool value = mIniFile.GetBoolValue(section.c_str(), key.c_str(), defaultVal);
	return value;
}