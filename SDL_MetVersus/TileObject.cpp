#include "TileObject.h"

TileObject::TileObject()
{
	source.x = 0;
	source.y = 0;
	source.w = 0;
	source.h = 0;
	destination.x = 0;
	destination.y = 0;
	destination.w = 0;
	destination.h = 0;
	textureID = "";

}
TileObject::~TileObject()
{
}
SDL_Rect* TileObject::getSource() { return &source; }
SDL_Rect* TileObject::getDestination() { return &destination; }
String TileObject::getTextureID() { return textureID; }
void TileObject::setSource(Uint32 x, Uint32 y, Uint32 width, Uint32 height)
{
	source.x = x;
	source.y = y;
	source.h = height;
	source.w = width;
}
void TileObject::setDestination(Uint32 x, Uint32 y, Uint32 width, Uint32 height)
{
	destination.x = x;
	destination.y = y;
	destination.h = height;
	destination.w = width;
}
void TileObject::setTexture(String id)
{
	textureID = id;
}


CollisionTile::~CollisionTile()
{

}
void CollisionTile::setCollisonType(TileTypes type)
{
	this->type = type;
}