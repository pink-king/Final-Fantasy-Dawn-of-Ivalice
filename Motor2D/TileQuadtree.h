#ifndef _TILEQUADTREE_H__
#define _TILEQUADTREE_H__

#include "Quadtree.h"
#include <vector>

struct TileData												
{
	TileData() {}
	TileData(uint id, iPoint position) :id(id),position(position) {};
	
	~TileData();
	uint id;
	iPoint position;
};

class TileQuadtree : public Quadtree
{
public:

	TileQuadtree(uint max_levels, SDL_Rect section, uint size, uint level=1);

	void CleanUp();

	void Split();

	void InsertTile(TileData tile);

	bool CheckVisibility();

	void DrawMap();

	//DEBUG

	void DrawQuadtree();

	

protected: 

	TileQuadtree* nodes[4];		//Subnodes of the current node
	
	std::vector<TileData> tiles;//Array which contains all the tiles

	uint	size;				//Amount of tiles this node can store
	uint	tiles_contained;	//Amount of tiles this node currently stores

	iPoint pixelTileOffset; // mesures the height between the end of y sprite texture to start of the paint(draw) itself (displacement)
	

};

#endif // !_TILEQUADTREE_H__
