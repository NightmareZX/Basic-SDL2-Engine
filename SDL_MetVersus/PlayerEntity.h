#pragma once
#include "BaseEntity.h"
#include "RigidBody.h"

class PlayerEntity : public BaseEntity
{
private:
	enum FacingDirection;
	bool mJumping;
	

	float mJumpTimer;
	float mJumpAirTime;
	float mJumpCooldown;
	float mJumpForce;

	FacingDirection mFacingDirection;
public:
	PlayerEntity(Properties* props);
	void Update(float timeDelta) override;
	void Draw() override;
	void Dispose() override;
	inline bool GetJumpingStatus() { return mJumping; }
	String GetPositionStatus()
	{
		StringStream ss;
		ss << "X: " << mCollider->GetBboxLeft() <<
			"  Y: " << mCollider->GetBboxTop() <<
			" XW: " << mCollider->GetBboxRight() <<
			" YH: " << mCollider->GetBboxBottom() <<
			" HM: " << mCollider->GetMiddlePointHorizontal();
		return ss.str();
	}
private:
	enum AnimStates
	{
		STANDING,
		RUNNING,
		RUNNING_AIMING_FORWARD,
		RUNNING_AIMING_DIAGONAL_UP,
		RUNNING_AIMING_DIAGONAL_DOWN,
		STANDING_TURNING,
		STRIGHT_JUMPING,
		SPIN_JUMPING,
		LANDING,
		SHOOTING,
		MIDAIR_SHOOTING,
		MIDAIR_AIMING_UP,
		MIDAIR_AIMING_DOWN,
		MIDAIR_AIMING_DIAGONAL_UP,
		MIDAIR_AIMING_DIAGONAL_DOWN,
		MIDAIR_TURING,
		AIMING_UP,
		AIMING_DIAGONAL_UP,
		AIMING_DIAGONAL_DOWN,
		CROUCHING,
		CROUCHING_TRANSITION,
		CROUCH_AIMING_UP,
		CROUCH_AIMING_DIAGONAL_UP,
		CROUCH_AIMING_DIAGONAL_DOWN,
		CROUCH_TRUNING,
		MORPHING_TRANSITION,
		MORPH_ROLLING

		
	};
	enum FacingDirection
	{
		LEFT,
		RIGHT
	};
	void InitialiseAnimStates();
	String GetAnimState();
};


