#include "Trigger.h"
#include "j1EntityFactory.h"

Trigger::Trigger(TRIGGER_TYPE type, float posx, float posy, std::string name) : j1Entity(ENTITY_TYPE::TRIGGER, posx, posy, name), triggerType(type)
{
	debugSubtile = App->entityFactory->debugsubtileTex;
}

Trigger::~Trigger()
{
	
}
bool Trigger::Update(float dt)
{

	return true;
}

void Trigger::DebugTrigger()
{

	for (std::vector<iPoint>::iterator iter = subTiles.begin(); iter != subTiles.end(); ++iter)
	{
		iPoint subTilePos = *iter;
		subTilePos = App->map->SubTileMapToWorld(subTilePos.x, subTilePos.y);
		App->render->Blit(debugSubtile, subTilePos.x, subTilePos.y, NULL);
	}

}

void Trigger::Draw()
{
	if (App->scene->debugSubtiles)
		DebugTrigger();
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
	if (numbreOfSubtile == 0)
	{
		iPoint p = App->map->WorldToSubtileMap((int)(position.x + pivot.x), (int)(position.y + pivot.y));
		App->entityFactory->AssignEntityToSubtilePos(this, p);
		subTiles.push_back(p);
	}
	else
	{
		for (int i = -numbreOfSubtile; i < numbreOfSubtile; ++i)
		{
			for (int j = -numbreOfSubtile; j < numbreOfSubtile; ++j)
			{
				iPoint p = App->map->WorldToSubtileMap((int)(position.x + pivot.x), (int)(position.y + pivot.y));
				p.x += i;
				p.y += j;
				App->entityFactory->AssignEntityToSubtilePos(this, p);
				subTiles.push_back(p);
			}
		}
	}
}

void Trigger::DeleteFromSubtiles(int numberOfSubtile)
{
	for (std::vector<iPoint>::iterator iter = subTiles.begin(); iter != subTiles.end(); ++iter)
	{
		iPoint p = *iter;
		//p = App->map->WorldToSubtileMap((int)(position.x + pivot.x), (int)(position.y + pivot.y));
		App->entityFactory->DeleteEntityFromSubtilePos(this, p);
	}
}
