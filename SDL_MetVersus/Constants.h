#pragma once
#include "SDL.h"

//window constants
constexpr int WINDOW_WIDTH = 600;
constexpr int WINDOW_HEIGHT = 400;

constexpr int MAX_X_VELOCITY = 40;
constexpr int MAX_Y_VELOCITY = 10;

constexpr float X_VELOCITY_DECAY = 0.5;

//===============================================================
//						Tile Constants
//===============================================================
//The tile size that the game will use
constexpr int TILE_SIZE = 16;

enum TileTypes : Uint8
{
	AIR = 0,//Typical air tile where the player can move trough no problem
	SOLID = 1,//Activly stops most objects including the player from going inside it
	LEFT_SLOPE = 2,
	RIGHT_SLOPE = 3,
	CEILING_LEFT_SLOPE = 4,
	CEILING_RIGHT_SLOPE = 5,


	NONE = 0xEF,
	UNKNOWN = 0xFF
};
