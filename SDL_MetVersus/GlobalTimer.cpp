#include "GlobalTimer.h"
#include "Logger.h"


GlobalTimer* GlobalTimer::sInstance = nullptr;

//float GlobalTimer::mTimeDelta = 0.0f;
//Uint32 GlobalTimer::mLastTime32 = 0;
//float GlobalTimer::mLastTimeF = 0.0f;
//float GlobalTimer::mAccumulator = 0.0f;

GlobalTimer::GlobalTimer()
{
	mLastTime32 = SDL_GetTicks();
	mTimeDelta = 0.0f;
	mLastTime32 = 0;
	mLastTimeF = 0.0f;
	mAccumulator = 0.0f;
}
void GlobalTimer::Tick()
{
	if (mLastTimeF == 0.0f) mLastTimeF = SDL_GetTicks() * (FPS_TARGET / 1000.0f);
	mTimeDelta = (SDL_GetTicks() - mLastTimeF)*(FPS_TARGET / 1000.0f);
	//SDL_assert(mTimeDelta < 1.5f);

	if (mTimeDelta > MAX_DELTA)
		mTimeDelta = MAX_DELTA;

	mLastTimeF = SDL_GetTicks();
	//Logger::GetInstance()->Log("Current Time Delta: " + to_string(mTimeDelta));
}
void GlobalTimer::TickFixedStep()
{
	Uint32 currentTime = SDL_GetTicks();
	Uint32 timeDifference = currentTime - mLastTime32;
	mLastTime32 = currentTime;

	mTimeDelta = timeDifference / 1000.0f;

	mAccumulator += mTimeDelta;//convert to seconds
}

