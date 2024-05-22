#include "ObjectManager.h"

void ObjectManager::Update(float timeDelta)
{
	if (mPlayer != nullptr)
	{
		mPlayer->Update(timeDelta);
		mCollisionHandlerInstance->HandlePlayerCollision(mPlayer, timeDelta);
		mPlayer->HandleAnimState(timeDelta);
	}
		
}

void ObjectManager::Draw()
{
	if (mPlayer != nullptr) mPlayer->Draw();
}

void ObjectManager::InitialisePlayer(Uint32 x, Uint32 y)
{
	if (mPlayer != nullptr) delete mPlayer;

	mPlayer = new PlayerEntity(x, y, mEventHandlerInstance, 
		new Animation(mAnimationManagerInstance->GetAnimation("basic_player_sheet"))
	);
}