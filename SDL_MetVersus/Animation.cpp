#include "Animation.h"
#include "GameEngine.h"
#include "Logger.h"

//a macro definition for accessing the map from a pointer because it can be a bit confusing to look at
#define AnimMapPtr(state) (*mAnimationMap)[state]

Animation::Animation(AnimationMetaData data)
{
	mAnimationMap = data.animMap;
	mSpriteSheetPtr = data.spriteSheetSurface;
	mSegnentedAniamtion = data.isSegmented;
	mCurrentSurface = nullptr;
	mCurrentTexture = nullptr;
	mSpriteSheetName = data.spriteSheetName;
	mLastAnimState = "";
	mCurrentFrame = 0;
	mCurrentTextureHeight = 0;
	mCurrentTextureWidth = 0;
	mRenderManagerInstance = data.renderManagerInstance;

}
void Animation::Update(String animState, float deltaTime, float cycleTimerOffset)
{
	//Update timers and other things here
	if (!ValidateState(animState))
	{
		Logger::Log("Animation.cpp: Error: The animation state '" + animState + "' is not registered!");
		return;
	}
	AnimationStateData currentAnimState = AnimMapPtr(animState);
	//if we are still on the same state
	if (animState == mLastAnimState)
	{
		//if the frame counter is still not 0 then 
		mFrameCounter -= deltaTime;
		if (mFrameCounter > 0) return;
		else
		{
			mCurrentFrame++;
			if (mCurrentFrame > currentAnimState.fCount)
				mCurrentFrame = 1;
			mFrameCounter = currentAnimState.cycleTime + cycleTimerOffset;
		}
	}
	//if we are on a different state 
	else
	{
		mFrameCounter = currentAnimState.cycleTime + cycleTimerOffset;
		mCurrentFrame = 1;
	}

	if (mFrameCounter < 0.0f)
		mFrameCounter = 0.0f;
	//mSpriteFrame = (SDL_GetTicks()/mAnimSpeed) % mFrameCount;

	//================================================================================================
	//								Segmented Animation Handling
	//================================================================================================
	if (mSegnentedAniamtion)
	{
		SDL_FreeSurface(mCurrentSurface);
		if (mCurrentTexture != nullptr) SDL_DestroyTexture(mCurrentTexture);
		SegmentList segList = currentAnimState.segmentList;

		//create a temporary surface to render our animation into so that we can create it into a texture and then render it
		SDL_Surface* currentSurface = SDL_CreateRGBSurface(0, currentAnimState.width, currentAnimState.height,32,0,0,0,0);

		//add the segments to the surface
		for (Sint32 i = segList.size() - 1; i >= 0; i--)
		{

			AnimationSegmentData* currentSegment = segList[i];
			Uint32 currFrame = mCurrentFrame;
			Uint32 sourceX = 0;

			//Modulo the current frame for this segment by the fcount so that we arent trying to access any frames beyond
			//the current segment's registered frames
			Uint32 frameMod = currFrame % currentSegment->fCount;
			//if the mod is not 0 then we are in the middle of the frames
			if (frameMod != 0)
				currFrame = frameMod;
			//if the mod is 0 then we are at the last frame
			else
				currFrame = currentSegment->fCount;

			//decrement the current frame by 1 so that we can use it for the offset calculation.
			//we do this because if we are at frame 1, we shouldnt add any offset since the initial index is already positioned
			//at the first frame
			currFrame--;

			SDL_Rect src = {0,0,static_cast<Sint32>(currentSegment->width), static_cast<Sint32>(currentSegment->height) };
			SDL_Surface* sourceSurface = nullptr;
			Uint32 XIndex = 0;

			Uint32 xIndexOffset = 0;

			SDL_Surface* flippedSegment = GetFlippedSegment(currentSegment, currentAnimState.flip_flags[i]);
			//If we are dealing with a flipped segment, then we do things sightly differently
			if (flippedSegment != nullptr)
			{
				xIndexOffset = currentSegment->width * currFrame;
				
				sourceSurface = flippedSegment;
			}
			else
			{
				//Get the offset that we will add onto the initial index, basically get how much we will add to the initial index to get
				//the top left X pixel position of our current frame
				xIndexOffset = (currentSegment->width * currFrame) + (currentSegment->borderSize * currFrame);

				src.y = static_cast<Sint32>(currentSegment->YIndex);
				XIndex = currentSegment->XIndex;

				sourceSurface = mSpriteSheetPtr;
			}

			//If the reverse flag is on, then we will take the offset away so we will go behind the initial X index
			if (currentAnimState.flags & AnimationFlags::REVERSE || currentSegment->flags & AnimationFlags::REVERSE)
				sourceX = XIndex - xIndexOffset;
			//Other wise add the offset so we go forward to the next frame
			else
				sourceX = XIndex + xIndexOffset;

			src.x = static_cast<Sint32>(sourceX);

			SDL_Rect dst = {
				static_cast<Sint32>(currentAnimState.blitXPos[i]),
				static_cast<Sint32>(currentAnimState.blitYPos[i]),
				static_cast<Sint32>(currentSegment->width),
				static_cast<Sint32>(currentSegment->height) };
			RenderManager::BlitSurface(sourceSurface, &src, currentSurface, &dst);
		}
		mCurrentSurface = currentSurface;
		mCurrentTexture = SDL_CreateTextureFromSurface(mRenderManagerInstance->GetRenderer(), currentSurface);
		mCurrentTextureWidth = currentSurface->w;
		mCurrentTextureHeight = currentSurface->h;
		mCurrentYDrawOffset = currentAnimState.YDrawOffset;
		mCurrentXDrawOffset = currentAnimState.XDrawOffset;
	}
	//================================================================================================
	//								Normal Animation Handling
	//================================================================================================
	else
	{
		//implement
	}
	mLastAnimState = animState;
}
void Animation::Draw(Sint32 x, Sint32 y)
{
	if (mSegnentedAniamtion)
	{
		mRenderManagerInstance->DrawTexture(mCurrentTexture,
			x + mCurrentXDrawOffset,
			y + mCurrentYDrawOffset,
			mCurrentTextureWidth,
			mCurrentTextureHeight);
	}
}
SDL_Surface* Animation::GetFlippedSegment(AnimationSegmentData* segmentData, SDL_RendererFlip flipFlags)
{
	SDL_Surface* flippedSurface = nullptr;
	FlippedSegmentList* flippedList = &segmentData->flippedVersions;
	for (size_t i = 0; i < flippedList->size(); i++)
	{
		if ((*flippedList)[i].first == static_cast<Uint32>(flipFlags))
		{
			flippedSurface = (*flippedList)[i].second;
		}
	}
	return flippedSurface;
}
bool Animation::ValidateState(String animState)
{
	if (mAnimationMap == nullptr) return false;
	if (mAnimationMap->find(animState) != mAnimationMap->end())
		return true;
	return false;
}
void Animation::Dispose()
{
	SDL_FreeSurface(mCurrentSurface);
	if (mCurrentTexture != nullptr) SDL_DestroyTexture(mCurrentTexture);

	//we are not deleting mSpriteSheetPtr and mAnimationMap because that is handled by other classes
}