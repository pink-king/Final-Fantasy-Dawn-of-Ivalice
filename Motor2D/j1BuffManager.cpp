#include "j1BuffManager.h"
#include <string.h>
#include "j1EntityFactory.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "UiItem_Image.h"
#include "j1ParticlesClassic.h"
#include "Brofiler/Brofiler.h"
#include "Boss_Flower.h"

j1BuffManager::j1BuffManager()
{
	name.assign("Buff");
}

j1BuffManager::~j1BuffManager()
{
}

bool j1BuffManager::Awake(pugi::xml_node &node)
{
	bool ret = true;

	return ret;
}

bool j1BuffManager::Start()
{
	burnedSFX = App->audio->LoadFx("audio/fx/States/burned.wav");
	freezedSFX = App->audio->LoadFx("audio/fx/States/freezed.wav");
	paralyzedSFX = App->audio->LoadFx("audio/fx/States/paralyzed.wav");
	poisonedSFX = App->audio->LoadFx("audio/fx/States/poisoned.wav");
	healingSFX = App->audio->LoadFx("audio/fx/States/Healing.wav");
	enemyHitbyMarche = App->audio->LoadFx("audio/fx/States/enemyHitbyMarche.wav");

	return true;
}

bool j1BuffManager::Update(float dt)
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
	{
		godMode = !godMode;

		godMode ? App->scene->god_label->hide = false : App->scene->god_label->hide = true;
	}
	return ret;
}

bool j1BuffManager::CleanUp()
{
	std::list<Buff*>::iterator item = buffs.begin();

	while (item != buffs.end())
	{
		if (*item != nullptr)
		{
			delete *item;
			*item = nullptr;
			buffs.remove(*item);

		}
		++item;
	}
	buffs.clear();

	std::list<j1Entity*>::iterator item2 = entitiesTimeDamage.begin();

	while (item2 != entitiesTimeDamage.end())
	{
		entitiesTimeDamage.remove(*item2);
		++item2;
	}
	entitiesTimeDamage.clear();

	return true;
}

Buff* j1BuffManager::CreateBuff(BUFF_TYPE type, ELEMENTAL_TYPE elementType, ROL rol, j1Entity* character, std::string stat, float value)
{

	Buff* newbuff = DBG_NEW Buff(type, character, stat, elementType, rol, value,nullptr);
	if (rol == ROL::ATTACK_ROL || rol == ROL::DEFENCE_ROL)
		buffs.push_back(newbuff);

	else
	{
		ChangeEntityVariables(character, type, rol, value);
		buffs.push_back(newbuff);
	}
	return newbuff;
}

void j1BuffManager::RemoveBuff(j1Entity* character)
{
	std::list<Buff*>::iterator item = buffs.begin();
	for (; item != buffs.end(); ++item)
	{
		if (character == (*item)->GetCharacter())
		{
			if ((*item)->GetRol() != ROL::ATTACK_ROL && (*item)->GetRol() != ROL::DEFENCE_ROL)
				ResetEntityVariables(*item);
			buffs.remove(*item);
		}
	}
}

float j1BuffManager::CalculateStat(const j1Entity* ent,float initialDamage, ELEMENTAL_TYPE elementType, ROL rol, std::string stat)
{
	float totalMult = 0.f;
	
	for (std::list<Buff*>::iterator iter = buffs.begin(); iter != buffs.end(); ++iter)
	{
		if (ent == App->entityFactory->player)
		{
			ent = App->entityFactory->player->selectedCharacterEntity;
		}
		if (rol == ROL::DEFENCE_ROL)
		{
			if ((elementType == (*iter)->GetElementType() || (*iter)->GetElementType() == ELEMENTAL_TYPE::ALL_ELEMENTS) && rol == (*iter)->GetRol() &&
				(ent == (*iter)->GetCharacter()))
			{
				if ((*iter)->GetType() == BUFF_TYPE::ADDITIVE)
					initialDamage += (*iter)->GetValue();

				else if ((*iter)->GetType() == BUFF_TYPE::MULTIPLICATIVE)
					totalMult += (*iter)->GetValue();
			}
		}
		else if (rol == ROL::ATTACK_ROL)
		{
			if ((elementType == (*iter)->GetElementType() || elementType == ELEMENTAL_TYPE::ALL_ELEMENTS || (*iter)->GetElementType() == ELEMENTAL_TYPE::ALL_ELEMENTS) && rol == (*iter)->GetRol() &&
				(ent == (*iter)->GetCharacter()))
			{
				if ((*iter)->GetType() == BUFF_TYPE::ADDITIVE)
					initialDamage += (*iter)->GetValue();

				else if ((*iter)->GetType() == BUFF_TYPE::MULTIPLICATIVE)
					totalMult += (*iter)->GetValue();
			}
		}
	}
	if (initialDamage != 0)
		return initialDamage * (1 + totalMult);
	else if (totalMult != 0)
		return 1 + totalMult;
	else
		return 0;
}


