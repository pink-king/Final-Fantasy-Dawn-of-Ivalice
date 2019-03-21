#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Textures.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name = "pathfinding";
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

bool j1PathFinding::Awake()
{
	debug_tex = App->tex->Load("maps/path2.png");

	return true;
}

bool j1PathFinding::PostUpdate()
{

	static iPoint origin;
	static bool origin_selected = false;
	static bool createdDebugPath = false;

	iPoint mousePos;
	App->input->GetMousePosition(mousePos.x, mousePos.y);
	iPoint p = App->render->ScreenToWorld(mousePos.x, mousePos.y);
	p = App->map->WorldToMap(p.x, p.y);

	/*if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{

			origin_selected = false;

			if (CreatePath(origin, p) != -1)
			{
				createdDebugPath = true;
			}

		}
		else
		{
			origin = p;
			origin_selected = true;
			createdDebugPath = false;
			debugPath.clear();

		}
	}*/

	if (createdDebugPath)
	{
		uint debugPathSize = debugPath.size();
		if (debugPathSize == 0)
		{

			const std::vector<iPoint>* path = App->pathfinding->GetLastPath();
			uint sizeArray = path->size();
			for (uint i = 0; i < sizeArray; ++i)
			{
				debugPath.push_back(path->at(i));
			}
		}
		else
		{
			for (uint i = 0; i < debugPathSize; ++i)
			{
				iPoint pos = App->map->MapToWorld(debugPath.at(i).x, debugPath.at(i).y);
				App->render->Blit(debug_tex, pos.x, pos.y);
			}
		}

	}
	return true;
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}



// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const std::vector<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
const PathNode* PathList::Find(const iPoint& point)
{
	std::list<PathNode>::iterator item = pathNodeList.begin();
	while (item != pathNodeList.end())
	{
		if ((*item).pos == point)
			return &(*item);
		item++;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
const PathNode* PathList::GetNodeLowestScore() const
{
	const PathNode* ret = NULL;
	int min = INT_MAX;

	std::list<PathNode>::const_iterator item = pathNodeList.end();
	while (item != pathNodeList.begin())
	{
		if ((*item).Score() < min)
		{
			min = (*item).Score();
			ret = &(*item);
		}
		item--;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.pathNodeList.size();


	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(-1, -1, cell, this));


	// DIAGONAL CASES 


	// north-west
	cell.create(pos.x + 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(-1, -1, cell, this));

	// south-west
	cell.create(pos.x - 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(-1, -1, cell, this));

	// north-west
	cell.create(pos.x + 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(-1, -1, cell, this));

	// south-est
	cell.create(pos.x - 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(-1, -1, cell, this));

	return list_to_fill.pathNodeList.size();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	// F of diagnal nodes can be modified

	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	if (origin == destination || !IsWalkable(origin) || !IsWalkable(destination))
		return -1;

	PathList openList;
	PathList closeList;

	// Added Origen to the Open List

	PathNode originNode(0, origin.DistanceTo(destination), origin, nullptr);
	openList.pathNodeList.push_back(originNode);

	bool findDestination = false;

	while (openList.pathNodeList.size() > 0 && !findDestination)
	{
		PathNode* lowerNode = (PathNode*)openList.GetNodeLowestScore();
		PathNode* currNode = lowerNode;

		// Move the closest node to the Closed List
		closeList.pathNodeList.push_back(*lowerNode);

		// Erase it from the Open List
		std::list<PathNode>::iterator iter = openList.pathNodeList.begin();
		while (iter != openList.pathNodeList.end()) {

			if (&(*iter) == &(*currNode))
				break;
			iter++;
		}
		openList.pathNodeList.erase(iter);


		// If its the goal break - functionality below
		if (currNode->pos == destination)
		{
			findDestination = true;
			break;
		}

		PathList neighbours;
		currNode->FindWalkableAdjacents(neighbours);

		std::list<PathNode>::iterator nodeNeighbour = neighbours.pathNodeList.begin();

		for (; nodeNeighbour != neighbours.pathNodeList.end(); nodeNeighbour++)
		{
			if (closeList.Find((*nodeNeighbour).pos) != NULL) {
				nodeNeighbour++;
				continue;
			}

			(*nodeNeighbour).CalculateF(destination);
			// If it is already in the open list, check if it is a better path (Better G)
			if (openList.Find((*nodeNeighbour).pos) != NULL) {

				// If it is a better path, Update the parent
				PathNode open_node = *openList.Find((*nodeNeighbour).pos);
				if ((*nodeNeighbour).g < open_node.g)
					open_node.parent = (*nodeNeighbour).parent;
			}
			else {
				// If it is NOT found, calculate its F and add it to the open list
				openList.pathNodeList.push_back(*nodeNeighbour);
			}
		}


	}

	if (findDestination)
	{
		last_path.clear();
		PathNode pathNode = closeList.pathNodeList.back();

		for (; pathNode.parent != nullptr; pathNode = *closeList.Find(pathNode.parent->pos))
		{
			last_path.push_back(pathNode.pos);
		}

		last_path.push_back(closeList.pathNodeList.front().pos);

		//Here we flip last_path order -------------------------------------
		std::vector<iPoint> aux;
		for (int i = last_path.size() - 1; i >= 0; --i)
		{
			aux.push_back(last_path.at(i));
		}

		last_path.swap(aux);

		return 1;
	}
	return -1;
}

