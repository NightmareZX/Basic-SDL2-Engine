#pragma once
#include "SDL.h"
class Vector2D
{
public:
	Sint32 X;
	Sint32 Y;
	Vector2D() : X(0), Y(0) {}
	Vector2D(Sint32 x, Sint32 y): X(x), Y(y) {}
	inline Vector2D operator+(const Vector2D& vectorOther){ return Vector2D(X + vectorOther.X, Y + vectorOther.Y); }
	inline Vector2D operator-(const Vector2D& vectorOther){ return Vector2D(X - vectorOther.X, Y - vectorOther.Y); }
	inline Vector2D operator*(const Vector2D& vectorOther){ return Vector2D(X * vectorOther.X, Y * vectorOther.Y); }
	inline Vector2D operator/(const Vector2D& vectorOther){ return Vector2D(X / vectorOther.X, Y / vectorOther.Y); }
	inline Vector2D operator*(const float scalar){ return Vector2D(static_cast<Uint32>(X * scalar), static_cast<Uint32>(Y * scalar)); }
	inline Vector2D& operator+=(const Vector2D& vectorOther){ X += vectorOther.X; Y += vectorOther.Y; return *this; }
	inline Vector2D& operator-=(const Vector2D& vectorOther){ X -= vectorOther.X; Y -= vectorOther.Y; return *this; }
	inline Vector2D& operator*=(const Vector2D& vectorOther){ X *= vectorOther.X; Y *= vectorOther.Y; return *this; }
	inline Vector2D& operator/=(const Vector2D& vectorOther){ X /= vectorOther.X; Y /= vectorOther.Y; return *this; }
	inline bool operator==(const Vector2D& vectorOther){ if (vectorOther.X == X && vectorOther.Y == Y) return true; else return false; }
	inline bool operator!=(const Vector2D& vectorOther){ if (vectorOther.X != X || vectorOther.Y != Y) return true; else return false; }
};

class FloatVector2D
{
public:
	float X;
	float Y;
	FloatVector2D() : X(0), Y(0) {}
	FloatVector2D(float x, float y) : X(x), Y(y) {}
	inline FloatVector2D operator+(const FloatVector2D& vectorOther) { return FloatVector2D(X + vectorOther.X, Y + vectorOther.Y); }
	inline FloatVector2D operator-(const FloatVector2D& vectorOther) { return FloatVector2D(X - vectorOther.X, Y - vectorOther.Y); }
	inline FloatVector2D operator*(const FloatVector2D& vectorOther) { return FloatVector2D(X * vectorOther.X, Y * vectorOther.Y); }
	inline FloatVector2D operator/(const FloatVector2D& vectorOther) { return FloatVector2D(X / vectorOther.X, Y / vectorOther.Y); }
	inline FloatVector2D operator*(const float scalar) { return FloatVector2D(X * scalar, Y * scalar); }
	inline FloatVector2D& operator+=(const FloatVector2D& vectorOther) { X += vectorOther.X; Y += vectorOther.Y; return *this; }
	inline FloatVector2D& operator-=(const FloatVector2D& vectorOther) { X -= vectorOther.X; Y -= vectorOther.Y; return *this; }
	inline FloatVector2D& operator*=(const FloatVector2D& vectorOther) { X *= vectorOther.X; Y *= vectorOther.Y; return *this; }
	inline FloatVector2D& operator/=(const FloatVector2D& vectorOther) { X /= vectorOther.X; Y /= vectorOther.Y; return *this; }
	inline bool operator==(const FloatVector2D& vectorOther) { if (vectorOther.X == X && vectorOther.Y == Y) return true; else return false; }
	inline bool operator!=(const FloatVector2D& vectorOther) { if (vectorOther.X != X || vectorOther.Y != Y) return true; else return false; }
};


