#include "PlayerEntity.h"


void PlayerEntity::Update()
{
	centrePoint = Vector2D( 
		( ColliderBox.x + ColliderBox.w )* 0.5,
		( ColliderBox.y + ColliderBox.h ) * 0.5);
}