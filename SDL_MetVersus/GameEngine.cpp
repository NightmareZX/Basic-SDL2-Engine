#include "GameEngine.h"

GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine()
{
	const String title = "EngineTest";
	const Uint32 xpos SDL_WINDOWPOS_CENTERED;
	const Uint32 ypos = SDL_WINDOWPOS_CENTERED;
	const Uint32 width = 1200;
	const Uint32 height = 600;
	bool fullscreen = false;
	mainWindow = nullptr;
	mainRenderer = nullptr;
	Uint16 flags = 0;
	if (fullscreen) flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE;

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
	mCurrentLevel = nullptr;
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
void GameEngine::InitialiseComponents()
{
	Logger::GetInstance()->ToggleLogger();
	GlobalTimer::GetInstance();
	MapParser::GetInstance()->LoadMap("testmap");
	EventHandler::GetInstance();
	mCurrentLevel = MapParser::GetInstance()->GetMap();
}

void GameEngine::GameLoop()
{
	InitialiseComponents();
	while (b_RunningStatus)
	{
		EventHandler::GetInstance()->ListenForEvents();
		update();
		render();
		GlobalTimer::GetInstance()->Tick();
	}
	DisposeComponents();
}

void GameEngine::update()
{
	if (mCurrentLevel != nullptr)
		mCurrentLevel->Update();
}
void GameEngine::render()
{
	SDL_RenderClear(mainRenderer);
	//render stuff
	mCurrentLevel->DrawMap();
	SDL_RenderPresent(mainRenderer);
}
void GameEngine::QuitEngine()
{
	b_RunningStatus = false;
}
void GameEngine::DisposeComponents()
{
	if (mCurrentLevel != nullptr) mCurrentLevel->DisposeMap();
	RenderManager::GetInstance()->Dispose();
	SDL_DestroyWindow(mainWindow);
	SDL_DestroyRenderer(mainRenderer);
	SDL_Quit();
}

bool GameEngine::isRunning(){ return b_RunningStatus; }
