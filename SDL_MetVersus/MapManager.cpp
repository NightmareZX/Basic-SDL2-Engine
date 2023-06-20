#include "MapManager.h"
#include <filesystem>

using LDProject = const ldtk::Project&;
using LDWorld = const ldtk::World&;
using LDLevel = const ldtk::Level&;
using LDLayer = const ldtk::Layer&;
using LDEntity = const ldtk::Entity&;
using LDTile = const ldtk::Tile&;
using LDIntPoint = const ldtk::IntPoint;
using LDTileSet = const ldtk::Tileset&;
using LDIntRect = const ldtk::IntRect;


bool MapManager::LoadMap(String mapname, MapType mapType)
{
	switch (mapType)
	{
	case TMX:
		return false;//ParseTMX(mapname, "maps/" + mapname + ".tmx");
	case LDTK:
		return false;
	default:
		return false;
	}
}
Room* MapManager::GetMap()
{
	return mLoadedMap;
}
void MapManager::Update(float timeDelta)
{
	if (mLoadedMap == nullptr || mObjectManagerInstance == nullptr) return;
	mLoadedMap->Update(timeDelta);
	mObjectManagerInstance->Update(timeDelta);
	mCameraInstance->Update(mObjectManagerInstance->GetMainPlayer()->GetCentrePoint());
}
void MapManager::Draw()
{
	if (mLoadedMap == nullptr || mObjectManagerInstance == nullptr) return;
	mLoadedMap->DrawBackground();
	mLoadedMap->DrawMiddleground();
	mObjectManagerInstance->Draw();
	mLoadedMap->DrawForeground();
}
void MapManager::Clean()
{
	
}
void MapManager::Dispose()
{

}

bool MapManager::LoadLDTK(String id, String source)
{

	mCurrentMapProject.loadFromFile(source + ".ldtk");

	//const LDWorld& mapWorld = mCurrentMapProject.getWorld();

	return true;
}
void MapManager::DebugLoadLevel(String sourceFile, String levelName)
{
	LoadLDTK("", sourceFile);
	mLoadedMap = ConstructRoom(levelName);
	mCollisionHandlerInstance->mCollisionTileMap = mLoadedMap->mCollisionTileMap;
}
Room* MapManager::ConstructRoom(String roomName)
{
	LDLevel room = mCurrentMapProject.getWorld().getLevel(roomName);

	Uint32 roomWidth = room.size.x / TILE_SIZE;
	Uint32 roomHeight = room.size.y / TILE_SIZE;	

	LDLayer collision = room.getLayer("Collision");

	if (collision.getType() != ldtk::LayerType::IntGrid)
	{
		mLoggerInstance->Log("MapManager.Cpp: Error: Collision layer is not an IntGrid!");
		return nullptr;
	}

	TileMap collisionTileMap(roomWidth, roomHeight, 0);

	for (size_t row = 0; row < roomHeight; row++)
	{
		for (size_t column = 0; column < roomWidth; column++)
		{
			Uint32 gridVal = static_cast<Uint32>(collision.getIntGridVal(column, row).value);
			collisionTileMap[row][column] = (gridVal == UINT32_MAX) ? 0 : gridVal;
		}
	}
	Vector2D playerStartPos(64,64);

	//this actually works lol (Get player start pos entity)
	const auto& entities = room.getLayer("Entities").getEntitiesByName("Player_Start");
	for (const auto& entity : entities)
	{
		const auto enumType = entity.get().getField<ldtk::EnumValue>("Player_Start_Types");
		if (enumType.value().name == "Debug")
		{
			LDIntPoint entPoint = entity.get().getPosition();
			playerStartPos.X = entPoint.x;
			playerStartPos.Y = entPoint.y;
		}
	}
	mObjectManagerInstance->InitialisePlayer(playerStartPos.X, playerStartPos.Y);

	Room* currentRoom = new Room(mCameraInstance, mRendererManagerInstance);
	currentRoom->mCollisionTileMap = collisionTileMap;
	currentRoom->mWidth = roomWidth;
	currentRoom->mHeight = roomHeight;
	currentRoom->AddLayer(ParseTileLayer("Foreground", room, roomWidth, roomHeight), FOREGROUND);
	//currentRoom->AddLayer(ParseTileLayer("Middleground", room, roomWidth, roomHeight), MIDDLEGROUND);
	return currentRoom;
}
TileLayer* MapManager::ParseTileLayer(String layerName, LDLevel level, Sint32 width, Sint32 height)
{
	LDLayer currentLayer = level.getLayer(layerName);
	LDTileSet tileSet = currentLayer.getTileset();

	TileMap tilemap(width, height, 0);

	if (mLoadedTileSets.find(tileSet.name) == mLoadedTileSets.end())
	{
		TileSetInfo tileset;
		//std::filesystem::path realPath(tileSet.path);
		//realPath = std::filesystem::absolute(realPath);
		tileset.tileSetPath = tileSet.path;
		tileset.tileSetPath.erase(0, 3);//TEMP
		mRendererManagerInstance->RegisterTexture(tileSet.name, tileset.tileSetPath);
		mLoadedTileSets.emplace(tileSet.name, tileset);
	}
	
	auto& tileSrcMap = mLoadedTileSets[tileSet.name].tileTextureSource;

	for (LDTile tile : currentLayer.allTiles())
	{
		LDIntPoint tilePos = tile.getGridPosition();

		Uint32 adjustedTileId = tile.tileId + 1;

		if (tileSrcMap.find(adjustedTileId) == tileSrcMap.end())
		{
			LDIntRect srcRect = tile.getTextureRect();

			tileSrcMap.emplace(static_cast<Uint32>(adjustedTileId),Vector2D(srcRect.x, srcRect.y));
		}
		Uint32 tileGameID = adjustedTileId;
		if (tile.flipX) tileGameID |= TileInformation::HORIZONTAL_FLIP;
		if (tile.flipY) tileGameID |= TileInformation::VERTICAL_FLIP;
		tilemap[tilePos.y][tilePos.x] = tileGameID;
	}

	return new TileLayer(height,width, tilemap, tileSet.name, mCameraInstance, mRendererManagerInstance, &tileSrcMap);
}

