#ifndef _J1LOOTMANAGER_H__
#define _J1LOOTMANAGER_H__

#include "j1Module.h"
#include "EquipableLoot.h"
#include "ConsumableLoot.h"
#include <vector>
#include <list>
#include "j1App.h"
#include"j1Entity.h"
#include "EnemyTest.h"

struct SDL_Texture;
class j1LootManager : public j1Module
{
public:
	j1LootManager();
	~j1LootManager();

	//bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	//void GetItem(LootEntity*, j1Entity*);
	//bool GetSelectedLoot();

	iPoint SetLootPos(int x, int y);

	void CreateLoot(int x, int y, std::string name);
	iPoint GetEnemySubtile(j1Entity*);

	LOOT_TYPE WillDrop();
	int GetRandomValue(int min, int max);
	j1Entity* CreateLootType(int x , int y);

	bool CollectLoot(LootEntity* etityLoot);
	bool LoadLootData(LootEntity* lootentity, pugi::xml_node&);
public:

	bool toDrop = false;

	int health;
	int lootChance = 25;

	iPoint lootPos;
	iPoint mynewpos;
	
	std::vector<int> goldValue;
	std::vector<int> EquipmentID;
	
};

#endif // !LOOTENTITYMAGANER_H
