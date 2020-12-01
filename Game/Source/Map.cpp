#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "ModuleCollisions.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// L06: TODO 7: Ask for the value of a custom property
int Properties::GetProperty(const char* value, int defaultValue) const
{
	//...
	int result = defaultValue;
	SString targetName(value);
	ListItem<Property*> *propList = list.start;
	while (propList != NULL)
	{
		SString name(propList->data->name.GetString());
		if (name == targetName)
		{
			result = propList->data->value;
			break;
		}
		propList = propList->next;
	}

	return result;
}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());

    return ret;
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
	//MapLayer* layer = data.layers.start->data;
	ListItem<MapLayer*>* layer;
	layer = data.layers.start;
	// L06: TODO 4: Make sure we draw all the layers and not just the first one
	TileSet* tileset = NULL;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Nodraw") == 0) // Layer draw property is false
		{
			layer = layer->next;
			continue;
		}

		// Draw function
		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tileId = layer->data->Get(x, y);
				if (tileId > 0)
				{
					tileset = GetTilesetFromTileId(tileId);
					SDL_Rect n = tileset->GetTileRect(tileId);
					iPoint pos = MapToWorld(x, y);
					app->render->DrawTexture(tileset->texture, pos.x, pos.y, &n);
				}
			}
		}
		layer = layer->next;
	}

}

// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	// L05: DONE 1: Add isometric map to world coordinates
	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tileWidth / 2);
		ret.y = (x + y) * (data.tileHeight / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

// L05: TODO 2: Add orthographic world to map coordinates
iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	// L05: DONE 3: Add the case for isometric maps to WorldToMap
	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float halfWidth = data.tileWidth * 0.5f;
		float halfHeight = data.tileHeight * 0.5f;
		ret.x = int((x / halfWidth + y / halfHeight) / 2);
		ret.y = int((y / halfHeight - (x / halfWidth)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

// L06: TODO 3: Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	//...

	while (item != NULL)
	{
		//LOG("%u", id);
		if (item->next == nullptr)
		{
			break;
		}

		if (id < item->next->data->firstgid)
		{
			return set = item->data;
		}
		
		item = item->next;
	}

	return set = item->data;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	// L04: DONE 7: Get relative Tile rectangle
	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));
	
	return rect;
}

int TileSet::GetTileRelativeId(int id)const
{
	return (id - firstgid);
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
    // Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.Clear();

	// L04: DONE 2: clean up all layer data
	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.Clear();

	// Clean up the pugui tree
	mapFile.reset();


	ListItem<Collider*>* item3;
	item3 = checkpointsList.start;

	while (item3 != NULL)
	{
		RELEASE(item3->data);
		item3 = item3->next;
	}
	checkpointsList.Clear();

    return true;
}

// Load new map
bool Map::Load(const char* filename)
{
    bool ret = true;
    SString tmp("%s%s", folder.GetString(), filename);

    pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
        ret = false;
    }

	// Load general info
    if(ret == true)
    {
        // L03: DONE 3: Create and call a private function to load and fill all your map data
		ret = LoadMap();
	}

    // L03: DONE 4: Create and call a private function to load a tileset
    // remember to support more any number of tilesets!
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);

		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.Add(set);
	}

	// L04: DONE 4: Iterate all layers and load each of them
	// Load layer info
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		// TODO 6: Add calls to load properties
		LoadProperties(layer, lay->properties);

		if (ret == true)
			data.layers.Add(lay);
	}
    
    if(ret == true)
    {
        // L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything
		// L04: TODO 4: LOG the info for each loaded layer
		LogInfo();
    }

    mapLoaded = ret;

    return ret;
}

// L03: TODO: Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		// L03: TODO: Load map general properties
		LOG("Filling Map info");

		SString strType(map.attribute("orientation").as_string());
		data.type = StrToMapType(strType);
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		SString color(map.attribute("backgroundcolor").as_string());
		color.Trim();
		sscanf_s(color.GetString(), "%02x%02x%02x", (uint)&data.backgroundColor.r, (uint)&data.backgroundColor.g, (uint)&data.backgroundColor.b);
		data.backgroundColor.a = 255;
	}

	return ret;
}

// L03: TODO: Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tilesetNode, TileSet* set)
{
	bool ret = true;
	
	// L03: TODO: Load Tileset attributes
	LOG("Filling TilesetDetails");
	set->firstgid = tilesetNode.attribute("firstgid").as_int();
	set->name = tilesetNode.attribute("name").as_string();
	set->tileWidth = tilesetNode.attribute("tilewidth").as_int();
	set->tileHeight = tilesetNode.attribute("tileheight").as_int();
	set->spacing = tilesetNode.attribute("spacing").as_int();
	set->margin = tilesetNode.attribute("margin").as_int();

	set->offsetX = tilesetNode.child("tileoffset").attribute("x").as_int();
	set->offsetY = tilesetNode.child("tileoffset").attribute("y").as_int();
	return ret;
}

// L03: TODO: Load Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tilesetNode, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tilesetNode.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		// L03: TODO: Load Tileset image
		LOG("Filling TilesetDetails");
		SString tmp("%s%s", folder.GetString(), image.attribute("source").as_string());
		set->texture = app->tex->Load(tmp.GetString());
		set->texWidth = image.attribute("width").as_int();
		set->texHeight = image.attribute("height").as_int();

		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
		set->offsetX = 0;
		set->offsetY = 0;
	}

	return ret;
}

bool Map::StoreId(pugi::xml_node& node, MapLayer* layer, int index)
{
	bool ret = true;

	layer->data[index] = node.attribute("gid").as_uint();

	return ret;
}

