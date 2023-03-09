#include "GameEngine.h"

GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine()
{
	const String title = "test";
	const Uint32 xpos SDL_WINDOWPOS_CENTERED;
	const Uint32 ypos = SDL_WINDOWPOS_CENTERED;
	const Uint32 width = WINDOW_WIDTH;
	const Uint32 height = WINDOW_HEIGHT;
	bool fullscreen = false;
	mainWindow = nullptr;
	mainRenderer = nullptr;
	Uint16 flags = 0;
	if (fullscreen) flags = SDL_WINDOW_FULLSCREEN;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) b_RunningStatus = false;
	else
	{
		mainWindow = SDL_CreateWindow(title.c_str(), xpos, ypos, width, height, flags);
		if (mainWindow != nullptr) 
		{
			mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);
			b_RunningStatus = true;
		}
		else b_RunningStatus = false;
	}
	currentLevel = nullptr;
}

GameEngine* GameEngine::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new GameEngine();
	}
	return instance;
}

GameEngine::~GameEngine()
{
	DisposeComponents();
}
void GameEngine::StartUp()
{
	currentLevel = new GameLevel();
	currentLevel->LoadMap("level.lvl");
}

void GameEngine::GameLoop()
{
	StartUp();
	while (b_RunningStatus)
	{
		handleEvents();
		render();
		update();
		updateDeltaTime();
	}
	DisposeComponents();
}

void GameEngine::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_EventType::SDL_QUIT:
		break;
	case SDL_EventType::SDL_KEYDOWN:
		break;
	default:
		break;
	}
}
void GameEngine::update()
{

}
void GameEngine::render()
{
	SDL_RenderClear(mainRenderer);
	//render stuff
	currentLevel->DrawMap();
	SDL_RenderPresent(mainRenderer);
}
void GameEngine::DisposeComponents()
{
	if (currentLevel != nullptr) currentLevel->DisposeMap();
	RenderManager::GetInstance()->Dispose();
	SDL_DestroyWindow(mainWindow);
	SDL_DestroyRenderer(mainRenderer);
	SDL_Quit();
}

bool GameEngine::isRunning(){ return b_RunningStatus; }
