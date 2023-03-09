#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "STL_Components.h"
#include "Constants.h"
#include "BaseGameObject.h"

class TileObject : public BaseGameObject
{
protected:
	SDL_Rect source;
	SDL_Rect destination;
	String textureID;//debug
public:
	TileObject();
	~TileObject();
	SDL_Rect* getSource();
	SDL_Rect* getDestination();
	String getTextureID();
	void setSource(Uint32 x, Uint32 y, Uint32 width, Uint32 height);
	void setDestination(Uint32 x, Uint32 y, Uint32 width, Uint32 height);
	void setTexture(String textureID);//debug
	void DrawTile(SDL_Renderer* mainRenderer);//debug
};

class CollisionTile : public TileObject
{
protected:
	//Defines what collision type the tile is
	TileTypes type;
public:
	CollisionTile(): TileObject(){}
	~CollisionTile();
	void setCollisonType(TileTypes type);
};

