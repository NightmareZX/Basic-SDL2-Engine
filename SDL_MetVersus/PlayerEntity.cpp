#include "PlayerEntity.h"
#include "RenderManager.h"
#include "EventHandler.h"
#include "CollisionHandler.h"

PlayerEntity::PlayerEntity(Properties* props) : BaseEntity(props)
{
	mRigidBody = new RigidBody();
	RenderManager::GetInstance()->RegisterTexture("basic_player.png", "basic_player.png");
	mAnimation = new Animation();
	mAnimation->SetProperties("basic_player.png", 0, 0, 1);

	mCollider = new Collider();
	mCollider->SetBuffer(0,0,0,0);
	mJumpForce = -0.8f;
}

void PlayerEntity::Update()
{
	//Basic movement
	if (EventHandler::GetInstance()->GetAction(MOVE_RIGHT))
		mRigidBody->ApplyForceX(0.5f);
	else if (EventHandler::GetInstance()->GetAction(MOVE_LEFT))
		mRigidBody->ApplyForceX(-0.5f);
	else
		mRigidBody->ResetForceX();


	//jumping
	mJumpCooldown -= GlobalTimer::GetInstance()->GetTimeDelta();
	if (mJumpCooldown < 0.0f) mJumpCooldown = 0.0f;
	if (EventHandler::GetInstance()->GetAction(JUMP) && mGrounded && mJumpCooldown == 0.0f)
	{
		mJumping = true;
		mGrounded = false;
		mRigidBody->ResetVelocityY();
		mJumpCooldown = 0.5f;
		mJumpForce = -99.8f;
	}
	else if (EventHandler::GetInstance()->GetAction(JUMP) && mJumping && mJumpTimer > 0.0f)
	{
		mJumpTimer -= GlobalTimer::GetInstance()->GetTimeDelta();
		if (mJumpTimer < 0.0f) mJumpTimer = 0.0f;
		mJumpForce *= GlobalTimer::GetInstance()->GetTimeDelta();
		mRigidBody->ApplyForceY(mJumpForce);
	}
	else if (mJumping && mJumpTimer > 0.0f)
	{
		mJumping = false;
		mRigidBody->ResetForceY();
	}
	else
	{
		//if (mJumping)
		//	mRigidBody->ResetVelocityY();
		mJumping = false;
		mJumpTimer = 8.0f;
		mJumpForce = -99.8f;
		mRigidBody->ResetForceY();
	}

	mRigidBody->Update();
	mLastSafePosition.X = mTransform->X;
	mTransform->X += mRigidBody->GetPosition().X;
	mCollider->Set(mTransform->X + 6, mTransform->Y,15,43);
	Uint32 slopeY = 0;
	bool shouldFallInSlope = false;
	TileData interactedTile = CollisionHandler::GetInstance()->MapCollision(mCollider->GetBox());
	if (interactedTile.second == TileTypes::SOLID)
	{
		mTransform->X = mLastSafePosition.X;
		mCollider->Set(mLastSafePosition.X + 6, mTransform->Y, 15, 43);
		mRigidBody->ResetPosition();
	}
	else if (interactedTile.second == TileTypes::LEFT_SLOPE && false)
	{
		SDL_Rect tempBox = mCollider->GetBox();
		Vector2D point(tempBox.x + 2, tempBox.y + tempBox.h);
		TileData bottomPointOnMap = CollisionHandler::GetInstance()->GetMapCollisionDataFromPoint(point);
		if (bottomPointOnMap.second == interactedTile.second)
		{
			Uint32 pointXinTile = point.X % TILE_SIZE;
			Uint32 pointYinTile = point.Y % TILE_SIZE;

			slopeY = LEFT_SLOPE_HEIGHTS[pointXinTile];
			if (pointYinTile == slopeY + 1);
			else if (pointYinTile > slopeY) mTransform->Y -= slopeY - pointYinTile + 1;
			else if (pointYinTile < slopeY);

		}
		//float modulo = fmod(mTransform->X, TILE_SIZE);
		//slopeY = LEFT_SLOPE_HEIGHTS[static_cast<Uint32>(modulo) - 1];

		//float modulo2 = fmod(mTransform->Y, TILE_SIZE);
		//Uint32 value = slopeY - static_cast<Uint32>(modulo2);
		//if (value != slopeY + 1) value = slopeY + 1;
		//mTransform->Y = mTransform->Y - value;

		//mTransform->X = mLastSafePosition.X;
		//mRigidBody->ResetPosition();
	}
	else if (interactedTile.second == TileTypes::RIGHT_SLOPE)
	{
		//mTransform->X = mLastSafePosition.X;
		//mRigidBody->ResetPosition();
	}

	mRigidBody->Update();
	mLastSafePosition.Y = mTransform->Y;
	mTransform->Y += mRigidBody->GetPosition().Y;
	mCollider->Set(mTransform->X + 6, mTransform->Y, 15, 43);
	interactedTile = CollisionHandler::GetInstance()->MapCollision(mCollider->GetBox());

	if (interactedTile.second == TileTypes::SOLID)
	{
		mGrounded = true;
		mTransform->Y = mLastSafePosition.Y;
		mCollider->Set(mLastSafePosition.X + 6, mLastSafePosition.Y, 15, 43);
		mRigidBody->ResetPosition();
		mRigidBody->ResetVelocityY();
	}
	else if (interactedTile.second == TileTypes::LEFT_SLOPE && false)
	{
		mGrounded = true;

		mRigidBody->ResetPosition();
		mRigidBody->ResetVelocityY();
	}
	else if (interactedTile.second == TileTypes::RIGHT_SLOPE && false)
	{
		mGrounded = true;
		mTransform->Y = mLastSafePosition.Y;
		mRigidBody->ResetPosition();
	}
	else
	{
		mGrounded = false;
	}

	mAnimation->Update();
}

void PlayerEntity::Draw()
{
	mAnimation->Draw(mTransform->X, mTransform->Y,26,43);
	RenderManager::GetInstance()->DrawRectangle(mCollider->GetBox());
}

void PlayerEntity::Dispose()
{

}