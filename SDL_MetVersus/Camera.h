#pragma once
#include "Constants.h"
#include "SDL.h"
#include "String.h"
#include "Vector2D.h"

class Camera
{
private:
	//The camera position relative to the game map
	SDL_Rect mViewBox;

	//The camera position relative to the screen
	SDL_Rect mDisplayBox;

	Vector2D mPosition;
	Vector2D mBoundPoint;

	
public:
	Camera();
	void Update(Vector2D entityCentrePoint);
	inline SDL_Rect GetViewBox() { return mViewBox; }
	inline SDL_Rect* GetDisplayBox() { return &mDisplayBox; }
	inline void SetCameraBounds(Vector2D boundPoint) { mBoundPoint = boundPoint; };
	inline void SetDisplayBox(Sint32 x, Sint32 y, Sint32 width, Sint32 height) { mDisplayBox = { x,y,width,height }; }
	FloatVector2D GetDrawingScaler();

	inline String GetPositionStatus()
	{
		StringStream ss;
		ss << "CamX: " << mViewBox.x << " CamY: " << mViewBox.y << " CamXW: " << mViewBox.x + mViewBox.w << " CamYH: " << mViewBox.y + mViewBox.h;
		return ss.str();
	}
};