bool j1BuffManager::DirectAttack(j1Entity* attacker, j1Entity* defender, float initialDamage, ELEMENTAL_TYPE elementType, std::string stat)
{
	BROFILER_CATEGORY("Direct Attack", Profiler::Color::ForestGreen);
	if (godMode && defender == App->entityFactory->player)
		return true;

	float lifeToSubstract = CalculateStat(attacker, initialDamage, elementType, ROL::ATTACK_ROL, stat) - CalculateStat(defender, defender->defence, elementType, ROL::DEFENCE_ROL, stat);
	if (lifeToSubstract <= 0)
	{
		lifeToSubstract  = 4;
	}
	
	defender->life -= lifeToSubstract;
	// add always a hitpoint
	// but if we have a previous one, unlink
	if (attacker->type == ENTITY_TYPE::ENEMY_TEST)
		App->audio->PlayFx(App->scene->goblinAttack, 0);

	
	if (defender->type == ENTITY_TYPE::FLOWERBOSS)
	{
		dynamic_cast<FlowerBossEntity*>(defender)->myBossLifeBar->doDamageToBoss(lifeToSubstract);
	}

	if (defender->type == ENTITY_TYPE::PLAYER)
	{
		App->gui->healthBar->damageInform.doDamage = true;
		App->gui->healthBar->damageInform.damageValue = lifeToSubstract;

		/*-------- HURT HUD ---------*/
		App->entityFactory->setPlayerDmageVec(getPlayerToEnemyVec(defender, attacker)); //vector to get player orientations from enemy

		//-------->
		if (App->scene->hit_counter < 3 && defender->life > defender->maxLife * 0.5f)
		{
			App->scene->previous_counter = App->scene->hit_counter;
			App->scene->hit_counter += 1;
			App->scene->decreaseAlpha = false;
		}
		else if (App->scene->hit_counter == 3 && defender->life > defender->maxLife * 0.5f)
		{
			App->scene->hit_counter -= 1;
			App->scene->previous_counter = App->scene->hit_counter - 1;
			App->scene->decreaseAlpha = false;
		}

		if (defender->life < defender->maxLife * 0.5f)
		{
			App->scene->previous_counter = App->scene->hit_counter;
			App->scene->hit_counter += 1;
			App->scene->decreaseAlpha = false;
		}
		//<-----------

		App->entityFactory->pushEF = true;
		App->input->DoGamePadRumble(200, 100);
		App->camera2D->AddTrauma(0.5f);
		App->scene->timeindmg.Start();
		/*----------------------------*/

		if (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetMarche())
		{
			App->audio->PlayFx(App->scene->marcheDamaged, 0);
		}

		else if (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetRitz())
		{
			App->audio->PlayFx(App->scene->RitzDamaged, 0);
		}

		else if (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetShara())
		{
			App->audio->PlayFx(App->scene->SharaDamaged, 0);
		}

		

	}


	if (attacker == App->entityFactory->player->GetMarche())
		App->audio->PlayFx(enemyHitbyMarche, 0);
	

	bool playerAttacks = false;
	if (attacker->type == ENTITY_TYPE::PLAYER)
	{
		if (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetMarche())
		{
			defender->entityPushback = true;
			App->entityFactory->setPlayerDmageVec(getPlayerToEnemyVec(defender, attacker));
			//defender->dmgEnemyVec = getPlayerToEnemyVec(defender, attacker);

			//defender->entityPushback = false;
		}
		playerAttacks = true;
	}
	// Calling Hit point labels
	if (defender->type != ENTITY_TYPE::BREAKABLE_ASSET)
	{
		switch (elementType)
		{
		case ELEMENTAL_TYPE::NO_ELEMENT:
			App->HPManager->callHPLabelSpawn(iPoint(defender->position.x, defender->position.y), lifeToSubstract, ELEMENTAL_TYPE::NO_ELEMENT, false, playerAttacks);
			break;
		case ELEMENTAL_TYPE::FIRE_ELEMENT:
			App->HPManager->callHPLabelSpawn(iPoint(defender->position.x, defender->position.y), lifeToSubstract, ELEMENTAL_TYPE::FIRE_ELEMENT, false, playerAttacks);
			break;
		case ELEMENTAL_TYPE::ICE_ELEMENT:
			App->HPManager->callHPLabelSpawn(iPoint(defender->position.x, defender->position.y), lifeToSubstract, ELEMENTAL_TYPE::ICE_ELEMENT, false, playerAttacks);

		case ELEMENTAL_TYPE::POISON_ELEMENT:
			App->HPManager->callHPLabelSpawn(iPoint(defender->position.x, defender->position.y), lifeToSubstract, ELEMENTAL_TYPE::POISON_ELEMENT, false, playerAttacks);

		case ELEMENTAL_TYPE::ALL_ELEMENTS:
			App->HPManager->callHPLabelSpawn(iPoint(defender->position.x, defender->position.y), lifeToSubstract, ELEMENTAL_TYPE::ALL_ELEMENTS, false, playerAttacks);
			break;

		default:
			break;
		}
	}
	

		
																													  // but, enemy can die no
	if (defender->life <= 0 && defender->type != ENTITY_TYPE::PLAYER) // ONLY FOR DELETE
	{
		RemoveBuff(defender);
		entitiesTimeDamage.remove(defender);
		//defender->to_delete = true;   
		// When we kill the player we will have a diying animation aswell (or tell him to delete), as for now, only come here ENEMIES or PLAYERS, so should be fine
		//If causes any trouble put it back without any problem
		App->entityFactory->DeleteEntityFromSubtile(defender);

		defender->isParalize = false;
		defender->to_die = true;
		
	} 
	else if (defender->life > 0 && attacker->type != ENTITY_TYPE::PLAYER)
	{
			if(App->entityFactory->GetRandomValue(1,2)==2 && defender->type == ENTITY_TYPE::ENEMY_TEST)
				App->audio->PlayFx(App->scene->goblinDamaged, 0);

			if (App->entityFactory->GetRandomValue(1, 2) == 2 && defender->type == ENTITY_TYPE::ENEMY_BOMB)
				App->audio->PlayFx(App->scene->bombgetHitSFX, 0);

		if (elementType == ELEMENTAL_TYPE::FIRE_ELEMENT && !defender->isBurned)
		{
			if (App->entityFactory->GetRandomValue(1, 10) == 1)
			{
				CreateBurned(attacker, defender, initialDamage*0.1, 10, "\0");
			}
		}

		if (elementType == ELEMENTAL_TYPE::ICE_ELEMENT && !defender->isParalize)
		{
			if (App->entityFactory->GetRandomValue(1, 10) == 1)
			{
				CreateParalize(attacker, defender, initialDamage*0.1, 8, "\0");
			}
		}
		if (elementType == ELEMENTAL_TYPE::POISON_ELEMENT && !defender->isPosioned)
		{
			if (App->entityFactory->GetRandomValue(1, 10) == 1)
			{
				CreatePoision(attacker, defender, initialDamage*0.1, 10, "\0");
			}
		}
	}
	else if (defender->life < 0 && defender->type == ENTITY_TYPE::PLAYER)
	{
		App->scene->isDeath = true;
		App->pause = true;
	}


	if (attacker == App->entityFactory->player->GetMarche())
	{
		iPoint drawRectified;
		drawRectified.x = defender->position.x + defender->size.x * 0.5f;
		drawRectified.y = defender->position.y + (defender->size.y * 0.5f) * 2;

		// flip particles pseudo randomly
		SDL_RendererFlip renderFlip = SDL_RendererFlip::SDL_FLIP_NONE;

		if (rand() % 2 == 0)
		{
			renderFlip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
		}
		iPoint bloodPivot = { 10, 10 };
		drawRectified -= bloodPivot;
		App->particles->AddParticle(App->particles->blood02, drawRectified.x, drawRectified.y - defender->pivot.y, { 0,0 }, 0u, renderFlip);
	}
	return true;
}

