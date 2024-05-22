#pragma once
#include "PlayerEntity.h"
#include "CollisionHandler.h"
#include "EventHandler.h"
#include "AnimationManager.h"
#include "MessageManager.h"


class ObjectManager
{
private:
	
	PlayerEntity* mPlayer;
	vector<BaseEntity*> mEntityList;

	CollisionHandler* mCollisionHandlerInstance;
	EventHandler* mEventHandlerInstance;
	AnimationManager* mAnimationManagerInstance;
	MessageManager* mMessageManagerInstance;
public:
	ObjectManager( CollisionHandler* collHandler, EventHandler* eventHandler, AnimationManager* animManager, MessageManager* msgManager): 
		mCollisionHandlerInstance(collHandler), mEventHandlerInstance(eventHandler), mAnimationManagerInstance(animManager)
	,mPlayer(nullptr), mMessageManagerInstance(msgManager)
	{
	}
	void InitialisePlayer(Uint32 x, Uint32 y);
	inline PlayerEntity* GetMainPlayer() { return mPlayer; }
	void Update(float deltaTime);
	void Draw();
	void Dispose();
};

