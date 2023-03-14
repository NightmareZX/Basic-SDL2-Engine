#include "Camera.h"

Camera* Camera::instance = nullptr;

Camera::Camera()
{
	target = nullptr;
	viewBox.x = 0;
	viewBox.y = 0;
	viewBox.w = WINDOW_WIDTH;
	viewBox.h = WINDOW_HEIGHT;
}

Camera* Camera::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Camera();
	}
	return instance;
};

void Camera::Update()
{
	if (target == nullptr)
	{
		return;
	}

	viewBox.x = target->GetCentrePoint().X - (WINDOW_WIDTH / 2);
	viewBox.y = target->GetCentrePoint().Y - (WINDOW_HEIGHT / 2);

	if (viewBox.x < 0) viewBox.x = 0;
	if (viewBox.y < 0) viewBox.y = 0;

	if (viewBox.x > 2 * WINDOW_WIDTH - viewBox.w) viewBox.x = WINDOW_WIDTH - viewBox.w;
	if (viewBox.y > 2 * WINDOW_HEIGHT - viewBox.h) viewBox.y = WINDOW_WIDTH - viewBox.h;

	position = Vector2D(viewBox.x, viewBox.y);
}