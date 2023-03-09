#include "DeltaTime.h"

void updateDeltaTime()
{
    static Uint64 lastFrameTime = SDL_GetPerformanceCounter();
    Uint64 currentFrameTime = SDL_GetPerformanceCounter();
    Uint64 frameTimeDiff = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    static double frequency = (double)SDL_GetPerformanceFrequency() / 1000.0;

    deltaTime = (float)(frameTimeDiff / frequency);
}