#pragma once
#include "BaseEntity.h"
#include "RigidBody.h"
#include "Animation.h"
#include "Collider.h"

class PlayerEntity : public BaseEntity
{
private:
	Animation* mAnimation;
	Collider* mCollider;
	PreciseVector2D mLastSafePosition;

	bool mJumping;
	bool mGrounded;

	float mJumpTimer;
	float mJumpAirTime;
	float mJumpCooldown;
	float mJumpForce;
public:
	PlayerEntity(Properties* props);
	void Update() override;
	void Draw() override;
	void Dispose() override;
};

