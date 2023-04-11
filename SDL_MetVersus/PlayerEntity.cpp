#include "PlayerEntity.h"
#include "RenderManager.h"
#include "EventHandler.h"
#include "CollisionHandler.h"

PlayerEntity::PlayerEntity(Properties* props) : BaseEntity(props)
{
	//RenderManager::GetInstance()->RegisterTexture("basic_player.png", "basic_player.png");
	//mAnimation->SetProperties("basic_player.png", 0, 0, 1);
	mAnimation->Initialise("basic_player_sheet");
	mCollider->SetDimensions(15, 43);
	mJumpForce = -0.8f;
	InitialiseAnimStates();
}

void PlayerEntity::Update(float timeDelta)
{
	//Basic movement
	if (EventHandler::GetInstance()->GetAction(MOVE_RIGHT))
		mRigidBody->ApplyForceX(0.5f);
	else if (EventHandler::GetInstance()->GetAction(MOVE_LEFT))
		mRigidBody->ApplyForceX(-0.5f);
	else
		mRigidBody->ResetForceX();


	//jumping
	mJumpCooldown -= timeDelta;
	if (mJumpCooldown < 0.0f) mJumpCooldown = 0.0f;
	if (EventHandler::GetInstance()->GetAction(JUMP) && mGrounded && mJumpCooldown == 0.0f)
	{
		mJumping = true;
		mGrounded = false;
		mRigidBody->ResetVelocityY();
		mJumpCooldown = 0.5f;
		mJumpForce = -5.0f;
	}
	else if (EventHandler::GetInstance()->GetAction(JUMP) && mJumping && mJumpTimer > 0.0f && !mHitCeiling)
	{
		mJumpTimer -= timeDelta;
		if (mJumpTimer < 0.0f) mJumpTimer = 0.0f;
		mJumpForce += timeDelta;
		if (mJumpForce > 1.5f) mJumpForce = 1.5f;
		mRigidBody->ApplyForceY(mJumpForce);
	}
	else if (mJumping && mJumpTimer > 0.0f)
	{
		mHitCeiling = false;
		if (mJumping)
			mRigidBody->ResetVelocityY();
		mJumping = false;
		mRigidBody->ResetForceY();
	}
	else
	{
		mJumping = false;
		mJumpTimer = 9.0f;
		mRigidBody->ResetForceY();
	}

	CollisionHandler::GetInstance()->HandlePlayerCollision(this, timeDelta);

	mAnimation->Update(GetAnimState(),timeDelta);
}

void PlayerEntity::Draw()
{
	SDL_Rect hBox = mCollider->GetBox();
	mAnimation->Draw(hBox.x, hBox.y);
}

void PlayerEntity::Dispose()
{
	if (mTransform != nullptr) 
	{
		delete mTransform;
	} 
	if (mAnimation != nullptr)
	{
		mAnimation->Dispose();
		delete mAnimation;
	}
		
	if (mCollider != nullptr)
	{
		delete mCollider;
	}	
}

void PlayerEntity::InitialiseAnimStates()
{

}

String PlayerEntity::GetAnimState()
{

	return "BasicStand";
}