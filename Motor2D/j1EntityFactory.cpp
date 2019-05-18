#include "j1EntityFactory.h"
#include "j1Render.h"
#include "p2Log.h"
#include "EnemyTest.h"
#include "EnemyBomb.h"
#include "EnemyArcher.h"
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
#include "BossEmmiter.h"
#include "BossEmmiterArrow.h"
#include "Portal.h"
#include "LobbyPortal.h"
#include "EnemyProjectile.h"
#include "WinTrigger.h"
#include "NoWalkableTrigger.h"
#include "ExitPortal.h"
#include "Brofiler/Brofiler.h"
#include "EarthShaker.h"
#include "WaveManager.h"
#include "GolemProjectile.h"
#include "j1PathFinding.h"
#include <ctime>
#include <algorithm>
#include "Boss_Flower.h"



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
	enemyGolemTex = App->tex->Load("textures/enemies/enemyGolemv2.png");
	debugsubtileTex = App->tex->Load("maps/tile_32x32_2.png");
	arrowsTexture = App->tex->Load("textures/spells/Shara_attacks/arrowTypes.png");
	ritzUltimateTex = App->tex->Load("textures/spells/Ritz_ultimate/Ritz_ultimate_WIP.png");
	ritzBasicTex = App->tex->Load("textures/spells/Ritz_attacks/ritzBasicTest.png");
	marcheTornadoTex = App->tex->Load("textures/spells/Marche_attacks/Marche_tornado_twisterSpin.png");
	lootItemsTex = App->tex->Load("textures/loot/loot_items.png");
	portalTex = App->tex->Load("textures/map_props/portal/portal.png");

	// Load SFX
	lootGroundSFX = App->audio->LoadFx("audio/fx/loot/lootgrounded.wav");
	potionGroundSFX = App->audio->LoadFx("audio/fx/loot/potion_grounded.wav");
	coinGroundedSFX = App->audio->LoadFx("audio/fx/loot/coinGrounded.wav");
	swapCharSFX = App->audio->LoadFx("audio/fx/Player/swapChar.wav");
	stepSFX = App->audio->LoadFx("audio/fx/Player/footstep-on-stone.wav");
	enemySpawn = App->audio->LoadFx("audio/fx/enemySpawnTest.wav");
	goblinDetection = App->audio->LoadFx("audio/fx/goblin_detection.wav");

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
				if ((*item)->type != ENTITY_TYPE::PLAYER && (*item)->type != ENTITY_TYPE::LOOT 
					&& (*item)->type != ENTITY_TYPE::PROJECTILE && (*item)->type != ENTITY_TYPE::TRIGGER 
					&& (*item)->type != ENTITY_TYPE::WAVE_MANAGER && (*item)->type != ENTITY_TYPE::FLOWERBOSS) //needs to be loot too, otherwise if player collects loot thereis teh cnahce to create loot again
				{
					AddExp(dynamic_cast<Enemy*>(*item));
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
		*entitiesItem = nullptr;
		++entitiesItem;
	}
	entities.clear();

	draw_entities.clear();

	RELEASE_ARRAY(entitiesDataMap);
	entitiesDataMap = nullptr;

	//unload texture
	App->tex->UnLoad(texture);
	texture = nullptr;
	App->tex->UnLoad(assetsAtlasTex);
	assetsAtlasTex = nullptr;
	App->tex->UnLoad(enemyZombieTex);
	enemyZombieTex = nullptr;
	App->tex->UnLoad(enemyBombTex);
	enemyBombTex = nullptr;
	App->tex->UnLoad(enemyGolemTex);
	enemyGolemTex = nullptr; 
	App->tex->UnLoad(debugsubtileTex);
	debugsubtileTex = nullptr;
	App->tex->UnLoad(arrowsTexture);
	arrowsTexture = nullptr;
	App->tex->UnLoad(ritzUltimateTex);
	ritzUltimateTex = nullptr;
	App->tex->UnLoad(ritzBasicTex);
	ritzBasicTex = nullptr;
	App->tex->UnLoad(marcheTornadoTex);
	marcheTornadoTex = nullptr;
	App->tex->UnLoad(lootItemsTex); 
	lootItemsTex = nullptr;
	App->tex->UnLoad(portalTex);
	portalTex = nullptr;


	player = nullptr;

	return ret;
}

