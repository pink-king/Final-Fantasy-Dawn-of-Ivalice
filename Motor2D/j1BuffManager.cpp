#include "j1BuffManager.h"
#include <string.h>
#include "j1EntityFactory.h"

#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "UiItem_Image.h"

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
	return true;
}

bool j1BuffManager::Update(float dt)
{
	bool ret = true;

	if (entitiesTimeDamage.size() != 0)
	{
		std::list<j1Entity*>::iterator item = entitiesTimeDamage.begin();
		for (; item != entitiesTimeDamage.end() && ret; ++item)
			
			if (DamageInTime(*item))
				entitiesTimeDamage.remove(*item);
	}
	return ret;
}

bool j1BuffManager::CleanUp()
{
	std::list<Buff*>::iterator item = buffs.begin();

	while (item != buffs.end())
	{
		buffs.remove(*item);
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


void j1BuffManager::CreateBuff(BUFF_TYPE type, ELEMENTAL_TYPE elementType, ROL rol, j1Entity* character, std::string stat, float value)
{
	bool exist = false;
	std::list<Buff*>::iterator item = buffs.begin();
	buffs.push_back(new Buff(type, character,stat, elementType, rol, value));
	if (rol != ROL::ATTACK_ROL && rol != ROL::DEFENCE_ROL)
		ChangeEntityVariables(character, type, rol, value);
}

void j1BuffManager::RemoveBuff(j1Entity* character)
{
	std::list<Buff*>::iterator item = buffs.begin();
	for (; item != buffs.end(); ++item)
		if(character == (*item)->GetCharacter())
			buffs.remove(*item);
}

float j1BuffManager::CalculateStat(const j1Entity* ent,float initialDamage, ELEMENTAL_TYPE elementType, ROL rol, std::string stat)
{

	float totalMult = 0.f;
	if (ent == App->entityFactory->player)
	{
		ent = App->entityFactory->player->selectedCharacterEntity;
	}
	for (std::list<Buff*>::iterator iter = buffs.begin(); iter != buffs.end(); ++iter)
	{
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
			if ((elementType == (*iter)->GetElementType() || elementType == ELEMENTAL_TYPE::ALL_ELEMENTS) && rol == (*iter)->GetRol() &&
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


void j1BuffManager::DirectAttack(j1Entity * attacker, j1Entity* defender, float initialDamage, ELEMENTAL_TYPE elementType, std::string stat)
{
	float lifeToSubstract = CalculateStat(attacker, initialDamage, elementType, ROL::ATTACK_ROL, stat) - CalculateStat(defender, defender->defence, elementType, ROL::DEFENCE_ROL, stat);
	if (lifeToSubstract <= 0)
	{
		lifeToSubstract  = 1;
	}
	else
		defender->life -= lifeToSubstract;
	// add always a hitpoint
	// but if we have a previous one, unlink
	/*if (defender->hitPoint != nullptr)
	{
		defender->hitPoint->attachedEntity = nullptr;

	}
	else
	{*/
		App->HPManager->callHPLabelSpawn(iPoint(defender->position.x, defender->position.y), lifeToSubstract, ELEMENTAL_TYPE::NO_ELEMENT); // must be overall improved /types of damage? calculate
//	}
		
	
		if (defender->type == ENTITY_TYPE::PLAYER)
		{
			if (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetMarche())
			{
				App->audio->PlayFx(App->entityFactory->marcheDamaged, 0);
			}

			else if (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetRitz())
			{
				App->audio->PlayFx(App->entityFactory->RitzDamaged, 0);
			}

			else if (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetShara())
			{
				App->audio->PlayFx(App->entityFactory->SharaDamaged, 0);
			}
		}

		if (defender->type == ENTITY_TYPE::ENEMY_TEST)
			App->audio->PlayFx(App->entityFactory->goblinDamaged, 0);
																													  
	if (elementType == ELEMENTAL_TYPE::FIRE_ELEMENT)
	{
		if (App->entityFactory->GetRandomValue(1, 10) == 1)
		{
			CreateBurned(attacker, defender, initialDamage*0.1, 10, "\0");
		}
	}

	if (elementType == ELEMENTAL_TYPE::ICE_ELEMENT)
	{
		if (App->entityFactory->GetRandomValue(1, 10) == 1)
		{
			CreateParalize(attacker, defender, 3);
		}
	}
																													  // but, enemy can die now
	
	if (defender->life <= 0 && defender->type != ENTITY_TYPE::PLAYER) // ONLY FOR DELETE
	{
		RemoveBuff(defender);
		entitiesTimeDamage.remove(defender);
		//defender->to_delete = true;   
		// When we kill the player we will have a diying animation aswell (or tell him to delete), as for now, only come here ENEMIES or PLAYERS, so should be fine
		//If causes any trouble put it back without any problem
		defender->to_die = true;
	} 

	
}

void j1BuffManager::CreateBurned(j1Entity* attacker, j1Entity* defender, float damageSecond, uint totalTime, std::string stat)
{
	entityStat* newStat = new entityStat(STAT_TYPE::BURNED_STAT,totalTime, damageSecond);
	newStat->secDamage = CalculateStat(attacker, newStat->secDamage, ELEMENTAL_TYPE::FIRE_ELEMENT, ROL::ATTACK_ROL, stat) + CalculateStat(defender, defender->defence, ELEMENTAL_TYPE::FIRE_ELEMENT, ROL::DEFENCE_ROL, stat);
	defender->stat.push_back(newStat);
	defender->isBurned = true;
	entitiesTimeDamage.push_back(defender);
	newStat->count.Start();
}

void j1BuffManager::CreateParalize(j1Entity * attacker, j1Entity * defender, uint time)
{
	entityStat* newStat = new entityStat(STAT_TYPE::PARALIZE_STAT, time);
	newStat->count.Start();
	defender->stat.push_back(newStat);
	defender->isParalize = true;
	entitiesTimeDamage.push_back(defender);
}

void j1BuffManager::CreateHealth(j1Entity* entity, float lifeSecond, uint time)
{
	entityStat* newStat = new entityStat(STAT_TYPE::POTION_STAT, time,lifeSecond);
	newStat->count.Start();
	entity->stat.push_back(newStat);
	entity->isPotionActive = true;
	entitiesTimeDamage.push_back(entity);
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
					player->coolDownData.dodge.cooldownTime *= value;
					player->coolDownData.special1.cooldownTime *= value;
					player->coolDownData.special2.cooldownTime *= value;
					player->coolDownData.ultimate.cooldownTime *= value;
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
			else if (entity->type == ENTITY_TYPE::ENEMY_TEST)
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
			else if (entity->type == ENTITY_TYPE::ENEMY_TEST)
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
			}
			else if (entity->type == ENTITY_TYPE::ENEMY_TEST)
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
		else if (buff->GetCharacter()->type == ENTITY_TYPE::ENEMY_TEST)
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
		else if (buff->GetCharacter()->type == ENTITY_TYPE::ENEMY_TEST)
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
		}
		else if (buff->GetCharacter()->type == ENTITY_TYPE::ENEMY_TEST)
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
			ChangeEntityVariables((*iter)->GetCharacter(), (*iter)->GetType(),(*iter)->GetRol(), (*iter)->GetValue());
	}
}


void j1BuffManager::RemoveItemStat(const LootEntity * item)
{
	std::list<Buff*>::iterator iter = buffs.begin();
	for (; iter != buffs.end(); ++iter)
	{
		if ((*iter)->GetItemObject() == item)
		{
			if ((*iter)->GetRol() == ROL::ATTACK_ROL || (*iter)->GetRol() == ROL::DEFENCE_ROL)
				buffs.push_back(*iter);
			buffs.remove(*iter);
		}		
	}
}

bool j1BuffManager::DamageInTime(j1Entity* entity)
{ 
	bool ret = false;
	std::list<entityStat*>::iterator item = entity->stat.begin();
	for (; item != entity->stat.end(); ++item)
	{
		switch ((*item)->type)
		{
		case STAT_TYPE::BURNED_STAT:

			if ((*item)->totalTime > 0)
			{
				if ((*item)->count.ReadSec() > 1)
				{
					entity->life -= (*item)->secDamage;
					(*item)->count.Start();
					--(*item)->totalTime;
					// remove previous hitpoint link
					
					if (entity->type == ENTITY_TYPE::ENEMY_TEST)
						App->audio->PlayFx(App->entityFactory->goblinDamaged, 0);
					//TODO: call create hitpoint label
				}
			}
			else
			{
				entity->isBurned = false;
				entity->stat.remove(*item);
			}
			break;
		case STAT_TYPE::PARALIZE_STAT:
			if ((*item)->totalTime == 0)
			{
				entity->stat.remove(*item);
				entity->isParalize = false;
			}
			else
			{
				if ((*item)->count.ReadSec() > 1)
				{
					--(*item)->totalTime;
					(*item)->count.Start();
				}
			}
			break;
		case STAT_TYPE::POTION_STAT:

			if ((*item)->totalTime > 0)
			{
				if ((*item)->count.ReadSec() > 1)
				{
					entity->life += (*item)->secDamage;
					(*item)->count.Start();
					--(*item)->totalTime;
					if (entity->life > entity->maxLife)
						entity->life = entity->maxLife;
				}
			}
			else
			{
				entity->isPotionActive = false;
				entity->stat.remove(*item);
			}
		case STAT_TYPE::NORMAL:
			break;
		default:
			break;
		}
	}
	if (entity->life <= 0 && entity->type != ENTITY_TYPE::PLAYER)
	{
		//entity->to_delete = true;
		// When we kill the player we will have a diying animation aswell (or tell him to delete), as for now, only come here ENEMIES or PLAYERS, so should be fine
		//If causes any trouble put it back without any problem
		entity->to_die = true;
		return true;
	}
	if (entity->stat.size() == 0)
		ret = true;


	return ret;
}
