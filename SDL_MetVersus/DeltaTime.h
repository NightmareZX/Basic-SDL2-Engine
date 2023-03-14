#pragma once
#include "SDL.h"

const Uint32 FPS_TARGET = 60;
const float MAX_DELTA = 1.5f;

class GlobalTimer
{
private:
	GlobalTimer(){}
	static GlobalTimer* sInstance;
	static float mTimeDelta;
	static float mLastTime;
public:
	void Tick();
	inline float GetTimeDelta() { return mTimeDelta; }
	inline static GlobalTimer* GetInstance() { if (sInstance == nullptr) sInstance = new GlobalTimer(); return sInstance; }
};