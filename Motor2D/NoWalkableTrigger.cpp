#include "NoWalkableTrigger.h"
#include "j1PathFinding.h"
#include "j1EntityFactory.h"

NoWalkableTrigger::NoWalkableTrigger(float posx, float posy) : Trigger(TRIGGER_TYPE::NOWALKABLE, posx, posy, "noWalkable")
{
	AssignInSubtiles(11);
}

NoWalkableTrigger::~NoWalkableTrigger()
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

	DeleteFromSubtiles(11);
}

bool NoWalkableTrigger::DoTriggerAction()
{
	if (!isActivated)
	{
		iPoint instantiationPoint = App->map->MapToWorld( 42,109 );
		instantiationPoint.x -= 34;
		instantiationPoint.x -= 38;
		App->entityFactory->CreateEntity(FLOWERBOSS, instantiationPoint.x, instantiationPoint.y, "flower_boss");
		App->audio->PlayMusic("audio/music/bossmusic.ogg", -1);
		App->entityFactory->CreateDialogTrigger(instantiationPoint.x, instantiationPoint.y, "BOSS", 10, false);     // boss dialog ready for lvl 2 interaction with boss

		CreateWalls();
		isActivated = true;
	}
	return true;
}

bool NoWalkableTrigger::Update(float dt)
{
	if (waveEntity == nullptr && isActivated)
		to_delete = true;

	return true;
}

void NoWalkableTrigger::CreateExitWall(iPoint pos)
{
	exit_wall_map_positions.push_back(pos);
}

void NoWalkableTrigger::CreateEntryWall(iPoint pos)
{
	entry_wall_map_positions.push_back(pos);
}

void NoWalkableTrigger::CreateWalls()
{
	// entry walls
	std::list<iPoint>::iterator iter = entry_wall_map_positions.begin();
	for (; iter != entry_wall_map_positions.end(); ++iter)
	{
		App->pathfinding->ActivateTile({ (*iter).x, (*iter).y });
		iPoint wallPoint = App->map->MapToWorld((*iter).x, (*iter).y);
		wallPoint.y -= 16; // sprite offset
		App->entityFactory->CreateAsset(EnvironmentAssetsTypes::WALL, { wallPoint.x, wallPoint.y }, { 0,873,64,64 });
	}

	// exit walls
	iter = exit_wall_map_positions.begin();
	for (; iter != exit_wall_map_positions.end(); ++iter)
	{
		App->pathfinding->ActivateTile({ (*iter).x, (*iter).y });
		iPoint wallPoint = App->map->MapToWorld((*iter).x, (*iter).y);
		wallPoint.y -= 16; // sprite offset
		exit_wall_entities.push_back(App->entityFactory->CreateAsset(EnvironmentAssetsTypes::WALL, { wallPoint.x, wallPoint.y }, { 0,873,64,64 }));
	}

}