void j1BuffManager::CreateBurned(j1Entity* attacker, j1Entity* defender, float damageSecond, uint totalTime, std::string stat, bool paralize)
{
	if (!defender->isBurned)
	{
		entityStat* newStat = DBG_NEW entityStat(STAT_TYPE::BURNED_STAT, totalTime, damageSecond, nullptr, paralize);
		float totalDamage = CalculateStat(attacker, newStat->secDamage, ELEMENTAL_TYPE::FIRE_ELEMENT, ROL::ATTACK_ROL, stat) - CalculateStat(defender, defender->defence, ELEMENTAL_TYPE::FIRE_ELEMENT, ROL::DEFENCE_ROL, stat);
		if (totalDamage < 0)
			totalDamage = 1;
		newStat->secDamage = totalDamage;
		defender->stat.push_back(newStat);
		defender->isBurned = true;
		bool isInList = false;
		for (std::list<j1Entity*>::iterator item = entitiesTimeDamage.begin(); item != entitiesTimeDamage.end(); ++item)
		{
			if ((*item) == defender)
				isInList = true;
		}

		if (!isInList)
		{
			entitiesTimeDamage.push_back(defender);
		}
		newStat->count.Start();
	}
}

void j1BuffManager::CreatePoision(j1Entity* attacker, j1Entity* defender, float damageSecond, uint totalTime, std::string stat, bool paralize)
{
	if (!defender->isPosioned)
	{
		entityStat* newStat = DBG_NEW entityStat(STAT_TYPE::POISON_STAT, totalTime, damageSecond, nullptr, paralize);
		float totalDamage = CalculateStat(attacker, newStat->secDamage, ELEMENTAL_TYPE::POISON_ELEMENT, ROL::ATTACK_ROL, stat) - CalculateStat(defender, defender->defence, ELEMENTAL_TYPE::POISON_ELEMENT, ROL::DEFENCE_ROL, stat);
		if (totalDamage < 0)
			totalDamage = 1;
		newStat->secDamage = totalDamage;
		defender->stat.push_back(newStat);
		defender->isPosioned = true;
		bool isInList = false;
		for (std::list<j1Entity*>::iterator item = entitiesTimeDamage.begin(); item != entitiesTimeDamage.end(); ++item)
		{
			if ((*item) == defender)
				isInList = true;
		}

		if (!isInList)
		{
			entitiesTimeDamage.push_back(defender);
		}
		newStat->count.Start();
	}
}

void j1BuffManager::CreateParalize(j1Entity* attacker, j1Entity* defender, float damageSecond, uint totalTime, std::string stat, bool paralize)
{
	if (!defender->isFrozen || !defender->to_die)
	{
		entityStat* newStat = DBG_NEW entityStat(STAT_TYPE::ICE_STAT, damageSecond, totalTime, nullptr, paralize);
		float totalDamage = CalculateStat(attacker, damageSecond, ELEMENTAL_TYPE::ICE_ELEMENT, ROL::ATTACK_ROL, stat) - CalculateStat(defender, defender->defence, ELEMENTAL_TYPE::ICE_ELEMENT, ROL::DEFENCE_ROL, stat);
		if (totalDamage < 0)
			totalDamage = 1;
		newStat->secDamage = totalDamage;
		newStat->count.Start();
		defender->stat.push_back(newStat);

		defender->isFrozen = true;
		bool isInList = false;
		
		for (std::list<j1Entity*>::iterator item = entitiesTimeDamage.begin(); item != entitiesTimeDamage.end(); ++item)
		{
			if ((*item) == defender)
				isInList = true;
		}

		if (!isInList)
		{
			entitiesTimeDamage.push_back(defender);
		}
		// check for entity animations speed
		AdjustEntityAnimationSpeed(defender);
	}
}

