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

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	//void GetItem(LootEntity*, j1Entity*);
	//bool GetSelectedLoot();


	bool CheckEnemyDeath(EnemyTest*);

	void SetLootPos(int x, int y);
	//void GetAttributes(LootEntity*);
	//void SetAttributes(LootEntity*);

	//iPoint GetPlayerSubtile(PlayerEntity*);
	void CreateLoot(int x, int y, std::string name);
	iPoint GetEnemySubtile(EnemyTest*);

	void WillDrop();
	int GetRandomValue(int min, int max);

public:
	bool enemyDead = false;
	bool toDrop = false;

	
	int health;
	int lootChance = 1;

	iPoint lootPos;
	iPoint mynewpos;
	//SDL_Texture * goldTex;
	//SDL_Texture* potionHPTex;
	SDL_Texture* LootTexture;

	std::vector<int> goldValue;
	std::vector<int> EquipmentID;
	Consumable *consumableLoot;
	Equipable *equipableLoot;
};
//class LootEntityManager : public j1Entity
//{
//public:
//
//	LootEntityManager(iPoint position);
//	~LootEntityManager();
//
//	bool Start();
//
//	bool PreUpdate();
//	bool Update(float dt);
//	bool PostUpdate();
//	bool CleanUp();
//	void SelectLoot();
//
//
//public:
//	Gold * gold = nullptr;
//	LOOT_TYPE lootSelectType;
//	LootEntity* selectedLootEntity = nullptr;
//
//	std::vector<LootEntity*> lootItems;
//	std::list<LootEntity*> lootList;
//};
#endif // !LOOTENTITYMAGANER_H__
