#include "RenderManager.h"
#include "GameEngine.h"
#include "Camera.h"
#include "Vector2D.h"

RenderManager* RenderManager::mInstance = nullptr;

RenderManager::RenderManager()
{
	mWindowHeight = 0;
	mWindowWidth = 0;
}
void RenderManager::RegisterTexture(String id, String filename)
{
	//load png data from zip
	//SDL_RWops* imageData = SDL_RWFromConstMem();
	//IMG_Load_RW(imageData,1);

	//It already exists so no point in registering again
	if (ValidateTexture(id)) return;

	SDL_Surface* surf = IMG_Load(filename.c_str());
	if (surf == nullptr) return;
	SDL_Texture* texture = SDL_CreateTextureFromSurface(GameEngine::GetInstance()->GetRenderer(), surf);
	mTextureMap.insert(pair<String, SDL_Texture*>(id, texture));
}
SDL_Surface* RenderManager::RegisterSurface(String id, String filename)
{
	if (ValidateSurface(id)) return mSurfaceMap[id];
	SDL_Surface* surf = IMG_Load(filename.c_str());
	if (surf == nullptr) return nullptr;
	mSurfaceMap.insert(pair<String, SDL_Surface*>(id, surf));
	return surf;
}
void RenderManager::RegisterFont(String id, String filename, Uint32 size)
{
	TTF_Font* font = TTF_OpenFont(filename.c_str(), size);
	if (font == nullptr)
	{
		Logger::GetInstance()->Log("RenderManager.cpp: TTF Error: couldn't load .ttf file. Reason: " + String(SDL_GetError()) );
	}
	SDL_Color colour = {255,255,255};
	FontData fData(colour,font);
	mFontMap.insert(pair<String, FontData>(id, fData));
}
void RenderManager::DrawText(String fontID, Sint32 x, Sint32 y, String Text)
{
	if (!ValidateFont(fontID)) return;
	if (Text == "") return;
	SDL_Surface* textSurface = TTF_RenderText_Solid(mFontMap[fontID].second,Text.c_str(), mFontMap[fontID].first);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(GameEngine::GetInstance()->GetRenderer(), textSurface);
	SDL_Rect dstRect = {x,y,textSurface->w,textSurface->h };
	SDL_RenderCopyEx(GameEngine::GetInstance()->GetRenderer(), textTexture,nullptr, &dstRect,0,nullptr, SDL_FLIP_NONE);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}
void RenderManager::Draw()
{
	
}
void RenderManager::DrawRectangle(SDL_Rect rectangle)
{
	rectangle.x = static_cast<Sint32>(rectangle.x);
	rectangle.y = static_cast<Sint32>(rectangle.y);
	rectangle.w = static_cast<Sint32>(rectangle.w);
	rectangle.h = static_cast<Sint32>(rectangle.h);
	//red
	SDL_SetRenderDrawColor(GameEngine::GetInstance()->GetRenderer(), 255, 0, 0, 255);
	SDL_RenderDrawRect(GameEngine::GetInstance()->GetRenderer(), &rectangle);
	//back to black
	SDL_SetRenderDrawColor(GameEngine::GetInstance()->GetRenderer(), 0, 0, 0, 255);
}

void RenderManager::DrawRectangleRelativeToCamera(SDL_Rect rectangle)
{
	SDL_Rect viewBox = Camera::GetInstance()->GetViewBox();
	SDL_Rect* displayBox = Camera::GetInstance()->GetDisplayBox();
	FloatVector2D scalers = Camera::GetInstance()->GetDrawingScaler();

	rectangle.x = static_cast<Sint32>(((rectangle.x - viewBox.x) * scalers.X) + displayBox->x);
	rectangle.y = static_cast<Sint32>(((rectangle.y - viewBox.y) * scalers.Y) + displayBox->y);
	rectangle.w = static_cast<Sint32>(rectangle.w * scalers.X);
	rectangle.h = static_cast<Sint32>(rectangle.h * scalers.Y);
	//red
	SDL_SetRenderDrawColor(GameEngine::GetInstance()->GetRenderer(), 255, 0, 0, 255);
	SDL_RenderDrawRect(GameEngine::GetInstance()->GetRenderer(), &rectangle);
	//back to black
	SDL_SetRenderDrawColor(GameEngine::GetInstance()->GetRenderer(), 0, 0, 0, 255);
}