void j1BuffManager::CreateHealth(j1Entity* entity, float lifeSecond, uint time)
{
	if (!entity->isPotionActive)
	{
		entityStat* newStat = DBG_NEW entityStat(STAT_TYPE::POTION_STAT, time, lifeSecond);
		newStat->count.Start();
		entity->stat.push_back(newStat);
		entity->isPotionActive = true;
		bool isInList = false;
		for (std::list<j1Entity*>::iterator item = entitiesTimeDamage.begin(); item != entitiesTimeDamage.end(); ++item)
		{
			if ((*item) == entity)
				isInList = true;
		}

		if (!isInList)
		{
			entitiesTimeDamage.push_back(entity);
		}

		

	}
}

void j1BuffManager::TemporalBuff(j1Entity * entity, BUFF_TYPE type, ELEMENTAL_TYPE element, ROL rol, float value, uint time)
{

	entityStat* newStat = nullptr;
	switch (rol)
	{
	case ROL::ATTACK_ROL:
		newStat = DBG_NEW entityStat(STAT_TYPE::ATTACK_BUFF, time, value);
		newStat->temporalBuff = CreateBuff(type, element, rol, entity, "\0", value);
		entity->stat.push_back(newStat);
		break;
	case ROL::DEFENCE_ROL:
		newStat = DBG_NEW entityStat(STAT_TYPE::DEFENCE_BUFF, time, value);
		newStat->temporalBuff = CreateBuff(type, element, rol, entity, "\0", value);
		entity->stat.push_back(newStat);
		break;
	case ROL::VELOCITY:
		newStat = DBG_NEW entityStat(STAT_TYPE::SPEED_BUFF, time, value, nullptr, false);
		newStat->temporalBuff = CreateBuff(type, element, rol, entity, "\0", value);
		entity->stat.push_back(newStat);
		break;
	case ROL::HEALTH:
		newStat = DBG_NEW entityStat(STAT_TYPE::HEALTH_BUFF, time, value, nullptr, false);
		newStat->temporalBuff = CreateBuff(type, element, rol, entity, "\0", value);
		entity->stat.push_back(newStat);
		break;
	case ROL::NO_ROL:
		break;
	default:
		break;
	}
	bool isInList = false;
	for (std::list<j1Entity*>::iterator item = entitiesTimeDamage.begin(); item != entitiesTimeDamage.end(); ++item)
	{
		if ((*item) == entity)
			isInList = true;
	}

	if (!isInList)
	{
		entitiesTimeDamage.push_back(entity);
	}

}

void j1BuffManager::ChangeEntityVariables(j1Entity* entity, BUFF_TYPE type, ROL rol, float value)
{
	if (entity != nullptr)
	{
		PlayerEntity* player = (PlayerEntity*)entity;
		Enemy* enemy = (Enemy*)entity;
		switch (rol)
		{
		case ROL::COOLDOWN:
			if (entity->type== ENTITY_TYPE::PLAYER)
			{
				if (type == BUFF_TYPE::MULTIPLICATIVE)
				{
					player->coolDownData.basic.cooldownTime *= value;
					if(player->coolDownData.basic.cooldownTime == 1)
						player->coolDownData.basic.cooldownTime = 1;
					player->coolDownData.dodge.cooldownTime *= value;
					if (player->coolDownData.dodge.cooldownTime <= 1)
						player->coolDownData.dodge.cooldownTime = 1;
					player->coolDownData.special1.cooldownTime *= value;
					if (player->coolDownData.special1.cooldownTime <= 1)
						player->coolDownData.special1.cooldownTime = 1;
					player->coolDownData.special2.cooldownTime *= value;
					if (player->coolDownData.special2.cooldownTime <= 1)
						player->coolDownData.special2.cooldownTime = 1;
					player->coolDownData.ultimate.cooldownTime *= value;
					if (player->coolDownData.ultimate.cooldownTime <= 1)
						player->coolDownData.ultimate.cooldownTime = 1;

				}
				else if (type == BUFF_TYPE::ADDITIVE)
				{
					player->coolDownData.basic.cooldownTime -= value;
					player->coolDownData.dodge.cooldownTime -= value;
					player->coolDownData.special1.cooldownTime -= value;
					player->coolDownData.special2.cooldownTime -= value;
					player->coolDownData.ultimate.cooldownTime -= value;
				}
			}
			else if (entity->type == ENTITY_TYPE::ENEMY_TEST ||
				entity->type == ENTITY_TYPE::ENEMY_BOMB ||
				entity->type == ENTITY_TYPE::ENEMY_ARCHER)
			{
				if (type == BUFF_TYPE::MULTIPLICATIVE)
					enemy->attackSpeed *= value;
				else if (type == BUFF_TYPE::ADDITIVE)
					enemy->attackSpeed -= value;
			}
			break;

		case ROL::VELOCITY:
			if (entity->type == ENTITY_TYPE::PLAYER)
			{
				if (type == BUFF_TYPE::MULTIPLICATIVE)
				{
					player->characterBaseSpeed.x *= value;
					player->characterBaseSpeed.y *= value;
				}
				else if (type == BUFF_TYPE::ADDITIVE)
				{
					player->characterBaseSpeed.x += value;
					player->characterBaseSpeed.y += value;
				}
			}
			else if (entity->type == ENTITY_TYPE::ENEMY_TEST ||
				entity->type == ENTITY_TYPE::ENEMY_BOMB ||
				entity->type == ENTITY_TYPE::ENEMY_ARCHER)
			{
				if (type == BUFF_TYPE::MULTIPLICATIVE)
				{
					enemy->speed *= value;
				}

				else if (type == BUFF_TYPE::ADDITIVE)
				{
					enemy->speed += value;				}
			}
			break;

		case ROL::HEALTH:
			if (entity->type == ENTITY_TYPE::PLAYER)
			{
				if (type == BUFF_TYPE::MULTIPLICATIVE)
				{
					App->entityFactory->player->maxLife *= value;
					App->entityFactory->player->life *= value;
				}
				else if (type == BUFF_TYPE::ADDITIVE)
				{
					App->entityFactory->player->maxLife += value;
					App->entityFactory->player->life += value;
				}



				App->gui->healthBar->RecalculateSection(); 

			}
			else if (entity->type == ENTITY_TYPE::ENEMY_TEST ||
				entity->type == ENTITY_TYPE::ENEMY_BOMB ||
				entity->type == ENTITY_TYPE::ENEMY_ARCHER)
			{
				if (type == BUFF_TYPE::MULTIPLICATIVE)
				{
					enemy->maxLife *= value;
					enemy->life *= value;
				}

				else if (type == BUFF_TYPE::ADDITIVE)
				{
					enemy->maxLife += value;
					enemy->life += value;
				}
			}
			break;
		default:
			break;
		}
	}
}

