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
	std::vector<wall*>::iterator item = wallsvec.begin();
	while (item != wallsvec.end())
	{
		if ((*item)->doorType == DOOR::Exit)
		{
			App->pathfinding->DeactivateTile({ (int)(*item)->entity->position.x, (int)(*item)->entity->position.y });
			App->entityFactory->entities.erase(
				std::remove(App->entityFactory->entities.begin(), App->entityFactory->entities.end(), (*item)->entity), App->entityFactory->entities.end());

			RELEASE(*item);
			*item = nullptr;
		}


		item = wallsvec.erase(item);
	}
	wallsvec.clear();

	DeleteFromSubtiles(4);
}

bool WaveTrigger::DoTriggerAction()
{
	if (!isActivated)
	{
		CreateWalls();
	}
	
	isActivated = true;
	return true;
}

void WaveTrigger::CreateWalls()
{
	std::list<iPoint>::iterator iter = wall_map_positions.begin();

	for (; iter != wall_map_positions.end(); ++iter) 
	{
		App->pathfinding->ActivateTile({ (*iter).x, (*iter).y });
		iPoint wallPoint = App->map->MapToWorld((*iter).x, (*iter).y );
		wallPoint.y -= 16; // sprite offset
		App->entityFactory->CreateAsset(EnvironmentAssetsTypes::WALL, { wallPoint.x, wallPoint.y }, { 0,384,64,64 });
	}
}

void WaveTrigger::CreateExitWall(iPoint pos)
{
	// load walls as entities
	iPoint positionOnWorld = pos; // x and y are on iso coords, needs conversion
	SDL_Rect destRect = { 64,384,64,64 };

	wall* newWall = new wall();
	newWall->doorType == DOOR::Exit;

	newWall->entity = App->entityFactory->CreateAsset(EnvironmentAssetsTypes::TRIGGERWALL, positionOnWorld, destRect);
	wallsvec.push_back(newWall);
}

void WaveTrigger::CreateEntryWall(iPoint pos)
{
	wall_map_positions.push_back(pos);
}