// L04: TODO 3: Create the definition for a function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	// Node points to layer

	// L04: TODO 3: Load a single layer
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	layer->data = new uint[(data.width * data.height * sizeof(uint))];
	memset(layer->data, 0, size_t(data.width * data.height * sizeof(uint)));
	pugi::xml_node gidNode;

	int i = 0;
	for (gidNode = node.child("data").child("tile"); gidNode && ret; gidNode = gidNode.next_sibling("tile"))
	{
		if (ret == true) ret = StoreId(gidNode, layer, i);
		++i;
	}

	LOG("Layer <<%s>> has loaded %d tiles", layer->name.GetString(), i);
	return ret;
}
MapTypes operator++(MapTypes& mode)
{
	mode = static_cast<MapTypes>((mode + 1) % 4);
	return mode;
}
MapTypes Map::StrToMapType(SString s)
{
	SString StrType[4];
	StrType[0]="unknown", StrType[1]="orthogonal", StrType[2]="isometric", StrType[3]="staggered";
	MapTypes type = MapTypes::MAPTYPE_UNKNOWN;
	for (int i = 0; i < 4; ++i)
	{
		if (s == StrType[i])
			return type;
		++type;
	}
	return MAPTYPE_UNKNOWN;
}

void Map::LogInfo()
{
	// L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything
	LOG("--------------------------------------------------------------------------");
	LOG("<< MAP DATA >>");
	LOG("Width=%d", data.width);
	LOG("Height=%d", data.height);
	LOG("TileWidth=%d", data.tileWidth);
	LOG("TileHeight=%d", data.tileHeight);
	LOG("<< END DATA >>\n");

	ListItem<TileSet*>* infoList;
	infoList = data.tilesets.start;
	while (infoList != NULL)
	{
		LOG("<< TILESET >>");
		LOG("Name=%s", infoList->data->name.GetString());
		LOG("Firstgid=%d", infoList->data->firstgid);
		LOG("Margin=%d", infoList->data->margin);
		LOG("Spacing=%d", infoList->data->spacing);
		LOG("Tile_width=%d", infoList->data->tileWidth);
		LOG("Tile_height=%d", infoList->data->tileHeight);

		LOG("texWidth=%d", infoList->data->texWidth);
		LOG("texHeight=%d", infoList->data->texHeight);
		LOG("numTilesWidth=%d", infoList->data->numTilesWidth);
		LOG("numTilesHeight=%d", infoList->data->numTilesHeight);

		LOG("offsetX=%d", infoList->data->offsetX);
		LOG("offsetY=%d", infoList->data->offsetY);
		LOG("<< END TILESET >>\n");

		infoList = infoList->next;
	}

	// L04: TODO 4: LOG the info for each loaded layer
	ListItem<MapLayer*>* layerList;
	layerList = data.layers.start;
	while (layerList != NULL)
	{
		LOG("<< LAYER >>");
		LOG("Name=%s", layerList->data->name.GetString());
		LOG("Width=%d", layerList->data->width);
		LOG("Height=%d", layerList->data->height);
		LOG("<< END LAYER >>\n");
		layerList = layerList->next;
	}
	LOG("--------------------------------------------------------------------------");
}

bool to_bool(std::string const& s) {
	return s != "0";
}

// L06: TODO 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node propertiesNode;
	propertiesNode = node.child("properties");

	pugi::xml_node propertyNode = propertiesNode.child("property");
	
	int i = 0;
	while (propertyNode != NULL)
	{
		Property* prop = new Property();

		prop->name = propertyNode.attribute("name").as_string();

		if (propertyNode.attribute("type"))
		{
			prop->value = propertyNode.attribute("value").as_bool();
		}
		else
		{
			prop->value = to_bool(propertyNode.attribute("value").as_string());
		}

		properties.list.Add(prop);
		propertyNode = propertyNode.next_sibling("property");
	}
	
	//...
	return ret;
}

int Map::LoadColliders()
{
	int count = 0;

	ListItem<MapLayer*>* layers;
	layers = data.layers.start;
	SString collLayerStr("Meta");
	while (layers != NULL)
	{
		SString name = layers->data->name;
		if (name == collLayerStr)
		{

			break;
		}
		layers = layers->next;
	}


	// Add collider
	TileSet* tileset = NULL;
	for (int y = 0; y < data.height; ++y)
	{
		for (int x = 0; x < data.width; ++x)
		{
			int tileId = layers->data->Get(x, y);
			if (tileId > 0)
			{
				iPoint pos = MapToWorld(x, y);
				SDL_Rect rect = { pos.x,pos.y,data.tileWidth,data.tileHeight };
				tileset = GetTilesetFromTileId(tileId);
				int id = tileset->GetTileRelativeId(tileId);

				Collider::Type type = static_cast<Collider::Type>(id);
				switch (type)
				{
				case Collider::Type::GROUND: // blue
					app->collisions->AddCollider(rect, Collider::Type::GROUND);
					break;
				case Collider::Type::DEATH: // blue
					app->collisions->AddCollider(rect, Collider::Type::DEATH);
					break;
				case Collider::Type::WIN: // blue
					app->collisions->AddCollider(rect, Collider::Type::WIN);
					break;
				case Collider::Type::CHECKPOINT: 
					checkpointsList.Add(app->collisions->AddCollider(rect, Collider::Type::CHECKPOINT));
					break;
				}
			}
		}
	}
	count = app->collisions->GetColliderCount();
	return count;
}