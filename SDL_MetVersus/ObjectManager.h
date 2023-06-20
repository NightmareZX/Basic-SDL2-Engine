#pragma once
#include "PlayerEntity.h"
#include "Logger.h"
#include "CollisionHandler.h"
#include "EventHandler.h"
#include "AnimationManager.h"
#include "MessageManager.h"
#include <array>


class ObjectManager
{
private:
	
	PlayerEntity* mPlayer;
	vector<BaseEntity*> mEntityList;

	Logger* mLoggerInstance;
	CollisionHandler* mCollisionHandlerInstance;
	EventHandler* mEventHandlerInstance;
	AnimationManager* mAnimationManagerInstance;
	MessageManager* mMessageManagerInstance;
public:
	ObjectManager(Logger* logger, CollisionHandler* collHandler, EventHandler* eventHandler, AnimationManager* animManager, MessageManager* msgManager): 
		mLoggerInstance(logger), mCollisionHandlerInstance(collHandler), mEventHandlerInstance(eventHandler), mAnimationManagerInstance(animManager)
	,mPlayer(nullptr), mMessageManagerInstance(msgManager)
	{
	}
	void InitialisePlayer(Uint32 x, Uint32 y);
	inline PlayerEntity* GetMainPlayer() { return mPlayer; }
	void Update(float deltaTime);
	void Draw();
	void Dispose();
};

