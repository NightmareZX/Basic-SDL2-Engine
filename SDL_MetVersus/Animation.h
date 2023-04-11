#pragma once
#include "STL_Components.h"
#include "SDL.h"

#pragma region Other_Declerations

typedef Uint8 AnimFlags;

enum AnimationFlags : Uint8 
{
	ANIM_NONE = 0x00,
	AGR_HEIGHT = 0x01,
	AGR_WIDTH = 0x02,
	REVERSE = 0x80,

};

/// <summary>
/// A structure that holds a particular segment which will be combined with other segment(s)
/// </summary>
struct AnimationSegmentData
{
	//
	Uint32 height = 0;
	//
	Uint32 width = 0;
	//
	Uint32 heightOffset = 0;
	//
	Uint32 widthOffset = 0;
	//
	//Uint32 XOffset = 0;
	//
	//Uint32 YOffset = 0;
	//
	Uint32 fCount = 0;
	//
	Uint32 XIndex = 0;
	//
	Uint32 YIndex = 0;
	//
	float cycleTime = 0;
	//
	Uint32 borderSize = 0;
	//
	Uint8 flags = 0;
};
using SegmentList = vector<AnimationSegmentData*>;

struct AnimationData
{
	//Width of the frame and subsequent frames
	Uint32 width = 0;
	//Height of the frame and subsequent frames
	Uint32 height = 0;
	//The amount of frames in the animation
	Uint32 fCount = 0;
	//How fast the frames should be played
	float cycleTime = 0;
	//The top left corner y position of the frame relative to the while image size
	Uint32 YIndex = 0;
	//The column index where the first frame of the animation as at
	Uint32 XIndex = 0;
	//The Y draw offset value, it will get added on to the Y position given in the Draw function parameter
	Sint32 YDrawOffset = 0;
	//The X draw offset value, it will get added on to the X position given in the Draw function parameter
	Sint32 XDrawOffset = 0;
	//The thickness of the border that seperates the frames, 0 is no border
	Uint32 borderSize = 0;
	//
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	//
	SegmentList segmentList;

	//Vectors that hold the segments' x and y offsets 

	vector<Sint32> x_offsets;
	vector<Sint32> y_offsets;

	//
	Uint8 flags = 0;

};

using AnimationMap = unordered_map<String, AnimationData>;

#pragma endregion
class Animation
{
private:
	AnimationMap* mAnimationMap;
	String mSpriteSheetName;
	String mLastAnimState;
	Uint32 mCurrentFrame;
	//Used as a timer between frames, effected by the animation's speed
	float mFrameCounter;

	SDL_Surface* mCurrentSurface;
	SDL_Texture* mCurrentTexture;

	Uint32 mCurrentTextureWidth;
	Uint32 mCurrentTextureHeight;
	Sint32 mCurrentYDrawOffset;
	Sint32 mCurrentXDrawOffset;

	SDL_Surface* mSpriteSheetPtr;

	bool mSegnentedAniamtion;
public:
	Animation();
	void Initialise(String spriteSheetName);
	void Update(String animState, float deltaTime, float cycleTimerOffset = 0.0f);
	void Draw(Sint32 x, Sint32 y);
	bool ValidateState(String animState);
	void Dispose();
};