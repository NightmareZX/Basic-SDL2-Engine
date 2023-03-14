#include "Animation.h"
#include "RenderManager.h"

void Animation::Update()
{
	//mSpriteFrame = (SDL_GetTicks()/mAnimSpeed) % mFrameCount;
}
void Animation::Draw(float x, float y, Uint32 spriteWidth, Uint32 spriteHeight)
{
	mSpriteFrame = 0;
	RenderManager::GetInstance()->DrawFrame(mTextureID,x,y,spriteWidth,spriteHeight,mSpriteRow,mSpriteFrame);
}
void Animation::SetProperties(String TextureID, Uint32 spriteRow, Uint32 frameCount, Uint32 animSpeed, SDL_RendererFlip flip )
{
	mTextureID = TextureID;
	mSpriteRow = spriteRow;
	mFrameCount = frameCount;
	mAnimSpeed = animSpeed;
	mFlip = flip;
}