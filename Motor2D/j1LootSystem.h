#ifndef _j1LOOTSYSTEM_H__
#define _j1LOOTSYSTEM_H__

#include "PugiXml/src/pugixml.hpp"
#include <list>
#include "j1Module.h"
#include<array>


class j1LootSystem : public j1Module
{
public:
	j1LootSystem();
	~j1LootSystem();
	bool Awake(pugi::xml_node &node);
	bool PostUpdate();
	
	void MatchGold(int id);

	//Gold* GetGoldFromID(int id);
	
public:
	bool trigger = false;
	int lootchance;
	int element;
	int goldearned;

	int gold_value[3];
	int gold_id[3];
	
private:
	
	int id;

	pugi::xml_node node;
	
	int GetRandomValue(int min,  int max, int ret_value);

	
	//std::list<uint> test;
	
};
#endif