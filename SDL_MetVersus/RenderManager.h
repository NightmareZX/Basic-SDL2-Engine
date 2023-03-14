#pragma once
#include "STL_Components.h"
#include "Constants.h"
#include "SDL.h"

class RenderManager
{
private:
	RenderManager();
	static RenderManager* instance;
	map<String, SDL_Texture*> textureMap;
	float mZoom;
public:
	
	static RenderManager* GetInstance();
	void RegisterTexture(String id, String filename);
	void Draw();
	void DrawRectangle(SDL_Rect rectangle);
	void DrawFrame
	(String textureID, Sint32 x, Sint32 y, Sint32 width, Sint32 Height,
		Uint32 row, Uint32 frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void DrawTile(String textureID, Sint32 x, Sint32 y, Uint32 column, Uint32 row,
		double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE, Uint32 tileSize = TILE_SIZE);
	void DropResource(String id);
	bool ValidateTexture(String id);
	void Dispose();
};

//Source SDL_Rects are used to specify which part of the image you want to use as a texture (For tile map pictures)

//Destination SDL_Rects are used to specify where the image should be drawn (x = 0,y = 0 is top left corner)

