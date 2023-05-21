#pragma once
#include "SDL.h"
#include "STL_Components.h"
#include "Constants.h"
#include "Layer.h"
#include "RenderManager.h"
#include "Vector2D.h"
#include "Matrix2D.h"
//#include "SimpleMatrix.h"

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
	VERTICAL_SOLID = 4,
	CEILING_LEFT_SLOPE = 5,
	CEILING_RIGHT_SLOPE = 6,

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

struct TileSetInfo
{
	String tileSetPath = "";

	Uint32 width = 0;
	Uint32 height = 0;
	//A map that keeps a specific tile's position in its tilemap texture, so that it can be used as a reference when rendering
	unordered_map<Uint32, Vector2D> tileTextureSource;
};

using TileSetList = vector<TileSet>;
using TileMap = Matrix2D<Uint32>;

class TileLayer : public Layer
{
private:
	Uint32 mHeight;
	Uint32 mWidth;

	TileMap mTilemap;
	String mTileSetName;

public:
	TileLayer(Uint32 height, Uint32 width, TileMap tilemap,String tileSetName):
		mHeight(height), mWidth(width), mTilemap(tilemap), mTileSetName(tileSetName){}
	virtual void Render();
	virtual void Update();
	inline TileMap GetTileMap() { return mTilemap; }
	//inline Uint32 AccessTile(size_t row, size_t column) {return}
	inline Uint32 GetRowCount() { return mHeight; }
	inline Uint32 GetColumnCount() { return mWidth; }
};
