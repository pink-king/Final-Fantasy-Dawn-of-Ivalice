#include "j1EntityFactory.h"
#include "j1Render.h"
#include "p2Log.h"
#include "EnemyTest.h"
#include "EnemyBomb.h"
#include "Enemy.h"
#include "j1BuffManager.h"
#include "j1Scene.h"
#include "LootEntity.h"
#include "BasicArrow.h"
#include "FireArrow.h"
#include "MagicBolt.h"
#include "ContagiousFireArrow.h"
#include "DeathCircle.h"
#include "Emmiter.h"
#include "EmmiterArrows.h"
#include "Medusa.h"
#include "Tornado.h"
#include "Brofiler/Brofiler.h"
#include "EarthShaker.h"
#include "DumbTrigger.h"
#include <ctime>
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

	// Load Textures 

	//assetsAtlasTex = App->tex->Load("maps/iso-64x64-building.png");
	assetsAtlasTex = App->tex->Load("maps/Tilesets/Level 1/tileset_level_1.png");
	enemyZombieTex = App->tex->Load("textures/enemies/enemyZombie.png");
	enemyBombTex = App->tex->Load("textures/enemies/enemyBomb.png");
	debugsubtileTex = App->tex->Load("maps/tile_32x32_2.png");
	arrowsTexture = App->tex->Load("textures/spells/Shara_attacks/arrowTypes.png");
	ritzUltimateTex = App->tex->Load("textures/spells/Ritz_ultimate/Ritz_ultimate_WIP.png");
	ritzBasicTex = App->tex->Load("textures/spells/Ritz_attacks/ritzBasicTest.png");
	marcheTornadoTex = App->tex->Load("textures/spells/Marche_attacks/Marche_tornado_twisterSpin.png");
	lootItemsTex = App->tex->Load("textures/loot/loot_items.png");

	// Load SFX
	lootGroundSFX = App->audio->LoadFx("audio/fx/loot/lootgrounded.wav");
	potionGroundSFX = App->audio->LoadFx("audio/fx/loot/potion_grounded.wav");
	coinGroundedSFX = App->audio->LoadFx("audio/fx/loot/coinGrounded.wav");
	swapCharSFX = App->audio->LoadFx("audio/fx/Player/swapChar.wav");
	enemySpawn = App->audio->LoadFx("audio/fx/enemySpawnTest.wav");
	goblinDetection = App->audio->LoadFx("audio/fx/goblin_detection.wav");

	stepSFX = App->audio->LoadFx("audio/fx/Player/footstep-on-stone.wav");

	marcheDamaged = App->audio->LoadFx("audio/fx/Player/Marche/Marche_damaged.wav");
	marcheBasic = App->audio->LoadFx("audio/fx/Player/Marche/marche_basic1Grunt.wav");
	marcheBasic2 = App->audio->LoadFx("audio/fx/Player/Marche/marche_basic2Grunt.wav");
	marcheAbility2 = App->audio->LoadFx("audio/fx/Player/Marche/marche_tornado.wav");
	marcheUltimateScream = App->audio->LoadFx("audio/fx/Player/Marche/marcheUltimate.wav");
	marcheAbility1 = App->audio->LoadFx("audio/fx/Player/Marche/marche_ability1.wav");
	marcheEarthShakeSFX = App->audio->LoadFx("audio/fx/Player/Marche/marche_earthShake.wav");
	marcheTornadoExplosion = App->audio->LoadFx("audio/fx/Player/Marche/marche_tornado_explosion.wav");

	RitzDamaged = App->audio->LoadFx("audio/fx/Player/Ritz/Ritz_damaged.wav");
	RitzBasic = App->audio->LoadFx("audio/fx/Player/Ritz/ritz_basic.wav");
	RitzBasicHit = App->audio->LoadFx("audio/fx/Player/Ritz/ritz_basic_hit.wav");
	RitzMedusa = App->audio->LoadFx("audio/fx/Player/Ritz/ritz_medusa.wav");
	RitzAbility2 = App->audio->LoadFx("audio/fx/Player/Ritz/Ritz_Ability2.wav");
	RitzAbility1 = App->audio->LoadFx("audio/fx/Player/Ritz/ritz_teleport.wav");
	RitzUltimate = App->audio->LoadFx("audio/fx/Player/Ritz/ritz_ultimateTest2.wav");
	

	SharaDamaged = App->audio->LoadFx("audio/fx/Player/Shara/Shara_damaged.wav");
	sharaBasic = App->audio->LoadFx("audio/fx/Player/Shara/sharaBasic.wav");
	basicBodyImp = App->audio->LoadFx("audio/fx/Player/Shara/arrow_impactBody.wav");
	
	strech_Shoot = App->audio->LoadFx("audio/fx/Player/Shara/strech&shoot.wav");
	SharaUltimateWoosh = App->audio->LoadFx("audio/fx/Player/Shara/Shara_ultimate_woosh.wav");
	emitter_explodeFire = App->audio->LoadFx("audio/fx/Player/Shara/SharaUltimateGrounding.wav");
	sharaAbility1 = App->audio->LoadFx("audio/fx/Player/Shara/shara_ability1.wav");
	sharaBasic_ImpactsWall = App->audio->LoadFx("audio/fx/Player/Shara/SharaBasicImpact.wav");
	sharaAbility1_ImpactsWall = App->audio->LoadFx("audio/fx/Player/Shara/sharaAbility1_impactswall.wav");
	sharaAbility2_ImpactsWall = App->audio->LoadFx("audio/fx/Player/Shara/sharaAbility2_impactswall.wav");
	sharaAbility2_shoot = App->audio->LoadFx("audio/fx/Player/Shara/SharaAbility2Shoot.wav");

	dash = App->audio->LoadFx("audio/fx/Player/dash.wav");

	goblinDamaged = App->audio->LoadFx("audio/fx/goblin_damaged.wav");
	goblinDeath = App->audio->LoadFx("audio/fx/goblin_death.wav");
	goblinLaugh = App->audio->LoadFx("audio/fx/goblin_laugh.wav");

	BombDeathSFX = App->audio->LoadFx("audio/fx/Enemies/bombDeath.wav");
	bombgetHitSFX = App->audio->LoadFx("audio/fx/Enemies/bombgetHit.wav");


	LoadSpawnGroups();

	gen.seed(rd()); //Standard mersenne_twister_engine seeded with rd()
	justGold = false;
	return true;
}

