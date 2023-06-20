#pragma once
#include "SDL.h"

const Uint32 FPS_TARGET = 60;
const float MAX_DELTA = 1.5f;

constexpr float FIXED_TIME_STEP = 1.0f / 60.0f;

class GlobalTimer
{
private:
	float mTimeDelta;

	Uint32 mLastTime32;
	float mLastTimeF;
	float mAccumulator;
public:
	GlobalTimer();
	void Tick();
	void TickFixedStep();
	inline float GetTimeDelta() { return mTimeDelta; }
	inline float* GetAccumulator() { return &mAccumulator; }

};