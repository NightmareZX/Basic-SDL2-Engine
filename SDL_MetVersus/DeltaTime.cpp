#include "DeltaTime.h"
#include "Logger.h"


GlobalTimer* GlobalTimer::sInstance = nullptr;
float GlobalTimer::mTimeDelta = 0.0f;
float GlobalTimer::mLastTime = 0.0f;

void GlobalTimer::Tick()
{
	mTimeDelta = (SDL_GetTicks() - mLastTime)*(FPS_TARGET / 1000.0f);
	SDL_GetPerformanceCounter();
	SDL_GetPerformanceFrequency();

	if (mTimeDelta > MAX_DELTA)
		mTimeDelta = MAX_DELTA;

	mLastTime = SDL_GetTicks();
	//Logger::GetInstance()->Log("Current Time Delta: " + to_string(mTimeDelta));
}
