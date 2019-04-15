#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Textures.h"
#include "j1EntityFactory.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name = "pathfinding";

}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

bool j1PathFinding::Start()
{
	debug_texture = App->tex->Load("maps/path2.png");

	return true;
}



bool j1PathFinding::PostUpdate()
{

	//static iPoint origin;
	//static bool origin_selected = false;
	//static bool createdDebugPath = false;

	//iPoint mousePos;
	//App->input->GetMousePosition(mousePos.x, mousePos.y);
	//iPoint p = App->render->ScreenToWorld(mousePos.x, mousePos.y);
	//p = App->map->WorldToMap(p.x, p.y);

	//if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	//{
	//	if (origin_selected == true)
	//	{

	//		origin_selected = false;

	//		if (CreatePath(origin, p) != -1)
	//		{
	//			createdDebugPath = true;
	//		}

	//	}
	//	else
	//	{
	//		origin = p;
	//		origin_selected = true;
	//		createdDebugPath = false;
	//		debugPath.clear();

	//	}
	//}

	//if (createdDebugPath)
	//{
	//	uint debugPathSize = debugPath.size();
	//	if (debugPathSize == 0)
	//	{

	//		const std::vector<iPoint>* path = App->pathfinding->GetLastPath();
	//		uint sizeArray = path->size();
	//		for (uint i = 0; i < sizeArray; ++i)
	//		{
	//			debugPath.push_back(path->at(i));
	//		}
	//	}
	//	else
	//	{
	//		for (uint i = 0; i < debugPathSize; ++i)
	//		{
	//			iPoint pos = App->map->MapToWorld(debugPath.at(i).x, debugPath.at(i).y);
	//			App->render->Blit(debug_texture, pos.x, pos.y);
	//			App->render->DrawCircle(pos.x, pos.y + App->map->data.tile_height * 0.5F, 10, 0, 255, 255, 255, false);
	//			//LOG("Tile to print %i %i ", debugPath.at(i).x, debugPath.at(i).y);
	//		}
	//	}

	//}
	return true;
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");
	App->tex->UnLoad(debug_texture);
	debug_texture = nullptr;

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
	return (pos.x >= 0 && pos.x < (int)width &&
		pos.y >= 0 && pos.y < (int)height);
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

	std::list<PathNode>::const_reverse_iterator item = pathNodeList.rbegin();
	while (item != pathNodeList.rend())
	{
		if ((*item).Score() < min)
		{
			min = (*item).Score();
			ret = &(*item);
		}
		item++;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill, const iPoint destination)
{
	iPoint cell;

	// DIAGONAL CASES

	// North - West
	cell.create(pos.x - 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));

	// North - East
	cell.create(pos.x + 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));

	// South - East
	cell.create(pos.x + 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));

	// South - Weast
	cell.create(pos.x - 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));

	


	// Queen cases

	// North 
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE, cell.DistanceManhattan(destination), cell, this));

	// East
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE, cell.DistanceManhattan(destination), cell, this));
	
	// South
	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE, cell.DistanceManhattan(destination), cell, this));

	// West
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE, cell.DistanceManhattan(destination), cell, this));

	return list_to_fill.pathNodeList.size();

}



