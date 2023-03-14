#pragma once
#include "STL_Components.h"
#include "SDL.h"
#include "DeltaTime.h"

class Animation
{
private:
	Uint32 mSpriteRow;
	Uint32 mSpriteFrame;
	Uint32 mAnimSpeed;
	Uint32 mFrameCount;
	String mTextureID;
	SDL_RendererFlip mFlip;
public:
	Animation() {}
	void Update();
	void Draw(float x, float y, Uint32 spriteWidth, Uint32 spriteHeight);
	void SetProperties(String TextureID, Uint32 spriteRow, Uint32 frameCount, Uint32 animSpeed, SDL_RendererFlip flip = SDL_FLIP_NONE);
};

