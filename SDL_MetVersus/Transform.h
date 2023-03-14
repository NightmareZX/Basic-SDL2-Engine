#pragma once
#include "Vector2D.h"

class Transform
{
public:
	float X;
	float Y;

	Transform() : X(0), Y(0){}
	Transform(float x, float y) : X(x), Y(y) {}
	inline void TranslateX(float x) { X += x; }
	inline void TranslateY(float y) { Y += y; }
	inline void Translate(Vector2D vector) { X += vector.X; Y += vector.Y; }
private:

};