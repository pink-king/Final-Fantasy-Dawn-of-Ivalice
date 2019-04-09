#include "j1EntityFactory.h"
#include "j1Render.h"
#include "p2Log.h"
#include "EnemyTest.h"
#include "Enemy.h"
#include "j1BuffManager.h"
#include "j1Scene.h"
#include "LootEntity.h"
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
	for (; item != entities.end();)
	{
		if ((*item) != nullptr) 
		{
			if (!(*item)->to_delete)
			{
				ret = (*item)->Update(dt);
				ret = ((*item)->Move(dt));
				// updates entity associated tile positions tile and subtile
				(*item)->UpdateTilePositions();
				//LOG("entity subtile: %i,%i", (*item)->GetSubtilePos().x, (*item)->GetSubtilePos().y);

				draw_entities.push_back(*item);

				++item;
			}
			else
			{
				(*item)->CleanUp();
				delete(*item);
				(*item) = nullptr;
				item = entities.erase(item);
			}
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

	RELEASE_ARRAY(entitiesDataMap);


	//unload texture

	return ret;
}

j1Entity* j1EntityFactory::CreateEntity(ENTITY_TYPE type, int positionX, int positionY, std::string name)
{
	j1Entity* ret = nullptr; 

	std::vector<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		if (*item == nullptr)
			break;
	}
	switch (type)
	{
	case NO_TYPE:
		break;
	case ENTITY_STATIC:
		//ret = new
		break;
	case ENTITY_DYNAMIC:
		break;

	case ENEMY_TEST:
		/*ret = new EnemyTest(iPoint(positionX, positionY));
		ret->type = ENEMY_TEST;
		ret->name = name; 
		entities.push_back(ret);
		LOG("Created a entity");*/
		break;
	case LOOT:
		ret = new LootEntity(positionX, positionY);
		ret->type = LOOT;
		ret->name = name;
		entities.push_back(ret);
		LOG("From factory Loot Entity");
		break;
	default:
		break;
	}

	return ret;
}

Enemy * j1EntityFactory::CreateEnemy(EnemyType etype,iPoint pos, uint speed, uint tilesDetectionRange, uint attackRange)
{
	Enemy* ret = nullptr; 

	switch (etype)
	{
	case EnemyType::TEST:
		ret = new EnemyTest(pos, speed, tilesDetectionRange, attackRange); 
		entities.push_back(ret);
		break; 

	case EnemyType::MELEE:
		break; 

	case EnemyType::DISTANCE:
		break; 

	case EnemyType::TRAP:
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



bool j1EntityFactory::SortByYPos(const j1Entity * entity1, const j1Entity * entity2)
{
	return entity1->pivot.y + entity1->position.y < entity2->pivot.y + entity2->position.y;
}

void j1EntityFactory::CreateEntitiesDataMap(int width, int height)
{
	subtileWidth = width;
	subtileHeight = height;

	if (entitiesDataMap != nullptr)
	{
		//RELEASE(entitiesDataMap);
		RELEASE_ARRAY(entitiesDataMap);
	}

	entitiesDataMap = new entityDataMap[subtileWidth * subtileHeight];
	memset(entitiesDataMap, NULL, subtileWidth*subtileHeight);

	LOG("");
}

std::vector<j1Entity*>* j1EntityFactory::GetSubtileEntityVectorAt(const iPoint pos) const
{
	if (CheckSubtileMapBoundaries(pos))
		return &entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities;
	else
	{
		//LOG("data out of boundaries, ignoring");
		return nullptr;
	}
}

bool j1EntityFactory::isThisSubtileEmpty(const iPoint pos) const
{
	if (CheckSubtileMapBoundaries(pos))
		return !entitiesDataMap[GetSubtileEntityIndexAt(pos)].isEmpty();
	else
		return false;
}

int j1EntityFactory::GetSubtileEntityIndexAt(const iPoint pos) const
{
	return (pos.y * subtileWidth) + pos.x;
}

void j1EntityFactory::AssignEntityToSubtile(j1Entity* entity) const
{
	if (CheckSubtileMapBoundaries(entity->GetSubtilePos()))
		entitiesDataMap[GetSubtileEntityIndexAt(entity->GetSubtilePos())].entities.push_back(entity);
	else
		LOG("Trying to assign entity out of boundaries, ignoring");
}

bool j1EntityFactory::DeleteEntityFromSubtile(j1Entity* entity) const
{
	bool ret = false;

	int index = GetSubtileEntityIndexAt(entity->GetPreviousSubtilePos());

	std::vector<j1Entity*>::iterator entityIterator = entitiesDataMap[index].entities.begin();

	for (; entityIterator != entitiesDataMap[index].entities.end(); ++entityIterator)
	{
		if (*entityIterator == entity)
		{
			//LOG("found");
			entitiesDataMap[index].entities.erase(entityIterator);
			ret = true;
			break;
		}
	}

	return ret;
}

bool j1EntityFactory::isPlayerAdjacent(const iPoint & pos) const
{
	iPoint playerPos = player->GetSubtilePos(); 
	
	return (abs(playerPos.x - pos.x) <= 1 &&  abs(playerPos.y - pos.y) <=1 );
}

iPoint j1EntityFactory::TranslateToRelativePlayerPos(const iPoint & pos) const
{
	iPoint playerPos = player->GetSubtilePos(); 
	return iPoint((pos.x - playerPos.x) +1, (pos.y - playerPos.y) +1);
}

int j1EntityFactory::GetAdjacentIndex(const iPoint & pos) const
{
	return (pos.y * 3) + pos.x;
}

void j1EntityFactory::ReserveAdjacent(const iPoint& pos)
{
	if (isPlayerAdjacent(pos))
	{
		iPoint reserve = TranslateToRelativePlayerPos(pos);
		reservedAdjacentSubtiles[GetAdjacentIndex(reserve)] = true; 
		LOG("Reserved adjacent: %i x , %i y.", reserve.x, reserve.y);
	}
}

void j1EntityFactory::FreeAdjacent(const iPoint & pos)
{
	if (isPlayerAdjacent(pos))
	{
		iPoint reserve = TranslateToRelativePlayerPos(pos);
		reservedAdjacentSubtiles[GetAdjacentIndex(reserve)] = false;
		LOG("Released reserved adjacent: %i x , %i y.", reserve.x, reserve.y);
	}
}

bool j1EntityFactory::isThisSubtileReserved(const iPoint & pos) const
{
	if (isPlayerAdjacent(pos))
	{
		iPoint aux = TranslateToRelativePlayerPos(pos);
		return reservedAdjacentSubtiles[GetAdjacentIndex(aux)];
	}
	else return false; 
}

void j1EntityFactory::ReleaseAllReservedSubtiles()
{
	for (uint i = 0; i < 8; ++i)
	{
		reservedAdjacentSubtiles[i] = false; 
	}
	LOG("RELEASED ALL RESERVED SUBTILES");
}

bool j1EntityFactory::CheckSubtileMapBoundaries(const iPoint pos) const
{
	return (pos.x >= 0 && pos.x < subtileWidth &&
		pos.y >= 0 && pos.y < subtileHeight);
}

