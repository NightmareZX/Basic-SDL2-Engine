#pragma once
#include "SDL.h"
#include "STL_Components.h"
#include "Constants.h"

class BaseGameObject
{
protected:
public:
	virtual ~BaseGameObject(){}
	virtual void Update(){}
};

