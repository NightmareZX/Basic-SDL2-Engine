#pragma once
#include "FileStream.h"
#include "SDL.h"

class FileHandler
{
public:
	//Write functions
	void WriteBinarySigned32Int(WriteFileStream& wStream, Sint32 number);
	void WriteBinaryUnsigned32Int(WriteFileStream& wStream, Uint32 number);
	void WriteBinaryUnsigned8Int(WriteFileStream& wStream, Uint8 number);

	//Read functions
	Sint32 ReadBinarySigned32Int(ReadFileStream& rStream);
	Uint32 ReadBinaryUnsigned32Int(ReadFileStream& rStream);
	Uint8 ReadBinaryUnsigned8Int(ReadFileStream& rStream);
};

