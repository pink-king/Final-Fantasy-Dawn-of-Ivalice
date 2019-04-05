#ifndef _LOOTENTITYMAGANER_H__
#define _LOOTENTITYMAGANER_H__

#include "j1Module.h"
#include "Gold.h"
#include <vector>
#include <list>
class LootEntityManager : public j1Entity
{
public:

	LootEntityManager(iPoint position);
	~LootEntityManager();

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void SelectLoot();


public:
	Gold * gold = nullptr;
	LOOT_TYPE lootSelectType;
	LootEntity* selectedLootEntity = nullptr;

	std::vector<LootEntity*> lootItems;
	std::list<LootEntity*> lootList;
};
#endif // !LOOTENTITYMAGANER_H__
