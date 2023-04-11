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

class GameLevel
{
private:
	Uint32 mHeight;//Rows of the collisionMap
	Uint32 mWidth;//Columns of the collisionMap 

	PlayerEntity* mPlayer;

	//Map Layers
	TileLayer* mCollisionLayer;
	TileLayer* mForegroundLayer;
	TileLayer* mMiddlegroundLayer;
	TileLayer* mBackgroundLayer;
	//make object layer
public:
	GameLevel();
	~GameLevel();
	void DrawMap();
	void Update(float timeDelta);
	void Dispose();
	inline Layer* GetCollisionLayer() { return mCollisionLayer; }
	inline Uint32 CurrentMapWidth() { return mWidth; }
	inline Uint32 CurrentMapHeight() { return mHeight; }
	inline PlayerEntity* GetMainPlayer() { return mPlayer; }
private:
	friend class MapParser;
	friend class CollisionHandler;
	void AddLayer(Layer* layer, ValidLayers type);
	inline void SetLevelHeight(Uint32 height) { mHeight = height; }
	inline void SetLevelWidth(Uint32 width) { mWidth = width; }
};