void RenderManager::DrawFrame(String textureID, Sint32 x, Sint32 y, Sint32 width, Sint32 height,
	Uint32 frameRowIndex, Uint32 frameColumnIndex,  SDL_RendererFlip flip, double angle)
{
	if (!ValidateTexture(textureID)) return;
	SDL_Rect viewBox = Camera::GetInstance()->GetViewBox();
	SDL_Rect* displayBox = Camera::GetInstance()->GetDisplayBox();
	FloatVector2D scalers = Camera::GetInstance()->GetDrawingScaler();

	SDL_Rect destination = { 
		static_cast<Sint32>(((x - viewBox.x) * scalers.X) + displayBox->x),
		static_cast<Sint32>(((y - viewBox.y)* scalers.Y) + displayBox->y),
		static_cast<Sint32>(width * scalers.X),
		static_cast<Sint32>(height * scalers.Y)};
	SDL_Rect source = { static_cast<Sint32>(frameColumnIndex), static_cast<Sint32>(frameRowIndex), width, height};
	SDL_RenderCopyEx(GameEngine::GetInstance()->GetRenderer(), mTextureMap[textureID],
		&source, &destination,
		angle, nullptr, flip);
}
void RenderManager::DrawTexture(SDL_Texture* texture, Sint32 x, Sint32 y, Sint32 width, Sint32 height)
{
	SDL_Rect viewBox = Camera::GetInstance()->GetViewBox();
	SDL_Rect* displayBox = Camera::GetInstance()->GetDisplayBox();
	FloatVector2D scalers = Camera::GetInstance()->GetDrawingScaler();
	SDL_Rect destination = {
	static_cast<Sint32>(((x - viewBox.x) * scalers.X) + displayBox->x),
	static_cast<Sint32>(((y - viewBox.y) * scalers.Y) + displayBox->y),
	static_cast<Sint32>(width * scalers.X),
	static_cast<Sint32>(height * scalers.Y) };
	SDL_RenderCopy(GameEngine::GetInstance()->GetRenderer(),texture,nullptr, &destination);
}
void RenderManager::DrawTile(String textureID, Sint32 x, Sint32 y, Uint32 column, Uint32 row, double angle, SDL_RendererFlip flip, Uint32 tileSize)
{
	if (!ValidateTexture(textureID)) return;
	SDL_Rect viewBox = Camera::GetInstance()->GetViewBox();
	SDL_Rect* displayBox = Camera::GetInstance()->GetDisplayBox();
	FloatVector2D scalers = Camera::GetInstance()->GetDrawingScaler();

	SDL_Rect destination = { 
		static_cast<Sint32>(((x - viewBox.x) * scalers.X) + displayBox->x),
		static_cast<Sint32>(((y - viewBox.y) * scalers.Y) + displayBox->y),
		static_cast<Sint32>(tileSize * scalers.X),
		static_cast<Sint32>(tileSize * scalers.Y)};

	SDL_Rect source = { static_cast<Sint32>(column * tileSize), static_cast<Sint32>(row * tileSize),
		static_cast<Sint32>(tileSize), static_cast<Sint32>(tileSize) };
	SDL_RenderCopyEx(GameEngine::GetInstance()->GetRenderer(), mTextureMap[textureID],
		&source, &destination,
		angle, nullptr, flip);
}
void RenderManager::DropResource(String id)
{
	mTextureMap.erase(id);
}
bool RenderManager::ValidateTexture(String id)
{
	if (mTextureMap.find(id) != mTextureMap.end())
		return true;
	return false;
}
bool RenderManager::ValidateSurface(String id)
{
	if (mSurfaceMap.find(id) != mSurfaceMap.end())
		return true;
	return false;
}
bool RenderManager::ValidateFont(String id)
{
	if (mFontMap.find(id) != mFontMap.end())
		return true;
	return false;
}
void RenderManager::Dispose()
{
	map<String, SDL_Texture*>::iterator mapIterator;
	for (mapIterator = mTextureMap.begin(); mapIterator != mTextureMap.end(); mapIterator++)
	{
		SDL_DestroyTexture(mapIterator->second);
	}
	mTextureMap.clear();
	delete mInstance;
}
void RenderManager::SetWindowDimensions(Uint32 width, Uint32 height)
{
	mWindowHeight = height;
	mWindowWidth = width;
	Camera::GetInstance()->SetDisplayBox(0,0,width,height);
}