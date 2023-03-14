#pragma once
#include "STL_Components.h"
#include "SDL.h"
#include "Constants.h"
#include "TileObject.h"
#include "FileHandler.h"
#include "RenderManager.h"
#include "TileLayer.h"
#include "CollisionHandler.h"
#include "PlayerEntity.h"

using LayerList = vector<Layer*>;
class GameLevel
{
private:
	Uint32 height;//Rows of the collisionMap
	Uint32 width;//Columns of the collisionMap 
	LayerList layers;

	PlayerEntity* player;
	TileLayer* collisionLayer;
public:
	GameLevel();
	~GameLevel();
	void DrawMap();
	void Update();
	void DisposeMap();
	inline void SetCollisionLayer(TileLayer* collisionLayer) { this->collisionLayer = collisionLayer; }
	inline Layer* GetCollisionLayer() { return collisionLayer; }
private:
	friend class MapParser;
	friend class CollisionHandler;
	void AddLayer(Layer* layer);
};

