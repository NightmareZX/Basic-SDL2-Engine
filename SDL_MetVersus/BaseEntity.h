#pragma once
#include "SDL.h"
#include "STL_Components.h"
#include "BaseGameObject.h"
#include "Vector2D.h"
#include "RigidBody.h"
#include "Animation.h"
#include "Collider.h"

class BaseEntity : public BaseGameObject
{
protected:
	String name;

	RigidBody* mRigidBody;
	Collider* mCollider;
	Animation* mAnimation;
	bool mGrounded;
	bool mHitCeiling;
public:
	//Vector2D position;
	BaseEntity(Properties* props): BaseGameObject(props), mRigidBody(new RigidBody), mAnimation(new Animation), mCollider(new Collider),
		mGrounded(false), mHitCeiling(false){}
	virtual void Draw() = 0;
	virtual void Update(float timeDelta) = 0;
	virtual void Dispose() = 0;
	inline RigidBody * GetRigidBody() { return mRigidBody; }
	inline Collider* GetCollider() { return mCollider; }
	inline Animation* GetAnimationHandler() { return mAnimation; };
	inline bool GetGroundedStatus() { return mGrounded; }
	inline void SetGroundedStatus(bool grounded) { mGrounded = grounded; };
	inline bool GetCeilingHitStatus() { return mHitCeiling; }
	inline void SetCeilingHitStatus(bool hitCeiling) { mHitCeiling = hitCeiling; };
	Vector2D GetCentrePoint() 
	{ 

		return Vector2D(mCollider->GetMiddlePointHorizontal(), mCollider->GetMiddlePointVertical()); 
	}
};

