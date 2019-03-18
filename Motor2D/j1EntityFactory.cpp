#include "j1EntityFactory.h"
#include "j1Render.h"
#include <algorithm>

j1EntityFactory::j1EntityFactory()
{
	name.assign("entities");
}

j1EntityFactory::~j1EntityFactory()
{
}

bool j1EntityFactory::Awake(pugi::xml_node & node)
{
	bool ret = true;

	pugi::xml_node entity;
	for (entity = node.child("entities").child("entity"); entity && ret; entity = entity.next_sibling("entity"))
	{
		EntityData* ent = new EntityData();

		ent->name = node.attribute("name").as_string();
		ent->position.x = node.attribute("positionX").as_float();
		ent->position.y = node.attribute("positionY").as_float();
		if (node.attribute("type").as_string() == "static")
			ent->type = ENTITY_TYPE::ENTITY_STATIC;
		else if (node.attribute("type").as_string() == "dinamic")
			ent->type = ENTITY_TYPE::ENTITY_DINAMIC;
	}
	return ret;
}

bool j1EntityFactory::Start()
{
	//create enemies
	std::vector<EntityData>::iterator item = entitiesInfo.begin();
	for (; item != entitiesInfo.end(); ++item)
		entities.push_back(CreateEntity(item->type, item->position.x, item->position.y, item->name));

	//load texture
	return true;
}

bool j1EntityFactory::Update(float dt)
{
	bool ret = true;

	std::vector<j1Entity*> draw_entities;
	uint entities_drawn = 0;

	std::vector<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		if ((*item) != nullptr) {
			ret = ((*item)->Move(dt));

			if (App->render->IsOnCamera((int)((*item)->position.x), (int)((*item)->position.x), (*item)->size.x, (*item)->size.y)) {
				draw_entities.push_back(*item);
			}
		}
	}

	std::sort(draw_entities.begin(), draw_entities.end(), j1EntityFactory::SortByYPos);

	for (item = draw_entities.begin(); item != draw_entities.end(); ++item)
	{
		(*item)->Draw(texture);
		entities_drawn++;
	}

	draw_entities.clear();

	static char title[30];
	sprintf_s(title, 30, " | Entities drawn: %u", entities_drawn);

	return ret;
}

bool j1EntityFactory::PostUpdate()
{
	return false;
}

bool j1EntityFactory::CleanUp()
{
	bool ret = true;

	std::vector<j1Entity*>::iterator entitiesItem = entities.begin();

	while (entitiesItem != entities.end())
	{
		RELEASE(*entitiesItem);
		++entitiesItem;
	}
	entities.clear();

	entitiesInfo.clear();

	//unload texture

	return ret;
}

j1Entity* j1EntityFactory::CreateEntity(ENTITY_TYPE type, int poositionX, int positionY, std::string name)
{
	return nullptr;
}

void j1EntityFactory::DestroyEntity(j1Entity * entity)
{
	if (entity != nullptr)
	{
		//destroy collider


		for (std::vector<j1Entity*>::iterator item = entities.begin(); item != entities.end(); ++item) {
			if (*item == entity) {
				delete *item;
				*item = nullptr;
			}
		}
	}
}

bool j1EntityFactory::SortByYPos(const j1Entity * entity1, const j1Entity * entity2)
{
	return entity1->pivot.y + entity1->position.y < entity2->pivot.y + entity2->position.y;
}
