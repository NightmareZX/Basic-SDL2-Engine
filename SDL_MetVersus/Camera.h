#pragma once
#include "Constants.h"
#include "SDL.h"
#include "BaseEntity.h"

class Camera
{
private:
	//The camera position relative to the game map
	SDL_Rect mViewBox;

	//The camera position relative to the screen
	SDL_Rect mDisplayBox;

	BaseEntity* mTargetEntity;

	Vector2D mPosition;
	Vector2D mBoundPoint;

	static Camera* mInstance;
	Camera();
public:
	
	inline static Camera* GetInstance() { if (mInstance == nullptr) mInstance = new Camera(); return mInstance; }

	void Update();
	inline void SetTarget(BaseEntity* targetEntity) { mTargetEntity = targetEntity; }
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

