#include "AnimationManager.h"
#include "RenderManager.h"
#include "IniFile.h"

AnimationManager* AnimationManager::mInstance = nullptr;

AnimationMetaData AnimationManager::GetAnimation(String spriteSheetName)
{
	if (!ValidateAnimation(spriteSheetName))
	{
		SharedAnimation* anim = LoadAnimation(spriteSheetName);
		mLoadedAnimations.insert(pair<String, SharedAnimation*>(spriteSheetName, anim));
	}
	else
	{
		mLoadedAnimations[spriteSheetName]->referenceCount++;
	}
	AnimationMetaData metaData;
	metaData.animMap = mLoadedAnimations[spriteSheetName]->animMap;
	metaData.isSegmented = mLoadedAnimations[spriteSheetName]->isSegmented;
	metaData.spriteSheetSurface = mLoadedAnimations[spriteSheetName]->spriteSheetSurface;

	return metaData;
}
void AnimationManager::RemoveAnimation(String spriteSheetID)
{
	if (ValidateAnimation(spriteSheetID))
	{
		mLoadedAnimations[spriteSheetID]->referenceCount--;
	}
}
bool AnimationManager::ValidateAnimation(String spriteSheetID)
{
	if (mLoadedAnimations.find(spriteSheetID) != mLoadedAnimations.end())
		return true;
	return false;
}

