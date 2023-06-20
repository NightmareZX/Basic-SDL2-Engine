#include "CollisionHandler.h"
#include "GameEngine.h"

#define ERROR_NUM 69


bool CollisionHandler::CheckCollision(SDL_Rect a, SDL_Rect b)
{
	bool x_overlap = (a.x < b.x + b.w) && (a.x + a.w > b.x);
	bool y_overlap = (a.y < b.y + b.h) && (a.y + a.h > b.y);
	return (x_overlap && y_overlap);
}

TileData CollisionHandler::MapCollisionFromPoint(Vector2D point)
{
	Vector2D pointOnMap;
	TileData tileData;
	pointOnMap.X = point.X / TILE_SIZE;
	pointOnMap.Y = point.Y / TILE_SIZE;
	tileData.first = pointOnMap;
	if ((pointOnMap.Y >= mCollisionTileMap.GetHeight() || pointOnMap.Y < 0) ||
		(pointOnMap.X >= mCollisionTileMap.GetWidth() || pointOnMap.X < 0))
	{
		tileData.second = TileTypes::OUT_OF_BOUNDS;
		return tileData;
	}
	if (pointOnMap.Y >= 0 && pointOnMap.Y < mCollisionTileMap.GetHeight())
	{
		if (pointOnMap.X >= 0 && pointOnMap.X < mCollisionTileMap.GetWidth())
		{
			tileData.second = static_cast<TileTypes>(mCollisionTileMap[pointOnMap.Y][pointOnMap.X]);
		}
	}
	return tileData;
}
bool CollisionHandler::IsTileFloorSlope(TileTypes slopeType)
{
	switch (slopeType)
	{
	case LEFT_SLOPE:
		return true;
	case RIGHT_SLOPE:
		return true;
	default:
		return false;
	}
}
const Uint8* CollisionHandler::GetSlopesHeightArray(TileTypes slopeType)
{
	switch (slopeType)
	{
	case LEFT_SLOPE:
		return LEFT_SLOPE_HEIGHTS;
	case RIGHT_SLOPE:
		return RIGHT_SLOPE_HEIGHTS;
	default:
		return nullptr;
	}
}
TileTypes CollisionHandler::ParseTileIntoType(Uint32 tileData)
{
	if (tileData == 0) return TileTypes::AIR;
	if (tileData & TileInformation::SOLID_TILE) return TileTypes::SOLID;
	if (tileData & TileInformation::SLOPE_TILE)
	{
		if (tileData & TileInformation::HORIZONTAL_FLIP)
		{
			if (tileData & TileInformation::VERTICAL_FLIP)
				return TileTypes::CEILING_RIGHT_SLOPE;
			else return TileTypes::RIGHT_SLOPE;
		}
		if (tileData& TileInformation::VERTICAL_FLIP)
			return TileTypes::CEILING_LEFT_SLOPE;
		return TileTypes::LEFT_SLOPE;
	}
	return TileTypes::AIR;
}
Sint32 CollisionHandler::GetSlopeYDifference(TileData slopeTile, Sint32 x, Sint32 y)
{
	const Uint8* slopeHeightArray = GetSlopesHeightArray(slopeTile.second);
	if (slopeHeightArray == nullptr) return ERROR_NUM;//this macro will return a number that will denote that the operation was invalid
	Sint32 slopeY = slopeHeightArray[x % TILE_SIZE];
	Sint32 yMod = y % TILE_SIZE;
	return yMod - slopeY;
}
Sint32 CollisionHandler::HandleSlopeCollision(BaseEntity* entity, TileData slopeTile, bool goingRight)
{
	if (!IsTileFloorSlope(slopeTile.second)) return 0;
	RigidBody* rigidBodyPtr = entity->GetRigidBody();
	Collider* colliderPtr = entity->GetCollider();
	Transform* transformPtr = entity->GetTransform();
	Sint32 x = colliderPtr->GetMiddlePointHorizontal();
	//if (goingRight && (colliderPtr->GetBoxWidth() % 2) == 0) x++;
	Sint32 y = colliderPtr->GetBboxBottom() - 1;

	Sint32 yDifference = GetSlopeYDifference(slopeTile,x,y);
	if (yDifference >= 0 && yDifference != ERROR_NUM)//if we are bellow the height at the specified index then put our y position above that height
	{
		transformPtr->Y -= (yDifference + 1);
		transformPtr->RoundDownY();
		rigidBodyPtr->ResetVelocityY();
		yDifference = -1;
		entity->SetGroundedStatus(true);
	}
	//if we are already properly positioned on the slope, then just set the grounded status again
	else if (yDifference == -1 && yDifference != ERROR_NUM) entity->SetGroundedStatus(true);


	return yDifference;
}
void CollisionHandler::WalkDownFloorSlope(BaseEntity* entity)
{
	RigidBody* rigidBodyPtr = entity->GetRigidBody();
	Collider* colliderPtr = entity->GetCollider();
	Transform* transformPtr = entity->GetTransform();
	Sint32 x = colliderPtr->GetMiddlePointHorizontal();
	Sint32 y = colliderPtr->GetBboxBottom() - 1;
	bool checkOther = true;

	TileData currentlyInteractingTile = MapCollisionFromPoint(Vector2D(x,y));

	//If we are walking down and currently inside a slope, then just walk down to that slopes correct height
	if (IsTileFloorSlope(currentlyInteractingTile.second))
	{
		Sint32 yDiff = GetSlopeYDifference(currentlyInteractingTile, x, y);
		if (yDiff < -1 && yDiff > -4)
		{
			transformPtr->Y += abs(yDiff) - 1;
			transformPtr->RoundDownY();
			rigidBodyPtr->ResetVelocityY();
			entity->SetGroundedStatus(true);
		}
		else if (yDiff == -1) checkOther = false;
	}
	//If we are not currently in a slope then try to check bellow the current tile we are standing in 
	else if (checkOther)
	{
		//if we are at the base of a tile
		Sint32 baseTileCheck = (colliderPtr->GetBboxBottom() - 1) % TILE_SIZE;
		if (baseTileCheck == TILE_SIZE - 1)
		{
			TileData tileNextAndBellow = MapCollisionFromPoint(Vector2D(x, colliderPtr->GetBboxBottom() + 1));
			if (IsTileFloorSlope(tileNextAndBellow.second))
			{
				Sint32 yDifference = GetSlopeYDifference(tileNextAndBellow, x, colliderPtr->GetBboxBottom() - 1);
				if (yDifference != ERROR_NUM)
				{
					transformPtr->Y += abs(yDifference);
					transformPtr->RoundDownY();
					entity->SetGroundedStatus(true);
				}
			}
		}
	}
}

