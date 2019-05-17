#include "Trigger.h"
#include "j1EntityFactory.h"

Trigger::Trigger(TRIGGER_TYPE type,float posx, float posy, std::string name) : j1Entity(ENTITY_TYPE::TRIGGER,posx,posy, name), triggerType(type)
{
}

Trigger::~Trigger()
{
	
}

bool Trigger::CleanUp()
{
	App->entityFactory->DeleteEntityFromSubtile(this);

	return true;
}

bool Trigger::Save(pugi::xml_node &node) const
{
	pugi::xml_node pos = node.append_child("position");
	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;
	return true;
}

bool Trigger::DoTriggerAction()
{
	return true;
}

void Trigger::AssignInSubtiles(int numbreOfSubtile)
{
	for (int i = -numbreOfSubtile; i < numbreOfSubtile; ++i)
	{
		for (int j = -numbreOfSubtile; j < numbreOfSubtile; ++j)
		{
			iPoint p = App->map->WorldToSubtileMap((int)position.x, (int)position.y);
			p.x += i;
			p.y += j;
			App->entityFactory->AssignEntityToSubtilePos(this, p);
		}
	}
}
