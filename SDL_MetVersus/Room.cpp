#include "Room.h"

Room::Room(Camera* camera, RenderManager* rendManager)
{
	mHeight = 0;
	mWidth = 0;
	//mCollisionLayer = nullptr;
	mBackgroundLayer = nullptr;
	mForegroundLayer = nullptr;
	mMiddlegroundLayer = nullptr;

	mRendererManagerInstance = rendManager;
	mCameraInstance = camera;

	mRendererManagerInstance->RegisterTexture("collission_tileset.png", "collission_tileset.png");
	mRendererManagerInstance->RegisterTexture("crateria_tileset.png", "crateria_tileset.png");


}
Room::~Room()
{
	Dispose();
}
void Room::AddLayer(Layer* layer, ValidLayers type)
{ 
	switch (type)
	{
	//case COLLISION:
	//	mCollisionLayer = dynamic_cast<TileLayer*>(layer);
	//	break;
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

void Room::Update(float timeDelta)
{
	//mPlayer->Update(timeDelta);
	mCameraInstance->SetCameraBounds(Vector2D(mWidth * 16, mHeight * 16));
}
void Room::Dispose()
{
	//if (mPlayer != nullptr)
	//{
	//	mPlayer->Dispose();
	//	delete mPlayer;
	//	mPlayer = nullptr;
	//}
	//if (mCollisionLayer != nullptr)
	//{
	//	delete mCollisionLayer;
	//}
}
