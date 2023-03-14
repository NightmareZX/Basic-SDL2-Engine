#pragma once
#include "STL_Components.h"
#include "SDL.h"
#include "Constants.h"
#include "GameLevel.h"
#include "DeltaTime.h"
#include "RenderManager.h"
#include "MapParser.h"
#include "Logger.h"
#include "EventHandler.h"

class GameEngine
{
private:

	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	
	bool b_RunningStatus;

	GameLevel* mCurrentLevel;
	static GameEngine* instance;

public:
	~GameEngine();

	void GameLoop();
	
	void QuitEngine();
	static GameEngine* GetInstance();
	GameLevel* GetCurrentMap() { return mCurrentLevel; }
	inline SDL_Renderer* GetRenderer() { return mainRenderer; }
private:
	GameEngine();
	void InitialiseComponents();

	

	void update();
	void render();

	void DisposeComponents();

	bool isRunning();
};

