#include "j1BuffManager.h"


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

	for (buffNode = node.child("Buffs").child("buff"); buffNode && ret; buffNode = buffNode.next_sibling("buff"))
	{
		if (node.attribute("type").as_string() == "additive")
			CreateBuff(BUFF_TYPE::ADDITIVE, node.attribute("character").as_string(), node.attribute("stat").as_string(), node.attribute("value").as_float());

		else if (node.attribute("type").as_string() == "multiplicative")
			CreateBuff(BUFF_TYPE::MULTIPLICATIVE, node.attribute("character").as_string(), node.attribute("stat").as_string(), node.attribute("value").as_float());
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


void j1BuffManager::CreateBuff(BUFF_TYPE type, std::string name, std::string stat, float value)
{
	buffs.push_back(new Buff(type, name, stat, value, GetNewSourceID()));
}

void j1BuffManager::RemoveBuff(std::string name)
{
	std::list<Buff*>::iterator item = buffs.begin();
	for (; item != buffs.end(); ++item)
		if(name.compare((*item)->GetCharacter()) != 0)
			buffs.remove((*item));
}

float j1BuffManager::CalculateStat(const j1Entity* ent, float& initialDamage)
{

	float totalMult = 0.f;
	for (std::list<Buff*>::iterator iter = buffs.begin(); iter != buffs.end(); ++iter)
	{
		if (ent->name.compare((*iter)->GetCharacter()) == 0 || ent->name.compare("all") == 0)
		{
			if ((*iter)->GetType() == BUFF_TYPE::ADDITIVE)
				initialDamage += (*iter)->GetValue();
			
			else if ((*iter)->GetType() == BUFF_TYPE::MULTIPLICATIVE)
				totalMult += (*iter)->GetValue();
		}
	}

	return initialDamage + initialDamage * totalMult;
}

uint j1BuffManager::GetNewSourceID()
{
	return ++lastSourceID;
}

void j1BuffManager::DirectAttack(j1Entity * attacker, j1Entity* defender, float initialDamage)
{

	float powerAttack = CalculateStat(attacker, initialDamage);
	defender->life -= powerAttack;
	if (defender->life < 0)
		defender->life = 0;
}

//void j1BuffManager::ZoneAttack(j1Entity * attacker, std::vector<j1Entity*> defenders, float initialDamage)
//{
//	float powerAttack = CalculateStat(attacker->name, initialDamage);
//}
