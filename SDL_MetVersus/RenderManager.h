#pragma once
#include "STL_Components.h"
#include "SDL.h"

class RenderManager
{
private:
	static RenderManager* instance;
	map<String, SDL_Texture*> textureMap;
public:
	static RenderManager* GetInstance();
	void RegisterTexture(String id, String filename);
	void Draw();
	void DrawFrame();
	void DrawTile(String textureID, SDL_Rect* source, SDL_Rect* dest, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void DropResource();
	void Dispose();
};

//Source SDL_Rects are used to specify which part of the image you want to use as a texture (For tile map pictures)

//Destination SDL_Rects are used to specify where the image should be drawn (x = 0,y = 0 is top left corner)

