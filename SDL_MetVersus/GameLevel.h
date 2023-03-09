#pragma once
#include "STL_Components.h"
#include "SDL.h"
#include "Constants.h"
#include "TileObject.h"
#include "FileHandler.h"
#include "RenderManager.h"


class GameLevel
{
private:
	//Tile object array that will hold the collision map
	CollisionTile*** collisionMap;
	Uint32 height;//Rows of the collisionMap
	Uint32 width;//Columns of the collisionMap 
public:
	GameLevel();
	~GameLevel();
	void LoadMap(String levelFileName);

	void DrawMap();

	void DisposeMap();
private:
	CollisionTile* CreateCollisionTile(TileTypes type, Uint32 tilePosX, Uint32 tilePosY);

	String getTextureTEMP(TileTypes type);
};

