#ifndef __CHESTASSET_H__
#define __CHESTASSET_H__

#include "j1Entity.h"

enum class ChestType; 

class ChestAsset : public j1Entity
{
public:

	ChestAsset(const iPoint& pos, bool isOpened, ChestType type);
	~ChestAsset();

	//bool Start() override;	// Entity Factory doesn't Start the entities when reloading scenes!
	bool PreUpdate() override;

	void Draw() override;

	bool Load(pugi::xml_node&) override;
	bool Save(pugi::xml_node&) const override;

	void SpawnLoot();
	void OpenChest();

private:

	Animation idle;
	Animation opening;

	ChestType type; 
	bool isBossChest = false; 
	bool isAssigned = false;
	bool isOpened = false;
	bool toSpawnLoot = false; 
};



#endif