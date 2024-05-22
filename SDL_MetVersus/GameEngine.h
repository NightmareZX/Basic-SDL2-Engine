#pragma once
#include "SDL.h"
#include "Constants.h"
#include "Room.h"
#include "RenderManager.h"
#include "MapManager.h"
#include "Logger.h"
#include "EventHandler.h"
#include "CollisionHandler.h"
#include "GlobalTimer.h"
#include "AudioManager.h"
#include "AnimationManager.h"
#include "ObjectManager.h"
#include "MessageManager.h"
#include "Camera.h"

class GameEngine
{
private:
	
	bool mRunningStatus;
	bool mFrameAdvanceMode;

	GlobalTimer mGlobalTimer;
	EventHandler mEventHandler;
	AudioManager mAudioManager;
	MapManager mMapManager;
	RenderManager mRendererManager;
	AnimationManager mAnimationManager;
	ObjectManager mObjectManager;
	CollisionHandler mCollisionHandler;
	MessageManager mMessageManager;
	Camera mCamera;

public:
	~GameEngine();
	GameEngine(int argc, char* argv[]): mCollisionHandler(), mCamera(), mMessageManager()
		, mAudioManager(), mRendererManager(&mCamera),
		mAnimationManager(&mRendererManager), mObjectManager(&mCollisionHandler, &mEventHandler, &mAnimationManager, &mMessageManager),
		mMapManager(&mRendererManager, &mObjectManager, &mCollisionHandler, &mCamera)
	{ 
		InitialiseComponents(argc, argv);
	}
	void GameLoop();

	inline void QuitEngine() { mRunningStatus = false; }
	//inline Room* GetCurrentMap() { return mCurrentLevel; }
	//inline SDL_Renderer* GetRenderer() { return mMainRenderer; }
	//inline SDL_Window* GetWindow() { return mMainWindow; }
private:
	void InitialiseComponents(int argc, char* argv[]);
	void EngineUpdate();
	void EngineRender();
	void DisposeComponents();
	bool isRunning();
};