uint PathNode::FindSubtileAdjacents(PathList & list_to_fill, const iPoint destination)
{
	iPoint cell;
	iPoint tile;
	// DIAGONAL CASES

	// North - West
	cell.create(pos.x - 1, pos.y - 1);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		if ((App->entityFactory->isThisSubtileEnemyFree(cell) && !App->entityFactory->isThisSubtileReserved(cell)) || cell == destination)
			list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}

	// North - East
	cell.create(pos.x + 1, pos.y - 1);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		if ((App->entityFactory->isThisSubtileEnemyFree(cell) && !App->entityFactory->isThisSubtileReserved(cell)) || cell == destination)
			list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}

	// South - East
	cell.create(pos.x + 1, pos.y + 1);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		if ((App->entityFactory->isThisSubtileEnemyFree(cell) && !App->entityFactory->isThisSubtileReserved(cell)) || cell == destination)
			list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}

	// South - Weast
	cell.create(pos.x - 1, pos.y + 1);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		if ((App->entityFactory->isThisSubtileEnemyFree(cell) && !App->entityFactory->isThisSubtileReserved(cell)) || cell == destination)
			list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}

	// Queen cases

	// North 
	cell.create(pos.x, pos.y - 1);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		if ((App->entityFactory->isThisSubtileEnemyFree(cell) && !App->entityFactory->isThisSubtileReserved(cell)) || cell == destination)
			list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}
	// East
	cell.create(pos.x + 1, pos.y);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		if ((App->entityFactory->isThisSubtileEnemyFree(cell) && !App->entityFactory->isThisSubtileReserved(cell)) || cell == destination)
			list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}
	// South
	cell.create(pos.x, pos.y + 1);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		if ((App->entityFactory->isThisSubtileEnemyFree(cell) && !App->entityFactory->isThisSubtileReserved(cell)) || cell == destination)
			list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}
	// West
	cell.create(pos.x - 1, pos.y);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		if ((App->entityFactory->isThisSubtileEnemyFree(cell) && !App->entityFactory->isThisSubtileReserved(cell)) || cell == destination)
			list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}
	return list_to_fill.pathNodeList.size();
}


uint PathNode::FindSubtileAdjacentsIgnoringColl(PathList & list_to_fill, const iPoint destination)
{
	iPoint cell;
	iPoint tile;
	// DIAGONAL CASES

	// North - West
	cell.create(pos.x - 1, pos.y - 1);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}

	// North - East
	cell.create(pos.x + 1, pos.y - 1);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}

	// South - East
	cell.create(pos.x + 1, pos.y + 1);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}

	// South - Weast
	cell.create(pos.x - 1, pos.y + 1);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}

	// Queen cases

	// North 
	cell.create(pos.x, pos.y - 1);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}
	// East
	cell.create(pos.x + 1, pos.y);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}
	// South
	cell.create(pos.x, pos.y + 1);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}
	// West
	cell.create(pos.x - 1, pos.y);
	tile = App->map->SubTileMapToWorld(cell.x, cell.y);
	tile = App->map->WorldToMap(tile.x, tile.y);
	if (App->pathfinding->IsWalkable(tile))
	{
		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
	}
	return list_to_fill.pathNodeList.size();
}

// NOT TRYING WALKABILITY

