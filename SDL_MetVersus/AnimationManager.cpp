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
	Sint32 xIndex = 0;
	bool isSegmented = false;

	SectionList sList = animationProps.GetAllSections();

	for (SectionList::iterator it = sList.begin(); it != sList.end(); it++)
	{
		String section = it->pItem;
		if (section == "BaseProperties")
		{
			isSegmented = (animationProps.GetBooleanValue(section, "is_dynamic"));
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
				AnimationData data;
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

				//if the height and width is 0, because then we need to change it to which ever segment has the biggest
				bool isHeightNull = !(data.width);
				bool isWidthNull = !(data.height);
				vector<String> segments = animationProps.GetMultipleStringValues(section, "segments");
				//loop trough the segments and handle them 
				for (size_t i = 0; i < segments.size(); i++)
				{
					AnimationSegmentData* currSegment = tempSegmentMap[segments[i]];
					data.segmentList.push_back(currSegment);

					if (isHeightNull && data.height < currSegment->height)
						data.height = currSegment->height;

					if (isWidthNull && data.width < currSegment->width)
						data.width = currSegment->width;

					data.x_offsets.push_back(animationProps.GetSigned32Value(section, segments[i] + "_x_offset"));
					data.y_offsets.push_back(animationProps.GetSigned32Value(section, segments[i] + "_y_offset"));

				}

				bool flipH = animationProps.GetBooleanValue(section, "flip_h");
				bool flipV = animationProps.GetBooleanValue(section, "flip_v");
				Uint32 flipVal = 0;
				if (flipH)
					flipVal |= SDL_FLIP_HORIZONTAL;
				if (flipV)
					flipVal |= SDL_FLIP_VERTICAL;
				data.flip = static_cast<SDL_RendererFlip>(flipVal);
				aMap->insert(pair<String, AnimationData>(section, data));
			}

		}
	}
	SDL_Surface* spriteSheetPtr = nullptr;
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
	SharedAnimation* sAnim = new SharedAnimation;
	sAnim->animMap = aMap;
	sAnim->referenceCount = 1;
	sAnim->animSegments = segList;
	sAnim->isSegmented = isSegmented;
	sAnim->spriteSheetSurface = spriteSheetPtr;
	return sAnim;
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