bool j1EntityFactory::Load(pugi::xml_node &node)
{

	for (pugi::xml_node nodeEntities = node.child("Entities"); nodeEntities; nodeEntities = nodeEntities.next_sibling("Entities"))
	{
		if (App->entityFactory->player != nullptr)
		{
			App->entityFactory->player->Load(nodeEntities);
		}
			
	}
	App->scene->ComeToDeath = false;
	return true;
}

bool j1EntityFactory::Save(pugi::xml_node &node) const
{
	
	std::vector<j1Entity*>::const_iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		if ((*item)->type == ENTITY_TYPE::PLAYER)
		{
			pugi::xml_node nodeEntities = node.append_child("Entities");
			(*item)->Save(nodeEntities);
		}
	}
	return true;
}

bool j1EntityFactory::LoadPortal(pugi::xml_node &node)
{
	if (loadEnemies)
	{
		for (pugi::xml_node node2 = node.child("Entities"); node2; node2 = node2.next_sibling("Entities"))
		{
			Enemy* ret = nullptr;

			pugi::xml_node nodeSpeed = node2.child("position");
			iPoint spawnPos = { nodeSpeed.attribute("x").as_int(),nodeSpeed.attribute("y").as_int() + 16 };

			std::string retType = node2.attribute("type").as_string();
			if (retType.compare("enemyBomb") == 0)
			{
				ret = CreateEnemy(EnemyType::BOMB, spawnPos, false);
				ret->level = node2.attribute("level").as_int();
				ret->life = node2.attribute("life").as_float();

				if (ret != nullptr)
				{
					App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, ret, "\0", CreateRandomBetween(5, 15) + 5 * ret->level);
					App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, ret, "\0", CreateRandomBetween(7, 17) + 5 * ret->level);
				}
			}

			if (retType.compare("enemyTest") == 0)
			{
				ret = CreateEnemy(EnemyType::TEST, spawnPos, false);
				ret->level = node2.attribute("level").as_int();
				ret->life = node2.attribute("life").as_float();
				if (ret != nullptr)
				{
					App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, ret, "\0", CreateRandomBetween(4, 10) + 5 * ret->level);
					App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, ret, "\0", CreateRandomBetween(10, 20) + 5 * ret->level);
				}
			}
		}
	}
	for (pugi::xml_node characterPlayer = node.child("Players"); characterPlayer; characterPlayer = characterPlayer.next_sibling("Players"))
	{
		if (App->entityFactory->player != nullptr)
		{
			App->entityFactory->player->Load(characterPlayer);
		}
	}
	//TODO create out portal
	return true;
}

