#pragma once
#include "SDL.h"
#include "STL_Components.h"
#include "Animation.h"

#pragma region StructureDeclerations

struct SharedAnimation
{
	AnimationMap* animMap = nullptr;
	SegmentList animSegments;
	Uint32 referenceCount = 0;
	bool isSegmented = false;
	SDL_Surface* spriteSheetSurface = nullptr;
	vector<SDL_Surface*> flippedSegments;
};
using LoadedAnimationsMap = map<String, SharedAnimation*>;

struct AnimationMetaData
{
	AnimationMap* animMap = nullptr;
	bool isSegmented = false;
	SDL_Surface* spriteSheetSurface = nullptr;
};

#pragma endregion


class AnimationManager
{
private:
	AnimationManager(){}
	static AnimationManager* mInstance;
	LoadedAnimationsMap mLoadedAnimations;

	bool ValidateAnimation(String spriteSheetID);
	SharedAnimation* LoadAnimation(String spriteSheetName);
	void HandleSegmentFlipping(AnimationSegmentData* segmentToFlip, Uint32 flipFlags, SDL_Surface* spriteSheet);
public:
	static inline AnimationManager* GetInstance() { if (mInstance == nullptr)mInstance = new AnimationManager; return mInstance; }
	SDL_Surface* GetFlippedSegment(AnimationSegmentData* segmentData, SDL_RendererFlip flipFlags);
	AnimationMetaData GetAnimation(String spriteSheetName);
	void RemoveAnimation(String spriteSheetID);
	void RefreshLoadedAnimations();
	void DisposeAnimationMap(AnimationMap* animMap);
	void DisposeSharedAnimation(SharedAnimation* animMap);
	void Dispose();
};

