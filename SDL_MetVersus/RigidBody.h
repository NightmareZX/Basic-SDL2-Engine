#pragma once
#include "Vector2D.h"

class RigidBody
{
public:
	RigidBody() { mMass = 1.0f; mGravity = 0.1f; }
	inline void SetMass(float mass) { this->mMass = mass; }
	inline void SetGravity(float grav) { mGravity = grav; }

	inline void ApplyForce(FloatVector2D force) { this->mForce = force; }
	inline void ApplyForceX(float Fx) { mForce.X = Fx; }
	inline void ApplyForceY(float Fy) { mForce.Y = Fy; }
	inline void ResetForceX() { mForce.X = 0; }
	inline void ResetForceY() { mForce.Y = 0; }

	inline void ApplyFrictionY(float fr) { mFriction.Y = fr; }
	inline void ApplyFrictionX(float fr) { mFriction.X = fr; }
	inline void ResetFrictionY() { mFriction.Y = 0; }
	inline void ResetFrictionX() { mFriction.X = 0; }

	inline void ResetPosition() { mPosition.X = 0; mPosition.Y = 0; }
	inline void ResetVelocityX() { mVelocity.X = 0; }
	inline void ResetVelocityY() { mVelocity.Y = 0; }
	inline void ResetAcceleration() { mAcceleration.X = 0; mAcceleration.Y = 0; }

	inline float GetMass() { return mMass; }
	inline FloatVector2D GetPosition() { return mPosition; }
	inline FloatVector2D GetVelocity() { return mVelocity; }
	inline FloatVector2D GetAcceleration() { return mAcceleration; }


	/// <summary>
	/// 
	/// </summary>
	/// <param name="addGravity:"> should the gravity be applied or not</param>
	void UpdateY(bool addGravity, float timeDelta)
	{

		mMaxVelocityY = 8.5f;
		
		if (addGravity) mAcceleration.Y = mGravity + mForce.Y / mMass;
		else mAcceleration.Y = (mForce.Y == 0.0f) ? mForce.Y : mForce.Y / mMass;
		

		mVelocity.Y += mAcceleration.Y * timeDelta;


		if (mVelocity.Y > 0.0f && mVelocity.Y > mMaxVelocityY)
			mVelocity.Y = mMaxVelocityY;
		else if (mVelocity.Y < 0.0f && mVelocity.X < -mMaxVelocityY)
			mVelocity.Y = -mMaxVelocityY;

		//if (mForce.Y == 0.0f)
		//{
		//	if (mVelocity.Y < 0.0f)
		//		mVelocity.Y += 0.1f;
		//	else if (mVelocity.Y > 0.0f)
		//		mVelocity.Y -= 0.1f;
		//}
		mPosition.Y = mVelocity.Y * timeDelta;
		
	}
	void UpdateX(float timeDelta)
	{
		float decay = 0.1f;
		mMaxVelocityX = 2.0f;
		mAcceleration.X = (mForce.X + mFriction.X) / mMass;

		mVelocity.X += mAcceleration.X * timeDelta;

		if (mVelocity.X > 0.0f && mVelocity.X > mMaxVelocityX)
			mVelocity.X = mMaxVelocityX;
		else if (mVelocity.X < 0.0f && mVelocity.X < -mMaxVelocityX)
			mVelocity.X = -mMaxVelocityX;

		if (mForce.X == 0.0f)
		{
			if (mVelocity.X > -0.1f && mVelocity.X < 0.1f) mVelocity.X = 0;
			else if (mVelocity.X < 0.0f)
				mVelocity.X += decay;
			else if (mVelocity.X > 0.0f)
				mVelocity.X -= decay;
		}

		mPosition.X = mVelocity.X * timeDelta;
	}
private:
	float mMass;
	float mGravity;

	float mMaxVelocityX;
	float mMaxVelocityY;

	FloatVector2D mForce;
	FloatVector2D mFriction;
	FloatVector2D mPosition;
	FloatVector2D mVelocity;
	FloatVector2D mAcceleration;
};