bool j1EntityFactory::SavePortal(pugi::xml_node &node) const
{
	std::vector<j1Entity*>::const_iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		if (((*item)->type == ENTITY_TYPE::ENEMY_BOMB || (*item)->type == ENTITY_TYPE::ENEMY_TEST) && (*item) != nullptr)
		{
			pugi::xml_node nodeEntities = node.append_child("Entities");
			(*item)->Save(nodeEntities);
		}

		if ((*item)->type == ENTITY_TYPE::PLAYER)
		{
			pugi::xml_node nodeEntities = node.append_child("Players");
			(*item)->Save(nodeEntities);
		}

	}

	return true;
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
		ret = CreateLootType(positionX, positionY, LOOT_TYPE::NO_LOOT);
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

	case FLOWERBOSS:
		ret = DBG_NEW FlowerBossEntity({ positionX, positionY });
		entities.push_back(ret);
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
		ret = DBG_NEW EnemyBomb(pos, dummy);
		entities.push_back(ret);
		break;
	case EnemyType::TEST:
		ret = DBG_NEW EnemyTest(pos, dummy);
		entities.push_back(ret);
		break; 

	case EnemyType::MELEE:
		break; 

	case EnemyType::ARCHER:
		ret = DBG_NEW EnemyArcher(pos, dummy); 
		entities.push_back(ret);
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
	uint numArchers = 0; 
	uint cont = 0;

	uint bombProbs = 3;
	uint testProbs = 8;
	uint archerProbs = 3;

	while (cont < numEnemies)
	{
		for (std::vector<EnemyType>::iterator typeIter = enemyTypes.begin(); typeIter != enemyTypes.end(); typeIter++)
		{
			Enemy* ret = nullptr;
			int enemyLevel = CreateRandomBetween(0, 2);
			iPoint spawnPos = { zone.x + (int)CreateRandomBetween(0, zone.w), zone.y + (int)CreateRandomBetween(0,zone.h) };
			
			spawnPos = App->map->IsoToWorld(spawnPos.x, spawnPos.y);
			spawnPos.x = spawnPos.x * 2;
			if (!App->pathfinding->IsWalkable(App->map->WorldToMap(spawnPos.x, spawnPos.y)))
				continue; 

			LOG("Spawn Position: %i, %i", spawnPos.x, spawnPos.y);

			switch (*typeIter)
			{
			case EnemyType::BOMB:
				if (CreateRandomBetween(1, 10) <= bombProbs && cont < numEnemies)
				{
					// Last paramater is dummy
					ret = CreateEnemy(EnemyType::BOMB, spawnPos, false);

					if (App->entityFactory->player != nullptr)
					{
						ret->level = App->entityFactory->player->level + enemyLevel;
					}
					if (ret != nullptr)
					{
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::FIRE_ELEMENT, ROL::ATTACK_ROL, ret, "\0", CreateRandomBetween(5, 15) + 5 * ret->level);
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, ret, "\0", CreateRandomBetween(7, 17) + 5 * ret->level);
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

					if (App->entityFactory->player != nullptr)
					{
						ret->level = App->entityFactory->player->level + enemyLevel;
					}
					if (ret != nullptr)
					{	
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::POISON_ELEMENT, ROL::ATTACK_ROL, ret, "\0", CreateRandomBetween(4, 10) + 5 * ret->level);
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, ret, "\0", CreateRandomBetween(10, 20) + 5 * ret->level);
						numTests++;
						cont++;
					}
				}
				break;

			case EnemyType::ARCHER:
				if (CreateRandomBetween(1, 10) <= archerProbs && cont < numEnemies)
				{
					// Last paramater is dummy
					ret = CreateEnemy(EnemyType::ARCHER, spawnPos, false);

					if (App->entityFactory->player != nullptr)
					{
						ret->level = App->entityFactory->player->level + enemyLevel;
					}
					if (ret != nullptr)
					{
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, ret, "\0", CreateRandomBetween(4, 10) + 5 * ret->level);
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, ret, "\0", CreateRandomBetween(5, 10) + 5 * ret->level);
						numArchers++;
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
	if (!App->scene->ComeToPortal) {
		for (std::vector<GroupInfo>::iterator iter = spawngroups.begin(); iter != spawngroups.end(); iter++) {
			CreateEnemiesGroup((*iter).types, (*iter).zone, (*iter).minEnemies, (*iter).maxEnemies);
		}
	}
	spawngroups.clear();
}

