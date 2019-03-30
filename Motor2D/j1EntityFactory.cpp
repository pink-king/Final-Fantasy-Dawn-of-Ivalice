#include "j1EntityFactory.h"
#include "j1Render.h"
#include "p2Log.h"
#include "EnemyTest.h"
#include "j1BuffManager.h"
#include "j1Scene.h"
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
		if (node.attribute("type").as_string() == "static")
			entities.push_back(CreateEntity(ENTITY_TYPE::ENTITY_STATIC, node.attribute("positionX").as_float(), node.attribute("positionY").as_float(), node.attribute("name").as_string()));

		else if (node.attribute("type").as_string() == "dinamic")
			entities.push_back(CreateEntity(ENTITY_TYPE::ENTITY_DYNAMIC, node.attribute("positionX").as_float(), node.attribute("positionY").as_float(), node.attribute("name").as_string()));
	}

	std::vector<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		(*item)->LoadEntitydata(node);
	}

	return ret;
}

bool j1EntityFactory::Start()
{
	std::vector<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		(*item)->Start();
	}
	//load texture
	return true;
}

bool j1EntityFactory::PreUpdate()
{
	bool ret = true;

	// logic / collisions
	std::vector<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		if ((*item) != nullptr)
		{
			ret = ((*item)->PreUpdate());
		}
	}

	return true;
}

bool j1EntityFactory::Update(float dt)
{


	bool ret = true;

	std::vector<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		if ((*item) != nullptr) 
		{
			ret = (*item)->Update(dt);
			if (ret)
				ret = ((*item)->Move(dt));

				draw_entities.push_back(*item);
			
		}
	}
	

	return ret;
}

bool j1EntityFactory::PostUpdate()
{
	
	std::vector<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		(*item)->PostUpdate();
	}
	std::sort(draw_entities.begin(), draw_entities.end(), j1EntityFactory::SortByYPos);

	std::vector<j1Entity*>::iterator drawItem = draw_entities.begin();
	for (; drawItem != draw_entities.end(); ++drawItem)
	{
		(*drawItem)->Draw();
		if(App->scene->debug)
			Debug(*drawItem);
	}

	draw_entities.clear();



	return true;
}

bool j1EntityFactory::CleanUp()
{
	bool ret = true;

	std::vector<j1Entity*>::reverse_iterator entitiesItem = entities.rbegin();

	while (entitiesItem != entities.rend())
	{
		(*entitiesItem)->CleanUp();
		RELEASE(*entitiesItem);
		++entitiesItem;
	}
	entities.clear();


	//unload texture

	return ret;
}

j1Entity* j1EntityFactory::CreateEntity(ENTITY_TYPE type, int positionX, int positionY, std::string name)
{
	j1Entity* ret = nullptr; 

	switch (type)
	{
	case NO_TYPE:
		break;
	case ENTITY_STATIC:
		break;
	case ENTITY_DYNAMIC:
		break;

	case ENEMY_TEST:
		ret = new EnemyTest(iPoint(positionX, positionY));
		ret->type = ENEMY_TEST;
		ret->name = name; 
		entities.push_back(ret);
		LOG("Created a entity");
		break;

	default:
		break;
	}

	return ret;
}

void j1EntityFactory::Debug(j1Entity* ent)
{
	fPoint entityPivotPos = ent->GetPivotPos();
	//App->render->DrawCircle(entityPivotPos.x, entityPivotPos.y, 3, 0, 255, 0,255 ,true); //TODO: improve drawcircle render (scale,camera)
	SDL_Rect section = { entityPivotPos.x - 1, entityPivotPos.y - 1, 3,3 };
	App->render->DrawQuad(section, 0, 255, 0, 255, true, true);
	//LOG("position:%f,%f", ent->position.x, ent->position.y);
}

PlayerEntityManager* j1EntityFactory::CreatePlayer(iPoint position)
{
	player = new PlayerEntityManager(position);

	if (player != nullptr)
	{
		entities.push_back(player);
		return player;
	}
	
	LOG("Failed to create player system");
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
