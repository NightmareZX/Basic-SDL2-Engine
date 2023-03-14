#include "RenderManager.h"
#include "GameEngine.h"

RenderManager* RenderManager::instance = nullptr;

RenderManager::RenderManager()
{
	mZoom = 2.0f;
}
RenderManager* RenderManager::GetInstance()
{
	if (instance == nullptr) instance = new RenderManager();
	return instance;
}
void RenderManager::RegisterTexture(String id, String filename)
{
	//load png data from zip
	//SDL_RWops* imageData = SDL_RWFromConstMem();
	//IMG_Load_RW(imageData,1);

	SDL_Surface* surf = IMG_Load(filename.c_str());
	if (surf == nullptr) return;
	SDL_Texture* texture = SDL_CreateTextureFromSurface(GameEngine::GetInstance()->GetRenderer(), surf);
	textureMap.insert(pair<String, SDL_Texture*>(id, texture));
}
void RenderManager::Draw()
{

}
void RenderManager::DrawRectangle(SDL_Rect rectangle)
{
	rectangle.x *= mZoom;
	rectangle.y *= mZoom;
	rectangle.w *= mZoom;
	rectangle.h *= mZoom;
	//red
	SDL_SetRenderDrawColor(GameEngine::GetInstance()->GetRenderer(), 255, 0, 0, 255);
	SDL_RenderDrawRect(GameEngine::GetInstance()->GetRenderer(), &rectangle);
	//back to black
	SDL_SetRenderDrawColor(GameEngine::GetInstance()->GetRenderer(), 0, 0, 0, 255);
}
void RenderManager::DrawFrame(String textureID, Sint32 x, Sint32 y, Sint32 width, Sint32 height, Uint32 row,
	Uint32 frame, SDL_RendererFlip flip )
{
	SDL_Rect destination = { static_cast<Sint32>(x * mZoom), static_cast<Sint32>(y*mZoom),
		static_cast<Sint32>(width * mZoom), static_cast<Sint32>(height * mZoom)};
	SDL_Rect source = { width * static_cast<Sint32>(frame), static_cast<Sint32>(row) * height, width, height};
	if (!ValidateTexture(textureID)) return;
	SDL_RenderCopyEx(GameEngine::GetInstance()->GetRenderer(), textureMap[textureID],
		&source, &destination,
		0, nullptr, flip);
}
void RenderManager::DrawTile(String textureID, Sint32 x, Sint32 y, Uint32 column, Uint32 row, double angle, SDL_RendererFlip flip, Uint32 tileSize)
{
	SDL_Rect destination = { static_cast<Sint32>(x * mZoom), static_cast<Sint32>(y * mZoom),
		static_cast<Sint32>(tileSize * mZoom), static_cast<Sint32>(tileSize * mZoom)};
	SDL_Rect source = { static_cast<Sint32>(column * tileSize), static_cast<Sint32>(row * tileSize),
		static_cast<Sint32>(tileSize), static_cast<Sint32>(tileSize) };
	if (!ValidateTexture(textureID)) return;
	SDL_RenderCopyEx(GameEngine::GetInstance()->GetRenderer(), textureMap[textureID],
		&source, &destination,
		angle, nullptr, flip);
}
void RenderManager::DropResource(String id)
{
	textureMap.erase(id);
}
bool RenderManager::ValidateTexture(String id)
{
	if (textureMap.find(id) != textureMap.end())
		return true;
	return false;
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