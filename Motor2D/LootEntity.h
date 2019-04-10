#ifndef _LOOT_ENTITY_H__
#define _LOOT_ENTITY_H__

#include "p2Point.h"
#include "j1Entity.h"
#include "Buff.h"
#include "EnemyTest.h"
#include "PlayerEntity.h"
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

	NO_OBJECT
};

enum class EQUIPABLE_TYPE
{
	SWORD,
	BOW,
	ROD,
	ARMOR,
	VEST,
	MANTLE

};
class LootEntity : public j1Entity
{

public:
	LootEntity(LOOT_TYPE type, int posX, int posY);
	~LootEntity();

	virtual bool Update(float dt);
	std::string GetName();
	LOOT_TYPE GetType();
	OBJECT_TYPE GetObjectType();
	void Draw();
	int GetRandomValue(int min, int max);

	void CreateBuff(BUFF_TYPE type, std::string character, std::string stat, ELEMENTAL_TYPE elementType, OBJECT_ROL rol, float value);
	EQUIPABLE_TYPE GetEquipable();
public:

	iPoint loot_pos;
	std::string lootname;
	PlayerEntity* player;
	EnemyTest* enemy;
	SDL_Rect loot_rect;
	SDL_Texture* goldTex;

	std::vector<Buff*>	stats;

	uint price = 10;
	EQUIPABLE_TYPE equipableType;
public:
	//---Equipable stats---//
	int itemLevel;
	float dmg;
	float burn;
	float cooldown;
	float dmg_back;
	float attck_spd;
	float slow;
	float poisonBack;
	float heal;
	float paralize;
	int chanceTo;
protected:

	OBJECT_TYPE objectType = OBJECT_TYPE::NO_OBJECT;
	LOOT_TYPE loot_type = LOOT_TYPE::NO_LOOT;

};
#endif