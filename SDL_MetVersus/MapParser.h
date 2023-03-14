#pragma once
#include "STL_Components.h"
#include "TinyXML/tinyxml.h"
#include "GameLevel.h"
#include "Logger.h"

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
	CollisionTile*** ParseCollisonMap(TiXmlElement* xmlTileset);
	GameLevel* GetMap();
	bool LoadMap(String mapname);
	void Clean();
	void Dispose();
};

