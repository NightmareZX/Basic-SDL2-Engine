#pragma once
#include "SDL.h"
#include "STL_Components.h"
#include "Constants.h"
#include "Layer.h"
#include "RenderManager.h"

//===============================================================
//						Tile Constants
//===============================================================
//The tile size that the game will use

enum TileTypes : Uint8
{
	AIR = 0,//Typical air tile where the player can move trough no problem
	SOLID = 1,//Activly stops most objects including the player from going inside it
	LEFT_SLOPE = 2,
	RIGHT_SLOPE = 3,
	CEILING_LEFT_SLOPE = 4,
	CEILING_RIGHT_SLOPE = 5,

	OUT_OF_BOUNDS = 0xDF,
	NONE = 0xEF,
	UNKNOWN = 0xFF
};


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
	Uint32 mHeight;
	Uint32 mWidth;

	TileMap mTilemap;
	TileSetList mTileSetList;

	ValidLayers mLayerType;
public:
	TileLayer(Uint32 height, Uint32 width, TileMap tilemap, TileSetList tilesets, ValidLayers type):
		mHeight(height), mWidth(width), mTilemap(tilemap), mTileSetList(tilesets), mLayerType(type){}
	virtual void Render();
	virtual void Update();
	inline TileMap GetTileMap() { return mTilemap; }
	inline Uint32 GetRowCount() { return mHeight; }
	inline Uint32 GetColumnCount() { return mWidth; }
	inline ValidLayers GetLayerType() { return mLayerType; }
};