//bool MapManager::ParseTMX(String id, String source)
//{
//	TiXmlDocument xml;
//	xml.LoadFile(source);
//
//	if (xml.Error())
//	{
//		Logger::GetInstance()->Log("MapParser.cpp: Map Load Error: Bad file, Source: " + source + ", ID: " + id);
//		return false;
//	}
//	
//	TiXmlElement* root = xml.RootElement();
//	Sint32 rowCount;//map height
//	Sint32 columnCount;//map width
//	Sint32 tileWidthCheck;
//	Sint32 tileHeightCheck;
//
//	root->Attribute("width", &columnCount);
//	root->Attribute("height", &rowCount);
//	root->Attribute("tilewidth", &tileWidthCheck);
//	root->Attribute("tileheight", &tileHeightCheck);
//	if (tileWidthCheck != 16 || tileHeightCheck != 16)//check if the tiles are actually 16x16
//	{
//		Logger::GetInstance()->Log("MapParser.cpp: Map Load Error: Invalid Tile Size, Source: " + source + ", ID: " + id);
//		return false;
//	}
//	TileSetList tilesets;
//	for (TiXmlElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
//	{
//		//when we found the tile set, add it to the tile set list
//		if (element->Value() == String("tileset"))
//		{
//			TileSet* tileset = ParseTMXTileset(element);
//			if (tileset != nullptr)
//			{
//				tilesets.push_back(*tileset);
//			}
//			delete tileset;
//		}
//	}
//	Room* map = new Room();
//	map->SetLevelHeight(rowCount);
//	map->SetLevelWidth(columnCount);
//	for (TiXmlElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
//	{
//		//when we found the tile set, add it to the tile set list
//		if (element->Value() == String("layer"))
//		{
//			TileLayer* layer = ParseTMXTileLayer(element, tilesets, rowCount, columnCount);
//			map->AddLayer(layer, layer->GetLayerType());
//		}
//	}
//	mLoadedMap = map;
//	return true;
//}
//TileSet* MapManager::ParseTMXTileset(TiXmlElement* xmlTileset)
//{
//	TileSet tileset;
//	tileset.name = xmlTileset->Attribute("name");
//	xmlTileset->Attribute("firstgid",&tileset.firstID);
//
//	if (xmlTileset->Attribute("tilecount") == nullptr)
//	{
//		Logger::GetInstance()->Log("MapParser.cpp: Tileset Load Error: Tileset is not embeded into level, Tileset Name: " + tileset.name);
//		return nullptr;
//	}
//
//	xmlTileset->Attribute("tilecount", &tileset.tileCount);
//	tileset.lastID = (tileset.firstID + tileset.tileCount) - 1;
//
//	xmlTileset->Attribute("columns", &tileset.columnCount);
//	tileset.rowCount = tileset.tileCount / tileset.columnCount;
//	tileset.tileSize = TILE_SIZE;
//
//	TiXmlElement* image = xmlTileset->FirstChildElement();
//	tileset.source = image->Attribute("source");
//
//	if (tileset.source.find('/') != String::npos)
//	{
//		size_t lastSlash = tileset.source.find_last_of('/');
//		if (lastSlash != String::npos)
//			tileset.source = tileset.source.substr(lastSlash + 1);
//	}
//	//if (tileset.source.find(".png") != String::npos)
//	//{
//	//	size_t lastindex = tileset.source.find_last_of(".png");
//	//	tileset.source = tileset.source.substr(0, lastindex);
//	//}
//	return new TileSet(tileset);
//}
//TileLayer* MapManager::ParseTMXTileLayer(TiXmlElement* xmlLayer, TileSetList tilesets,Uint32 rowCount, Uint32 columnCount, Uint32 tilesize)
//{
//	ValidLayers layerType = ValidLayers::NONEVALID;
//	TiXmlElement* data = nullptr;
//	String layerName = "";
//	const char* value = xmlLayer->Attribute("name");
//
//	if (value) layerName = value;
//	for (size_t i = 0; i < layerName.size(); i++) layerName[i] = tolower(layerName[i]);
//
//	if (layerName == "collision")
//	{
//		layerType = ValidLayers::COLLISION;
//		//The collision layer should not draw, so just clear its contents
//		tilesets.clear();
//	}
//	else if (layerName == "foreground") layerType = ValidLayers::FOREGROUND;
//	else if (layerName == "middleground") layerType = ValidLayers::MIDDLEGROUND;
//	else if (layerName == "background") layerType = ValidLayers::BACKGROUND;
//
//	if (layerType == ValidLayers::NONEVALID)
//	{
//		Logger::GetInstance()->Log("MapParser.cpp: Tile Layer Parse Error: Invalid Layer Name! The Layer " + layerName + " is not valid!");
//		return nullptr;
//	}	
//	//Search for the section "data" which holds a 2d array of all the positions of the tiles
//	for (TiXmlElement* element = xmlLayer->FirstChildElement();element != nullptr; element = element->NextSiblingElement())
//	{
//		//if we found the 2d array we can get to using it
//		if (element->Value() == String("data"))
//		{
//			data = element;
//			break;
//		}
//	}
//	if (data == nullptr)
//	{
//		Logger::GetInstance()->Log("MapParser.cpp: Tile Layer Parse Error: Can't find Matrix!");
//		return nullptr;
//	}
//		
//	//Get the matrix that holds the positions of the tiles
//	String matrix(data->GetText());
//	InputStringStream iss(matrix);
//	String id;
//
//	TileMap tilemap(rowCount, vector<Uint32>(columnCount, 0));
//
//	for (size_t row = 0; row < rowCount; row++)
//	{
//		for (size_t column = 0; column < columnCount; column++)
//		{
//			getline(iss, id, ',');
//			StringStream convertor(id);
//			//Uint32 tileData = 0;
//			convertor >> tilemap[row][column];
//
//			//if the stream is bad for whatever reason then just end the loop
//			if (iss.eof())
//			{
//				break;
//			}
//			if (iss.fail())
//			{
//				Logger::GetInstance()->Log
//				("MapParser.cpp: Tile Layer Parse Error: Matrix Error at Row : " 
//					+ to_string(row) + ", Column : " + to_string(column) + " For layer: "+ layerName);
//				break;
//			}
//				
//		}
//	}
//	return new TileLayer(rowCount, columnCount, tilemap, tilesets, layerType);
//}