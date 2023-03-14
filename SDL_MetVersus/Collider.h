#pragma once
#include "SDL.h"

class Collider
{
private:
	SDL_Rect box;
	SDL_Rect buffer;
public:
	inline SDL_Rect GetBox() { return box; }
	inline void SetBuffer(Sint32 x, Sint32 y, Sint32 w, Sint32 h){ buffer = { x,y,w,h }; }
	void Set(Sint32 x, Sint32 y, Sint32 w, Sint32 h)
	{
		box = { x - buffer.x, y - buffer.y, w - buffer.w, h - buffer.h };
	}
};