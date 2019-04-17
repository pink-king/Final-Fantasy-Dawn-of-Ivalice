#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include <list>
#include "p2Point.h"
#include "j1Module.h"


#define MARGIN_TILE 1
#define SUBTILE_RESOLUTION 0.5f
#define SUBTILE_MULTIPLIER 2

struct Properties
{
	struct Property
	{
		std::string name;
		int value;
	};

	~Properties()
	{
		std::list<Property*>:: iterator item;
		item = list.begin();

		while(*item != NULL)
		{
			list.remove(*item);
			++item;
		}

		list.clear();
	}

	float parallaxSpeed = 1.0f;

	int Get(const char* name, int default_value = 0) const;

	std::list<Property*>	list;
};

struct PlayerData
{
	std::string name;
	Properties properties;
	int x, y = 0;
};

struct MapObjects
{
	std::string name;
	uint id = 0;
	//Collider* colliders[MAX_OBJECTGROUP_COLLIDERS] = { nullptr };
	Properties properties;

	~MapObjects()
	{
		//RELEASE_ARRAY(colliders[MAX_OBJECTGROUP_COLLIDERS]);
	}
};
// ----------------------------------------------------
struct MapLayer
{
	std::string	name;
	int			width;
	int			height;
	uint*		data;
	Properties	properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y*width) + x];
	}
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	std::string			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int						width;
	int						height;
	int						tile_width;
	int						tile_height;
	SDL_Color				background_color;
	MapTypes				type;
	std::list<TileSet*>		tilesets;
	std::list<MapLayer*>	layers;
	std::list<MapObjects>	mapObjects;
	std::list<std::string>	levels;
	std::string				loadedLevel;
	Properties				properties;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();
	void DebugDraw();
	void ToggleDebugDraw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	iPoint SubTileMapToWorld(int x, int y) const;
	iPoint WorldToSubtileMap(int x, int y) const;
	iPoint IsoToWorld(int x, int y) const;
	iPoint WorldToIso(int x, int y) const;

	iPoint IsoTo2D(int x, int y) const;
	iPoint TwoDToIso(int x, int y) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);
	bool LoadMapColliders(pugi::xml_node& node);
	bool LoadMapAssets(pugi::xml_node& node);
	bool LoadSpawns(pugi::xml_node& node); 

	TileSet* GetTilesetFromTileId(int id) const;

public:
	MapData data;
	PlayerData			playerData;
	bool				map_loaded;
	bool				showNavLayer = false;
	iPoint pixelTileOffset; // mesures the height between the end of y sprite texture to start of the paint(draw) itself (displacement)

private:

	pugi::xml_document	map_file;
	std::string			folder;
	SDL_Texture*		texture = nullptr;
	bool debugDraw = true;
};

#endif // __j1MAP_H__