#include "RenderManager.h"
#include "GameEngine.h"
#include "Camera.h"
#include "Vector2D.h"

RenderManager::RenderManager( Camera* camera)
{
	const String title = "EngineTest";
	const Uint32 xpos SDL_WINDOWPOS_CENTERED;
	const Uint32 ypos = SDL_WINDOWPOS_CENTERED;
	const Uint32 width = 1200;
	const Uint32 height = 600;
	bool fullscreen = false;
	mMainWindow = nullptr;
	mMainRenderer = nullptr;
	Uint16 flags = 0; //SDL_WINDOW_MAXIMIZED
	if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) mFailFlag = true;
	else
	{
		mMainWindow = SDL_CreateWindow(title.c_str(), xpos, ypos, width, height, flags);
		if (mMainWindow != nullptr)
		{
			mMainRenderer = SDL_CreateRenderer(mMainWindow, -1, 0);
			mFailFlag = false;
		}
		else mFailFlag = true;
	}

	mWindowHeight = height;
	mWindowWidth = width;
	mCameraInstance = camera;
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
	SDL_Texture* texture = SDL_CreateTextureFromSurface(mMainRenderer, surf);
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
		Logger::Log("RenderManager.cpp: TTF Error: couldn't load .ttf file. Reason: " + String(SDL_GetError()) );
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
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(mMainRenderer, textSurface);
	SDL_Rect dstRect = {x,y,textSurface->w,textSurface->h };
	SDL_RenderCopyEx(mMainRenderer, textTexture,nullptr, &dstRect,0,nullptr, SDL_FLIP_NONE);
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
	SDL_SetRenderDrawColor(mMainRenderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(mMainRenderer, &rectangle);
	//back to black
	SDL_SetRenderDrawColor(mMainRenderer, 0, 0, 0, 255);
}

