#ifndef _LOOTENTITYMAGANER_H__
#define _LOOTENTITYMAGANER_H__

#include "j1Module.h"
#include "Gold.h"

#include <vector>

class LootEntityManager : public j1Entity
{
public:

	LootEntityManager(iPoint position);
	~LootEntityManager();

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	//bool CleanUp();

	Gold* gold = nullptr;

	void SelectedItem();
public:

	LOOT_TYPE lootSelectType;
	LootEntity* selectedLootEntity = nullptr;

	std::vector<LootEntity*> lootItems;
};
#endif // !LOOTENTITYMAGANER_H__