bool j1EntityFactory::PreUpdate()
{
	BROFILER_CATEGORY("Entities PreUpdate", Profiler::Color::LavenderBlush);

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
	BROFILER_CATEGORY("Entities Update", Profiler::Color::Fuchsia);
	
	std::vector<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end();)
	{
		if ((*item) != nullptr) 
		{
			bool createLoot = false;
			iPoint enemypos;

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
				//if entit is diffetent to player create loot
				if ((*item)->type != ENTITY_TYPE::PLAYER && (*item)->type != ENTITY_TYPE::LOOT && (*item)->type != ENTITY_TYPE::PROJECTILE) //needs to be loot too, otherwise if player collects loot thereis teh cnahce to create loot again
				{
					createLoot = true;
					enemypos = {GetEnemySubtile((*item)).x, GetEnemySubtile((*item)).y };
				}
				App->buff->entitiesTimeDamage.remove(*item);
				(*item)->CleanUp();
				delete(*item); 
				(*item) = nullptr;
				item = entities.erase(item);
			}
			if (createLoot)
			{
				//CreateLoot(SetLootPos(enemypos.x, enemypos.y).x, SetLootPos(enemypos.x, enemypos.y).y);
				CreateEntity(ENTITY_TYPE::LOOT, SetLootPos(enemypos.x, enemypos.y).x, SetLootPos(enemypos.x, enemypos.y).y, "lootitem");
				justGold = true;
				CreateGold(SetLootPos(enemypos.x, enemypos.y).x, SetLootPos(enemypos.x, enemypos.y).y);
				


			}
			
		}


	}

	return ret;
}

