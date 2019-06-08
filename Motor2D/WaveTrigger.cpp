#include "WaveTrigger.h"
#include "j1PathFinding.h"
#include "j1EntityFactory.h"
#include "NoWalkableTrigger.h"

WaveTrigger::WaveTrigger(float posx, float posy, const SDL_Rect& zone, uint level) : waveZone(zone), level(level), Trigger(TRIGGER_TYPE::WAVE, posx, posy, "Wave")
{
	AssignInSubtiles(11); 
}

WaveTrigger::~WaveTrigger()
{
	// delete wall entities
	std::list<j1Entity*>::iterator wallsIter = exit_wall_entities.begin();
	for (; wallsIter != exit_wall_entities.end(); ++wallsIter)
	{
		if((*wallsIter) != nullptr)
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

bool WaveTrigger::DoTriggerAction()
{
	if (!isActivated)
	{
		if (level == 1)
		{
			waveEntity = App->entityFactory->CreateWave(waveZone, 5, WAVE_TYPE::LEVEL_1, this);
			rect = { 0,384,64,64 };
		}
		else if (level == 2)
		{
			waveEntity = App->entityFactory->CreateWave(waveZone, 6, WAVE_TYPE::LEVEL_2, this);
			rect = { 0,873,64,64 };
		}
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
		App->entityFactory->CreateAsset(EnvironmentAssetsTypes::WALL, { wallPoint.x, wallPoint.y }, rect);
	}

	// exit walls
	iter = exit_wall_map_positions.begin();
	for (; iter != exit_wall_map_positions.end(); ++iter)
	{
		App->pathfinding->ActivateTile({ (*iter).x, (*iter).y });
		iPoint wallPoint = App->map->MapToWorld((*iter).x, (*iter).y);
		wallPoint.y -= 16; // sprite offset
		exit_wall_entities.push_back(App->entityFactory->CreateAsset(EnvironmentAssetsTypes::WALL, { wallPoint.x, wallPoint.y }, rect));
	}

}

bool WaveTrigger::Update(float dt)
{
	if (waveEntity == nullptr && isActivated)
		to_delete = true; 

	return true;
}

bool WaveTrigger::Save(pugi::xml_node &node) const
{
	node.append_attribute("type") = (int)triggerType;
	node.append_attribute("posX") = (int)position.x;
	node.append_attribute("posY") = (int)position.y;

	node.append_attribute("zoneX") = waveZone.x;
	node.append_attribute("zoneY") = waveZone.y;
	node.append_attribute("zoneW") = waveZone.w;
	node.append_attribute("zoneH") = waveZone.h;

	node.append_attribute("level") = level;

	std::list<iPoint>::const_iterator iter = entry_wall_map_positions.begin();
	for (; iter != entry_wall_map_positions.end(); ++iter)
	{
		pugi::xml_node nodeEntryWall = node.append_child("entrytWall");
		nodeEntryWall.append_attribute("posX") = (int)(*iter).x;
		nodeEntryWall.append_attribute("posY") = (int)(*iter).y;
	}

	for (iter = exit_wall_map_positions.begin(); iter != exit_wall_map_positions.end(); ++iter)
	{
		pugi::xml_node nodeExitWall = node.append_child("exitWall");
		nodeExitWall.append_attribute("posX") = (int)(*iter).x;
		nodeExitWall.append_attribute("posY") = (int)(*iter).y;
	}
	return true;
}

void WaveTrigger::CreateExitWall(iPoint pos)
{
	exit_wall_map_positions.push_back(pos);
}

void WaveTrigger::CreateEntryWall(iPoint pos)
{
	entry_wall_map_positions.push_back(pos);
}
