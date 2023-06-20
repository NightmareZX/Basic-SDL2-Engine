#pragma once
#include "BaseEntity.h"
#include "RigidBody.h"
#include "EventHandler.h"


class PlayerEntity : public BaseEntity
{
private:

	float mJumpTimer;
	float mJumpAirTime;
	float mJumpCooldown;
	float mJumpForce;
	float mTurnAroundTimer;

	bool mJumping;

	EventHandler* mEventHandlerInstance;
public:
	PlayerEntity(float x, float y, EventHandler* eventHandler, Animation* anim);
	void Update(float timeDelta) override;
	void Draw() override;
	void Dispose() override;
	inline bool GetJumpingStatus() { return mJumping; }
	String GetPositionStatus();
	void HandleAnimState(float timeDelta);
protected:
	enum ColliderStates
	{
		STANDING_CS,
		STRAIGHT_JUMPING_CS,
		SPIN_JUMPING_CS,
		CROUCHING_CS,
		MORPHED_CS
	};
	void SetColliderState(ColliderStates state);
	void InitialiseTimers();
	void InitialiseAnimStates();
};


