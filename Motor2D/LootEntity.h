#ifndef _LOOTENTITY_H__
#define _LOOTENTITY_H__

#include "p2Point.h"
#include "j1Entity.h"
#include "Buff.h"
#include "PlayerEntity.h"
#include "j1PerfTimer.h"
#include <random>

//#include "pcg_random.hpp"
#include "j1Scene.h"

#include <string>
#include "j1Gui.h"


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

	float EaseOutBack(float t);
	void CreateBuff(BUFF_TYPE type, j1Entity* character, std::string stat, ELEMENTAL_TYPE elementType, ROL rol, float value, LootEntity* item);
	fPoint Lerp(fPoint actual, fPoint destination, float t);
	float LerpX(float actual, float destination, float t);
	fPoint SetDestinationPos(float x, float y);
	EQUIPABLE_TYPE GetEquipable();

	void DecideExplosion();
	void ExplosionMaker(float dt);
public:
	iPoint Getoriginpos();
	iPoint GetFinalPos();
	iPoint lootSubtile;
	iPoint loot_pos;
	std::string lootname;

	SDL_Rect loot_rect;
	//SDL_Texture* goldTex;

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
	void CheckClampedCrossHairToSpawnDescription();

	bool clampedByCrosshair = false;

protected:

	int dmg;
	std::string itemname;

	bool endReached;
	float initialPosX;
	float initialPosY;
	fPoint goalPos;
	bool start;
	bool grounded;
	bool checkgrounded;
protected:
	iPoint originPos;
	float timeTest;
	float timeXmid;
	float timeYmid;
	float incrementY;
	float incrementX;
	float decrementY;
	float decrementX;
	j1PerfTimer displacementTime;
	LOOT_TYPE loot_type = LOOT_TYPE::NO_LOOT;

	std::random_device rd;
	std::mt19937 engine;
	
	//pcg_extras::seed_seq_from <std::random_device> seed_source;

	
};

#endif