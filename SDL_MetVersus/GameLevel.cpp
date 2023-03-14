#include "GameLevel.h"

GameLevel::GameLevel()
{
	height = 0;
	width = 0;
	collisionLayer = nullptr;

	RenderManager::GetInstance()->RegisterTexture("solid", "solid_tile.png");
	RenderManager::GetInstance()->RegisterTexture("lslope", "left_slope_tile.png");
	RenderManager::GetInstance()->RegisterTexture("rslope", "right_slope_tile.png");

	RenderManager::GetInstance()->RegisterTexture("collission_tileset.png", "collission_tileset.png");
	RenderManager::GetInstance()->RegisterTexture("crateria_tileset.png", "crateria_tileset.png");

	Properties* props = new Properties("basic_player.png",100, 60, 26, 43);
	player = new PlayerEntity(props);
}
GameLevel::~GameLevel()
{
	DisposeMap();
}
void GameLevel::AddLayer(Layer* layer) { layers.push_back(layer); }
void GameLevel::DrawMap()
{
	for (size_t i = 0; i < layers.size(); i++)
	{
		layers[i]->Render();
	}
	player->Draw();
}
void GameLevel::Update()
{
	player->Update();
}
void GameLevel::DisposeMap()
{

}
