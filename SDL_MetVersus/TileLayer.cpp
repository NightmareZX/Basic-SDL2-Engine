#include "TileLayer.h"
#include "Camera.h"
#include "MapParser.h"

void TileLayer::Render()
{
	//get the camera bounds so that we know what tiles to render because we shouldnt be rendering tiles if they are outside the camera
	SDL_Rect cameraViewBox = Camera::GetInstance()->GetViewBox();
	Uint32 boundsX = cameraViewBox.x / TILE_SIZE;
	Uint32 boundsY = cameraViewBox.y / TILE_SIZE;
	Uint32 boundsXW = ((cameraViewBox.w + cameraViewBox.x) / TILE_SIZE) + 1;
	Uint32 boundsYH = ((cameraViewBox.h + cameraViewBox.y) / TILE_SIZE) + 1;

	if (boundsXW >= mWidth)boundsXW = mWidth;
	if (boundsYH >= mHeight)boundsYH = mHeight;

	for (size_t row = boundsY; row < boundsYH; row++)
	{
		for (size_t column = boundsX; column < boundsXW; column++)
		{
			Uint32 tileID = mTilemap[row][column];
			Uint32 flipFlags = 0;
			//Create bit mask to mask away the flip bits, this will be used to 0 out the flip flags so that it can be
			//found in a tileset, if this isnt done then the tileset detection will fail
			Uint32 bitMask = ~(TileInformation::DIAGONAL_FLIP | TileInformation::HORIZONTAL_FLIP | TileInformation::VERTICAL_FLIP);
			//Store the flags in a seperate variable for later
			flipFlags = tileID & ~bitMask;
			//Mask away the flags from the tile id now that we stored them
			tileID &= bitMask;
			//Continue if the tile is just air
			if (tileID == 0) continue;
			//Find tile in the tile set
			else
			{
				Uint32 index = 0;
				if (mTileSetList.size() > 1)
				{
					//Loop trough each tileset
					for (size_t i = 0; i < mTileSetList.size(); i++)
					{
						//If the tile is in the current tileset, then 
						if (tileID >= static_cast<Uint32>(mTileSetList[i].firstID) && tileID <= static_cast<Uint32>(mTileSetList[i].lastID))
						{
							tileID = tileID + mTileSetList[i].tileCount - mTileSetList[i].lastID;
							index = static_cast<Uint32>(i);
							break;
						}
					}
				}
				TileSet tilesetTemp = mTileSetList[index];
				Uint32 tileRow = tileID / tilesetTemp.columnCount;
				Uint32 tileCol = tileID - tileRow * tilesetTemp.columnCount - 1;

				//if the tile is on the last column
				if (tileID % tilesetTemp.columnCount == 0)
				{
					tileRow--;
					tileCol = tilesetTemp.columnCount - 1;
				}
				if (flipFlags != 0)
				{
					if (flipFlags & TileInformation::HORIZONTAL_FLIP)
					{
						flipFlags |= SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
						flipFlags ^= TileInformation::HORIZONTAL_FLIP;
					}
					if (flipFlags & TileInformation::VERTICAL_FLIP)
					{
						flipFlags |= SDL_RendererFlip::SDL_FLIP_VERTICAL;
						flipFlags ^= TileInformation::VERTICAL_FLIP;
					}
					if (flipFlags & TileInformation::DIAGONAL_FLIP)
					{
						flipFlags ^= TileInformation::DIAGONAL_FLIP;
					}
				}
				
				RenderManager::GetInstance()->DrawTile(tilesetTemp.source, 
					static_cast<Sint32>(column * TILE_SIZE), 
					static_cast<Sint32>(row * TILE_SIZE),
					tileCol, tileRow , 0, static_cast<SDL_RendererFlip>(flipFlags));
			}
		}
	}
}
void TileLayer::Update()
{

}