void RenderManager::DrawRectangleRelativeToCamera(SDL_Rect rectangle)
{
	SDL_Rect viewBox = mCameraInstance->GetViewBox();
	SDL_Rect* displayBox = mCameraInstance->GetDisplayBox();
	FloatVector2D scalers = mCameraInstance->GetDrawingScaler();

	rectangle.x = static_cast<Sint32>(((rectangle.x - viewBox.x) * scalers.X) + displayBox->x);
	rectangle.y = static_cast<Sint32>(((rectangle.y - viewBox.y) * scalers.Y) + displayBox->y);
	rectangle.w = static_cast<Sint32>(rectangle.w * scalers.X);
	rectangle.h = static_cast<Sint32>(rectangle.h * scalers.Y);
	//red
	SDL_SetRenderDrawColor(mMainRenderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(mMainRenderer, &rectangle);
	//back to black
	SDL_SetRenderDrawColor(mMainRenderer, 0, 0, 0, 255);
}

void RenderManager::DrawFrame(String textureID, Sint32 x, Sint32 y, Sint32 width, Sint32 height,
	Uint32 frameRowIndex, Uint32 frameColumnIndex,  SDL_RendererFlip flip, double angle)
{
	if (!ValidateTexture(textureID)) return;
	SDL_Rect viewBox = mCameraInstance->GetViewBox();
	SDL_Rect* displayBox = mCameraInstance->GetDisplayBox();
	FloatVector2D scalers = mCameraInstance->GetDrawingScaler();

	SDL_Rect destination = { 
		static_cast<Sint32>(((x - viewBox.x) * scalers.X) + displayBox->x),
		static_cast<Sint32>(((y - viewBox.y)* scalers.Y) + displayBox->y),
		static_cast<Sint32>(width * scalers.X),
		static_cast<Sint32>(height * scalers.Y)};
	SDL_Rect source = { static_cast<Sint32>(frameColumnIndex), static_cast<Sint32>(frameRowIndex), width, height};
	SDL_RenderCopyEx(mMainRenderer, mTextureMap[textureID],
		&source, &destination,
		angle, nullptr, flip);
}
void RenderManager::DrawTexture(SDL_Texture* texture, Sint32 x, Sint32 y, Sint32 width, Sint32 height)
{
	SDL_Rect viewBox = mCameraInstance->GetViewBox();
	SDL_Rect* displayBox = mCameraInstance->GetDisplayBox();
	FloatVector2D scalers = mCameraInstance->GetDrawingScaler();
	SDL_Rect destination = {
	static_cast<Sint32>(((x - viewBox.x) * scalers.X) + displayBox->x),
	static_cast<Sint32>(((y - viewBox.y) * scalers.Y) + displayBox->y),
	static_cast<Sint32>(width * scalers.X),
	static_cast<Sint32>(height * scalers.Y) };
	SDL_RenderCopy(mMainRenderer,texture,nullptr, &destination);
}
void RenderManager::DrawTile(String textureID, Sint32 x, Sint32 y, Uint32 column, Uint32 row, double angle, SDL_RendererFlip flip, Uint32 tileSize)
{
	if (!ValidateTexture(textureID)) return;
	SDL_Rect viewBox = mCameraInstance->GetViewBox();
	SDL_Rect* displayBox = mCameraInstance->GetDisplayBox();
	FloatVector2D scalers = mCameraInstance->GetDrawingScaler();

	SDL_Rect destination = { 
		static_cast<Sint32>(((x - viewBox.x) * scalers.X) + displayBox->x),
		static_cast<Sint32>(((y - viewBox.y) * scalers.Y) + displayBox->y),
		static_cast<Sint32>(tileSize * scalers.X),
		static_cast<Sint32>(tileSize * scalers.Y)};

	SDL_Rect source = { static_cast<Sint32>(column), static_cast<Sint32>(row),
		static_cast<Sint32>(tileSize), static_cast<Sint32>(tileSize) };
	SDL_RenderCopyEx(mMainRenderer, mTextureMap[textureID],
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
	SDL_DestroyWindow(mMainWindow);
	SDL_DestroyRenderer(mMainRenderer);
}
void RenderManager::SetWindowDimensions(Uint32 width, Uint32 height)
{
	mWindowHeight = height;
	mWindowWidth = width;
	mCameraInstance->SetDisplayBox(0,0,width,height);
}
void RenderManager::InitCamera()
{
	mCameraInstance->SetDisplayBox(0, 0, mWindowWidth, mWindowHeight);
}
void RenderManager::BlitSurface(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Surface* dst, SDL_Rect* dstrect, SDL_RendererFlip flip)
{
	if (flip == SDL_FLIP_NONE)
	{
		SDL_UpperBlit(src, srcrect, dst, dstrect);
		return;
	}
	//Create a temporary surface so that we can use it to flip the pixels
	SDL_Surface* tempSurface = SDL_CreateRGBSurfaceWithFormat(0, srcrect->w, srcrect->h, src->format->BytesPerPixel, src->format->format);
	SDL_UpperBlit(src,srcrect, tempSurface, nullptr);

	if (flip & SDL_FLIP_HORIZONTAL)
		SurfaceFlipHorizontal(tempSurface);
	if (flip & SDL_FLIP_VERTICAL)
		SurfaceFlipVertical(tempSurface);

	SDL_UpperBlit(tempSurface,nullptr,dst,dstrect);
}
void RenderManager::SurfaceFlipHorizontal(SDL_Surface* surface)
{
	SDL_LockSurface(surface);
	Sint32 pitch = surface->pitch;
	Uint8* pixels = reinterpret_cast<Uint8*>(surface->pixels);
	Uint8 bytesPerPixel = surface->format->BytesPerPixel;

	Uint8 rowLenght = surface->w * bytesPerPixel;
	for (size_t row = 0; row < surface->h; row++)
	{
		Uint8* currentRow = pixels + (row * pitch);

		Uint8* start = currentRow;
		Uint8* end = currentRow + rowLenght - bytesPerPixel;
		while (start < end) 
		{
			for (size_t i = 0; i < bytesPerPixel; i++) 
			{
				std::swap(*(start + i), *(end + i));
			}
			start += bytesPerPixel;
			end -= bytesPerPixel;
		}

	}
	SDL_UnlockSurface(surface);
}
void RenderManager::SurfaceFlipVertical(SDL_Surface* surface)
{
	SDL_LockSurface(surface);
	Sint32 pitch = surface->pitch;
	Uint8* tempBuffer = new Uint8[pitch];
	Uint8* pixels = reinterpret_cast<Uint8*>(surface->pixels);

	for (size_t i = 0; i < surface->h / 2; i++)
	{
		Uint8* row1 = pixels + i * pitch;
		Uint8* row2 = pixels + (surface->h - i - 1) * pitch;

		memcpy(tempBuffer, row1, pitch);
		memcpy(row1, row2, pitch);
		memcpy(row2, tempBuffer, pitch);
	}

	delete[] tempBuffer;

	SDL_UnlockSurface(surface);
}