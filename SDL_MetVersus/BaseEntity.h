#pragma once
#include "SDL.h"
#include "STL_Components.h"
#include "BaseGameObject.h"
#include "Vector2D.h"

class BaseEntity : public BaseGameObject
{
protected:
	//x and y offsets of an entity
	Vector2D centrePoint;
	SDL_Rect ColliderBox;
public:
	Uint32 Xpos;
	Uint32 Ypos;

	BaseEntity();
	Vector2D GetCentrePoint() { return centrePoint; }
};