bool j1EntityFactory::PostUpdate()
{
	BROFILER_CATEGORY("Entities PostUpdate", Profiler::Color::BurlyWood);

	std::vector<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		(*item)->PostUpdate();
	}
	std::sort(draw_entities.begin(), draw_entities.end(), j1EntityFactory::SortByYPos);

	BROFILER_CATEGORY("Entities Drawing", Profiler::Color::BurlyWood);

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
	App->tex->UnLoad(texture);
	App->tex->UnLoad(assetsAtlasTex);
	App->tex->UnLoad(enemyZombieTex);
	App->tex->UnLoad(enemyBombTex);
	App->tex->UnLoad(debugsubtileTex);
	App->tex->UnLoad(arrowsTexture);
	App->tex->UnLoad(ritzUltimateTex);
	App->tex->UnLoad(ritzBasicTex);
	App->tex->UnLoad(marcheTornadoTex);
	App->tex->UnLoad(lootItemsTex); 

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
		//ret = new
		break;
	case ENTITY_DYNAMIC:
		break;
	case TRIGGERWIN:
		ret = new DumbTrigger({ positionX, positionY });
		ret->type = type; 
		entities.push_back(ret);
		break; 

	case ENEMY_TEST:
		/*ret = new EnemyTest(iPoint(positionX, positionY));
		ret->type = ENEMY_TEST;
		ret->name = name; 
		entities.push_back(ret);
		LOG("Created a entity");*/
		break;
	case LOOT:
		ret = CreateLootType(positionX, positionY);
		if (ret != nullptr)
		{
			ret->type = LOOT;
			ret->name = name;
			LoadLootData((LootEntity*)ret, App->config);
			
			entities.push_back(ret);
		}

			//GenerateDescriptionForLootItem((LootEntity*)ret);
		
		LOG("From factory Loot Entity");
		break;
	default:
		break;
	}

	return ret;
}

Enemy * j1EntityFactory::CreateEnemy(EnemyType etype,iPoint pos, bool dummy)
{
	Enemy* ret = nullptr; 

	switch (etype)
	{
	case EnemyType::BOMB:
		ret = new EnemyBomb(pos, dummy);
		entities.push_back(ret);
		break;
	case EnemyType::TEST:
		ret = new EnemyTest(pos, dummy); 
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

void j1EntityFactory::CreateEnemiesGroup(std::vector<EnemyType> enemyTypes, SDL_Rect zone, uint minNum, uint maxNum)
{
	uint numEnemies = CreateRandomBetween(minNum, maxNum);

	uint numBombs = 0;
	uint numTests = 0;
	uint cont = 0;

	uint bombProbs = 3;
	uint testProbs = 8;

	while (cont < numEnemies)
	{
		for (std::vector<EnemyType>::iterator typeIter = enemyTypes.begin(); typeIter != enemyTypes.end(); typeIter++)
		{
			j1Entity* ret = nullptr;
			iPoint spawnPos = { zone.x + (int)CreateRandomBetween(0, zone.w), zone.y + (int)CreateRandomBetween(0,zone.h) };
			spawnPos = App->map->IsoToWorld(spawnPos.x, spawnPos.y);
			spawnPos.x = spawnPos.x * 2;
			LOG("Spawn Position: %i, %i", spawnPos.x, spawnPos.y);
			switch (*typeIter)
			{
			case EnemyType::BOMB:
				if (CreateRandomBetween(1, 10) <= bombProbs)
				{
					// Last paramater is dummy
					ret = CreateEnemy(EnemyType::BOMB, spawnPos, false);

					if (ret != nullptr)
					{
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, ret, "\0", CreateRandomBetween(0, 15));
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, ret, "\0", CreateRandomBetween(0, 20));
						numBombs++;
						cont++;
					}
				}

				break;

			case EnemyType::TEST:
				if (CreateRandomBetween(1, 10) <= testProbs && cont < numEnemies)
				{	
					// Last paramater is dummy
					ret = CreateEnemy(EnemyType::TEST, spawnPos, false);

					if (ret != nullptr)
					{
					App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, ret, "\0", CreateRandomBetween(0, 10));
					App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, ret, "\0", CreateRandomBetween(0, 28));
					numTests++;
					cont++;
					}
				}
				break;

			default:
				break;
			}
		}
	}
	LOG("Created %i Enemies", numEnemies);
	LOG("Ceated %i TESTS", numTests);
	LOG("Ceated %i BOMBS", numBombs);


}

void j1EntityFactory::LoadSpawnGroups()
{
	for (std::vector<GroupInfo>::iterator iter = spawngroups.begin(); iter != spawngroups.end(); iter++){
		CreateEnemiesGroup((*iter).types, (*iter).zone, (*iter).minEnemies, (*iter).maxEnemies);
	}
	spawngroups.clear();
}

