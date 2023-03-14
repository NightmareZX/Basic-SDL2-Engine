#include "CollisionHandler.h"
#include "GameEngine.h"

CollisionHandler* CollisionHandler::instance = nullptr;

bool CollisionHandler::CheckCollision(SDL_Rect a, SDL_Rect b)
{
	bool x_overlap = (a.x < b.x + b.w) && (a.x + a.w > b.x);
	bool y_overlap = (a.y < b.y + b.h) && (a.y + a.h > b.y);
	return (x_overlap && y_overlap);
}
TileData CollisionHandler::MapCollision(SDL_Rect a)
{
	GameLevel* currentLevel = GameEngine::GetInstance()->GetCurrentMap();
	Uint32 mapColumnCount = currentLevel->collisionLayer->GetColumnCount();
	Uint32 mapRowCount = currentLevel->collisionLayer->GetRowCount();
	TileMap collisionTileMap = currentLevel->collisionLayer->GetTileMap();
	TileTypes tileType = TileTypes::NONE;
	Uint32 leftTile = a.x / TILE_SIZE;
	Uint32 rightTile = (a.x + a.w) / TILE_SIZE;

	Uint32 topTile = a.y / TILE_SIZE;
	Uint32 bottomTile = (a.y + a.h) / TILE_SIZE;

	if (leftTile < 0) leftTile = 0;
	if (rightTile > mapColumnCount) rightTile = mapColumnCount;
	if (topTile < 0) topTile = 0;
	if (bottomTile > mapRowCount) bottomTile = mapRowCount;

	Uint32 column = 0;
	Uint32 row = 0;
	for (column = leftTile; column <= rightTile; column++)
	{
		for (row = topTile; row <= bottomTile; row++)
		{
			//if the player is trying to collide outside of the map
			if (row >= collisionTileMap.size()) tileType = TileTypes::SOLID;
			else if (column >= collisionTileMap[row].size()) tileType = TileTypes::SOLID;
			else if (collisionTileMap[row][column] > 0)
			{
				tileType = static_cast<TileTypes>(collisionTileMap[row][column]);
				//Logger::GetInstance()->Log("CollisionHandler.cpp: Currently colliding with type: "+ to_string(tileType) 
				//	+ " at Row: " + to_string(row) + " Column: " + to_string(column));
			}
		}
	}
	Vector2D tilePosition(column, row);
	TileData tileData(tilePosition, tileType);
	return tileData;
}

TileData CollisionHandler::GetMapCollisionDataFromPoint(Vector2D point)
{
	Vector2D pointOnMap;
	TileData tileData;
	GameLevel* currentLevel = GameEngine::GetInstance()->GetCurrentMap();
	TileMap collisionTileMap = currentLevel->collisionLayer->GetTileMap();
	pointOnMap.X = point.X / TILE_SIZE;
	pointOnMap.Y = point.Y / TILE_SIZE;
	tileData.first = pointOnMap;
	if (pointOnMap.Y >= 0 && pointOnMap.Y < collisionTileMap.size())
	{
		if (pointOnMap.X >= 0 && pointOnMap.X < collisionTileMap[pointOnMap.Y].size())
		{
			tileData.second = static_cast<TileTypes>(collisionTileMap[pointOnMap.Y][pointOnMap.X]);
		}
	}
	return tileData;
}