void j1BuffManager::ResetEntityVariables(Buff* buff)
{
	PlayerEntity* player = (PlayerEntity*)buff->GetCharacter();
	Enemy* enemy = (Enemy*)buff->GetCharacter();
	switch (buff->GetRol())
	{
	case ROL::COOLDOWN:
		if (buff->GetCharacter()->type == ENTITY_TYPE::PLAYER)
		{
			if (buff->GetType() == BUFF_TYPE::MULTIPLICATIVE)
			{
				player->coolDownData.basic.cooldownTime /= buff->GetValue();
				player->coolDownData.dodge.cooldownTime /= buff->GetValue();
				player->coolDownData.special1.cooldownTime /= buff->GetValue();
				player->coolDownData.special2.cooldownTime /= buff->GetValue();
				player->coolDownData.ultimate.cooldownTime /= buff->GetValue();


			}
			else if (buff->GetType() == BUFF_TYPE::ADDITIVE)
			{
				player->coolDownData.basic.cooldownTime += buff->GetValue();
				player->coolDownData.dodge.cooldownTime += buff->GetValue();
				player->coolDownData.special1.cooldownTime += buff->GetValue();
				player->coolDownData.special2.cooldownTime += buff->GetValue();
				player->coolDownData.ultimate.cooldownTime += buff->GetValue();
			}
		}
		else if (buff->GetCharacter()->type == ENTITY_TYPE::ENEMY_TEST ||
			buff->GetCharacter()->type == ENTITY_TYPE::ENEMY_BOMB ||
			buff->GetCharacter()->type == ENTITY_TYPE::ENEMY_ARCHER)
		{
			if (buff->GetType() == BUFF_TYPE::MULTIPLICATIVE)
				enemy->attackSpeed /= buff->GetValue();
			else if (buff->GetType() == BUFF_TYPE::ADDITIVE)
				enemy->attackSpeed += buff->GetValue();
		}
		break;

	case ROL::VELOCITY:
		if (buff->GetCharacter()->type == ENTITY_TYPE::PLAYER)
		{
			if (buff->GetType() == BUFF_TYPE::MULTIPLICATIVE)
			{
				player->characterBaseSpeed.x /= buff->GetValue();
				player->characterBaseSpeed.y /= buff->GetValue();
			}
			else if (buff->GetType() == BUFF_TYPE::ADDITIVE)
			{
				player->characterBaseSpeed.x -= buff->GetValue();
				player->characterBaseSpeed.y -= buff->GetValue();
			}
		}
		else if (buff->GetCharacter()->type == ENTITY_TYPE::ENEMY_TEST || 
			buff->GetCharacter()->type == ENTITY_TYPE::ENEMY_BOMB ||
			buff->GetCharacter()->type == ENTITY_TYPE::ENEMY_ARCHER)
		{
			if (buff->GetType() == BUFF_TYPE::MULTIPLICATIVE)
			{
				enemy->speed /= buff->GetValue();
			}

			else if (buff->GetType() == BUFF_TYPE::ADDITIVE)
			{
				enemy->speed -= buff->GetValue();
			}
		}
		break;
	case ROL::HEALTH:
		if (buff->GetCharacter()->type == ENTITY_TYPE::PLAYER)
		{
			if (buff->GetType() == BUFF_TYPE::MULTIPLICATIVE)
			{
				App->entityFactory->player->maxLife /= buff->GetValue();
				App->entityFactory->player->life /= buff->GetValue();
			}
			else if (buff->GetType() == BUFF_TYPE::ADDITIVE)
			{
				App->entityFactory->player->maxLife -= buff->GetValue();
				App->entityFactory->player->life -= buff->GetValue();
			}



			App->gui->healthBar->RecalculateSection(); 

		}
		else if (buff->GetCharacter()->type == ENTITY_TYPE::ENEMY_TEST ||
			buff->GetCharacter()->type == ENTITY_TYPE::ENEMY_BOMB ||
			buff->GetCharacter()->type == ENTITY_TYPE::ENEMY_ARCHER)
		{
			if (buff->GetType() == BUFF_TYPE::MULTIPLICATIVE)
			{
				enemy->maxLife /= buff->GetValue();
				enemy->life /= buff->GetValue();
			}

			else if (buff->GetType() == BUFF_TYPE::ADDITIVE)
			{
				enemy->maxLife -= buff->GetValue();
				enemy->life -= buff->GetValue();
			}
		}
		break;

	default:
		break;
	}
}

