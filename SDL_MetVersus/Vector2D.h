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

class PreciseVector2D
{
public:
	float X;
	float Y;
	PreciseVector2D() : X(0), Y(0) {}
	PreciseVector2D(float x, float y) : X(x), Y(y) {}
	inline PreciseVector2D operator+(const PreciseVector2D& vectorOther) { return PreciseVector2D(X + vectorOther.X, Y + vectorOther.Y); }
	inline PreciseVector2D operator-(const PreciseVector2D& vectorOther) { return PreciseVector2D(X - vectorOther.X, Y - vectorOther.Y); }
	inline PreciseVector2D operator*(const PreciseVector2D& vectorOther) { return PreciseVector2D(X * vectorOther.X, Y * vectorOther.Y); }
	inline PreciseVector2D operator/(const PreciseVector2D& vectorOther) { return PreciseVector2D(X / vectorOther.X, Y / vectorOther.Y); }
	inline PreciseVector2D operator*(const float scalar) { return PreciseVector2D(X * scalar, Y * scalar); }
	inline PreciseVector2D& operator+=(const PreciseVector2D& vectorOther) { X += vectorOther.X; Y += vectorOther.Y; return *this; }
	inline PreciseVector2D& operator-=(const PreciseVector2D& vectorOther) { X -= vectorOther.X; Y -= vectorOther.Y; return *this; }
	inline PreciseVector2D& operator*=(const PreciseVector2D& vectorOther) { X *= vectorOther.X; Y *= vectorOther.Y; return *this; }
	inline PreciseVector2D& operator/=(const PreciseVector2D& vectorOther) { X /= vectorOther.X; Y /= vectorOther.Y; return *this; }
	inline bool operator==(const PreciseVector2D& vectorOther) { if (vectorOther.X == X && vectorOther.Y == Y) return true; else return false; }
	inline bool operator!=(const PreciseVector2D& vectorOther) { if (vectorOther.X != X || vectorOther.Y != Y) return true; else return false; }
};


