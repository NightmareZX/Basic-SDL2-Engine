#pragma once
#include "STL_Components.h"
#include "SDL.h"
#include "TileLayer.h"
#include "Vector2D.h"

//X is the column and Y is the row in the collision layer array 
using TileData = pair<Vector2D, TileTypes>;
class CollisionHandler
{
private:
	//TileMap layerTilemap;
	CollisionHandler(){}
	static CollisionHandler* instance;
public:
	inline static CollisionHandler* GetInstance() { if (instance == nullptr)instance = new CollisionHandler; return instance; }

	bool CheckCollision(SDL_Rect a, SDL_Rect b);
	TileData MapCollision(SDL_Rect a);
	TileData GetMapCollisionDataFromPoint(Vector2D point);
};

