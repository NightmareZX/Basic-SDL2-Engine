#pragma once
#include "Containers.h"
#include "TinyXML/tinyxml.h"
#include "Room.h"
#include "Logger.h"
#include "LDtkLoader/Project.hpp"
#include "ObjectManager.h"

/// <summary>
/// This enumerator holds information about a tile in 32 bits, which holds fliping information as well as tile type.
/// </summary>
enum TileInformation : Uint32
{
	SOLID_TILE = 0x00000001,
	SLOPE_TILE = 0x00000002,

	HORIZONTAL_FLIP = 0x80000000,
	VERTICAL_FLIP = 0x40000000,
	DIAGONAL_FLIP = 0x20000000
};

enum MapType : Uint8
{
	TMX = 1,
	LDTK = 2
};

class MapManager final
{
private:
	Logger* mLoggerInstance;
	RenderManager* mRendererManagerInstance;
	ObjectManager* mObjectManagerInstance;
	Room* mLoadedMap;
	ldtk::Project mCurrentMapProject;

	map<String, TileSetInfo> mLoadedTileSets;

	Camera* mCameraInstance;
	CollisionHandler* mCollisionHandlerInstance;
public:
	MapManager(Logger* logger, RenderManager* rendManager, ObjectManager* objManager, CollisionHandler* collHandler, Camera* camera): 
		mLoadedMap(nullptr), 
		mLoggerInstance(logger), mObjectManagerInstance(objManager), mRendererManagerInstance(rendManager), mCollisionHandlerInstance(collHandler),
		mCameraInstance(camera){}
	bool LoadLDTK(String id, String source);

	Room* ConstructRoom(String roomName);
	TileLayer* ParseTileLayer(String layerName, const ldtk::Level& level, Sint32 width, Sint32 height);


	//bool ParseTMX(String id, String source);
	//TileSet* ParseTMXTileset(TiXmlElement* xmlTileset);
	//TileLayer* ParseTMXTileLayer(TiXmlElement* xmlLayer, TileSetList tilesets, Uint32 rowCount, Uint32 columnCount, Uint32 tilesize = TILE_SIZE);

	Room* GetMap();

	void Update(float timeDelta);
	void Draw();

	void DebugLoadLevel(String sourceFile, String levelName);
	bool LoadMap(String mapname, MapType mapType = LDTK);
	void Clean();
	void Dispose();
};

