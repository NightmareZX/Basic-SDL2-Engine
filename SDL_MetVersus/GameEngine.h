#pragma once
#include "STL_Components.h"
#include "SDL.h"
#include "Constants.h"
#include "GameLevel.h"
#include "DeltaTime.h"
#include "RenderManager.h"

class GameEngine
{
private:

	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	
	bool b_RunningStatus;

	GameLevel* currentLevel;
	static GameEngine* instance;

public:
	~GameEngine();

	void GameLoop();
	

	static GameEngine* GetInstance();
	inline SDL_Renderer* GetRenderer() { return mainRenderer; }
private:
	GameEngine();
	void StartUp();

	void handleEvents();
	void update();
	void render();
	void DisposeComponents();

	bool isRunning();
};

