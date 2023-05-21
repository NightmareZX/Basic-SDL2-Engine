#pragma once
#include "SDL.h"

class GameRules
{
private:
	GameRules(){}
	static GameRules* mInstance;
public:
	static inline GameRules* GetInstance() { if (mInstance == nullptr) mInstance = new GameRules; return mInstance; }

};

