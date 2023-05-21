#include "PlayerEntity.h"
#include "RenderManager.h"
#include "EventHandler.h"
#include "CollisionHandler.h"

constexpr float TURN_AROUND_TIME = 1.0f;

PlayerEntity::PlayerEntity(float x, float y) : BaseEntity(x,y)
{
	//RenderManager::GetInstance()->RegisterTexture("basic_player.png", "basic_player.png");
	//mAnimation->SetProperties("basic_player.png", 0, 0, 1);
	mAnimation->Initialise("basic_player_sheet");
	SetColliderState(STANDING_CS);
	mJumpForce = -0.8f;
	InitialiseAnimStates();
	InitialiseTimers();
}

void PlayerEntity::Update(float timeDelta)
{
	//Basic movement
	mTurnAroundTimer = (mTurnAroundTimer <= 0.0f) ? 0.0f : mTurnAroundTimer - timeDelta;
	if (EventHandler::GetInstance()->GetAction(MOVE_RIGHT))
	{
		if (mFacingDirection != RIGHT_FD && mTurnAroundTimer == 0.0f)
		{
			mTurnAroundTimer = TURN_AROUND_TIME;
			mFacingDirection = RIGHT_FD;
		}
		else if (mTurnAroundTimer == 0.0f)
		{
			mRigidBody->ApplyForceX(0.5f);
		}
	}
		
	else if (EventHandler::GetInstance()->GetAction(MOVE_LEFT))
	{
		if (mFacingDirection != LEFT_FD && mTurnAroundTimer == 0.0f)
		{
			mTurnAroundTimer = TURN_AROUND_TIME;
			mFacingDirection = LEFT_FD;
		}
		else if (mTurnAroundTimer == 0.0f)
		{
			mRigidBody->ApplyForceX(-0.5f);
		}
	}
	else
		mRigidBody->ResetForceX();


	//jumping
	mJumpCooldown = (mJumpCooldown <= 0.0f) ? 0.0f : mJumpCooldown - timeDelta;
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

	HandleAnimState(timeDelta);
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
//Samus Super metroid hitbox sizes in pixels
//standing : w10    h42
//straight jumping : w10   h38
//spin jumping : w10 h24
//crouching : w10 h32
//morphed(ball) : w10 h14
//aiming down mid air : w10 h20
void PlayerEntity::SetColliderState(ColliderStates state)
{
	switch (state)
	{
	case PlayerEntity::STANDING_CS:
		mCollider->SetDimensions(10, 42);
		return;
	case PlayerEntity::STRAIGHT_JUMPING_CS:
		return;
	case PlayerEntity::SPIN_JUMPING_CS:
		return;
	case PlayerEntity::CROUCHING_CS:
		return;
	case PlayerEntity::MORPHED_CS:
		return;
	default:
		return;
	}
}

void PlayerEntity::InitialiseTimers()
{
	mJumpAirTime = 0.0f;
	mJumpCooldown = 0.0f;
	mJumpTimer = 0.0f;
	mTurnAroundTimer = 0.0f;
}

/// <summary>
/// Gets a string that represents the current animation state
/// Structure:
/// <UpperSegmentState>_<LowerSegmentState>
/// </summary>
/// <returns>The animation state in string format</returns>
void PlayerEntity::HandleAnimState(float timeDelta)
{

	if (mFacingDirection == LEFT_FD)
	{
		mAnimation->Update("BasicTestL", timeDelta);
	}
	else if (mFacingDirection == RIGHT_FD)
	{
		mAnimation->Update("BasicTest", timeDelta);
	}


	
}

String PlayerEntity::GetPositionStatus()
{
	StringStream ss;
	ss << "X: " << mCollider->GetBboxLeft() <<
		"  Y: " << mCollider->GetBboxTop() <<
		" XW: " << mCollider->GetBboxRight() <<
		" YH: " << mCollider->GetBboxBottom() <<
		" HM: " << mCollider->GetMiddlePointHorizontal() <<
		" FD: " << mFacingDirection;
	return ss.str();
}