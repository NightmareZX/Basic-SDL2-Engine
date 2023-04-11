#include "GameLevel.h"

GameLevel::GameLevel()
{
	mHeight = 0;
	mWidth = 0;
	mCollisionLayer = nullptr;

	RenderManager::GetInstance()->RegisterTexture("collission_tileset.png", "collission_tileset.png");
	RenderManager::GetInstance()->RegisterTexture("crateria_tileset.png", "crateria_tileset.png");

	Properties* props = new Properties("basic_player.png",100, 60, 26, 43);
	mPlayer = new PlayerEntity(props);
	Camera::GetInstance()->SetTarget(mPlayer);
}
GameLevel::~GameLevel()
{
	Dispose();
}
void GameLevel::AddLayer(Layer* layer, ValidLayers type)
{ 
	switch (type)
	{
	case COLLISION:
		mCollisionLayer = dynamic_cast<TileLayer*>(layer);
		break;
	case FOREGROUND:
		mForegroundLayer = dynamic_cast<TileLayer*>(layer);
		break;
	case MIDDLEGROUND:
		mMiddlegroundLayer = dynamic_cast<TileLayer*>(layer);
		break;
	case BACKGROUND:
		mBackgroundLayer = dynamic_cast<TileLayer*>(layer);
		break;
	case PARALAX1:
		break;
	case PARALAX2:
		break;
	case NONEVALID:
		break;
	default:
		break;
	} 
}
void GameLevel::DrawMap()
{
	//Draw the background layers first
	if (mBackgroundLayer != nullptr) mBackgroundLayer->Render();
	if (mMiddlegroundLayer != nullptr) mMiddlegroundLayer->Render();
	//Objects such as enemies get rendered between middle ground and foreground (usually)
	mPlayer->Draw();
	if (mForegroundLayer != nullptr) mForegroundLayer->Render();
}
void GameLevel::Update(float timeDelta)
{
	mPlayer->Update(timeDelta);
	Camera::GetInstance()->SetCameraBounds(Vector2D(mWidth * 16, mHeight * 16));
	Camera::GetInstance()->Update();
}
void GameLevel::Dispose()
{
	if (mPlayer != nullptr)
	{
		mPlayer->Dispose();
		delete mPlayer;
		mPlayer = nullptr;
	}
	if (mCollisionLayer != nullptr)
	{
		delete mCollisionLayer;
	}
}
