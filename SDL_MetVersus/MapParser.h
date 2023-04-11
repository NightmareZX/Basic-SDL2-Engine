#pragma once
#include "STL_Components.h"
#include "TinyXML/tinyxml.h"
#include "GameLevel.h"
#include "Logger.h"

/// <summary>
/// This enumerator holds information about a tile in 32 bits, which holds fliping information as well as tile type.
/// </summary>
enum TileInformation : Uint32
{
	SOLID_TILE = 0x00000001,
	SLOPE_TILE = 0x00000002,

	HORIZONTAL_FLIP = 0x80000000,
	VERTICAL_FLIP = 0x40000000,
	DIAGONAL_FLIP = 0x20000000
};

class MapParser
{
private:
	static MapParser* instance;
	GameLevel* loadedMap;
public:
	MapParser(){}
	inline static MapParser* GetInstance()
	{
		if (instance == nullptr) instance = new MapParser();
		return instance;
	}
	bool Parse(String id, String source);
	TileSet* ParseTileset(TiXmlElement* xmlTileset);
	TileLayer* ParseTileLayer(TiXmlElement* xmlLayer, TileSetList tilesets, Uint32 rowCount, Uint32 columnCount, Uint32 tilesize = TILE_SIZE);
	GameLevel* GetMap();
	bool LoadMap(String mapname);
	void Clean();
	void Dispose();
};

