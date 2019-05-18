#include "NoWalkableTrigger.h"
#include "j1PathFinding.h"
#include "j1EntityFactory.h"

NoWalkableTrigger::NoWalkableTrigger(float posx, float posy) : Trigger(TRIGGER_TYPE::NOWALKABLE, posx, posy, "noWalkable")
{
	AssignInSubtiles(4);
}

NoWalkableTrigger::~NoWalkableTrigger()
{
	std::vector<wall*>::iterator item = walls.begin();
	while (item != walls.end())
	{
		if ((*item)->doorType == DOOR::Exit)
		{
			App->pathfinding->DeactivateTile({ (int)(*item)->entity->position.x, (int)(*item)->entity->position.y });
			App->entityFactory->entities.erase(
					std::remove(App->entityFactory->entities.begin(), App->entityFactory->entities.end(), (*item)->entity), App->entityFactory->entities.end());

			RELEASE(*item);
			*item = nullptr;
		}

		
		item = walls.erase(item);
	}
	walls.clear();

}

bool NoWalkableTrigger::DoTriggerAction()
{
	std::vector<wall*>::iterator item = walls.begin();
	while (item != walls.end() && !isActived)
	{
		iPoint pos = App->map->WorldToMap((int)(*item)->entity->position.x, (int)(*item)->entity->position.y);
		pos.x += 2;
		pos.y += 1;
		App->pathfinding->ActivateTile({ pos.x, pos.y });
		App->entityFactory->CreateAsset(EnvironmentAssetsTypes::WALL, pos, { 64,384,64,64 });
		App->entityFactory->entities.push_back((*item)->entity);
		++item;
	}
	isActived = true;
	return true;
}

void NoWalkableTrigger::CreateExitWall(iPoint pos)
{
	// load walls as entities
	iPoint positionOnWorld = pos; // x and y are on iso coords, needs conversion
	SDL_Rect destRect = { 64,384,64,64 };

	wall* newWall = new wall();
	newWall->doorType == DOOR::Exit;

	newWall->entity = App->entityFactory->CreateAsset(EnvironmentAssetsTypes::TRIGGERWALL, positionOnWorld, destRect);
	walls.push_back(newWall);
}

void NoWalkableTrigger::CreateEntryWall(iPoint pos)
{
}