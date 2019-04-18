#ifndef __QUADTREE__H_
#define __QUADTREE__H_

class Quadtree {

public:

	//Constructor
	Quadtree(uint max_levels, SDL_Rect section, uint level = 1);

	//Split the node into 4 subnodes
	virtual void Split() {};

	virtual void DrawQuadtree() const;

	virtual void CleanUp() {};

	bool CheckTouch(const SDL_Rect& rect);	//Check if a rectangle fits inside the quadtree

protected:
	
	bool divided;			//True if this node has subnodes
	uint max_levels;		//Max number of levels a quadtree will have
	uint level;				//Position in the tree
	SDL_Rect section;		//Position and size
	
};

#endif // !__QUADTREE__H_

