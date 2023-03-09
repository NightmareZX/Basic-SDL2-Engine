#include "GameLevel.h"

GameLevel::GameLevel()
{
	collisionMap = nullptr;
	height = 0;
	width = 0;

	RenderManager::GetInstance()->RegisterTexture("solid", "solid_tile.png");
	RenderManager::GetInstance()->RegisterTexture("lslope", "left_slope_tile.png");
	RenderManager::GetInstance()->RegisterTexture("rslope", "right_slope_tile.png");
}
GameLevel::~GameLevel()
{
	DisposeMap();
}
void GameLevel::LoadMap(String levelFileName)
{
	ReadFileStream rStream(levelFileName, ios::binary);
	if (!rStream)
	{
		//report debug
		return;
	}
	FileHandler fhandle;

	height = fhandle.ReadBinaryUnsigned32Int(rStream);//Rows are the height(y)
	width = fhandle.ReadBinaryUnsigned32Int(rStream);//Collumns are the width(x)
	
	
	//Also get background colour

	//Initiallise the collision map
	collisionMap = new CollisionTile ** [height];
	for (size_t i = 0; i < height; i++)
	{
		collisionMap[i] = new CollisionTile * [width];
	}

	for (size_t rowIndex = 0; rowIndex < width; rowIndex++)
	{
		for (size_t columnIndex = 0; columnIndex < height; columnIndex++)
		{
			Uint8 intType = fhandle.ReadBinaryUnsigned8Int(rStream);
			//UNDEFINED BEHAVIOUR WARNING!
			//Make sure that the value that is read in, corresponds to a value in the enum
			//because if it doesn't it will cause undefined behaviour
			TileTypes type = static_cast<TileTypes>(intType);
			collisionMap[rowIndex][columnIndex] = CreateCollisionTile(type, columnIndex, rowIndex);
		}
	}

}
void GameLevel::DrawMap()
{
	for (size_t columnIndex = 0; columnIndex < width; columnIndex++)
	{
		for (size_t rowIndex = 0; rowIndex < height; rowIndex++)
		{
			CollisionTile* currentTile = collisionMap[rowIndex][columnIndex];
			if (currentTile != nullptr)
			{
				RenderManager::GetInstance()->DrawTile(currentTile->getTextureID(),
					currentTile->getSource(), currentTile->getDestination());
			}
		}
	}
}
void GameLevel::DisposeMap()
{
	for (size_t rowIndex = 0; rowIndex < height; rowIndex++)
	{
		for (size_t columnIndex = 0; columnIndex < width; columnIndex++)
		{
			delete collisionMap[rowIndex][columnIndex];
		}
		delete[] collisionMap[rowIndex];
	}
	delete[] collisionMap;
}

CollisionTile* GameLevel::CreateCollisionTile(TileTypes type, Uint32 tilePosX, Uint32 tilePosY)
{
	if (type == TileTypes::AIR) return nullptr;
	CollisionTile* tempTile = new CollisionTile();
	tempTile->setDestination(tilePosX * TILE_SIZE, tilePosY * TILE_SIZE, TILE_SIZE, TILE_SIZE);
	tempTile->setSource(0, 0, TILE_SIZE, TILE_SIZE);
	tempTile->setCollisonType(type);
	tempTile->setTexture(getTextureTEMP(type));
	return tempTile;
}

String GameLevel::getTextureTEMP(TileTypes type)
{
	if (type == TileTypes::SOLID) return "solid";
	if (type == TileTypes::LEFT_SLOPE) return "lslope";
	if (type == TileTypes::RIGHT_SLOPE) return "rslope";
}