//uint PathNode::FindSubtileAdjacents(PathList & list_to_fill, const iPoint destination)
//{
//	iPoint cell;
//	iPoint tile;
//	// DIAGONAL CASES
//
//	// North - West
//	cell.create(pos.x - 1, pos.y - 1);
//	if (App->entityFactory->isThisSubtileEmpty(cell) || cell == destination)
//		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
//
//	// North - East
//	cell.create(pos.x + 1, pos.y - 1);
//	if (App->entityFactory->isThisSubtileEmpty(cell) || cell == destination)
//		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
//
//	// South - East
//	cell.create(pos.x + 1, pos.y + 1);
//	if (App->entityFactory->isThisSubtileEmpty(cell) || cell == destination)
//		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
//
//	// South - Weast
//	cell.create(pos.x - 1, pos.y + 1);
//	if (App->entityFactory->isThisSubtileEmpty(cell) || cell == destination)
//		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE + 1, cell.DistanceManhattan(destination), cell, this));
//
//
//	// Queen cases
//
//	// North 
//	cell.create(pos.x, pos.y - 1);
//	if (App->entityFactory->isThisSubtileEmpty(cell) || cell == destination)
//		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE, cell.DistanceManhattan(destination), cell, this));
//
//	// East
//	cell.create(pos.x + 1, pos.y);
//	if (App->entityFactory->isThisSubtileEmpty(cell) || cell == destination)
//		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE, cell.DistanceManhattan(destination), cell, this));
//
//	// South
//	cell.create(pos.x, pos.y + 1);
//	if (App->entityFactory->isThisSubtileEmpty(cell) || cell == destination)
//		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE, cell.DistanceManhattan(destination), cell, this));
//
//	// West
//	cell.create(pos.x - 1, pos.y);
//	if (App->entityFactory->isThisSubtileEmpty(cell) || cell == destination)
//		list_to_fill.pathNodeList.push_back(PathNode(g + COST_TO_MOVE, cell.DistanceManhattan(destination), cell, this));
//
//	return list_to_fill.pathNodeList.size();
//}

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
	last_path.clear();
	uint cont = 0; 
	if (origin == destination || !IsWalkable(origin) || !IsWalkable(destination))
		return -1;

	// Create two lists: open, close
	PathList open;
	PathList close;

	// Add the origin tile to open
	PathNode originNode(0, origin.DistanceTo(destination), origin, nullptr);
	open.pathNodeList.push_back(originNode);

	// Iterate while we have tile in the open list
	while (open.pathNodeList.size() > 0)
	{

		// Move the lowest score cell from open list to the closed list
		PathNode* curr = (PathNode*)open.GetNodeLowestScore();

		close.pathNodeList.push_back(*curr);

		// Erase element from list -----
		std::list<PathNode>::iterator it = open.pathNodeList.begin();
		while (it != open.pathNodeList.end()) {

			if (&(*it) == &(*curr))
				break;
			it++;
		}
		open.pathNodeList.erase(it);
		// Erase element from list -----

		// If we just added the destination, we are done!
		// Backtrack to create the final path
		if (close.pathNodeList.back().pos == destination) {

			for (PathNode iterator = close.pathNodeList.back(); iterator.parent != nullptr;
				iterator = *close.Find(iterator.parent->pos)) {

				last_path.push_back(iterator.pos);
			}

			last_path.push_back(close.pathNodeList.front().pos);

			// Flip() the path when you are finish
			reverse(last_path.begin(), last_path.end());

			return last_path.size();
		}

		else
		{
			// Fill a list of all adjancent nodes
			PathList neighbors;
			close.pathNodeList.back().FindWalkableAdjacents(neighbors, destination);

			// Iterate adjancent nodes:
			std::list<PathNode>::iterator iterator = neighbors.pathNodeList.begin();

			while (iterator != neighbors.pathNodeList.end()) {
				// ignore nodes in the closed list
				if (close.Find((*iterator).pos) != NULL) {
					iterator++;
					continue;
				}

				(*iterator).CalculateF(destination);
				// If it is already in the open list, check if it is a better path (compare G)
				if (open.Find((*iterator).pos) != NULL) {

					// If it is a better path, Update the parent
					PathNode open_node = *open.Find((*iterator).pos);
					if ((*iterator).g < open_node.g)
						open_node.parent = (*iterator).parent;
				}
				else {
					// If it is NOT found, calculate its F and add it to the open list
					open.pathNodeList.push_back(*iterator);
				}
				iterator++;
			}
			neighbors.pathNodeList.clear();
			cont++; 
		}
		if (cont > 20)
			return -1;
	}

}

int j1PathFinding::CreateSubtilePath(const iPoint & origin, const iPoint & destination, bool ignoringCollisions)
{
	int cont = 0; 
	last_path.clear();

	if (origin == destination /*|| !IsWalkable(origin) || !IsWalkable(destination)*/)
		return -1;

	// Create two lists: open, close
	PathList open;
	PathList close;

	// Add the origin tile to open
	PathNode originNode(0, origin.DistanceTo(destination), origin, nullptr);
	open.pathNodeList.push_back(originNode);

	// Iterate while we have tile in the open list
	while (open.pathNodeList.size() > 0)
	{

		// Move the lowest score cell from open list to the closed list
		PathNode* curr = (PathNode*)open.GetNodeLowestScore();

		close.pathNodeList.push_back(*curr);

		// Erase element from list -----
		std::list<PathNode>::iterator it = open.pathNodeList.begin();
		while (it != open.pathNodeList.end()) {

			if (&(*it) == &(*curr))
				break;
			it++;
		}
		open.pathNodeList.erase(it);
		// Erase element from list -----

		// If we just added the destination, we are done!
		// Backtrack to create the final path
		if (close.pathNodeList.back().pos == destination) {

			for (PathNode iterator = close.pathNodeList.back(); iterator.parent != nullptr;
				iterator = *close.Find(iterator.parent->pos)) {

				last_path.push_back(iterator.pos);
			}

			last_path.push_back(close.pathNodeList.front().pos);

			// Flip() the path when you are finish
			reverse(last_path.begin(), last_path.end());

			return last_path.size();
		}

		else
		{
			// Fill a list of all adjancent nodes
			PathList neighbors;
			if(!ignoringCollisions)		// Avoid other enemies 
				close.pathNodeList.back().FindSubtileAdjacents(neighbors, destination);

			else close.pathNodeList.back().FindSubtileAdjacentsIgnoringColl(neighbors, destination);
			// Iterate adjancent nodes:
			std::list<PathNode>::iterator iterator = neighbors.pathNodeList.begin();

			while (iterator != neighbors.pathNodeList.end()) {
				// ignore nodes in the closed list
				if (close.Find((*iterator).pos) != NULL) {
					iterator++;
					continue;
				}

				(*iterator).CalculateF(destination);
				// If it is already in the open list, check if it is a better path (compare G)
				if (open.Find((*iterator).pos) != NULL) {

					// If it is a better path, Update the parent
					PathNode open_node = *open.Find((*iterator).pos);
					if ((*iterator).g < open_node.g)
						open_node.parent = (*iterator).parent;
				}
				else {
					// If it is NOT found, calculate its F and add it to the open list
					open.pathNodeList.push_back(*iterator);
				}
				iterator++;
			}
			neighbors.pathNodeList.clear();
			cont++;
		}
		if (cont > 25)
			return -1; 
	}
	return 0;
}