j1Entity* j1EntityFactory::CreateArrow(fPoint pos, fPoint destination, uint speed, const j1Entity* owner, PROJECTILE_TYPE type, uint lifeTime)
{
	j1Entity* ret = nullptr;
	switch (type)
	{
	case PROJECTILE_TYPE::BASIC_ARROW:
		ret = new BasicArrow(pos, destination, speed, owner);
		entities.push_back(ret);
		break;
	case PROJECTILE_TYPE::CONTAGIOUS_ARROW:
		ret = new ContagiousFireArrow(pos, destination, speed, owner);
		entities.push_back(ret);
		break;
	case PROJECTILE_TYPE::FIRE_ARROW:
		ret = new FireArrow(pos, destination, speed, owner);
		entities.push_back(ret);
		break;

	case PROJECTILE_TYPE::MAGIC_BOLT:
		ret = new MagicBolt(pos, destination, speed, owner); 
		entities.push_back(ret); 
		break; 

	case PROJECTILE_TYPE::DEATH_CIRCLE:
		ret = new DeathCircle(pos, owner);
		entities.push_back(ret); 
		break; 

	case PROJECTILE_TYPE::EMMITER_ARROWS:
		ret = new EmmiterArrows(pos, destination, speed, owner, lifeTime);
		entities.push_back(ret);
		break;

	case PROJECTILE_TYPE::EMMITER:
		ret = new Emmiter(pos, owner);
		entities.push_back(ret);
		break;

	case PROJECTILE_TYPE::MEDUSA:
		ret = new Medusa(pos, owner);
		entities.push_back(ret);
		break;

	case PROJECTILE_TYPE::TORNADO:
		ret = new Tornado(pos, destination, speed, owner);
		entities.push_back(ret);
		break;

	case PROJECTILE_TYPE::EARTH_SHAKER:
		ret = new EarthShaker(pos, owner);
		entities.push_back(ret);
		break;

	case PROJECTILE_TYPE::NO_ARROW:
		break;

	default:
		break;
	}
	

	return ret;
}

LootEntity* j1EntityFactory::CreateLoot(/*LOOT_TYPE lType,*/ int posX, int posY)
{
	LootEntity* ret = nullptr;
	 

	switch (WillDrop())
	{
	case LOOT_TYPE::EQUIPABLE:
		ret = new Equipable(posX, posY);
			LoadLootData(ret, App->config);
			entities.push_back(ret);
			break;

	case LOOT_TYPE::CONSUMABLE:
		ret = new Consumable(posX, posY);
		//ret->objectType = OBJECT_TYPE::GOLD;
		LoadLootData(ret, App->config);
		entities.push_back(ret);
		break;
	}
	return ret;
	
}

LootEntity* j1EntityFactory::CreateGold(int posX, int posY)
{
	LootEntity* ret = nullptr;
	if (GetRandomValue(1, 4) == 1)
	{
		ret = new Consumable(posX, posY);
		LoadLootData(ret, App->config);
		entities.push_back(ret);
	}
	return nullptr;
}

