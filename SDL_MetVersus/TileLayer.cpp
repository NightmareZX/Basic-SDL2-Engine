#include "TileLayer.h"

void TileLayer::Render()
{
	for (size_t row = 0; row < rowCount; row++)
	{
		for (size_t column = 0; column < columnCount; column++)
		{
			Uint32 tileID = tilemap[row][column];
			if (tileID == 0) continue;
			else
			{
				Uint32 index = 0;
				if (tileSetList.size() > 1)
				{
					for (size_t i = 0; i < tileSetList.size(); i++)
					{
						if (tileID >= tileSetList[i].firstID && tileID <= tileSetList[i].lastID)
						{
							tileID = tileID + tileSetList[i].tileCount - tileSetList[i].lastID;
							index = i;
							break;
						}
					}
				}
				TileSet tilesetTemp = tileSetList[index];
				Uint32 tileRow = tileID / tilesetTemp.columnCount;
				Uint32 tileCol = tileID - tileRow * tilesetTemp.columnCount - 1;

				//if the tile is on the last column
				if (tileID % tilesetTemp.columnCount == 0)
				{
					tileRow--;
					tileCol = tilesetTemp.columnCount - 1;
				}
				RenderManager::GetInstance()->DrawTile(tilesetTemp.source, column * TILE_SIZE, row * TILE_SIZE, tileCol, tileRow);
			}
		}
	}
}
void TileLayer::Update()
{

}