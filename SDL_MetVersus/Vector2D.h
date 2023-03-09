#pragma once
#include "SDL.h"
class Vector2D
{
public:
	Uint32 X;
	Uint32 Y;
	Vector2D() : X(0), Y(0) {}
	Vector2D(Uint32 x, Uint32 y): X(x), Y(y) {}
};

