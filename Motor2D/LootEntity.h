#ifndef _LOOTENTITY_H__
#define _LOOTENTITY_H__

#include "p2Point.h"
#include "j1Entity.h"
#include "Buff.h"
#include "PlayerEntity.h"
#include "j1PerfTimer.h"
#include <random>
#include <functional>
//#include "pcg_random.hpp"
#include "j1Scene.h"
#include "j1EasingSplines.h"

#include <string>
#include "j1Gui.h"

#define NUM_NEIGH_SUBTILETILE_FALL 32

struct SDL_Texture;

enum class LOOT_TYPE
{
	CONSUMABLE,
	EQUIPABLE,
	NO_LOOT
};

enum class OBJECT_TYPE
{
	WEAPON_OBJECT,
	ARMOR_OBJECT,
	HEAD_OBJECT,
	POTIONS,
	GOLD,
	PHOENIX_TAIL,

	NO_OBJECT
};

enum class EQUIPABLE_TYPE
{
	SWORD,
	BOW,
	ROD,
	ARMOR,
	VEST,
	MANTLE,
	NO_EQUIPABLE

};
enum class EXPLOSION_DIRECTION
{
	EAST,
	WEST,
	NORTHEAST,
	NORTHWEST,
	SOUTHEAST,
	SOUTHWEST
};
class LootEntity : public j1Entity
{

public:
	LootEntity(LOOT_TYPE type, int posX, int posY);
	LootEntity() {};
	~LootEntity();

	bool Update(float dt);
	bool Start();
	bool CleanUp();

	bool Load(pugi::xml_node& node, LootEntity* loot);
	bool Save(pugi::xml_node&) const;

	std::string GetName();
	LOOT_TYPE GetType();
	OBJECT_TYPE GetObjectType();
	void Draw();
	iPoint GetPosition();
	int GetRandomValue(int min, int max);
	//void GetAttributesForDescription(); 

	void CreateBuff(BUFF_TYPE type, j1Entity* character, std::string stat, ELEMENTAL_TYPE elementType, ROL rol, float value, LootEntity* item);
	float LerpX(float actual, float destination, float t);
	EQUIPABLE_TYPE GetEquipable();
	void ReRECTlootToLegRect(EQUIPABLE_TYPE equipable);

	std::list<iPoint> GetGroundTilePoints();
	void SetSplineToFall();
	void SelectSubtileToGround();


public:
	std::list<iPoint> groundSubtilePoints;
	iPoint adjacentTileNeighboursToGround[NUM_NEIGH_SUBTILETILE_FALL];

	iPoint groundSubtileDestination;

	iPoint Getoriginpos();
	iPoint lootSubtile;
	iPoint loot_pos;
	std::string lootname;
	SDL_Texture* objShadow = nullptr;

	SDL_Rect loot_rect;
	SDL_Rect shadow_rect;
	j1Timer shadowcastdelay;


	uint level;

	std::vector<Buff*>	stats;

	uint price = 10;
	uint vendorPrice = 20; 

	EQUIPABLE_TYPE equipableType = EQUIPABLE_TYPE::NO_EQUIPABLE;
	ELEMENTAL_TYPE elemetalType = ELEMENTAL_TYPE::NO_ELEMENT;

	j1Entity* character = nullptr;
	OBJECT_TYPE objectType = OBJECT_TYPE::NO_OBJECT;

	// - - - - - - - LootEntity stats in XML - - - - - - - // 
	std::string name; 


	// - - - - - - - The attached description  - - - - - - - // 
	UiItem_Description* MyDescription = nullptr; 
	bool spawnedDescription = false;
	//void CheckClampedCrossHairToSpawnDescription();

	bool clampedByCrosshair = false;
	bool clampedByPlayerOnTop = false; 

protected:
	int PlaySFXtime = 5;
	int dmg;
	std::string itemname;
	LOOT_TYPE loot_type = LOOT_TYPE::NO_LOOT;

	
	
	iPoint distanceTotravel;
	void GetDistanceTotravel();
	std::random_device rd;
	std::mt19937 engine;
	//pcg_extras::seed_seq_from <std::random_device> seed_source;

	
};

#endif