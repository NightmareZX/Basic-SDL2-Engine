#pragma once
#include "Vector2D.h"
#include "DeltaTime.h"

class RigidBody
{
public:
	RigidBody() { mMass = 1.0f; mGravity = 0.1f; }
	inline void SetMass(float mass) { this->mMass = mass; }
	inline void SetGravity(float grav) { mGravity = grav; }

	inline void ApplyForce(PreciseVector2D force) { this->mForce = force; }
	inline void ApplyForceX(float Fx) { mForce.X = Fx; }
	inline void ApplyForceY(float Fy) { mForce.Y = Fy; }
	inline void ResetForceX() { mForce.X = 0; }
	inline void ResetForceY() { mForce.Y = 0; }

	inline void ApplyFriction(PreciseVector2D Fr) { mFriction = Fr; }
	inline void ResetFriction() { mFriction.X = 0; mFriction.Y = 0; }

	inline void ResetPosition() { mPosition.X = 0; mPosition.Y = 0; }
	inline void ResetVelocityX() { mVelocity.X = 0; }
	inline void ResetVelocityY() { mVelocity.Y = 0; }
	inline void ResetAcceleration() { mAcceleration.X = 0; mAcceleration.Y = 0; }

	inline float GetMass() { return mMass; }
	inline PreciseVector2D GetPosition() { return mPosition; }
	inline PreciseVector2D GetVelocity() { return mVelocity; }
	inline PreciseVector2D GetAcceleration() { return mAcceleration; }



	void Update()
	{
		float decay = 0.01f;
		mMaxVelocityX = 2.0f;
		mMaxVelocityY = 8.5f;
		mAcceleration.X = (mForce.X + mFriction.X) / mMass;
		mAcceleration.Y = mGravity + mForce.Y / mMass;

		mVelocity += mAcceleration * GlobalTimer::GetInstance()->GetTimeDelta();

		if (mVelocity.X > 0.0f && mVelocity.X > mMaxVelocityX)
			mVelocity.X = mMaxVelocityX;
		else if (mVelocity.X < 0.0f && mVelocity.X < -mMaxVelocityX)
			mVelocity.X = -mMaxVelocityX;

		if (mVelocity.Y > 0.0f && mVelocity.Y > mMaxVelocityY)
			mVelocity.Y = mMaxVelocityY;
		else if (mVelocity.Y < 0.0f && mVelocity.X < -mMaxVelocityY)
			mVelocity.Y = -mMaxVelocityY;

		if (mForce.X == 0.0f)
		{
			if (mVelocity.X > -0.1f && mVelocity.X < 0.1f) mVelocity.X = 0;
			else if (mVelocity.X < 0.0f)
				mVelocity.X += decay;
			else if (mVelocity.X > 0.0f)
				mVelocity.X -= decay;
		}

		//if (mForce.Y == 0.0f)
		//{
		//	if (mVelocity.Y < 0.0f)
		//		mVelocity.Y += 0.1f;
		//	else if (mVelocity.Y > 0.0f)
		//		mVelocity.Y -= 0.1f;
		//}
		mPosition = mVelocity * GlobalTimer::GetInstance()->GetTimeDelta();
		
	}
private:
	float mMass;
	float mGravity;

	float mMaxVelocityX;
	float mMaxVelocityY;

	PreciseVector2D mForce;
	PreciseVector2D mFriction;
	PreciseVector2D mPosition;
	PreciseVector2D mVelocity;
	PreciseVector2D mAcceleration;
};