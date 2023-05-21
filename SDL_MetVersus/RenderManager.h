#pragma once
#include "STL_Components.h"
#include "Constants.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

using FontData = pair<SDL_Color, TTF_Font*>;

class RenderManager
{
private:
	RenderManager();
	Uint32 mWindowHeight;
	Uint32 mWindowWidth;
	static RenderManager* mInstance;
	map<String, SDL_Texture*> mTextureMap;
	map<String, SDL_Surface*> mSurfaceMap;
	map<String, FontData> mFontMap;
public:
	
	inline static RenderManager* GetInstance(){ if (mInstance == nullptr) mInstance = new RenderManager(); return mInstance; }
	static void SurfaceFlipHorizontal(SDL_Surface* surface);
	static void SurfaceFlipVertical(SDL_Surface* surface);
	static void BlitSurface(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Surface* dst, SDL_Rect* dstrect, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void RegisterTexture(String id, String filename);
	SDL_Surface* RegisterSurface(String id, String filename);
	void RegisterFont(String id, String filename, Uint32 size);
	void Draw();
	void DrawRectangle(SDL_Rect rectangle);
	void DrawRectangleRelativeToCamera(SDL_Rect rectangle);
	void DrawFrame
	(String textureID, Sint32 x, Sint32 y, Sint32 width, Sint32 Height,
		Uint32 frameRowIndex, Uint32 frameColumnIndex, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0);
	void DrawTexture(SDL_Texture* texture ,Sint32 x, Sint32 y, Sint32 width, Sint32 height);
	void DrawTile(String textureID, Sint32 x, Sint32 y, Uint32 column, Uint32 row,
		double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE, Uint32 tileSize = TILE_SIZE);
	void DropResource(String id);
	void DrawText(String fontID, Sint32 x, Sint32 y, String Text);
	bool ValidateTexture(String id);
	bool ValidateSurface(String id);
	bool ValidateFont(String id);
	void SetWindowDimensions(Uint32 width, Uint32 height);
	void Dispose();
};

//Source SDL_Rects are used to specify which part of the image you want to use as a texture (For tile map pictures)

//Destination SDL_Rects are used to specify where the image should be drawn (x = 0,y = 0 is top left corner)

