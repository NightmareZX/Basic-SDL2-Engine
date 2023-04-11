#pragma once
#include "SDL.h"

class Collider
{
private:
	SDL_Rect box;
	SDL_Rect buffer;
public:
	Collider() { buffer = { 0,0,0,0 }; box = { 0,0,0,0 }; }
	inline SDL_Rect GetBox() { return box; }
	inline void SetBuffer(Sint32 x, Sint32 y, Sint32 w, Sint32 h){ buffer = { x,y,w,h }; }
	inline void SetDimensions(Sint32 w, Sint32 h) { box.w = w - buffer.w; box.h = h - buffer.h; }
	void SetPosition(Sint32 x, Sint32 y)
	{
		box.x = x - buffer.x;
		box.y = y - buffer.y;
	}
	//This function gets the vertical middle point of the bounding box
	inline Sint32 GetMiddlePointVertical() { return box.y + (box.h / 2); }
	//This function gets the vertical middle point between the top and main middle point
	inline Sint32 GetMiddlePointVerticalTop() { return box.y + ((box.h / 2) / 2); }
	//This function gets the vertical middle point between the bottom and main middle point
	inline Sint32 GetMiddlePointVerticalBottom() { return box.y + ((box.h / 2) + (box.h / 2) / 2); }
	//This function gets the horizontal middle point of the bounding box
	inline Sint32 GetMiddlePointHorizontal() { return box.x + (box.w / 2); }
	//This function gets the left edge position of the bounding box
	inline Sint32 GetBboxLeft() { return box.x; }
	//This function gets the right edge position of the bounding box
	inline Sint32 GetBboxRight() { return box.x + box.w; }
	//This function gets the top edge position of the bounding box
	inline Sint32 GetBboxTop() { return box.y; }
	//This function gets the bottom edge position of the bounding box
	inline Sint32 GetBboxBottom() { return box.y + box.h; }
	inline Sint32 GetBoxHeight() { return box.h; }
	inline Sint32 GetBoxWidth() { return box.w; }
	inline SDL_Rect GetUnbufferedBox() { SDL_Rect unbuffered = { box.x + buffer.x, box.y + buffer.y, box.w + buffer.w, box.h + buffer.h }; return unbuffered; }
};