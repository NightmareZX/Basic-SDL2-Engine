#pragma once
#include "SDL.h"
#include "STL_Components.h"
#include "Constants.h"
#include "Layer.h"
#include "RenderManager.h"

//A structure that holds data about a tileset
struct TileSet
{
	//This number represents where its tiles begin (First GID)
	Sint32 firstID;
	//This number represents the last GID therefore the last tile
	Sint32 lastID;
	//Total row of tiles
	Sint32 rowCount;
	//total columns of tiles
	Sint32 columnCount;
	//
	Sint32 tileCount;
	//The tile size (should always be 16)
	Sint32 tileSize;
	//The name of the tile set
	String name;
	//The source file of the tile set
	String source;
};
using TileSetList = vector<TileSet>;
using TileMap = vector<vector<Uint32>>;

class TileLayer : public Layer
{
private:
	Uint32 rowCount;
	Uint32 columnCount;

	TileMap tilemap;
	TileSetList tileSetList;

	ValidLayers layerType;
public:
	TileLayer(Uint32 rowCount, Uint32 columnCount, TileMap tilemap, TileSetList tilesets, ValidLayers type):
		rowCount(rowCount), columnCount(columnCount), tilemap(tilemap), tileSetList(tilesets), layerType(type){}
	virtual void Render();
	virtual void Update();
	inline TileMap GetTileMap() { return tilemap; }
	inline Uint32 GetRowCount() { return rowCount; }
	inline Uint32 GetColumnCount() { return columnCount; }
	inline ValidLayers GetLayerType() { return layerType; }
};

const Uint32 LEFT_SLOPE_HEIGHTS[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
