#include "Trigger.h"
#include "j1EntityFactory.h"
#include "j1Map.h"
#include "j1PathFinding.h"
Trigger::Trigger(TRIGGER_TYPE type, float posx, float posy, std::string name) : j1Entity(ENTITY_TYPE::TRIGGER, posx, posy, name), triggerType(type)
{
}

Trigger::~Trigger()
{
	
}
bool Trigger::Update(float dt)
{
	return true;
}

void Trigger::Draw()
{
}

bool Trigger::CleanUp()
{
	App->entityFactory->DeleteEntityFromSubtile(this);
	return true;
}

bool Trigger::Save(pugi::xml_node &node) const
{
	
	return true;
}

bool Trigger::DoTriggerAction()
{
	return true;
}

void Trigger::AssignInSubtiles(int numbreOfSubtile)
{
	if (numbreOfSubtile != 0)
	{
		for (int i = -numbreOfSubtile; i < numbreOfSubtile; ++i)
		{
			for (int j = -numbreOfSubtile; j < numbreOfSubtile; ++j)
			{
				iPoint p;
				p.x += App->map->MapToWorld(position.x, position.y).x + i;
				p.y += App->map->MapToWorld(position.x, position.y).y + j;
				App->entityFactory->AssignEntityToSubtilePos(this, p);
			}
		}
	}
	else
	{
		iPoint p;
		p.x += App->map->MapToWorld(position.x, position.y).x;
		p.y += App->map->MapToWorld(position.x, position.y).y;
		App->entityFactory->AssignEntityToSubtilePos(this, p);
	}
}

void Trigger::DeleteFromSubtiles(int numberOfSubtile)
{
	if (numberOfSubtile)
	{
		for (int i = -numberOfSubtile; i < numberOfSubtile; ++i)
		{
			for (int j = -numberOfSubtile; j < numberOfSubtile; ++j)
			{
				iPoint p;
				p.x += App->map->MapToWorld(position.x, position.y).x + i;
				p.y += App->map->MapToWorld(position.x, position.y).y + j;
				if(App->entityFactory->isThisSubtileEmpty(p))
					App->entityFactory->DeleteEntityFromSubtilePos(this, p);
			}
		}
	}
	else
	{
		iPoint p;
		p.x += App->map->MapToWorld(position.x, position.y).x;
		p.y += App->map->MapToWorld(position.x, position.y).y;
		if (App->entityFactory->isThisSubtileEmpty(p))
			App->entityFactory->AssignEntityToSubtilePos(this, p);
	}
}
