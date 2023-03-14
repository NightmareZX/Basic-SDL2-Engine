#include "MapParser.h"


MapParser* MapParser::instance = nullptr;

bool MapParser::LoadMap(String mapname)
{
	return Parse(mapname, "maps/" + mapname + ".tmx");
}
GameLevel* MapParser::GetMap()
{
	return loadedMap;
}

void MapParser::Clean()
{

}
void MapParser::Dispose()
{

}
bool MapParser::Parse(String id, String source)
{
	TiXmlDocument xml;
	xml.LoadFile(source);

	if (xml.Error())
	{
		Logger::GetInstance()->Log("MapParser.cpp: Map Load Error: Bad file, Source: " + source + ", ID: " + id);
		return false;
	}
	
	TiXmlElement* root = xml.RootElement();
	Sint32 rowCount;//map height
	Sint32 columnCount;//map width
	Sint32 tileWidthCheck;
	Sint32 tileHeightCheck;

	root->Attribute("width", &columnCount);
	root->Attribute("height", &rowCount);
	root->Attribute("tilewidth", &tileWidthCheck);
	root->Attribute("tileheight", &tileHeightCheck);
	if (tileWidthCheck != 16 || tileHeightCheck != 16)//check if the tiles are actually 16x16
	{
		Logger::GetInstance()->Log("MapParser.cpp: Map Load Error: Invalid Tile Size, Source: " + source + ", ID: " + id);
		return false;
	}
	TileSetList tilesets;
	for (TiXmlElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		//when we found the tile set, add it to the tile set list
		if (element->Value() == String("tileset"))
		{
			TileSet* tileset = ParseTileset(element);
			if (tileset != nullptr)
			{
				tilesets.push_back(*tileset);
			}
			delete tileset;
		}
	}
	GameLevel* map = new GameLevel();
	for (TiXmlElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		//when we found the tile set, add it to the tile set list
		if (element->Value() == String("layer"))
		{
			TileLayer* layer = ParseTileLayer(element, tilesets, rowCount, columnCount);
			if (layer->GetLayerType() == ValidLayers::COLLISION)
				map->SetCollisionLayer(layer);
			else
				map->AddLayer(layer);
		}
	}
	loadedMap = map;
	return true;
}
TileSet* MapParser::ParseTileset(TiXmlElement* xmlTileset)
{
	TileSet tileset;
	tileset.name = xmlTileset->Attribute("name");
	xmlTileset->Attribute("firstgid",&tileset.firstID);

	if (xmlTileset->Attribute("tilecount") == nullptr)
	{
		Logger::GetInstance()->Log("MapParser.cpp: Tileset Load Error: Tileset is not embeded into level, Tileset Name: " + tileset.name);
		return nullptr;
	}

	xmlTileset->Attribute("tilecount", &tileset.tileCount);
	tileset.lastID = (tileset.firstID + tileset.tileCount) - 1;

	xmlTileset->Attribute("columns", &tileset.columnCount);
	tileset.rowCount = tileset.tileCount / tileset.columnCount;
	tileset.tileSize = TILE_SIZE;

	TiXmlElement* image = xmlTileset->FirstChildElement();
	tileset.source = image->Attribute("source");

	if (tileset.source.find('/') != String::npos)
	{
		size_t lastSlash = tileset.source.find_last_of('/');
		if (lastSlash != String::npos)
			tileset.source = tileset.source.substr(lastSlash + 1);
	}
	//if (tileset.source.find(".png") != String::npos)
	//{
	//	size_t lastindex = tileset.source.find_last_of(".png");
	//	tileset.source = tileset.source.substr(0, lastindex);
	//}
	return new TileSet(tileset);
}
TileLayer* MapParser::ParseTileLayer(TiXmlElement* xmlLayer, TileSetList tilesets,Uint32 rowCount, Uint32 columnCount, Uint32 tilesize)
{
	ValidLayers layerType = ValidLayers::NONEVALID;
	TiXmlElement* data = nullptr;
	String layerName = "";
	const char* value = xmlLayer->Attribute("name");

	if (value) layerName = value;
	for (size_t i = 0; i < layerName.size(); i++) layerName[i] = tolower(layerName[i]);

	if (layerName == "collision")
	{
		layerType = ValidLayers::COLLISION;
		//The collision layer should not draw, so just clear its contents
		tilesets.clear();
	}
	else if (layerName == "foreground") layerType = ValidLayers::FOREGROUND;

	if (layerType == ValidLayers::NONEVALID)
	{
		Logger::GetInstance()->Log("MapParser.cpp: Tile Layer Parse Error: Invalid Layer Name! The Layer " + layerName + " is not valid!");
		return nullptr;
	}	
	//Search for the section "data" which holds a 2d array of all the positions of the tiles
	for (TiXmlElement* element = xmlLayer->FirstChildElement();element != nullptr; element = element->NextSiblingElement())
	{
		//if we found the 2d array we can get to using it
		if (element->Value() == String("data"))
		{
			data = element;
			break;
		}
	}
	if (data == nullptr)
	{
		Logger::GetInstance()->Log("MapParser.cpp: Tile Layer Parse Error: Can't find Matrix!");
		return nullptr;
	}
		
	//Get the matrix that holds the positions of the tiles
	String matrix(data->GetText());
	InputStringStream iss(matrix);
	String id;

	TileMap tilemap(rowCount, vector<Uint32>(columnCount, 0));

	for (size_t row = 0; row < rowCount; row++)
	{
		for (size_t column = 0; column < columnCount; column++)
		{
			getline(iss, id, ',');
			StringStream convertor(id);
			convertor >> tilemap[row][column];

			//if the stream is bad for whatever reason then just end the loop
			if (iss.eof())
			{
				break;
			}
			if (iss.fail())
			{
				Logger::GetInstance()->Log
				("MapParser.cpp: Tile Layer Parse Error: Matrix Error at Row : " 
					+ to_string(row) + ", Column : " + to_string(column) + " For layer: "+ layerName);
				break;
			}
				
		}
	}
	return new TileLayer(rowCount, columnCount, tilemap, tilesets, layerType);
}
CollisionTile*** MapParser::ParseCollisonMap(TiXmlElement* xmlTileset)
{
	return nullptr;
}