void CollisionHandler::HandlePlayerCollision(PlayerEntity* player, float timeDelta)
{
	RigidBody* rigidBodyPtr = player->GetRigidBody();
	Collider* colliderPtr = player->GetCollider();
	Transform* transformPtr = player->GetTransform();
	TileData interactedTile;
	bool foundSolid = false;
	//store if we were grounded the previous frame
	//interactedTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetMiddlePointHorizontal(), colliderPtr->GetBboxBottom() + 1));
	//bool wasGroundedPreviousFrame = (GetSlopeYDifference(interactedTile, colliderPtr->GetMiddlePointHorizontal(), colliderPtr->GetBboxBottom() + 1) >= 0)
	//	|| interactedTile.second == TileTypes::SOLID;

	//figure out how to walk down slope
	bool wasGroundedPreviousFrame = player->GetGroundedStatus();

	rigidBodyPtr->UpdateX(timeDelta);
	transformPtr->X += rigidBodyPtr->GetPosition().X;
	colliderPtr->SetPosition(static_cast<Sint32>(transformPtr->X), static_cast<Sint32>(transformPtr->Y));
	//Check for horizontal collisions first
	if (rigidBodyPtr->GetVelocity().X > 0.0f)//Right collisions
	{
		//Check if we are standing on a slope
		TileData interactedSlopeTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetMiddlePointHorizontal(), colliderPtr->GetBboxBottom() - 1));
		//Get the tile at our head height
		interactedTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxRight() - 1,colliderPtr->GetBboxTop()));//Check the top tile
		if (interactedTile.second == TileTypes::SOLID)foundSolid = true;
		if (!foundSolid)
		{
			interactedTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxRight() - 1, colliderPtr->GetMiddlePointVerticalTop()));//
			if (interactedTile.second == TileTypes::SOLID) foundSolid = true;
		}
		if (!foundSolid)
		{
			interactedTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxRight() - 1, colliderPtr->GetMiddlePointVertical()));//Check the middle tile
			if (interactedTile.second == TileTypes::SOLID) foundSolid = true;
		}
		if (!foundSolid)
		{
			interactedTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxRight() - 1, colliderPtr->GetMiddlePointVerticalBottom()));//
			if (interactedTile.second == TileTypes::SOLID) foundSolid = true;
		}
		if (!foundSolid)//&& !IsTileFloorSlope(interactedSlopeTile.second)
		{
			interactedTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxRight() - 1, colliderPtr->GetBboxBottom() - 1));//Check the bottom tile
			if (interactedTile.second == TileTypes::SOLID) foundSolid = true;
		}
		if (foundSolid)
		{
			transformPtr->X = static_cast<float>(colliderPtr->GetBboxRight() & ~(TILE_SIZE - 1));
			transformPtr->X -= static_cast<float>(colliderPtr->GetBoxWidth());
			transformPtr->RoundDownX();
			colliderPtr->SetPosition(static_cast<Sint32>(transformPtr->X), static_cast<Sint32>(transformPtr->Y));
			rigidBodyPtr->ResetVelocityX();
		}
	}
	else if (rigidBodyPtr->GetVelocity().X < 0.0f) //Left Collisions
	{
		//Check if we are standing on a slope
		TileData interactedSlopeTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetMiddlePointHorizontal(), colliderPtr->GetBboxBottom() - 1));
		//Get the tile at our head height
		interactedTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxLeft(), colliderPtr->GetBboxTop()));//Check the top tile
		if (interactedTile.second == TileTypes::SOLID)foundSolid = true;
		if (!foundSolid)
		{
			interactedTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxLeft(), colliderPtr->GetMiddlePointVerticalTop()));//
			if (interactedTile.second == TileTypes::SOLID) foundSolid = true;
		}
		if (!foundSolid)
		{
			interactedTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxLeft(), colliderPtr->GetMiddlePointVertical()));//Check the middle tile
			if (interactedTile.second == TileTypes::SOLID) foundSolid = true;
		}
		if (!foundSolid)
		{
			interactedTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxLeft(), colliderPtr->GetMiddlePointVerticalBottom()));//
			if (interactedTile.second == TileTypes::SOLID) foundSolid = true;
		}
		if (!foundSolid)//&& !IsTileFloorSlope(interactedSlopeTile.second)
		{
			interactedTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxLeft(), colliderPtr->GetBboxBottom() - 1));//Check the bottom tile
			if (interactedTile.second == TileTypes::SOLID) foundSolid = true;
		}
		if (foundSolid)
		{
			transformPtr->X = static_cast<float>(colliderPtr->GetBboxLeft() & ~(TILE_SIZE - 1));
			transformPtr->X += static_cast<float>(TILE_SIZE);
			transformPtr->RoundDownX();
			colliderPtr->SetPosition(static_cast<Sint32>(transformPtr->X), static_cast<Sint32>(transformPtr->Y));
			rigidBodyPtr->ResetVelocityX();
		}
	}

	foundSolid = false;
	rigidBodyPtr->UpdateY(!player->GetGroundedStatus(),timeDelta);
	transformPtr->Y += rigidBodyPtr->GetPosition().Y;
	colliderPtr->SetPosition(static_cast<Sint32>(transformPtr->X), static_cast<Sint32>(transformPtr->Y));
	Sint32 slopeYdist = 0;
	bool slopeBellow = false;
	//Check for vertical Collision
	if (rigidBodyPtr->GetVelocity().Y > 0.0f)//Downwards collision
	{
		TileData interactedSlopeTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetMiddlePointHorizontal(), colliderPtr->GetBboxBottom() - 1));
		if (interactedSlopeTile.second > 1) 
			slopeYdist = HandleSlopeCollision(player, interactedSlopeTile);
		if (!IsTileFloorSlope(interactedSlopeTile.second))
		{
			interactedTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxLeft(), colliderPtr->GetBboxBottom() - 1));
			if (interactedTile.second == TileTypes::SOLID || interactedTile.second == TileTypes::VERTICAL_SOLID)foundSolid = true;
		}
		if (!foundSolid && !IsTileFloorSlope(interactedSlopeTile.second))
		{
			interactedTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxRight() - 1, colliderPtr->GetBboxBottom() - 1));
			if (interactedTile.second == TileTypes::SOLID || interactedTile.second == TileTypes::VERTICAL_SOLID) foundSolid = true;
		}
		if (foundSolid)
		{
			transformPtr->Y = static_cast<float>(colliderPtr->GetBboxBottom() & ~(TILE_SIZE - 1));
			transformPtr->Y -= static_cast<float>(colliderPtr->GetBoxHeight());
			transformPtr->RoundDownY();
			colliderPtr->SetPosition(static_cast<Sint32>(transformPtr->X), static_cast<Sint32>(transformPtr->Y));
			rigidBodyPtr->ResetVelocityY();
			player->SetGroundedStatus(true);
		}
		//debug info
		StringStream ss;
		ss << "TX: " << interactedSlopeTile.first.X <<
			"  TY: " << interactedSlopeTile.first.Y <<
			" TT: " << static_cast<Uint16>(interactedSlopeTile.second);
		statusCol = ss.str();

		interactedSlopeTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetMiddlePointHorizontal(), colliderPtr->GetBboxBottom() + 1));
		slopeBellow = (IsTileFloorSlope(interactedSlopeTile.second));
	}
	else if (rigidBodyPtr->GetVelocity().Y < 0.0f)//Upwards Collisions
	{
		interactedTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxLeft(), colliderPtr->GetBboxTop()));
		if (interactedTile.second == TileTypes::SOLID)foundSolid = true;
		if (!foundSolid)
		{
			interactedTile = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxRight() - 1, colliderPtr->GetBboxTop()));
			if (interactedTile.second == TileTypes::SOLID) foundSolid = true;
		}
		if (foundSolid)
		{
			transformPtr->Y = static_cast<float>(colliderPtr->GetBboxTop() & ~(TILE_SIZE - 1));
			transformPtr->Y += static_cast<float>(TILE_SIZE);
			transformPtr->RoundDownY();
			colliderPtr->SetPosition(static_cast<Sint32>(transformPtr->X), static_cast<Sint32>(transformPtr->Y));
			rigidBodyPtr->ResetVelocityY();
			player->SetCeilingHitStatus(true);
		}
	}
	else
	{
		bool checkSolidCollision = true;
		TileData bellowTileSlopeCheck = MapCollisionFromPoint(Vector2D(colliderPtr->GetMiddlePointHorizontal(), colliderPtr->GetBboxBottom() - 1));
		if (IsTileFloorSlope(bellowTileSlopeCheck.second))
		{
			//bool groundedStatusBefore = entity->GetGroundedStatus();
			Sint32 yDiff = HandleSlopeCollision(player, bellowTileSlopeCheck);
			if (yDiff < -1) player->SetGroundedStatus(false);
			if (player->GetGroundedStatus()) checkSolidCollision = false;
		}
		
		if (checkSolidCollision)
		{
			TileData bellowTileCheck = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxLeft(), colliderPtr->GetBboxBottom()));
			if (bellowTileCheck.second == TileTypes::SOLID || bellowTileCheck.second == TileTypes::VERTICAL_SOLID)
			{
				player->SetGroundedStatus(true);
				//do another check to make sure we arent slightly in the ground
				bellowTileCheck = MapCollisionFromPoint(Vector2D(colliderPtr->GetMiddlePointHorizontal(), colliderPtr->GetBboxBottom()));
				if (bellowTileCheck.second == TileTypes::VERTICAL_SOLID)
				{
					transformPtr->Y = static_cast<float>(colliderPtr->GetBboxBottom() & ~(TILE_SIZE - 1));
					transformPtr->Y -= static_cast<float>(colliderPtr->GetBoxHeight());
					transformPtr->RoundDownY();
					colliderPtr->SetPosition(static_cast<Sint32>(transformPtr->X), static_cast<Sint32>(transformPtr->Y));
				}
			}
			else
			{
				bellowTileCheck = MapCollisionFromPoint(Vector2D(colliderPtr->GetBboxRight() - 1, colliderPtr->GetBboxBottom()));
				if (bellowTileCheck.second == TileTypes::SOLID || bellowTileCheck.second == TileTypes::VERTICAL_SOLID)
					player->SetGroundedStatus(true);
				else
					player->SetGroundedStatus(false);
			}
		}
	}
	//fix this later
	if (wasGroundedPreviousFrame && !player->GetJumpingStatus() && slopeBellow);
	//	WalkDownFloorSlope(player);
}


