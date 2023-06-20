#include "GameEngine.h"
#include "SDL_ttf.h"
#include <SDL_mixer.h>
#include "AnimationManager.h"
#include "GlobalTimer.h"
#include "AudioManager.h"
#include "GameRules.h"


GameEngine::~GameEngine()
{
	DisposeComponents();
}
void GameEngine::InitialiseComponents(int argc, char* argv[])
{

	mFrameAdvanceMode = false;
	TTF_Init();
	mLogger.ToggleLogger();

	mRunningStatus = !mRendererManager.mFailFlag;

	GameRules::GetInstance();
	mRendererManager.InitCamera();
	//Test initialisations
	mRendererManager.RegisterFont("arial.ttf", "arial.ttf", 24);
	mMapManager.DebugLoadLevel("TestWordLDtk\\test","Room_0");
	//mCurrentLevel = MapManager::GetInstance()->GetMap();
	mAudioManager.RegisterMusic("music_test", MUS_OGG);
	mAudioManager.SetChannelVolume(CHAN_MUSIC, 5);
}

void GameEngine::GameLoop()
{
	//InitialiseComponents();
	//mAudioManager.PlayMusic("music_test", false, true, 0, 5000);
	while (mRunningStatus)
	{
		mGlobalTimer.TickFixedStep();
		mEventHandler.ListenForEvents();
		if (mEventHandler.GetQuitStatus()) break;

		if (mEventHandler.GetActionOnce(DEBUG_ENABLE_FRAME_ADVANCE))
		{
			mFrameAdvanceMode = !mFrameAdvanceMode;
			float* accumulator = mGlobalTimer.GetAccumulator();
			*accumulator = 0.0f;
		}
			
		if (mFrameAdvanceMode)
		{
			if (mEventHandler.GetActionOnce(DEBUG_ADVANCE_FRAME))
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
			float* accumulator = mGlobalTimer.GetAccumulator();
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
	mMapManager.Update(1.0f);//using 1.0f delta time since we are in fixed step rn

	//DEBUG
	if (mEventHandler.GetAction(DEBUG_LOG_SDL_ERROR))
	{
		mLogger.Log("Debug: Current SDL Error: " + String(SDL_GetError()) );
	}
	if (mEventHandler.GetAction(DEBUG_RESTART_MUSIC))
	{
		mAudioManager.PlayMusic("music_test", true, true, 3000, 3000);
	}
}
void GameEngine::EngineRender()
{
	mMapManager.Draw();

	//DEBUG DRAWS
	//mRendererManager.DrawRectangle(Camera::GetInstance()->GetViewBox());
	//mRendererManager.DrawRectangle(*Camera::GetInstance()->GetDisplayBox());
	//mRendererManager.DrawText("arial.ttf", 16, 48, mCurrentLevel->GetMainPlayer()->GetPositionStatus());
	//mRendererManager.DrawText("arial.ttf", 16, 68, Camera::GetInstance()->GetPositionStatus());
	//mRendererManager.DrawText("arial.ttf", 16, 88, CollisionHandler::GetInstance()->GetStatusCol());
	//mRendererManager.DrawRectangleRelativeToCamera(mCurrentLevel->GetMainPlayer()->GetCollider()->GetBox());

	mRendererManager.RendererPresent();
	mRendererManager.RendererClear();
}
void GameEngine::DisposeComponents()
{
	//Dispose custom objects
	//if (mCurrentLevel != nullptr) mCurrentLevel->Dispose();
	//RenderManager::GetInstance()->Dispose();
	//GlobalTimer::GetInstance()->Dispose();
	//EventHandler::GetInstance()->Dispose();
	//mAudioManager.Dispose();
	//Camera::GetInstance()->Dispose();
	//AnimationManager::GetInstance()->Dispose();

	//Destroy sdl objects
	TTF_Quit();
	SDL_Quit();
}

bool GameEngine::isRunning(){ return mRunningStatus; }