void j1BuffManager::DeleteBuff(Buff* buff)
{
	std::list<Buff*>::iterator item = buffs.begin();
	for (; item != buffs.end(); ++item)
	{
		if ((*item) == buff)
		{
			if ((*item)->GetRol() != ROL::ATTACK_ROL && (*item)->GetRol() != ROL::DEFENCE_ROL)
				ResetEntityVariables(*item);

			buffs.remove(*item);
		}
	}
}

void j1BuffManager::AddItemStats(LootEntity * item)
{
	std::vector<Buff*>::iterator iter = item->stats.begin();
	for (; iter != item->stats.end(); ++iter)
	{
		if ((*iter)->GetRol() == ROL::ATTACK_ROL || (*iter)->GetRol() == ROL::DEFENCE_ROL)
			buffs.push_back(*iter);

		else
		{
			ChangeEntityVariables((*iter)->GetCharacter(), (*iter)->GetType(), (*iter)->GetRol(), (*iter)->GetValue());
			buffs.push_back(*iter);
		}

		
	}
}


void j1BuffManager::RemoveItemStat(const LootEntity * item)
{
	/*std::list<Buff*>::iterator iter = buffs.begin();
	for (; iter != buffs.end(); ++iter)
	{
		if ((*iter)->GetItemObject() != nullptr && item == nullptr)
		{
			if ((*iter)->GetItemObject() == item)
			{
				if ((*iter)->GetRol() != ROL::ATTACK_ROL && (*iter)->GetRol() != ROL::DEFENCE_ROL)
					App->buff->ResetEntityVariables(*iter);

				buffs.remove(*iter);
			}
		}
	}*/
	std::vector<Buff*>::const_iterator iter = item->stats.begin();
	for (; iter != item->stats.end(); ++iter)
	{
		

		if ((*iter)->GetRol() == ROL::HEALTH)
			App->gui->healthBar->RecalculateSection();
		
		if ((*iter)->GetRol() != ROL::ATTACK_ROL && (*iter)->GetRol() != ROL::DEFENCE_ROL)
			App->buff->ResetEntityVariables(*iter);
		buffs.remove(*iter);


		
	}
}

