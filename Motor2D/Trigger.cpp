#include "Trigger.h"
#include "j1EntityFactory.h"

Trigger::Trigger(TRIGGER_TYPE type,float posx, float posy, std::string name) : j1Entity(ENTITY_TYPE::TRIGGER,posx,posy, name), type(type)
{
}

Trigger::~Trigger()
{
	
}

bool Trigger::CleanUp()
{
	App->entityFactory->DeleteEntityToAdjacentsSubtiles(this, nSubtiles);
	return true;
}

bool Trigger::DoTriggerAction()
{
	return true;
}

void Trigger::AssignInSubtiles(int numbreOfSubtile)
{
	App->entityFactory->AssignEntityToSubtile(this);
	App->entityFactory->AssignEntityToAdjacentsSubtiles(this, numbreOfSubtile);
}
