#pragma once
#include "STL_Components.h"
#include "SDL.h"
#include "Constants.h"
#include "FileHandler.h"
#include "RenderManager.h"
#include "TileLayer.h"
#include "CollisionHandler.h"
#include "PlayerEntity.h"
#include "Camera.h"

class Room
{
private:
	Uint32 mHeight;//Rows of the collisionMap
	Uint32 mWidth;//Columns of the collisionMap 

	PlayerEntity* mPlayer;

	//Map Layers
	TileMap mCollisionTileMap;
	TileLayer* mForegroundLayer;
	TileLayer* mMiddlegroundLayer;
	TileLayer* mBackgroundLayer;
	//make object layer
public:
	Room(Uint32 playerX, Uint32 playerY);
	~Room();
	void DrawMap();
	void Update(float timeDelta);
	void Dispose();
	inline TileMap GetCollisionLayer() { return mCollisionTileMap; }
	inline Uint32 CurrentMapWidth() { return mWidth; }
	inline Uint32 CurrentMapHeight() { return mHeight; }
	inline PlayerEntity* GetMainPlayer() { return mPlayer; }
private:
	friend class MapManager;
	friend class CollisionHandler;
	void AddLayer(Layer* layer, ValidLayers type);
	inline void SetLevelHeight(Uint32 height) { mHeight = height; }
	inline void SetLevelWidth(Uint32 width) { mWidth = width; }
};