uint j1EntityFactory::CreateRandomBetween(uint min, uint max)
{
	std::uniform_real_distribution<float> dis(min, max);

	return (uint)dis(gen);

	/*uint x = min + (std::rand() % (max - min));*/
	//return x; 
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

bool j1EntityFactory::isThisSubtileEnemyFree(const iPoint pos) const
{
	bool ret = true; 

	if (!isThisSubtileEmpty(pos))
	{
		std::vector<j1Entity*>::iterator entityIterator = entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities.begin();
		for (; entityIterator != entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities.end(); ++entityIterator)
		{
			if ((*entityIterator)->type == ENTITY_TYPE::ENEMY_TEST || (*entityIterator)->type == ENTITY_TYPE::ENEMY_BOMB) // || other enemy types 
			{
				ret = false;
				break;
			}
		}
	}

	return ret;
}

bool j1EntityFactory::isThisSubtileTriggerFree(const iPoint pos) const
{

	bool ret = true;

	if (!isThisSubtileEmpty(pos))
	{
		std::vector<j1Entity*>::iterator entityIterator = entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities.begin();
		for (; entityIterator != entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities.end(); ++entityIterator)
		{
			if ((*entityIterator)->type == ENTITY_TYPE::TRIGGERWIN) 
			{
				ret = false;
				break;
			}
		}
	}

	return ret;
}

int j1EntityFactory::GetSubtileEntityIndexAt(const iPoint pos) const
{
	return (pos.y * subtileWidth) + pos.x;
}

bool j1EntityFactory::areAllSubtilesReserved() const
{
	bool ret = true;
	for (uint i = 0; i < 8; ++i)
	{
		if (reservedAdjacentSubtiles[i] == false)
		{
			if (i != 4)
			{
				ret = false;
				break;
			}
		}
	}

	return ret;
}


void j1EntityFactory::AssignEntityToSubtile(j1Entity* entity) const
{
	if (CheckSubtileMapBoundaries(entity->GetSubtilePos()))
		entitiesDataMap[GetSubtileEntityIndexAt(entity->GetSubtilePos())].entities.push_back(entity);
	else
		LOG("Trying to assign entity out of boundaries, ignoring");
}

void j1EntityFactory::AssignEntityToSubtilePos(j1Entity * entity, iPoint subtile)
{
	if (CheckSubtileMapBoundaries(subtile))
		entitiesDataMap[GetSubtileEntityIndexAt(subtile)].entities.push_back(entity);

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

bool j1EntityFactory::DeleteEntityFromSubtilePos(j1Entity * entity, iPoint subtile)
{
	bool ret = false; 

	int index = GetSubtileEntityIndexAt(subtile);

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

void j1EntityFactory::CreateAsset(EnvironmentAssetsTypes type, iPoint worldPos, SDL_Rect atlasSpriteRect)
{
	j1Entity* assetEntity = nullptr;

	switch (type)
	{
	case EnvironmentAssetsTypes::NO_TYPE:
		break;
	case EnvironmentAssetsTypes::WALL:
		assetEntity = new j1Entity(worldPos, atlasSpriteRect);
		entities.push_back(assetEntity);
		break;
	case EnvironmentAssetsTypes::WALL1:
		break;		
	case EnvironmentAssetsTypes::MAX:
		break;
	default:
		break;
	}
}

bool j1EntityFactory::LoadLootData(LootEntity * lootEntity, pugi::xml_node & config)
{
	int randID = 0;
	int id;
	int min, max;
	switch (lootEntity->GetObjectType())
	{
	case OBJECT_TYPE::POTIONS:
		for (auto node : config.child("loot").child("potions").children("health_potion"))
		{
			lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, App->entityFactory->player, "\0", ELEMENTAL_TYPE::NO_ELEMENT, ROL::DEFENCE_ROL, node.attribute("value").as_int(), lootEntity);
			LOG("looking for potions");
		}
		break;

	case OBJECT_TYPE::GOLD:

		min = config.child("loot").child("gold").attribute("min").as_int();
		max = config.child("loot").child("gold").attribute("max").as_int();
		lootEntity->price = GetRandomValue(min, max);
		LOG("gold amount %i", lootEntity->price);
		break;

	case OBJECT_TYPE::WEAPON_OBJECT:

		randID = GetRandomValue(1, 3);

		switch (GetRandomValue(1, 12))
		{
		case 1:
			lootEntity->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
			lootEntity->level = 1;
			break;
		case 2:
			lootEntity->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;
			lootEntity->level = 1;
			break;
		case 3:
			lootEntity->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
			lootEntity->level = 1;
			break;
		case 4:
			lootEntity->elemetalType = ELEMENTAL_TYPE::NO_ELEMENT;
			lootEntity->level = 1;
			break;
		case 5:
			lootEntity->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
			lootEntity->level = 2;
			break;
		case 6:
			lootEntity->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;
			lootEntity->level = 2;
			break;
		case 7:
			lootEntity->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
			lootEntity->level = 2;
			break;
		case 8:
			lootEntity->elemetalType = ELEMENTAL_TYPE::NO_ELEMENT;
			lootEntity->level = 2;
			break;
		case 9:
			lootEntity->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
			lootEntity->level = 3;
			break;
		case 10:
			lootEntity->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;
			lootEntity->level = 3;
			break;
		case 11:
			lootEntity->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
			lootEntity->level = 3;
			break;
		case 12:
			lootEntity->elemetalType = ELEMENTAL_TYPE::ALL_ELEMENTS;
			lootEntity->level = 3;
			break;
		default:
			break;
		}

		switch (lootEntity->GetEquipable())
		{
		case EQUIPABLE_TYPE::SWORD:
			lootEntity->character = App->entityFactory->player->GetMarche();
			for (auto node : config.child("loot").child("equipable").child("sword").children("equipment"))
			{
				//weapon type
				id = node.attribute("id").as_int();

				if (id == randID)
				{
					lootEntity->lootname = node.attribute("name").as_string();

					if (id == 1)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(20, 25), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(75, 100)* 0.01, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(26, 30), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(50, 74)* 0.01, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(31, 35), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(25, 49)* 0.01, lootEntity);
						}


					}

					else if (id == 2)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 20) * 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(2, 8), lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(16, 30) * 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(9, 14), lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(31, 45) * 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 22), lootEntity);
						}

					}
					else
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(1, 5), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(10, 15)* 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(75, 100)* 0.01, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(6, 10), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(16, 20)* 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(50, 74)* 0.01, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(11, 15), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(21, 25)* 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(25, 49)* 0.01, lootEntity);
						}
					}
				}
			}
			break;
		case EQUIPABLE_TYPE::BOW:
			lootEntity->character = App->entityFactory->player->GetShara();
			for (auto node : config.child("loot").child("equipable").child("bow").children("equipment"))
			{
				//weapon type
				id = node.attribute("id").as_int();

				if (id == randID)
				{
					lootEntity->lootname = node.attribute("name").as_string();

					if (id == 1)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 15), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(75, 100)* 0.01, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(16, 20), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(50, 74)* 0.01, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(21, 25), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(50, 49)* 0.01, lootEntity);
						}


					}

					else if (id == 2)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 20) * 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(2, 8)*0.1, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(16, 30) * 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(4, 7)*0.1, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(31, 45) * 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(8, 12)*0.1, lootEntity);
						}

					}
					else
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 15), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(75, 100)* 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(0, 4)*0.1, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(16, 20), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(50, 74)* 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(4, 8)*0.1, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(21, 25), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(25, 49)* 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(8, 12)*0.1, lootEntity);
						}
					}
				}
			}
			break;

		case EQUIPABLE_TYPE::ROD:
			lootEntity->character = App->entityFactory->player->GetRitz();
			for (auto node : config.child("loot").child("equipable").child("rod").children("equipment"))
			{
				//weapon type
				id = node.attribute("id").as_int();

				if (id == randID)
				{
					lootEntity->lootname = node.attribute("name").as_string();

					if (id == 1)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 20), lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(21, 30), lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(31, 40), lootEntity);
						}
					}

					else if (id == 2)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(5, 10), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(75, 100) * 0.01, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(7, 17), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(50, 74) * 0.01, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(16, 26), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(25, 49) * 0.01, lootEntity);
						}

					}
					else
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 20), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(65, 85) * 0.01, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(21, 30), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(65, 85) * 0.01, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(31, 40), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(65, 85) * 0.01, lootEntity);
						}
					}
				}
			}
			break;
		default:
			break;
		}
	case OBJECT_TYPE::ARMOR_OBJECT:
		randID = GetRandomValue(1, 3);

		switch (GetRandomValue(1, 12))
		{
		case 1:
			lootEntity->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
			lootEntity->level = 1;
			break;
		case 2:
			lootEntity->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;
			lootEntity->level = 1;
			break;
		case 3:
			lootEntity->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
			lootEntity->level = 1;
			break;
		case 4:
			lootEntity->elemetalType = ELEMENTAL_TYPE::NO_ELEMENT;
			lootEntity->level = 1;
			break;
		case 5:
			lootEntity->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
			lootEntity->level = 2;
			break;
		case 6:
			lootEntity->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;
			lootEntity->level = 2;
			break;
		case 7:
			lootEntity->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
			lootEntity->level = 2;
			break;
		case 8:
			lootEntity->elemetalType = ELEMENTAL_TYPE::NO_ELEMENT;
			lootEntity->level = 2;
			break;
		case 9:
			lootEntity->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
			lootEntity->level = 3;
			break;
		case 10:
			lootEntity->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;
			lootEntity->level = 3;
			break;
		case 11:
			lootEntity->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
			lootEntity->level = 3;
			break;
		case 12:
			lootEntity->elemetalType = ELEMENTAL_TYPE::ALL_ELEMENTS;
			lootEntity->level = 3;
			break;
		default:
			break;
		}

		switch (lootEntity->GetEquipable())
		{
		case EQUIPABLE_TYPE::ARMOR:
			lootEntity->character = App->entityFactory->player->GetMarche();
			for (auto node : config.child("loot").child("equipable").child("armor").children("equipment"))
			{
				//weapon type
				id = node.attribute("id").as_int();

				if (id == randID)
				{
					lootEntity->lootname = node.attribute("name").as_string();

					if (id == 1)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(10, 15), lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(17, 27), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(15, 20), lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(25, 35), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(20, 25), lootEntity);
						}


					}

					else if (id == 2)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(2, 6)*0.1, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(18, 23), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::VELOCITY, GetRandomValue(7, 11)*0.1, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(20, 26), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::VELOCITY, GetRandomValue(12, 15)*0.1, lootEntity);
						}

					}
					else
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(2, 5)*0.1, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(10, 15), lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(17, 27), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::VELOCITY, GetRandomValue(6, 10)*0.1, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(15, 20), lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(25, 35), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::VELOCITY, GetRandomValue(11, 13)*0.1, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(20, 25), lootEntity);
						}
					}
				}
			}
			break;

		case EQUIPABLE_TYPE::VEST:

			lootEntity->character = App->entityFactory->player->GetShara();
			for (auto node : config.child("loot").child("equipable").child("vest").children("equipment"))
			{
				//weapon type
				id = node.attribute("id").as_int();

				if (id == randID)
				{
					lootEntity->lootname = node.attribute("name").as_string();

					if (id == 1)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(5, 10), lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(17, 27), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(11, 27), lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(25, 35), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(25, 30), lootEntity);
						}


					}

					else if (id == 2)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(2, 9)*0.1, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(18, 23), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::VELOCITY, GetRandomValue(9, 14)*0.1, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(20, 26), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::VELOCITY, GetRandomValue(15, 22)*0.1, lootEntity);
						}

					}
					else
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(2, 5)*0.1, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(10, 15), lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(17, 27), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::VELOCITY, GetRandomValue(9, 14)*0.1, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(15, 20), lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(25, 35), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::VELOCITY, GetRandomValue(15, 22)*0.1, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(20, 25), lootEntity);
						}
					}
				}
			}
			break;

		case EQUIPABLE_TYPE::MANTLE:

			lootEntity->character = App->entityFactory->player->GetRitz();
			for (auto node : config.child("loot").child("equipable").child("vest").children("equipment"))
			{
				//weapon type
				id = node.attribute("id").as_int();

				if (id == randID)
				{
					lootEntity->lootname = node.attribute("name").as_string();

					if (id == 1)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(5, 10), lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(17, 27), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(11, 27), lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(25, 35), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(25, 30), lootEntity);
						}


					}

					else if (id == 2)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(2, 9)*0.1, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(18, 23), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::VELOCITY, GetRandomValue(9, 14)*0.1, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(20, 26), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::VELOCITY, GetRandomValue(15, 22)*0.1, lootEntity);
						}

					}
					else
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(2, 5)*0.1, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(10, 15), lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(17, 27), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::VELOCITY, GetRandomValue(9, 14)*0.1, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(15, 20), lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(25, 35), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::VELOCITY, GetRandomValue(15, 22)*0.1, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(20, 25), lootEntity);
						}
					}
				}
			}
			break;
		}
		break;
	}
	return true;
}

