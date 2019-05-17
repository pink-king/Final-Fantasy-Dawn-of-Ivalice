#include "NoWalkableTrigger.h"
#include "j1PathFinding.h"
NoWalkableTrigger::NoWalkableTrigger(float posx, float posy): Trigger(TRIGGER_TYPE::NOWALKABLE,posx,posy,"noWalkable")
{
	AssignInSubtiles(4);
}

NoWalkableTrigger::~NoWalkableTrigger()
{
	std::vector<j1Entity*>::iterator item = walls.begin();
	while (item != walls.end())
	{
		delete *item;
		*item = nullptr;
		item = walls.erase(item);
	}
}

bool NoWalkableTrigger::DoTriggerAction()
{
	return true;
}

void NoWalkableTrigger::CreateWall(iPoint pos)
{
//	App->pathfinding->
}
