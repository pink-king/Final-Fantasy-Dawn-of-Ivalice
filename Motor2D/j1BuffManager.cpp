#include "j1BuffManager.h"
#include "UiItem_HitPointManager.h"
#include <string.h>

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

	for (buffNode = node.child("buff"); buffNode && ret; buffNode = buffNode.next_sibling("buff"))
	{
		std::string add = buffNode.attribute("type").as_string();
		if (add.compare("additive") == 0)
			CreateBuff(BUFF_TYPE::ADDITIVE, buffNode.attribute("name").as_string(), buffNode.attribute("character").as_string(), buffNode.attribute("stat").as_string(), buffNode.attribute("value").as_float());
		
		else if (add.compare("multiplicative") == 0)
			CreateBuff(BUFF_TYPE::MULTIPLICATIVE, buffNode.attribute("name").as_string(), buffNode.attribute("character").as_string(), buffNode.attribute("stat").as_string(), buffNode.attribute("value").as_float());
	}

	burnedDamagesecond = node.child("timebuff").attribute("burnedInSecond").as_float();
	burnedTotalDamage = node.child("timebuff").attribute("burnedTotalDamage").as_float();
	paralizetime = node.child("timebuff").attribute("paralizeTime").as_float();
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


void j1BuffManager::CreateBuff(BUFF_TYPE type, std::string name, std::string character, std::string stat, float value)
{
	buffs.push_back(new Buff(type, name, character , stat, value, GetNewSourceID()));
}

void j1BuffManager::RemoveBuff(std::string name)
{
	std::list<Buff*>::iterator item = buffs.begin();
	for (; item != buffs.end(); ++item)
		if(name.compare((*item)->GetCharacter()) != 0)
			buffs.remove((*item));
}

float j1BuffManager::CalculateStat(const j1Entity* ent, float& initialDamage, std::string stat)
{

	float totalMult = 0.f;
	for (std::list<Buff*>::iterator iter = buffs.begin(); iter != buffs.end(); ++iter)
	{
		if (ent != nullptr && (*iter)->isActive)
		{
			if (stat.compare((*iter)->GetStat()) == 0)
			{
				if (ent->name.compare((*iter)->GetCharacter()) == 0 || ent->name.compare("all") == 0)
				{
					if ((*iter)->GetType() == BUFF_TYPE::ADDITIVE)
						initialDamage += (*iter)->GetValue();

					else if ((*iter)->GetType() == BUFF_TYPE::MULTIPLICATIVE)
						totalMult += (*iter)->GetValue();
				}
			}
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
}

void j1BuffManager::CreateBurned(j1Entity* attacker, j1Entity* defender, float damage)
{
	entityStat* newStat = new entityStat(STAT_TYPE::BURNED_STAT, damage);
	newStat->maxDamage = App->buff->CalculateStat(attacker, newStat->maxDamage, "basic") - App->buff->CalculateStat(defender, defender->defence, "deffence");
	newStat->count.Start();
	defender->stat.push_back(newStat);
	defender->isBurned = true;
	entitiesTimeDamage.push_back(defender);
}

void j1BuffManager::CreateParalize(j1Entity * attacker, j1Entity * defender)
{
	entityStat* newStat = new entityStat(STAT_TYPE::PARALIZE_STAT, 0.f);
	newStat->count.Start();
	defender->stat.push_back(newStat);
	defender->isParalize = true;
	entitiesTimeDamage.push_back(defender);
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

			if ((*item)->maxDamage + burnedDamagesecond > burnedDamagesecond)
			{
				if ((*item)->count.ReadSec() > 0.5)
				{
					entity->life -= burnedDamagesecond;
					(*item)->maxDamage -= burnedDamagesecond;
					(*item)->count.Start();
				}
			}
			else
			{
				entity->isBurned = false;
				entity->stat.remove(*item);
			}
			break;
		case STAT_TYPE::PARALIZE_STAT:
			if ((*item)->count.ReadSec() > paralizetime)
			{
				entity->stat.remove(*item);
				entity->isParalize = false;
			}
			break;
		case STAT_TYPE::NORMAL:
			break;
		default:
			break;
		}
	}
	if (entity->life < 0)
		entity->life = 0;
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