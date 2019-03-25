#include "j1BuffManager.h"
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
	return ret;
}

bool j1BuffManager::Start()
{
	return true;
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
	defender->life -= powerAttack;
	if (defender->life < 0)
		defender->life = 0;
}

//void j1BuffManager::ZoneAttack(j1Entity * attacker, std::vector<j1Entity*> defenders, float initialDamage)
//{
//	float powerAttack = CalculateStat(attacker->name, initialDamage);
//}
