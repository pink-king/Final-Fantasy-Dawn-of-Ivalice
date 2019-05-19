#include "WaveTrigger.h"
#include "j1PathFinding.h"
#include "j1EntityFactory.h"
#include "NoWalkableTrigger.h"

WaveTrigger::WaveTrigger(float posx, float posy, uint level) : level(level), Trigger(TRIGGER_TYPE::WAVE, posx, posy, "Wave")
{
	AssignInSubtiles(11); 
}

WaveTrigger::~WaveTrigger()
{
	// delete wall entities
	std::list<j1Entity*>::iterator wallsIter = exit_wall_entities.begin();
	for (; wallsIter != exit_wall_entities.end(); ++wallsIter)
	{
		(*wallsIter)->to_delete = true;
	}
	exit_wall_entities.clear();

	// delete walkability
	std::list<iPoint>::iterator exitWallPositionsIter = exit_wall_map_positions.begin();
	for (; exitWallPositionsIter != exit_wall_map_positions.end(); ++exitWallPositionsIter)
		App->pathfinding->DeactivateTile({ (*exitWallPositionsIter).x, (*exitWallPositionsIter).y });

	// check this
	DeleteFromSubtiles(4);
}

bool WaveTrigger::DoTriggerAction()
{
	if (!isActivated)
	{
		CreateWalls();
		isActivated = true;
	}
	
	return true;
}

void WaveTrigger::CreateWalls()
{
	// entry walls
	std::list<iPoint>::iterator iter = entry_wall_map_positions.begin();
	for (; iter != entry_wall_map_positions.end(); ++iter) 
	{
		App->pathfinding->ActivateTile({ (*iter).x, (*iter).y });
		iPoint wallPoint = App->map->MapToWorld((*iter).x, (*iter).y );
		wallPoint.y -= 16; // sprite offset
		App->entityFactory->CreateAsset(EnvironmentAssetsTypes::WALL, { wallPoint.x, wallPoint.y }, { 0,384,64,64 });
	}

	// exit walls
	iter = exit_wall_map_positions.begin();
	for (; iter != exit_wall_map_positions.end(); ++iter)
	{
		App->pathfinding->ActivateTile({ (*iter).x, (*iter).y });
		iPoint wallPoint = App->map->MapToWorld((*iter).x, (*iter).y);
		wallPoint.y -= 16; // sprite offset
		exit_wall_entities.push_back(App->entityFactory->CreateAsset(EnvironmentAssetsTypes::WALL, { wallPoint.x, wallPoint.y }, { 0,384,64,64 }));
	}

}

void WaveTrigger::CreateExitWall(iPoint pos)
{
	exit_wall_map_positions.push_back(pos);
}

void WaveTrigger::CreateEntryWall(iPoint pos)
{
	entry_wall_map_positions.push_back(pos);
}