SharedAnimation* AnimationManager::LoadAnimation(String spriteSheetName)
{
	IniFile animationProps;
	bool fileLoaded = animationProps.LoadFile(spriteSheetName + ".ini");
	if (!fileLoaded) return nullptr;
	AnimationMap* aMap = new AnimationMap;
	map<String, AnimationSegmentData*> tempSegmentMap;
	SegmentList segList;
	vector<SDL_Surface*> flippedSegments;
	Sint32 xIndex = 0;
	bool isSegmented = false;

	SectionList sList = animationProps.GetAllSections();

	SDL_Surface* spriteSheetPtr = nullptr;

	for (SectionList::iterator it = sList.begin(); it != sList.end(); it++)
	{
		String section = it->pItem;
		if (section == "BaseProperties")
		{
			isSegmented = (animationProps.GetBooleanValue(section, "is_dynamic"));

			if (!RenderManager::GetInstance()->ValidateTexture(spriteSheetName))
			{
				if (isSegmented)
				{
					spriteSheetPtr = RenderManager::GetInstance()->RegisterSurface(spriteSheetName, spriteSheetName + ".png");
				}
				else
				{
					RenderManager::GetInstance()->RegisterTexture(spriteSheetName, spriteSheetName + ".png");
				}
			}
		}
		else
		{
			// if we are dealing with a segment
			if (animationProps.GetBooleanValue(section, "is_segment") && isSegmented)
			{
				AnimationSegmentData* segment = new AnimationSegmentData;
				segment->YIndex = animationProps.GetUnsigned32Value(section, "yindex");
				segment->XIndex = animationProps.GetUnsigned32Value(section, "xindex");
				segment->heightOffset = animationProps.GetSigned32Value(section, "height_offset");
				segment->widthOffset = animationProps.GetSigned32Value(section, "width_offset");
				segment->width = animationProps.GetUnsigned32Value(section, "width");
				segment->height = animationProps.GetUnsigned32Value(section, "height");
				segment->fCount = animationProps.GetUnsigned32Value(section, "count");
				segment->cycleTime = animationProps.GetFloatValue(section, "cycle_time");
				segment->borderSize = animationProps.GetUnsigned32Value(section, "border_size");
				if (animationProps.GetBooleanValue(section, "aggregate_height"))
					segment->flags ^= AnimationFlags::AGR_HEIGHT;
				if (animationProps.GetBooleanValue(section, "aggregate_width"))
					segment->flags ^= AnimationFlags::AGR_WIDTH;
				if (animationProps.GetBooleanValue(section, "reverse"))
					segment->flags ^= AnimationFlags::REVERSE;

				segList.push_back(segment);
				tempSegmentMap.insert(pair<String, AnimationSegmentData*>(section,segment));
			}
			//If we are handling an animation state
			else
			{
				AnimationStateData data;
				data.YIndex = animationProps.GetUnsigned32Value(section, "yindex");
				data.XIndex = animationProps.GetUnsigned32Value(section, "xindex");
				data.YDrawOffset = animationProps.GetSigned32Value(section, "y_draw_offset");
				data.XDrawOffset = animationProps.GetSigned32Value(section, "x_draw_offset");
				data.width = animationProps.GetUnsigned32Value(section, "width");
				data.height = animationProps.GetUnsigned32Value(section, "height");
				data.fCount = animationProps.GetUnsigned32Value(section, "count");
				data.cycleTime = animationProps.GetFloatValue(section, "cycle_time");
				data.borderSize = animationProps.GetUnsigned32Value(section, "border_size");
				if (animationProps.GetBooleanValue(section, "reverse"))
					data.flags ^= AnimationFlags::REVERSE;

				//
				Uint32 savedWidth = data.width;
				Uint32 savedHeight = data.height;
				bool aggregatedHeight = false;
				bool aggregatedWidth = false;
				Uint32 largestHeight = 0;
				Uint32 largestWidth = 0;
				vector<String> segments = animationProps.GetMultipleStringValues(section, "segments");
				//loop trough the segments and handle them 
				for (size_t i = 0; i < segments.size(); i++)
				{
					AnimationSegmentData* currSegment = tempSegmentMap[segments[i]];
					data.segmentList.push_back(currSegment);

					Sint32 x_offset = animationProps.GetSigned32Value(section, segments[i] + "_x_offset");
					Sint32 y_offset = animationProps.GetSigned32Value(section, segments[i] + "_y_offset");

					if (currSegment->flags & AnimationFlags::AGR_HEIGHT)
					{
						if (i == 0) data.blitYPos.push_back(0);
						else data.blitYPos.push_back(data.height + y_offset);
						data.height += currSegment->height - currSegment->heightOffset;
						aggregatedHeight = true;
					}
					else
					{
						data.blitYPos.push_back(y_offset);
					}
						
					if (currSegment->flags & AnimationFlags::AGR_WIDTH)
					{
						if (i == 0) data.blitXPos.push_back(0);
						else data.blitXPos.push_back(data.width + x_offset);
						data.width += currSegment->width - currSegment->widthOffset;
						aggregatedWidth = true;
					}
					else
					{
						data.blitXPos.push_back(x_offset);
					}

					if (data.height < currSegment->height && !aggregatedHeight)
						data.height = currSegment->height;

					if (data.width < currSegment->width && !aggregatedWidth)
						data.width = currSegment->width;

					if (animationProps.GetBooleanValue(section, segments[i] + "_add_x_offset"))
						data.width += abs(x_offset);
					if (animationProps.GetBooleanValue(section, segments[i] + "_add_y_offset"))
						data.height += abs(y_offset);

					Sint32 flipFlags = 0;
					if (animationProps.GetBooleanValue(section, segments[i] + "_h_flip"))
						flipFlags ^= SDL_FLIP_HORIZONTAL;
					if (animationProps.GetBooleanValue(section, segments[i] + "_v_flip"))
						flipFlags ^= SDL_FLIP_VERTICAL;
					HandleSegmentFlipping(currSegment, flipFlags, spriteSheetPtr);

					data.flip_flags.push_back(static_cast<SDL_RendererFlip>(flipFlags));
				}
				//If there was an explicit definition of the height or width, then use those definitions to override automatic calculations
				if (savedHeight > 0) data.height = savedHeight;
				if (savedWidth > 0) data.width = savedWidth;

				bool flipH = animationProps.GetBooleanValue(section, "flip_h");
				bool flipV = animationProps.GetBooleanValue(section, "flip_v");
				Uint32 flipVal = 0;
				if (flipH)
					flipVal |= SDL_FLIP_HORIZONTAL;
				if (flipV)
					flipVal |= SDL_FLIP_VERTICAL;
				data.flip = static_cast<SDL_RendererFlip>(flipVal);
				aMap->insert(pair<String, AnimationStateData>(section, data));
			}

		}
	}
	SharedAnimation* sAnim = new SharedAnimation;
	sAnim->animMap = aMap;
	sAnim->referenceCount = 1;
	sAnim->animSegments = segList;
	sAnim->isSegmented = isSegmented;
	sAnim->spriteSheetSurface = spriteSheetPtr;
	sAnim->flippedSegments = flippedSegments;
	return sAnim;
}
void AnimationManager::HandleSegmentFlipping(AnimationSegmentData* segmentToFlip, Uint32 flipFlags, SDL_Surface* spriteSheet)
{
	if (flipFlags == 0) return;
	for (size_t i = 0; i < segmentToFlip->flippedVersions.size(); i++)
	{
		//if there is already a flipped version of the segment with these flags, then just exit
		if (segmentToFlip->flippedVersions[i].first == flipFlags) 
			return;
	}
	//The surface where all flipped segments will be stored in
	SDL_Surface* flippedSegment = SDL_CreateRGBSurfaceWithFormat(0,
		segmentToFlip->width * segmentToFlip->fCount, segmentToFlip->height, spriteSheet->format->BytesPerPixel, spriteSheet->format->format);
	for (size_t i = 0; i < segmentToFlip->fCount; i++)
	{
		SDL_Rect currentFrame;
		currentFrame.x = segmentToFlip->XIndex + (i * segmentToFlip->width) + (i * segmentToFlip->borderSize);
		currentFrame.y = segmentToFlip->YIndex;
		currentFrame.w = segmentToFlip->width;
		currentFrame.h = segmentToFlip->height;

		SDL_Surface* flippedFrame = SDL_CreateRGBSurfaceWithFormat(0,
			segmentToFlip->width, segmentToFlip->height, spriteSheet->format->BytesPerPixel, spriteSheet->format->format);
		SDL_UpperBlit(spriteSheet, &currentFrame, flippedFrame, nullptr);

		if (flipFlags & SDL_FLIP_HORIZONTAL) RenderManager::SurfaceFlipHorizontal(flippedFrame);
		if (flipFlags & SDL_FLIP_VERTICAL) RenderManager::SurfaceFlipVertical(flippedFrame);
		SDL_Rect dest;
		dest.x = segmentToFlip->width * i;//MIGHT NEED TO ADD 1 HERE
		dest.y = 0;
		dest.w = segmentToFlip->width;
		dest.h = segmentToFlip->height;
		SDL_UpperBlit(flippedFrame, nullptr, flippedSegment, &dest);
		SDL_FreeSurface(flippedFrame);
	}
	segmentToFlip->flippedVersions.push_back(pair<Uint32, SDL_Surface*>(flipFlags, flippedSegment));
}

SDL_Surface* AnimationManager::GetFlippedSegment(AnimationSegmentData* segmentData, SDL_RendererFlip flipFlags)
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
void AnimationManager::RefreshLoadedAnimations()
{
	for (LoadedAnimationsMap::iterator it = mLoadedAnimations.begin(); it != mLoadedAnimations.end(); it++)
	{
		if (it->second->referenceCount == 0)
		{
			DisposeSharedAnimation(it->second);
			mLoadedAnimations.erase(it);
		}
	}
}

void AnimationManager::DisposeAnimationMap(AnimationMap* animMap)
{

	delete animMap;
}
void AnimationManager::DisposeSharedAnimation(SharedAnimation* animMap)
{
	DisposeAnimationMap(animMap->animMap);
	for (size_t i = 0; animMap->animSegments.size(); i++)
	{
		delete animMap->animSegments[i];
	}
}