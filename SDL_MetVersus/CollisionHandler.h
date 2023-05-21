#pragma once
#include "STL_Components.h"
#include "SDL.h"
#include "TileLayer.h"
#include "Vector2D.h"
#include "BaseEntity.h"
#include "PlayerEntity.h"


//X is the column and Y is the row in the collision layer array 
using TileData = pair<Vector2D, TileTypes>;

//This class handles the collision beteen entities and
class CollisionHandler
{
private:
	CollisionHandler(){}
	static CollisionHandler* instance;
	String statusCol;
	Sint32 HandleSlopeCollision(BaseEntity* entity, TileData slopeTile, bool goingRight = false);
	void WalkDownFloorSlope(BaseEntity* entity);
	const Uint8* GetSlopesHeightArray(TileTypes slopeType);
	Sint32 GetSlopeYDifference(TileData slopeTile, Sint32 x, Sint32 y);
	bool IsTileFloorSlope(TileTypes slopeType);
	TileTypes ParseTileIntoType(Uint32 tileData);
public:
	inline static CollisionHandler* GetInstance() { if (instance == nullptr)instance = new CollisionHandler; return instance; }
	bool CheckCollision(SDL_Rect a, SDL_Rect b);

	TileData MapCollisionFromPoint(Vector2D point);
	/// <summary>
	/// A function that handles player like collisions for entities, which include the player them selves.
	/// The movement of the rigidbody should be applied before the call of this function, but not updated
	/// using the "Update()" function of the rigidbody
	/// </summary>
	/// <param name="entity: ">A pointer to the entity that needs its collision checked (typically the "this" pointer)</param>
	void HandlePlayerCollision(PlayerEntity* player, float timeDelta);
	inline String GetStatusCol() { return statusCol; }
};

constexpr Uint8 LEFT_SLOPE_HEIGHTS[TILE_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
constexpr Uint8 RIGHT_SLOPE_HEIGHTS[TILE_SIZE] = { 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 };
