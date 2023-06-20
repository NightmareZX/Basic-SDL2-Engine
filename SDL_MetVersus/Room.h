#pragma once
#include "Containers.h"
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

	//Map Layers
	TileMap mCollisionTileMap;
	TileLayer* mForegroundLayer;
	TileLayer* mMiddlegroundLayer;
	TileLayer* mBackgroundLayer;
	
	Camera* mCameraInstance;
	RenderManager* mRendererManagerInstance;

public:
	Room(Camera* camera, RenderManager* rendManager);
	~Room();
	void Update(float timeDelta);
	void Dispose();
	inline TileMap GetCollisionLayer() { return mCollisionTileMap; }
	inline Uint32 CurrentMapWidth() { return mWidth; }
	inline Uint32 CurrentMapHeight() { return mHeight; }
private:
	friend class MapManager;
	friend class CollisionHandler;
	void AddLayer(Layer* layer, ValidLayers type);
	inline void SetLevelHeight(Uint32 height) { mHeight = height; }
	inline void SetLevelWidth(Uint32 width) { mWidth = width; }
	inline void DrawForeground(){ if (mForegroundLayer != nullptr) mForegroundLayer->Render(); }
	inline void DrawMiddleground(){ if (mMiddlegroundLayer != nullptr) mMiddlegroundLayer->Render(); }
	inline void DrawBackground(){ if (mBackgroundLayer != nullptr) mBackgroundLayer->Render(); }
};

