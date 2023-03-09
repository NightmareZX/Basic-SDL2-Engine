#include "RenderManager.h"
#include "GameEngine.h"

RenderManager* RenderManager::instance = nullptr;

RenderManager* RenderManager::GetInstance()
{
	if (instance == nullptr) instance = new RenderManager();
	return instance;
}
void RenderManager::RegisterTexture(String id, String filename)
{
	SDL_Surface* surf = IMG_Load(filename.c_str());
	if (surf == nullptr) return;
	SDL_Texture* texture = SDL_CreateTextureFromSurface(GameEngine::GetInstance()->GetRenderer(), surf);
	textureMap.insert(pair<String, SDL_Texture*>(id, texture));
}
void RenderManager::Draw()
{

}
void RenderManager::DrawFrame()
{

}
void RenderManager::DrawTile(String textureID, SDL_Rect* source, SDL_Rect* dest, double angle, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(GameEngine::GetInstance()->GetRenderer(), textureMap[textureID],
		source, dest,
		angle, nullptr, flip);
}
void RenderManager::DropResource()
{

}
void RenderManager::Dispose()
{
	map<String, SDL_Texture*>::iterator mapIterator;
	for (mapIterator = textureMap.begin(); mapIterator != textureMap.end(); mapIterator++)
	{
		SDL_DestroyTexture(mapIterator->second);
	}
	textureMap.clear();
	delete instance;
}