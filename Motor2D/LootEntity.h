#ifndef _LOOT_ENTITY_H__
#define _LOOT_ENTITY_H__

#include "p2Point.h"
#include "j1Entity.h"
#include "Buff.h"
#include "EnemyTest.h"
#include "PlayerEntity.h"
#include "j1PerfTimer.h"
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
	~LootEntity();

	bool Update(float dt);
	bool Start();
	std::string GetName();
	LOOT_TYPE GetType();
	OBJECT_TYPE GetObjectType();
	void Draw();
	iPoint GetPosition();
	int GetRandomValue(int min, int max);

	float EaseOutBack(float t);
	void CreateBuff(BUFF_TYPE type, j1Entity* character, std::string stat, ELEMENTAL_TYPE elementType, OBJECT_ROL rol, float value, LootEntity* item);
	fPoint Lerp(fPoint actual, fPoint destination, float t);
	float LerpX(float actual, float destination, float t);
	fPoint SetDestinationPos(float x, float y);
	EQUIPABLE_TYPE GetEquipable();

	void DecideExplosion();
	void ExplosionMaker(float dt);
public:

	uint level;

	iPoint Getoriginpos();
	iPoint GetFinalPos();
	iPoint lootSubtile;
	iPoint loot_pos;
	std::string lootname;
	PlayerEntity* player;
	EnemyTest* enemy;
	SDL_Rect loot_rect;
	SDL_Texture* goldTex;

	std::vector<Buff*>	stats;

	uint price = 10;
	EQUIPABLE_TYPE equipableType;
	ELEMENTAL_TYPE elemetalType;

	j1Entity* character = nullptr;

	int dmg;
	std::string itemname;

	bool endReached;
	float initialPosX;
	float initialPosY;
	fPoint goalPos;
	bool start;
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
	OBJECT_TYPE objectType = OBJECT_TYPE::NO_OBJECT;
	LOOT_TYPE loot_type = LOOT_TYPE::NO_LOOT;

};
#endif