#ifndef _j1LOOTSYSTEM_H__
#define _j1LOOTSYSTEM_H__

#include "PugiXml/src/pugixml.hpp"
#include <list>
#include "j1Module.h"
#include "p2Point.h"
#include<array>


class j1LootSystem : public j1Module
{
public:
	j1LootSystem();
	~j1LootSystem();
	bool Awake(pugi::xml_node &node);
	bool PostUpdate();
	
	bool Update(float dt);
	bool PreUpdate();
	void MatchGold(int id);

	//Gold* GetGoldFromID(int id);
	int GetRandomValue(int min, int max);

public:
	bool trigger = false;
	int lootchance;
	int element;
	int goldearned;

	fPoint convTest;
	int gold_value[3];
	int gold_id[3];

	iPoint loot_pos;
	int loot_posX =0;
	int loot_posY =0;

	bool GoldSystem = false;
private:

	int id;

	pugi::xml_node node;

	
};
#endif