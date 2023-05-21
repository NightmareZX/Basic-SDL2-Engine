#include "GameEngine.h"
#include "SDL_ttf.h"
#include <SDL_mixer.h>
#include "AnimationManager.h"
#include "GlobalTimer.h"
#include "AudioManager.h"
#include "GameRules.h"

GameEngine* GameEngine::mInstance = nullptr;

GameEngine::GameEngine()
{
	const String title = "EngineTest";
	const Uint32 xpos SDL_WINDOWPOS_CENTERED;
	const Uint32 ypos = SDL_WINDOWPOS_CENTERED;
	const Uint32 width = 1200;
	const Uint32 height = 600;
	bool fullscreen = false;
	mMainWindow = nullptr;
	mMainRenderer = nullptr;
	Uint16 flags = 0; //SDL_WINDOW_MAXIMIZED
	if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) mRunningStatus = false;
	else
	{
		mMainWindow = SDL_CreateWindow(title.c_str(), xpos, ypos, width, height, flags);
		if (mMainWindow != nullptr)
		{
			mMainRenderer = SDL_CreateRenderer(mMainWindow, -1, 0);
			mRunningStatus = true;
		}
		else mRunningStatus = false;
	}
	mFrameAdvanceMode = false;

	mCurrentLevel = nullptr;
	RenderManager::GetInstance()->SetWindowDimensions(width, height);
}
GameEngine::~GameEngine()
{
	DisposeComponents();
}
void GameEngine::InitialiseComponents()
{
	TTF_Init();
	//Initialise singleton instances
	Logger::GetInstance()->ToggleLogger();
	GlobalTimer::GetInstance();
	EventHandler::GetInstance();
	AudioManager::GetInstance();
	Camera::GetInstance();
	AnimationManager::GetInstance();
	GameRules::GetInstance();
	//Test initialisations
	RenderManager::GetInstance()->RegisterFont("arial.ttf", "arial.ttf", 24);
	mCurrentLevel = MapManager::GetInstance()->DebugLoadLevel("TestWordLDtk\\test","Room_0");
	//mCurrentLevel = MapManager::GetInstance()->GetMap();
	AudioManager::GetInstance()->RegisterMusic("music_test", MUS_OGG);
	AudioManager::GetInstance()->SetChannelVolume(CHAN_MUSIC, 5);
}

void GameEngine::GameLoop()
{
	InitialiseComponents();
	//AudioManager::GetInstance()->PlayMusic("music_test", false, true, 0, 5000);
	while (mRunningStatus)
	{
		GlobalTimer::GetInstance()->TickFixedStep();
		EventHandler::GetInstance()->ListenForEvents();

		if (EventHandler::GetInstance()->GetActionOnce(DEBUG_ENABLE_FRAME_ADVANCE))
		{
			mFrameAdvanceMode = !mFrameAdvanceMode;
			float* accumulator = GlobalTimer::GetInstance()->GetAccumulator();
			*accumulator = 0.0f;
		}
			
		if (mFrameAdvanceMode)
		{
			if (EventHandler::GetInstance()->GetActionOnce(DEBUG_ADVANCE_FRAME))
			{
				EngineUpdate();
				EngineRender();
			}
		}
		else
		{
			//==========================================================================
			//							Fixed Time Step Update
			//==========================================================================
			//We use fixed time step to update game logic for easier consistency because
			//variable time step would be too annoying to make work properly
			float* accumulator = GlobalTimer::GetInstance()->GetAccumulator();
			while (*accumulator >= FIXED_TIME_STEP)
			{
				EngineUpdate();
				EngineRender();
				*accumulator -= FIXED_TIME_STEP;
			}
		}


		//GlobalTimer::GetInstance()->Tick();
	}
	DisposeComponents();
}

void GameEngine::EngineUpdate()
{
	if (mCurrentLevel != nullptr)
		mCurrentLevel->Update(1.0f);//using 1.0f delta time since we are in fixed step rn

	//DEBUG
	if (EventHandler::GetInstance()->GetAction(DEBUG_LOG_SDL_ERROR))
	{
		Logger::GetInstance()->Log("Debug: Current SDL Error: " + String(SDL_GetError()) );
	}
	if (EventHandler::GetInstance()->GetAction(DEBUG_RESTART_MUSIC))
	{
		AudioManager::GetInstance()->PlayMusic("music_test", true, true, 3000, 3000);
	}
}
void GameEngine::EngineRender()
{
	//Put stuff to render under the clear but above the present
	mCurrentLevel->DrawMap();

	//DEBUG DRAWS
	//RenderManager::GetInstance()->DrawRectangle(Camera::GetInstance()->GetViewBox());
	//RenderManager::GetInstance()->DrawRectangle(*Camera::GetInstance()->GetDisplayBox());
	RenderManager::GetInstance()->DrawText("arial.ttf", 16, 48, mCurrentLevel->GetMainPlayer()->GetPositionStatus());
	RenderManager::GetInstance()->DrawText("arial.ttf", 16, 68, Camera::GetInstance()->GetPositionStatus());
	//RenderManager::GetInstance()->DrawText("arial.ttf", 16, 88, CollisionHandler::GetInstance()->GetStatusCol());
	RenderManager::GetInstance()->DrawRectangleRelativeToCamera(mCurrentLevel->GetMainPlayer()->GetCollider()->GetBox());

	SDL_RenderPresent(mMainRenderer);
	SDL_RenderClear(mMainRenderer);
}
void GameEngine::DisposeComponents()
{
	//Dispose custom objects
	if (mCurrentLevel != nullptr) mCurrentLevel->Dispose();
	RenderManager::GetInstance()->Dispose();
	//GlobalTimer::GetInstance()->Dispose();
	//EventHandler::GetInstance()->Dispose();
	AudioManager::GetInstance()->Dispose();
	//Camera::GetInstance()->Dispose();
	//AnimationManager::GetInstance()->Dispose();

	//Destroy sdl objects
	TTF_Quit();
	SDL_DestroyWindow(mMainWindow);
	SDL_DestroyRenderer(mMainRenderer);
	SDL_Quit();
}

bool GameEngine::isRunning(){ return mRunningStatus; }
