#pragma once
#include "Constants.h"
#include "SDL.h"
#include "BaseEntity.h"

class Camera
{
private:
	SDL_Rect viewBox;
	BaseEntity* target;

	Vector2D position;

	static Camera* instance;
public:
	
	Camera();

	static Camera* GetInstance();

	void Update();
};