j1Entity* j1EntityFactory::CreateArrow(fPoint pos, fPoint destination, uint speed, const j1Entity* owner, PROJECTILE_TYPE type, uint lifeTime)
{
	j1Entity* ret = nullptr;
	switch (type)
	{
	case PROJECTILE_TYPE::BASIC_ARROW:
		ret = DBG_NEW BasicArrow(pos, destination, speed, owner);
		entities.push_back(ret);
		break;
	case PROJECTILE_TYPE::CONTAGIOUS_ARROW:
		ret = DBG_NEW ContagiousFireArrow(pos, destination, speed, owner);
		entities.push_back(ret);
		break;
	case PROJECTILE_TYPE::FIRE_ARROW:
		ret = DBG_NEW FireArrow(pos, destination, speed, owner);
		entities.push_back(ret);
		break;

	case PROJECTILE_TYPE::MAGIC_BOLT:
		ret = DBG_NEW MagicBolt(pos, destination, speed, owner);
		entities.push_back(ret); 
		break; 

	case PROJECTILE_TYPE::DEATH_CIRCLE:
		ret = DBG_NEW DeathCircle(pos, owner);
		entities.push_back(ret); 
		break; 

	case PROJECTILE_TYPE::EMMITER_ARROWS:
		ret = DBG_NEW EmmiterArrows(pos, destination, speed, owner, lifeTime);
		entities.push_back(ret);
		break;

	case PROJECTILE_TYPE::EMMITER:
		ret = DBG_NEW Emmiter(pos, owner);
		entities.push_back(ret);
		break;

	case PROJECTILE_TYPE::MEDUSA:
		ret = DBG_NEW Medusa(pos, owner);
		entities.push_back(ret);
		break;

	case PROJECTILE_TYPE::TORNADO:
		ret = DBG_NEW Tornado(pos, destination, speed, owner);
		entities.push_back(ret);
		break;

	case PROJECTILE_TYPE::EARTH_SHAKER:
		ret = DBG_NEW EarthShaker(pos, owner);
		entities.push_back(ret);
		break;

	case PROJECTILE_TYPE::ENEMY_ARROW:
		ret = DBG_NEW EnemyProjectile(pos, destination, speed, owner); 
		entities.push_back(ret); 
		break; 

	case PROJECTILE_TYPE::GOLEM_ARROW:
		ret = DBG_NEW GolemProjectile(pos, destination, speed, owner); 
		entities.push_back(ret); 
		break; 
	case PROJECTILE_TYPE::BOSS_EMMITER:
		ret = DBG_NEW BossEmmiter(pos, owner,lifeTime);
		entities.push_back(ret);
	case PROJECTILE_TYPE::BOSS_EMMITER_ARROWS:
		ret = DBG_NEW BossEmmiterArrow(pos, destination, speed, owner, lifeTime);
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
		ret = DBG_NEW Equipable(posX, posY);
			LoadLootData(ret, App->config);
			entities.push_back(ret);
			break;

	case LOOT_TYPE::CONSUMABLE:
		ret = DBG_NEW Consumable(posX, posY);
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
	if (GetRandomValue(1, 2) == 1)
	{
		ret = DBG_NEW Consumable(posX, posY);
		LoadLootData(ret, App->config);
		entities.push_back(ret);
	}
	return nullptr;
}

Trigger * j1EntityFactory::CreateTrigger(TRIGGER_TYPE type, float posX, float posY, SceneState scene, Color color)
{
	Trigger* ret = nullptr;
	switch (type)
	{
	case TRIGGER_TYPE::PORTAL:	
		ret = new Portal(posX, posY,scene,color);
		entities.push_back(ret);
		break;
	case TRIGGER_TYPE::LOBBYPORTAL:
		ret = new LobbyPortal(posX, posY, scene, color);
		entities.push_back(ret);
		break;
	case TRIGGER_TYPE::WIN:
		ret = new WinTrigger(posX, posY,scene,color);
		entities.push_back(ret);
		break;
	case TRIGGER_TYPE::NOWALKABLE:
		ret = new NoWalkableTrigger(posX, posY);
		entities.push_back(ret);
		break;
	case TRIGGER_TYPE::EXITPORTAL:
		ret = new ExitPortal(posX, posY);
		entities.push_back(ret);
		break;
	case TRIGGER_TYPE::NO_TRIGGER:
		break;
	default:
		break;
	}
	return ret;
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
	player = DBG_NEW PlayerEntityManager(position);

	if (player != nullptr)
	{
		entities.push_back(player);
		return player;
	}
	
	LOG("Failed to create player system");
	return nullptr;
}

WaveManager * j1EntityFactory::CreateWave(const SDL_Rect & zone, uint numWaves)
{
	waveManager = DBG_NEW WaveManager(zone, 5); 

	if (waveManager != nullptr)
	{
		entities.push_back(waveManager); 
		return waveManager; 
	}

	LOG("Failed to create Wave Manager");
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

	entitiesDataMap = DBG_NEW entityDataMap[subtileWidth * subtileHeight];
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
			if ((*entityIterator)->type == ENTITY_TYPE::ENEMY_TEST || (*entityIterator)->type == ENTITY_TYPE::ENEMY_BOMB || (*entityIterator)->type == ENTITY_TYPE::ENEMY_ARCHER || // ||other enemy types 
			(*entityIterator)->type == ENTITY_TYPE::FLOWERBOSS)
			{
				ret = false;
				break;
			}
		}
	}

	return ret;
}

j1Entity* j1EntityFactory::isThisSubtileTriggerFree(const iPoint pos) const
{

	j1Entity* ret = nullptr;

	if (!isThisSubtileReserved(pos))
	{
		std::vector<j1Entity*>::iterator entityIterator = entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities.begin();
		for (; entityIterator != entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities.end(); ++entityIterator)
		{
			if ((*entityIterator)->type == ENTITY_TYPE::TRIGGER)
			{
				ret = *entityIterator;
				if (ret != nullptr)
				{
					App->entityFactory->DeleteEntityFromSubtile(ret);

					return ret;
				}
			}
		}
	}
	return nullptr;
}