int j1EntityFactory::GetRandomValue(int min, int max)
{
	static std::default_random_engine generator;
	std::uniform_int_distribution<int> range(min, max);
	int ret_value = range(generator);

	LOG("RANDOM VALUE %i", ret_value);
	//LOG("value %i", ret_value);

	return ret_value;
}

j1Entity * j1EntityFactory::CreateLootType(int x, int y)
{
	j1Entity * ret = nullptr;

	switch (WillDrop())
	{
	case LOOT_TYPE::CONSUMABLE:

		ret = new Consumable(x, y);
		break;

	case LOOT_TYPE::EQUIPABLE:
		ret = new Equipable(x, y);
		break;

	default:
		break;
	}

	return ret;
}

LOOT_TYPE j1EntityFactory::WillDrop()
{
	int randvalue = GetRandomValue(1, 100);

	if (randvalue <= lootChance)
	{
		if (randvalue <= 5)
			return  LOOT_TYPE::CONSUMABLE;

		else
			return  LOOT_TYPE::EQUIPABLE;


	}

	else return LOOT_TYPE::NO_LOOT;
	
}

iPoint j1EntityFactory::GetEnemySubtile(j1Entity * enemy)
{
	return  enemy->GetSubtilePos();

}

iPoint j1EntityFactory::SetLootPos(int x, int y)
{
	return App->map->SubTileMapToWorld(x, y);
}