//int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
//{
//
//	if (!IsWalkable(origin) || !IsWalkable(destination) || origin == destination) {
//		LOG("Invalid origin or destination: Origin or destination are not walkable or are the same.");
//		return -1;
//	}
//
//	PathList openList;
//	PathList closedList;
//	openList.pathNodeList.push_back(PathNode(0, origin.DistanceManhattan(destination), origin, nullptr));
//
//	while (openList.pathNodeList.size() > 0) {
//		PathNode* lowestNode = (PathNode*)openList.GetNodeLowestScore();
//		closedList.pathNodeList.push_back(*lowestNode);
//
//		PathNode currNode = closedList.pathNodeList.back();
//
//		//openList.pathNodeList.del(*lowestNode);
//
//		std::list<PathNode>::iterator it = openList.pathNodeList.begin();
//		while (it != openList.pathNodeList.end()) {
//			if ((*it).pos == currNode.pos)
//			{
//				break;
//			}
//			it++;
//		}
//		openList.pathNodeList.erase(it);
//
//
//
//		if (currNode.pos == destination) {
//			last_path.clear();
//			for (PathNode * pathIterator = &currNode; pathIterator != nullptr && pathIterator->pos != origin; pathIterator = (*pathIterator).parent) 
//			{
//				last_path.push_back(pathIterator->pos);  // Current problem resides here, parents do not work properly. 
//			}
//			// adds start node too
//			last_path.push_back(closedList.pathNodeList.front().pos);
//			// Flip() the path when you are finish
//
//			reverse(last_path.begin(), last_path.end());
//			LOG("Succesful path: The algorithm has found a path from the origin(%i, %i) to the destination(%i, %i)", origin.x, origin.y, destination.x, destination.y);
//			return last_path.size();
//		}
//
//		PathList adjacentNodes;
//		currNode.FindWalkableAdjacents(adjacentNodes, destination);
//
//		for (std::list<PathNode>::iterator adjacentNodeIterator = adjacentNodes.pathNodeList.begin(); adjacentNodeIterator != adjacentNodes.pathNodeList.end(); adjacentNodeIterator++) {
//			if (closedList.Find((*adjacentNodeIterator).pos) != NULL) {
//				continue;
//			}
//			PathNode* duplicateNode = (PathNode*)openList.Find((*adjacentNodeIterator).pos);
//			if (duplicateNode == NULL) {
//				openList.pathNodeList.push_back((*adjacentNodeIterator));
//			}
//			else if ((*adjacentNodeIterator).g < (*duplicateNode).g) {
//				(*duplicateNode).g = (*adjacentNodeIterator).g;
//				(*duplicateNode).parent = &currNode;
//			}
//		}
//	}
//
//	LOG("Invalid path: The algorithm has extended to all the possible nodes and hasn't found a path to the destination.");
//	return -1;
//}