j1Entity* j1EntityFactory::isThisSubtileLootFree(const iPoint pos) const
{

	j1Entity* ret = nullptr;

	if (!isThisSubtileEmpty(pos))
	{
		std::vector<j1Entity*>::iterator entityIterator = entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities.begin();
		for (; entityIterator != entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities.end(); ++entityIterator)
		{
			if ((*entityIterator)->type == ENTITY_TYPE::LOOT)
			{
				ret = *entityIterator;
				return ret;
			}
		}
	}
	return nullptr;
}

bool j1EntityFactory::BoolisThisSubtileTriggerFree(const iPoint pos) const
{

	bool ret = false;

	if (!isThisSubtileReserved(pos))
	{
		std::vector<j1Entity*>::iterator entityIterator = entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities.begin();
		for (; entityIterator != entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities.end(); ++entityIterator)
		{
			if ((*entityIterator)->type == ENTITY_TYPE::TRIGGER && (*entityIterator)->to_delete == false)
			{
				ret = true;
				break;
			}
		}
	}
	return ret;
}

bool j1EntityFactory::isThisSubtilePlayerFree(const iPoint pos) const
{
	bool ret = true;

	if (!isThisSubtileEmpty(pos))
	{
		std::vector<j1Entity*>::iterator entityIterator = entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities.begin();
		for (; entityIterator != entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities.end(); ++entityIterator)
		{
			if ((*entityIterator)->type == ENTITY_TYPE::PLAYER)
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
	//LOG("RELEASED ALL RESERVED SUBTILES");
}

bool j1EntityFactory::CheckSubtileMapBoundaries(const iPoint pos) const
{
	return (pos.x >= 0 && pos.x < subtileWidth &&
		pos.y >= 0 && pos.y < subtileHeight);
}

j1Entity* j1EntityFactory::CreateAsset(EnvironmentAssetsTypes type, iPoint worldPos, SDL_Rect atlasSpriteRect)
{
	j1Entity* assetEntity = nullptr;

	switch (type)
	{
	case EnvironmentAssetsTypes::NO_TYPE:
		break;
	case EnvironmentAssetsTypes::WALL:
		assetEntity = DBG_NEW j1Entity(worldPos, atlasSpriteRect);
		entities.push_back(assetEntity);
		break;
	case EnvironmentAssetsTypes::WALL1:
		break;
	case EnvironmentAssetsTypes::TRIGGERWALL:
		assetEntity = DBG_NEW j1Entity(worldPos, atlasSpriteRect);
		break;
	case EnvironmentAssetsTypes::MAX:
		break;
	default:
		break;
	}
	return assetEntity;
}

bool j1EntityFactory::LoadLootData(LootEntity* lootEntity, pugi::xml_node& config)
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
			if(lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level;
			break;
		case 2:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;	
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level;
			break;
		case 3:
			lootEntity->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level;
			break;
		case 4:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::NO_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level;
			break;
		case 5:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level + 1;
			break;
		case 6:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level + 1;
			break;
		case 7:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level + 1;
			break;
		case 8:
			lootEntity->elemetalType = ELEMENTAL_TYPE::NO_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level + 1;
			break;
		case 9:
			lootEntity->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level - 1;
			break;
		case 10:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level - 1;
			break;
		case 11:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level - 1;
			break;
		case 12:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::ALL_ELEMENTS;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level;
			break;
		default:
			break;
		}

		if (lootEntity->level < 1)
			lootEntity->level = 1;


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
					lootEntity->name.assign(lootEntity->lootname.data());
					if (id == 1)
					{
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 15) + lootEntity->level * 10, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(75, 100) * 0.01 - lootEntity->level * 5, lootEntity);

					}
					else if (id == 2)
					{
						lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 20) * 0.01 + lootEntity->level * 0.1, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(5, 15) + lootEntity->level * 5, lootEntity);
					}
					else
					{
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 15) + lootEntity->level * 10, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(5, 15) + lootEntity->level * 5, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(75, 100) * 0.01 - lootEntity->level * 5, lootEntity);

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
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 15) + lootEntity->level * 10, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(75, 100) * 0.01 - lootEntity->level * 0.05, lootEntity);
					}

					else if (id == 2)
					{

						lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 20) * 0.01, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(1, 2) * 0.1 + lootEntity->level*0.02, lootEntity);
					}
					else
					{
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 15) + lootEntity->level * 10, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(75, 100) * 0.01 - lootEntity->level * 0.05, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(1, 2) * 0.1 + lootEntity->level * 0.02, lootEntity);
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
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 20) + lootEntity->level * 15, lootEntity);
					}

					else if (id == 2)
					{
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(5, 10) + lootEntity->level * 8, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(75, 100) * 0.01 + lootEntity->level * 0.05, lootEntity);
					}
					else
					{
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 20) + lootEntity->level * 15, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(65, 85) * 0.01 + lootEntity->level * 0.05, lootEntity);
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
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level;
			break;
		case 2:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level;
			break;
		case 3:
			lootEntity->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level;
			break;
		case 4:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::NO_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level;
			break;
		case 5:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level + 1;
			break;
		case 6:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level + 1;
			break;
		case 7:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level + 1;
			break;
		case 8:
			lootEntity->elemetalType = ELEMENTAL_TYPE::NO_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level + 1;
			break;
		case 9:
			lootEntity->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level - 1;
			break;
		case 10:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level - 1;
			break;
		case 11:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level - 1;
			break;
		case 12:
			if (lootEntity->elemetalType == ELEMENTAL_TYPE::NO_ELEMENT)
				lootEntity->elemetalType = ELEMENTAL_TYPE::ALL_ELEMENTS;
			if (lootEntity->level == 0)
				lootEntity->level = App->entityFactory->player->level;
			break;
		default:
			break;
		}
		if (lootEntity->level < 1)
			lootEntity->level = 1;

		if (lootEntity->level < 1)
			lootEntity->level = 1;

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
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(10, 15) + lootEntity->level * 5, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(10, 15) + lootEntity->level * 10, lootEntity);
					}

					else if (id == 2)
					{
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20) + lootEntity->level * 5, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(1, 2) * 0.1 + lootEntity->level * 0.02, lootEntity);
					}
					else
					{
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20) + lootEntity->level * 5, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(1, 2) * 0.1 + lootEntity->level * 0.02, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(10, 15) + lootEntity->level * 5, lootEntity);
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
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20) + lootEntity->level * 5, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(5, 10) + lootEntity->level * 10, lootEntity);
					}

					else if (id == 2)
					{
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20) + lootEntity->level * 2.5, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(1, 2) * 0.1 + lootEntity->level * 0.05, lootEntity);
					}
					else
					{
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20) + lootEntity->level * 5, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(1, 2) * 0.1 + lootEntity->level * 0.05, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(10, 15) + lootEntity->level * 5, lootEntity);
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
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20) + lootEntity->level * 10, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(5, 10) + lootEntity->level * 5, lootEntity);
					}

					else if (id == 2)
					{
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20) + lootEntity->level * 10, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(1, 2) * 0.1 + lootEntity->level * 0.05, lootEntity);
					}
					else
					{
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 20) + lootEntity->level * 10, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(1, 2) * 0.1 + lootEntity->level * 0.05, lootEntity);
						lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, GetRandomValue(10, 15) + lootEntity->level * 5, lootEntity);
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

