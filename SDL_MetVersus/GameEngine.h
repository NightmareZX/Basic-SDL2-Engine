#pragma once
#include "STL_Components.h"
#include "SDL.h"
#include "Constants.h"
#include "GameLevel.h"
#include "RenderManager.h"
#include "MapParser.h"
#include "Logger.h"
#include "EventHandler.h"
#include "CollisionHandler.h"

class GameEngine
{
private:

	SDL_Window* mMainWindow;
	SDL_Renderer* mMainRenderer;
	
	bool mRunningStatus;

	GameLevel* mCurrentLevel;
	static GameEngine* mInstance;

public:
	~GameEngine();
	void GameLoop();

	inline void QuitEngine() { mRunningStatus = false; }
	inline static GameEngine* GetInstance() { if (mInstance == nullptr) mInstance = new GameEngine(); return mInstance; }
	inline GameLevel* GetCurrentMap() { return mCurrentLevel; }
	inline SDL_Renderer* GetRenderer() { return mMainRenderer; }
	inline SDL_Window* GetWindow() { return mMainWindow; }
private:
	GameEngine();
	void InitialiseComponents();
	void EngineUpdate();
	void EngineRender();
	void DisposeComponents();
	bool isRunning();
};

