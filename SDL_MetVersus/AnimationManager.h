#pragma once
#include "SDL.h"
#include "Animation.h"
#include "Containers.h"

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

#pragma endregion


class AnimationManager final
{
private:
	LoadedAnimationsMap mLoadedAnimations;
	RenderManager* mRendererManagerInstance;

	bool ValidateAnimation(String spriteSheetID);
	SharedAnimation* LoadAnimation(String spriteSheetName);
	void HandleSegmentFlipping(AnimationSegmentData* segmentToFlip, Uint32 flipFlags, SDL_Surface* spriteSheet);
public:
	AnimationManager(RenderManager* rendManager):mRendererManagerInstance(rendManager) {}

	AnimationMetaData GetAnimation(String spriteSheetName);
	void RemoveAnimation(String spriteSheetID);
	void RefreshLoadedAnimations();
	void DisposeAnimationMap(AnimationMap* animMap);
	void DisposeSharedAnimation(SharedAnimation* animMap);
	inline RenderManager* GetAnimationRendererInstance() { return mRendererManagerInstance; }
	void Dispose();
};