j1Entity * j1EntityFactory::CreateLootType(int x, int y, LOOT_TYPE defined)
{
	j1Entity * ret = nullptr;

	if (defined == LOOT_TYPE::NO_LOOT)
	{
		switch (WillDrop())
		{
		case LOOT_TYPE::CONSUMABLE:

		ret = DBG_NEW Consumable(x, y);
		break;

	case LOOT_TYPE::EQUIPABLE:
		ret = DBG_NEW Equipable(x, y);
		break;

		default:
			break;
		}

    }
	else if(defined == LOOT_TYPE::EQUIPABLE)
	{
		ret = DBG_NEW Equipable(x, y);
	}
	else if (defined == LOOT_TYPE::CONSUMABLE)
	{
		ret = DBG_NEW Consumable(x, y);
	}

	return ret;
}

LOOT_TYPE j1EntityFactory::WillDrop()
{
	int randvalue = GetRandomValue(1, 100);

	if (randvalue <= 50)
	{
		if (randvalue <= 25)
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

void j1EntityFactory::AddExp(Enemy * enemy)
{
	uint expToAdd = 100;
	uint bonusLevel = (enemy->level - player->level) * 25;
	player->exp = expToAdd + bonusLevel;

	if (player->exp > player->maxExpInLevel)
	{
		++player->level;
		player->exp -= player->maxExpInLevel;
	}
}

fPoint j1EntityFactory::getplayerDamagevec()
{
	return dmg_vec;
}

void j1EntityFactory::setPlayerDmageVec(fPoint unitari)
{
	dmg_vec = unitari;
}