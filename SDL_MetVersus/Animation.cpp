#include "Animation.h"
#include "AnimationManager.h"
#include "RenderManager.h"
#include "GameEngine.h"

//a macro definition for accessing the map from a pointer because it can be a bit confusing to look at
#define AnimMapPtr(state) (*mAnimationMap)[state]

Animation::Animation()
{
	mAnimationMap = nullptr;
	mSpriteSheetPtr = nullptr;
	mCurrentSurface = nullptr;
	mCurrentTexture = nullptr;
	mSpriteSheetName = "";
	mLastAnimState = "";
	mCurrentFrame = 0;
	mCurrentTextureHeight = 0;
	mCurrentTextureWidth = 0;

}
void Animation::Initialise(String spriteSheetName)
{
	AnimationMetaData metaData = AnimationManager::GetInstance()->GetAnimation(spriteSheetName);
	mAnimationMap = metaData.animMap;
	mSegnentedAniamtion = metaData.isSegmented;
	mSpriteSheetPtr = metaData.spriteSheetSurface;
	mSpriteSheetName = spriteSheetName;
}
void Animation::Update(String animState, float deltaTime, float cycleTimerOffset)
{
	//Update timers and other things here
	if (!ValidateState(animState)) return;
	//if we are still on the same state
	if (animState == mLastAnimState)
	{
		//if the frame counter is still not 0 then 
		mFrameCounter -= deltaTime;
		if (mFrameCounter > 0) return;
		else
		{
			mCurrentFrame++;
			if (mCurrentFrame > AnimMapPtr(animState).fCount)
				mCurrentFrame = 1;
			mFrameCounter = AnimMapPtr(animState).cycleTime + cycleTimerOffset;
		}
	}
	//if we are on a different state 
	else
	{
		mFrameCounter = AnimMapPtr(animState).cycleTime + cycleTimerOffset;
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
		Uint32 surfHeight = 0;
		Uint32 surfWidth = 0;
		SegmentList segList = AnimMapPtr(animState).segmentList;
		//A vector of positions for all segments, they defines where in the current surface will they be placed in
		vector<Uint32> blitYPos;
		vector<Uint32> blitXPos;
		//
		bool aggregatedHeight = false;
		bool aggregatedWidth = false;

		//Add up the width and height of segments
		for (Sint32 i = 0; i < segList.size(); i++)
		{
			//get the offsets that are relative to the surface, the offsets will displace the segment
			//within the surface that the animation will be stitched together in
			Sint32 segmentYOffset = AnimMapPtr(animState).y_offsets[i];
			Sint32 segmentXOffset = AnimMapPtr(animState).x_offsets[i];

			if (segList[i]->flags & AnimationFlags::AGR_HEIGHT)
			{
				if (i == 0) blitYPos.push_back(0);
				else blitYPos.push_back(surfHeight + segmentYOffset);
				surfHeight += segList[i]->height - segList[i]->heightOffset;
				aggregatedHeight = true;
			}
			else blitYPos.push_back(segmentYOffset);
			if (segList[i]->flags & AnimationFlags::AGR_WIDTH)
			{
				if (i == 0) blitXPos.push_back(0);
				else blitXPos.push_back(surfWidth + segmentXOffset);
				surfWidth += segList[i]->width - segList[i]->widthOffset;
				aggregatedWidth = true;
			}
			else blitXPos.push_back(segmentXOffset);
		}

		if (!aggregatedWidth)
			surfWidth = AnimMapPtr(animState).width;
		if (!aggregatedHeight)
			surfHeight = AnimMapPtr(animState).height;

		//create a temporary surface to render our animation into so that we can create it into a texture and then render it
		SDL_Surface* currentSurface = SDL_CreateRGBSurface(0,surfWidth,surfHeight,32,0,0,0,0);

		//add the segments to the surface
		for (Sint32 i = segList.size() - 1; i >= 0; i--)
		{

			AnimationSegmentData* currentSegment = segList[i];
			Uint32 currFrame = mCurrentFrame;
			Uint32 sourceX = 0;
			Uint32 sourceY = 0;

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

			//Get the offset that we will add onto the initial index, basically get how much we will add to the initial index to get
			//the top left X pixel position of our current frame
			Uint32 xIndexOffset = (currentSegment->width * currFrame) + (currentSegment->borderSize * currFrame);

			//If the reverse flag is on, then we will take the offset away so we will go behind the initial X index
			if (AnimMapPtr(animState).flags & AnimationFlags::REVERSE || currentSegment->flags & AnimationFlags::REVERSE)
				sourceX = currentSegment->XIndex - xIndexOffset;
			//Other wise add the offset so we go forward to the next frame
			else
				sourceX = currentSegment->XIndex + xIndexOffset;

			SDL_Rect src = { 
				static_cast<Sint32>(sourceX),
				static_cast<Sint32>(currentSegment->YIndex),
				static_cast<Sint32>(currentSegment->width),
				static_cast<Sint32>(currentSegment->height) };
			SDL_Rect dst = { 
				static_cast<Sint32>(blitXPos[i]),
				static_cast<Sint32>(blitYPos[i]),
				static_cast<Sint32>(currentSegment->width),
				static_cast<Sint32>(currentSegment->height)};
			SDL_UpperBlit(mSpriteSheetPtr, &src, currentSurface, &dst);
		}
		mCurrentSurface = currentSurface;
		mCurrentTexture = SDL_CreateTextureFromSurface(GameEngine::GetInstance()->GetRenderer(), currentSurface);
		mCurrentTextureWidth = currentSurface->w;
		mCurrentTextureHeight = currentSurface->h;
		mCurrentYDrawOffset = AnimMapPtr(animState).YDrawOffset;
		mCurrentXDrawOffset = AnimMapPtr(animState).XDrawOffset;
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
		RenderManager::GetInstance()->DrawTexture(mCurrentTexture,
			x + mCurrentXDrawOffset,
			y + mCurrentYDrawOffset,
			mCurrentTextureWidth,
			mCurrentTextureHeight);
	}
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