#include "j1BuffManager.h"
#include "UiItem_HitPointManager.h"
#include <string.h>
#include "j1EntityFactory.h"
#include "j1Window.h"

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

	static char title[30];
	std::string name;
	std::list<Buff*>::iterator item =buffs.begin();
	for (; item != buffs.end(); ++item)
	{

		name = (*item)->GetName();
		sprintf_s(title, 30, " |  buff: %s", name.data());
		App->win->AddStringToTitle(title);
		
	}
	App->win->ClearTitle();

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


void j1BuffManager::CreateBuff(BUFF_TYPE type,OBJECT_TYPE clas, std::string name, std::string character, std::string stat, float value)
{
	bool exist = false;
	std::list<Buff*>::iterator item = buffs.begin();
	if(buffs.size() == 0)
		buffs.push_back(new Buff(type, clas, name, character, stat, value));
	else
	{
		for (; item != buffs.end(); ++item)
		{
			if (name.compare((*item)->GetName()) == 0 && ((*item)->GetCharacter()).compare(character) == 0)
				exist = true;
		}

		if (!exist)
		{
			std::list<Buff*>::iterator item2 = buffs.begin();
			for (; item2 != buffs.end(); ++item2)
			{
				if (clas == (*item2)->GetObjectType())
					buffs.remove(*item2);
			}
			buffs.push_back(new Buff(type, clas, name, character, stat, value));
		}
	}
}

void j1BuffManager::RemoveBuff(std::string name)
{
	std::list<Buff*>::iterator item = buffs.begin();
	for (; item != buffs.end(); ++item)
		if(name.compare((*item)->GetCharacter()) != 0)
			buffs.remove(*item);
}

float j1BuffManager::CalculateStat(const j1Entity* ent,float initialDamage, std::string stat)
{

	float totalMult = 0.f;
	for (std::list<Buff*>::iterator iter = buffs.begin(); iter != buffs.end(); ++iter)
	{
		if (stat.compare((*iter)->GetStat()) == 0 && (ent->name.compare((*iter)->GetCharacter()) == 0 || ent->name.compare("all") == 0))
		{
					if ((*iter)->GetType() == BUFF_TYPE::ADDITIVE)
						initialDamage += (*iter)->GetValue();

					else if ((*iter)->GetType() == BUFF_TYPE::MULTIPLICATIVE)
						totalMult += (*iter)->GetValue();	
		}
	}
	
	return initialDamage * (1 + totalMult);
}

uint j1BuffManager::GetNewSourceID()
{
	return ++lastSourceID;
}

void j1BuffManager::DirectAttack(j1Entity * attacker, j1Entity* defender, float initialDamage, std::string stat)
{

	float powerAttack = CalculateStat(attacker, initialDamage, stat);
	defender->life -= powerAttack;                                           // TODO: call HitPoint creation in HitPointManager here

	App->HPManager->callHPLabelSpawn(defender, powerAttack);

	if (defender->life < 0)
		defender->life = 0;

	defender->life -= CalculateStat(attacker, initialDamage, stat) - CalculateStat(attacker, defender->defence, stat);
	if (defender->life <= 0 && defender->name.compare("Marche") != 0 && defender->name.compare("Ritz") != 0 && defender->name.compare("Shara") != 0)
		defender->to_delete = true;
}

void j1BuffManager::CreateBurned(j1Entity* attacker, j1Entity* defender, float damageSecond, uint totalTime)
{
	entityStat* newStat = new entityStat(STAT_TYPE::BURNED_STAT,totalTime, damageSecond);
	newStat->secDamage = App->buff->CalculateStat(attacker, newStat->secDamage, "hability") - App->buff->CalculateStat(defender, defender->defence, "defence");
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
	std::list<Buff*>::iterator item = buffs.begin();
	for (; item != buffs.end(); ++item)
	{
		if (buffName.compare((*item)->GetName()) == 0)
			character.assign((*item)->GetCharacter());

		else
		{
			if (((*item)->GetCharacter()).compare(character) == 0 && clasType == (*item)->GetObjectType())
				buffs.remove(*item);
		}
			
	}
}

void j1BuffManager::DeleteBuff(std::string buffName)
{
	std::list<Buff*>::iterator item = buffs.begin();
	for (; item != buffs.end(); ++item)
	{
		if (buffName.compare((*item)->GetName()) == 0)
			buffs.remove(*item);
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

//void j1BuffManager::ZoneAttack(j1Entity * attacker, std::vector<j1Entity*> defenders, float initialDamage)
//{
//	float powerAttack = CalculateStat(attacker->name, initialDamage);
//}

float j1BuffManager::GetBurnedDamage()
{
	return burnedTotalDamage;
}