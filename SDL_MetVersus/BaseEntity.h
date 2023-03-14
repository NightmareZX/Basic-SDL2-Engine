#pragma once
#include "SDL.h"
#include "STL_Components.h"
#include "BaseGameObject.h"
#include "Vector2D.h"
#include "RigidBody.h"

class BaseEntity : public BaseGameObject
{
protected:
	//x and y offsets of an entity
	Vector2D centrePoint;
	//SDL_Rect ColliderBox;
	String name;

	RigidBody* mRigidBody;
public:
	//Vector2D position;
	BaseEntity(Properties* props): BaseGameObject(props){}
	virtual void Draw() = 0;
	virtual void Update() = 0;
	virtual void Dispose() = 0;
	Vector2D GetCentrePoint() { return centrePoint; }
};

