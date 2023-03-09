#include "FileHandler.h"

//Yes i know, reinterpret_casts are not very safe but rn it works so its fine

//Writing
void FileHandler::WriteBinarySigned32Int(WriteFileStream& wStream, Sint32 number)
{
	wStream.write(reinterpret_cast<char*>(&number), sizeof(number));
}
void FileHandler::WriteBinaryUnsigned32Int(WriteFileStream& wStream, Uint32 number)
{
	wStream.write(reinterpret_cast<char*>(&number), sizeof(number));
}
void FileHandler::WriteBinaryUnsigned8Int(WriteFileStream& wStream, Uint8 number)
{
	wStream.write(reinterpret_cast<char*>(&number), sizeof(number));
}


//Reading
Sint32 FileHandler::ReadBinarySigned32Int(ReadFileStream& rStream)
{
	Sint32 number;
	rStream.read(reinterpret_cast<char*>(&number), sizeof(number));
	return number;
}
Uint32 FileHandler::ReadBinaryUnsigned32Int(ReadFileStream& rStream)
{
	Uint32 number;
	rStream.read(reinterpret_cast<char*>(&number), sizeof(number));
	return number;
}
Uint8 FileHandler::ReadBinaryUnsigned8Int(ReadFileStream& rStream)
{
	Uint8 number;
	rStream.read(reinterpret_cast<char*>(&number), sizeof(number));
	return number;
}