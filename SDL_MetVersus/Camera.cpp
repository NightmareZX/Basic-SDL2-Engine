#include "Camera.h"

//Default Widescreen
constexpr Uint16 VIEWBOX_WIDESCREEN_WIDTH = 400;
constexpr Uint16 VIEWBOX_WIDESCREEN_HEIGHT = 192;

Camera* Camera::mInstance = nullptr;

Camera::Camera()
{
	mTargetEntity = nullptr;
	mViewBox.x = 0;
	mViewBox.y = 0;
	mViewBox.w = VIEWBOX_WIDESCREEN_WIDTH;
	mViewBox.h = VIEWBOX_WIDESCREEN_HEIGHT;

	mDisplayBox = mViewBox;

	mBoundPoint.X = mViewBox.w;
	mBoundPoint.Y = mViewBox.h;
}

void Camera::Update()
{
	if (mTargetEntity == nullptr)
	{
		return;
	}
	Vector2D entityCentrePoint = mTargetEntity->GetCentrePoint();
	mViewBox.x = entityCentrePoint.X - (mViewBox.w / 2);
	mViewBox.y = entityCentrePoint.Y - (mViewBox.h / 2);

	if (mViewBox.x < 0) mViewBox.x = 0;
	if (mViewBox.y < 0) mViewBox.y = 0;

	if (mViewBox.x + mViewBox.w >= mBoundPoint.X) mViewBox.x = mBoundPoint.X - mViewBox.w;
	if (mViewBox.y + mViewBox.h >= mBoundPoint.Y) mViewBox.y = mBoundPoint.Y - mViewBox.h;


	//if (mViewBox.x > 2 * WINDOW_WIDTH - mViewBox.w) mViewBox.x = WINDOW_WIDTH - mViewBox.w;
	//if (mViewBox.y > 2 * WINDOW_HEIGHT - mViewBox.h) mViewBox.y = WINDOW_WIDTH - mViewBox.h;

	mPosition = Vector2D(mViewBox.x, mViewBox.y);
}
FloatVector2D Camera::GetDrawingScaler()
{
	Sint32 tileWidth = mDisplayBox.w / (mViewBox.w / TILE_SIZE);
	Sint32 tileHeight = mDisplayBox.h / (mViewBox.h / TILE_SIZE);

	FloatVector2D scalers(
		static_cast<float>(tileWidth) / static_cast<float>(TILE_SIZE),
		static_cast<float>(tileHeight) / static_cast<float>(TILE_SIZE));
	return scalers;
}