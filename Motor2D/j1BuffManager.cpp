#include "j1BuffManager.h"
#include <string.h>
#include "j1EntityFactory.h"
#include "j1Window.h"
#include "j1Map.h"
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
	else
	{
		std::list<j1Entity*>::iterator item = entitiesTimeDamage.begin();
		for (; item != entitiesTimeDamage.end() && ret; ++item)
			DamageInTime(*item);
	}

	/*static char title[30];
	std::string name;
	std::list<Buff*>::iterator item =buffs.begin();
	for (; item != buffs.end(); ++item)
	{

		name = (*item)->GetName();
		sprintf_s(title, 30, " |  buff: %s", name.data());
		App->win->AddStringToTitle(title);
		
	}
	App->win->ClearTitle();
*/
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


void j1BuffManager::CreateEnemyBuff(BUFF_TYPE type, ELEMENTAL_TYPE elementType, OBJECT_ROL rol, std::string character, float value)
{
	bool exist = false;
	std::list<Buff*>::iterator item = buffs.begin();
	if(buffs.size() == 0)
		buffs.push_back(new Buff(type, character, elementType, rol, value));
	else
	{
		for (; item != buffs.end(); ++item)
		{
			if ((*item)->GetIfExist(type, character, elementType, rol))
				exist = true;
		}

		if (!exist)
		{
			buffs.push_back(new Buff(type, character, elementType, rol, value));
		}
	}
}

void j1BuffManager::RemoveEnemyBuff(std::string character)
{
	std::list<Buff*>::iterator item = buffs.begin();
	for (; item != buffs.end(); ++item)
		if(name.compare((*item)->GetCharacter()) == 0)
			buffs.remove(*item);
}

float j1BuffManager::CalculateStat(const j1Entity* ent,float initialDamage, ELEMENTAL_TYPE elementType, OBJECT_ROL rol)
{

	float totalMult = 0.f;
	for (std::list<Buff*>::iterator iter = buffs.begin(); iter != buffs.end(); ++iter)
	{
		if (elementType == (*iter)->GetElementType() && rol == (*iter)->GetRol() &&
			(ent->name.compare((*iter)->GetCharacter()) == 0 || ent->name.compare("all") == 0))
		{
					if ((*iter)->GetType() == BUFF_TYPE::ADDITIVE)
						initialDamage += (*iter)->GetValue();

					else if ((*iter)->GetType() == BUFF_TYPE::MULTIPLICATIVE)
						totalMult += (*iter)->GetValue();	
		}
	}
	
	return initialDamage * (1 + totalMult);
}


void j1BuffManager::DirectAttack(j1Entity * attacker, j1Entity* defender, float initialDamage, ELEMENTAL_TYPE elementType)
{
	defender->life -= CalculateStat(attacker, initialDamage, elementType, OBJECT_ROL::ATTACK_ROL) - CalculateStat(attacker, defender->defence, elementType, OBJECT_ROL::DEFENCE_ROL);
	if (defender->life <= 0 && defender->name.compare("Marche") != 0 && defender->name.compare("Ritz") != 0 && defender->name.compare("Shara") != 0)
	{
		
		defender->to_delete = true;
		

	} 
}

void j1BuffManager::CreateBurned(j1Entity* attacker, j1Entity* defender, float damageSecond, uint totalTime)
{
	entityStat* newStat = new entityStat(STAT_TYPE::BURNED_STAT,totalTime, damageSecond);
	newStat->secDamage = CalculateStat(attacker, newStat->secDamage, ELEMENTAL_TYPE::FIRE_ELEMENT, OBJECT_ROL::ATTACK_ROL) + CalculateStat(defender, defender->defence, ELEMENTAL_TYPE::FIRE_ELEMENT, OBJECT_ROL::DEFENCE_ROL);
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


void j1BuffManager::ActiveBuff(std::string buffName, std::string character, OBJECT_TYPE clasType)
{
	
}

void j1BuffManager::DeleteBuff(Buff* buff)
{
	std::list<Buff*>::iterator item = buffs.begin();
	for (; item != buffs.end(); ++item)
	{
		if ((*item) == buff)
			buffs.remove(*item);
	}
}

void j1BuffManager::AddItemStats(items * item)
{
	std::vector<Buff*>::iterator iter = item->stats.begin();
	for (; iter != item->stats.end(); ++iter)
	{
		buffs.push_back(*iter);
	}
}

void j1BuffManager::RemoveItemStat(items * item)
{
	std::vector<Buff*>::iterator iter = item->stats.begin();
	for (; iter != item->stats.end(); ++iter)
	{
		buffs.remove(*iter);
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
		case STAT_TYPE::NORMAL:
			break;
		default:
			break;
		}
	}
	if (entity->life <= 0 && entity->type != ENTITY_TYPE::PLAYER)
		entity->to_delete = true;
	if (entity->stat.size() == 0)
		ret = true;


	return ret;
}