void j1EntityFactory::GenerateDescriptionForLootItem(LootEntity* lootItem)
{

	SDL_Rect destRect = App->scene->lootPanelRect;
	iPoint pos(App->render->WorldToScreen(lootItem->Getoriginpos().x, lootItem->Getoriginpos().y));   // pos or origin pos ? 

	if (lootItem->GetType() == LOOT_TYPE::EQUIPABLE)
	{
		if (lootItem->GetObjectType() == OBJECT_TYPE::WEAPON_OBJECT)
		{


			float attack, resistance;
			attack = resistance = 0.0f;



			std::vector<Buff*>::iterator iter = lootItem->stats.begin();
			for (; iter != lootItem->stats.end(); ++iter)
			{
				if ((*iter)->GetRol() == ROL::ATTACK_ROL)
				{
					attack = (*iter)->GetValue();
				}
				else if ((*iter)->GetRol() == ROL::DEFENCE_ROL)
				{
					resistance = (*iter)->GetValue();
				}

			}

			lootItem->MyDescription = App->gui->AddDescriptionToWeapon(pos, lootItem->lootname, &destRect, &lootItem->loot_rect, attack, resistance, lootItem->level, lootItem, App->scene->inGamePanel);


			// add the icon image in the description, pass it the same texture as loot, and print it from that texture

			lootItem->MyDescription->iconImage = App->gui->AddSpecialImage(iPoint(0, 0), &lootItem->loot_rect, lootItem->MyDescription, lootItem->entityTex);
			lootItem->MyDescription->iconImage->printFromLoot = true;
			lootItem->MyDescription->iconImage->scaleFactor = 4.0f;

		}
		else if (lootItem->GetObjectType() == OBJECT_TYPE::ARMOR_OBJECT || lootItem->GetObjectType() == OBJECT_TYPE::HEAD_OBJECT)
		{


			int defense, HP, velocity;
			defense = HP = velocity = 666;

			std::vector<Buff*>::iterator iter = lootItem->stats.begin();
			for (; iter != lootItem->stats.end(); ++iter)
			{
				if ((*iter)->GetRol() == ROL::DEFENCE_ROL)
				{
					defense = (int)(*iter)->GetValue();
				}
				else if ((*iter)->GetRol() == ROL::HEALTH)
				{
					HP = (int)(*iter)->GetValue();
				}
				else if ((*iter)->GetRol() == ROL::VELOCITY)
				{
					velocity = (int)(*iter)->GetValue();
				}

			}

			EquipmentStatType data;
			data.HP = HP;
			data.velocity = velocity;

			lootItem->MyDescription = App->gui->AddDescriptionToEquipment(pos, lootItem->lootname, &destRect, &lootItem->loot_rect, defense, data, lootItem->level, lootItem, App->scene->inGamePanel);


			// add the icon image in the description, pass it the same texture as loot, and print it from that texture

			lootItem->MyDescription->iconImage = App->gui->AddSpecialImage(iPoint(0, 0), &lootItem->loot_rect, lootItem->MyDescription, lootItem->entityTex);
			lootItem->MyDescription->iconImage->printFromLoot = true;
			lootItem->MyDescription->iconImage->scaleFactor = 4.0f;



		}

	}

	else if (lootItem->GetType() == LOOT_TYPE::CONSUMABLE)
	{                                                       // TODO: Also capture the potion duration when available

		uint HP = 0;

		std::vector<Buff*>::iterator iter = lootItem->stats.begin();
		for (; iter != lootItem->stats.end(); ++iter)
		{
			if ((*iter)->GetRol() == ROL::DEFENCE_ROL)
			{
				HP = (*iter)->GetValue();
			}

		}

		lootItem->MyDescription = App->gui->AddDescriptionToPotion(pos, lootItem->lootname, &destRect, &lootItem->loot_rect, "default", iPoint(HP, 0), lootItem, App->scene->inGamePanel);

		// add the icon image in the description, pass it the same texture as loot, and print it from that texture

		lootItem->MyDescription->iconImage = App->gui->AddSpecialImage(iPoint(0, 0), &lootItem->loot_rect, lootItem->MyDescription, lootItem->entityTex);
		lootItem->MyDescription->iconImage->printFromLoot = true;
		lootItem->MyDescription->iconImage->scaleFactor = 6.0f;
	}



	// hide all elements until the item is focused by the Corsshair 

	//lootItem->MyDescription->HideAllElements(true);


}

void j1EntityFactory::DoDescriptionComparison(LootEntity * lootItem)
{

	if (lootItem->spawnedDescription)
	{
			if (lootItem->MyDescription->hasToCompare)
			{
				lootItem->MyDescription->ChangeComparisonLabels();   // this compares and puts the "compare" boolean to false
			}

	}
	

}