bool j1BuffManager::DamageInTime(j1Entity* entity)
{ 

	BROFILER_CATEGORY("Damage in Time", Profiler::Color::ForestGreen);
	if (godMode && entity == App->entityFactory->player)
		return true;
	bool ret = false;
	iPoint drawRectified;
	if (entity == App->entityFactory->player)
	{
		drawRectified.x = dynamic_cast<PlayerEntityManager*>(entity)->selectedCharacterEntity->position.x + dynamic_cast<PlayerEntityManager*>(entity)->selectedCharacterEntity->size.x * 0.5f;
		drawRectified.y = dynamic_cast<PlayerEntityManager*>(entity)->selectedCharacterEntity->position.y + dynamic_cast<PlayerEntityManager*>(entity)->selectedCharacterEntity->size.y * 0.5f;
		
	}
	else
	{
		drawRectified.x = entity->position.x + entity->size.x * 0.5f;
		drawRectified.y = entity->position.y + (entity->size.y * 0.5f) * 2;
	}
	iPoint bloodRect = drawRectified;

	// flip particles pseudo randomly
	SDL_RendererFlip renderFlip = SDL_RendererFlip::SDL_FLIP_NONE;

	if (rand() % 2 == 0)
	{
		renderFlip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	}

	std::list<entityStat*>::iterator item = entity->stat.begin();
	for (; item != entity->stat.end(); ++item)
	{
		if (*item != nullptr)
		{
			if ((*item)->to_paralitze == true)
			{
				if (!entity->isParalize)
					AdjustEntityAnimationSpeed(entity);
				entity->isParalize = true;
			}
			switch ((*item)->type)
			{
			case STAT_TYPE::BURNED_STAT:

				if ((*item)->totalTime > 0)
				{
					if ((*item)->count.ReadSec() > 1)
					{
						if (entity->type == ENTITY_TYPE::PLAYER)
						{
							App->entityFactory->player->life -= (*item)->secDamage;
							App->entityFactory->dmgInTimeFdbck = true;
							App->input->DoGamePadRumble(70, 50);


							App->gui->healthBar->TotalLabel->ChangeTextureIdle(std::to_string((int)App->entityFactory->player->life), NULL, NULL);

						}
						else
						{
							entity->life -= (*item)->secDamage;
							// Calling hitpoint label when it deals dmg
							App->HPManager->callHPLabelSpawn(iPoint(entity->position.x, entity->position.y), (*item)->secDamage, ELEMENTAL_TYPE::FIRE_ELEMENT);
						}
						(*item)->count.Start();
						--(*item)->totalTime;

						//add particles burned 
						iPoint fire01Pivot = { 8, 48 };
						drawRectified -= fire01Pivot;
						App->particles->AddParticle(App->particles->fire01, drawRectified.x, drawRectified.y + entity->pivot.y/2, { 0,0 }, 0u, renderFlip);
						App->audio->PlayFx(burnedSFX);
						//if (entity->type == ENTITY_TYPE::ENEMY_TEST)
						//	App->audio->PlayFx(App->entityFactory->goblinDamaged, 0);
						//TODO: call create hitpoint label


						// Reset drawRectified
						drawRectified += fire01Pivot;

						//add blood particles 
						iPoint bloodPivot = { 10, 10 };
						bloodRect -= bloodPivot;
						App->particles->AddParticle(App->particles->blood02, bloodRect.x, bloodRect.y, { 0,0 }, 0u, renderFlip);
						if ((*item)->to_paralitze == true)
						{
							iPoint stonePivot = { 8, 48 };
							drawRectified -= stonePivot;
							// TODO Add SFX ?
							//App->audio->PlayFx(healingSFX, 0);
							App->particles->AddParticle(App->particles->stone01, drawRectified.x + 10, drawRectified.y, { 0,0 }, 0u, renderFlip);
						}
					}
				}
				else
				{
					entity->isBurned = false;
					entity->stat.remove(*item);
				}
				break;
			case STAT_TYPE::ICE_STAT:
				if ((*item)->totalTime == 0)
				{
					entity->stat.remove(*item);
					entity->isFrozen = false;
					if ((*item)->to_paralitze)
					{
						if (entity->isParalize)
							AdjustEntityAnimationSpeed(entity);
						entity->isParalize = false;
					}

				}
				else
				{
					if ((*item)->count.ReadSec() > 1)
					{
						--(*item)->totalTime;
						(*item)->count.Start();
						//add particles ice 
						iPoint ice01Pivot = { 8, 48 };

						drawRectified -= ice01Pivot;
						App->particles->AddParticle(App->particles->ice02, drawRectified.x, drawRectified.y, { 0,0 }, 0u, renderFlip);
						App->audio->PlayFx(freezedSFX);
						// Reset drawRectified
						drawRectified += ice01Pivot;
						if (entity->type == ENTITY_TYPE::PLAYER)
						{
							App->entityFactory->player->life -= (*item)->secDamage;
							App->entityFactory->dmgInTimeFdbck = true;
							App->input->DoGamePadRumble(70, 50);


							App->gui->healthBar->TotalLabel->ChangeTextureIdle(std::to_string((int)App->entityFactory->player->life), NULL, NULL);
						}
						else
						{
							entity->life -= (*item)->secDamage;
							// Calling hitpoint label when it deals dmg
							App->HPManager->callHPLabelSpawn(iPoint(entity->position.x, entity->position.y), (*item)->secDamage, ELEMENTAL_TYPE::ICE_ELEMENT);
						}
						// remove previous hitpoint link

						//add blood particles
						iPoint bloodPivot = { 10, 10 };
						bloodRect -= bloodPivot;
						App->particles->AddParticle(App->particles->blood02, bloodRect.x, bloodRect.y, { 0,0 }, 0u, renderFlip);

						if (entity->type == ENTITY_TYPE::ENEMY_TEST)
							App->audio->PlayFx(App->scene->goblinDamaged, 0);

						if (entity->type == ENTITY_TYPE::ENEMY_BOMB)
							App->audio->PlayFx(App->scene->bombgetHitSFX, 0);
						
						//TODO: call create hitpoint label
					}
				}
				break;

			case STAT_TYPE::POISON_STAT:
				if ((*item)->totalTime > 0)
				{
					if ((*item)->count.ReadSec() > 1)
					{
						if (entity->type == ENTITY_TYPE::PLAYER)
						{
							App->entityFactory->player->life -= (*item)->secDamage;
							App->entityFactory->dmgInTimeFdbck = true;
							App->input->DoGamePadRumble(70, 50);


							App->gui->healthBar->TotalLabel->ChangeTextureIdle(std::to_string((int)App->entityFactory->player->life), NULL, NULL);
						}
						else
						{
							entity->life -= (*item)->secDamage;
							// Calling hitpoint label when it deals dmg
							App->HPManager->callHPLabelSpawn(iPoint(entity->position.x, entity->position.y), (*item)->secDamage, ELEMENTAL_TYPE::POISON_ELEMENT);
						}
						(*item)->count.Start();
						--(*item)->totalTime;

						//add particles poison
						iPoint Poison01Pivot = { 8, 16 };
						drawRectified -= Poison01Pivot;
						App->particles->AddParticle(App->particles->poison01, drawRectified.x - 15, drawRectified.y - entity->pivot.y*0.5, { 0,0 }, 0u, renderFlip);
						App->particles->AddParticle(App->particles->poison01, drawRectified.x + 15, drawRectified.y - entity->pivot.y*0.5, { 0,0 }, 0u);
						App->audio->PlayFx(poisonedSFX, 0);
						if (entity->type == ENTITY_TYPE::ENEMY_TEST)
								App->audio->PlayFx(App->scene->goblinDamaged, 0);
						if (entity->type == ENTITY_TYPE::ENEMY_BOMB)
							App->audio->PlayFx(App->scene->bombgetHitSFX, 0);
						//TODO: call create hitpoint label

						// Reset drawRectified
						drawRectified += Poison01Pivot;

						//add blood paricle
						iPoint bloodPivot = { 10, 10 };
						bloodRect -= bloodPivot;
						App->particles->AddParticle(App->particles->blood02, bloodRect.x, bloodRect.y, { 0,0 }, 0u, renderFlip);
					}
				}
				else
				{
					if ((*item)->to_paralitze)
					{
						if (entity->isParalize)
							AdjustEntityAnimationSpeed(entity);
						entity->isParalize = false;
					}
					entity->isPosioned = false;
					entity->stat.remove(*item);
				}
				break;

			case STAT_TYPE::POTION_STAT:

				if ((*item)->totalTime > 0)
				{
					if ((*item)->count.ReadSec() > 1)
					{
						entity->life += (*item)->secDamage;
						// TODO Add hp label of health
						App->HPManager->callHPLabelSpawn(iPoint(entity->position.x, entity->position.y), (*item)->secDamage, ELEMENTAL_TYPE::NO_ELEMENT, true);
						(*item)->count.Start();
						--(*item)->totalTime;

						//add particles healing
						iPoint healthPivot = { 8, 40 };
						drawRectified -= healthPivot;
						App->audio->PlayFx(healingSFX, 0);
						App->particles->AddParticle(App->particles->healing, drawRectified.x + 10, drawRectified.y, { 0,0 }, 0u, renderFlip);



						App->gui->healthBar->TotalLabel->ChangeTextureIdle(std::to_string((int)App->entityFactory->player->life), NULL, NULL);


						if (entity->life > entity->maxLife)
							entity->life = entity->maxLife;
					}
				}
				else
				{
					entity->isPotionActive = false;
					entity->stat.remove(*item);
				}
				break;
			case STAT_TYPE::ATTACK_BUFF:
				if ((*item)->totalTime > 0)
				{
					if ((*item)->count.ReadSec() > 1)
					{
						(*item)->count.Start();
						--(*item)->totalTime;
					}
				}
				else
				{
					DeleteBuff((*item)->temporalBuff);
					(*item)->temporalBuff = nullptr;
					entity->stat.remove(*item);
				}
				break;
			case STAT_TYPE::DEFENCE_BUFF:
				if ((*item)->totalTime > 0)
				{
					if ((*item)->count.ReadSec() > 1)
					{
						(*item)->count.Start();
						--(*item)->totalTime;
					}
				}
				else
				{
					DeleteBuff((*item)->temporalBuff);
					(*item)->temporalBuff = nullptr;
					entity->stat.remove(*item);
				}
				break;
			case STAT_TYPE::SPEED_BUFF:
				if ((*item)->totalTime > 0)
				{
					if ((*item)->count.ReadSec() > 1)
					{
						(*item)->count.Start();
						--(*item)->totalTime;
					}
				}
				else
				{
					DeleteBuff((*item)->temporalBuff);
					(*item)->temporalBuff = nullptr;
					entity->stat.remove(*item);
				}
				break;
			case STAT_TYPE::HEALTH_BUFF:
				if ((*item)->totalTime > 0)
				{
					if ((*item)->count.ReadSec() > 1)
					{
						(*item)->count.Start();
						--(*item)->totalTime;
					}
				}
				else
				{
					DeleteBuff((*item)->temporalBuff);
					(*item)->temporalBuff = nullptr;
					entity->stat.remove(*item);
				}
				break;
			case STAT_TYPE::NORMAL:
				break;
			default:
				break;
			}
		}
	}
	if (entity->life <= 0 && entity->type != ENTITY_TYPE::PLAYER)
	{
		
		App->entityFactory->DeleteEntityFromSubtile(entity);
		entity->isParalize = false;
		entity->to_die = true;
		return true;
	}
	else if (entity->life < 0 && entity->type == ENTITY_TYPE::PLAYER && !App->scene->ComeToDeath)
	{
		App->scene->isDeath = true;
		App->pause = true;
	}
	if (entity->stat.size() == 0)
		ret = true;


	return ret;
}

