#pragma once
#include "SDL.h"
#include "STL_Components.h"
#include "Constants.h"
#include "IGameObject.h"
#include "Transform.h"

//struct Properties
//{
//	Properties(String textureID, float x, float y, Uint32 width, Uint32 height, SDL_RendererFlip flip = SDL_FLIP_NONE)
//	{
//		this->textureID = textureID;
//		X = x;
//		Y = y;
//		this->width = width;
//		this->height = height;
//		this->flip = flip;
//	}
//	String textureID;
//	float X;
//	float Y;
//	Uint32 width;
//	Uint32 height;
//	SDL_RendererFlip flip;
//};


class BaseGameObject : public IGameObject
{
protected:
	Transform* mTransform;
public:
	BaseGameObject(float x, float y)
	{
		mTransform = new Transform(x, y);
	}
	virtual void Update(float timeDelta) = 0;
	virtual void Draw() = 0;
	virtual void Dispose() = 0;
	inline Transform* GetTransform() { return mTransform; }
};