void j1BuffManager::AdjustEntityAnimationSpeed(j1Entity* entity)
{
	switch (entity->type)
	{
	case ENTITY_TYPE::PLAYER:
	{
		if (!entity->isParalize && entity->currentAnimation->speed != 0.f)
		{
			dynamic_cast<PlayerEntity*>(entity)->lastAnimationSpeed = dynamic_cast<PlayerEntity*>(entity)->currentAnimation->speed;
			dynamic_cast<PlayerEntity*>(entity)->currentAnimation->speed = 0.f;
		}
		else
			dynamic_cast<PlayerEntity*>(entity)->currentAnimation->speed = dynamic_cast<PlayerEntity*>(entity)->lastAnimationSpeed;
		break;
	}
	
	case ENTITY_TYPE::ENEMY_TEST || ENTITY_TYPE::ENEMY_BOMB || ENTITY_TYPE::ENEMY_ARCHER:
	{
		if (!entity->isParalize && entity->currentAnimation->speed != 0.f)
		{
			dynamic_cast<Enemy*>(entity)->lastAnimationSpeed = dynamic_cast<Enemy*>(entity)->currentAnimation->speed;
			dynamic_cast<Enemy*>(entity)->currentAnimation->speed = 0.f;
		}
		else
			dynamic_cast<Enemy*>(entity)->currentAnimation->speed = dynamic_cast<Enemy*>(entity)->lastAnimationSpeed;
		break;
	}
	default:
		break;
	}
}


fPoint j1BuffManager::getPlayerToEnemyVec(j1Entity* origin, j1Entity* dest)
{
	fPoint vec;

	vec.x = origin->GetPivotPos().x - dest->GetPivotPos().x;
	vec.y = origin->GetPivotPos().y - dest->GetPivotPos().y;
	
	vec.Normalize();
	LOG("xlabel %f", vec.x);
	LOG("ylabel %f", vec.y);
	
	return vec;
}

bool j1BuffManager::Load(pugi::xml_node &node)
{

	return true;
}

// TODO: random crash here
// sometimes when the player dies
bool j1BuffManager::Save(pugi::xml_node &node) const
{

	return true;
}