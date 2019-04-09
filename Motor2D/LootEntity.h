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
public:
	
	iPoint loot_pos;
	std::string lootname;
	PlayerEntity* player;
	EnemyTest* enemy;
	SDL_Rect loot_rect;
	SDL_Texture* goldTex;

	std::vector<Buff*>	stats;

	uint price = 10;

protected:

	OBJECT_TYPE objectType = OBJECT_TYPE::NO_OBJECT;
	LOOT_TYPE loot_type = LOOT_TYPE::NO_LOOT;

};

//class LootEntity : public j1Entity
//{
//public:
//	LootEntity(int posX, int posY);
//	~LootEntity();
//
//	bool PreUpdate();
//	
//	bool Update(float dt);
//	bool PostUpdate();
//	//virtual bool CleanUp();
//
//	void Draw();
//	LOOT_TYPE loot_type;
//	bool enemy_dead = false;
//	void SetLootSubtile(int x, int y);
//
//	SDL_Texture* lootTex;
//	SDL_Rect rect_test;
//	PlayerEntity* player;
//	EnemyTest* enemy;
//
//	int value;
//	//Pasar textures i posició de l'enemic, 
//	//també pasar les variables del LootSystem
//};
#endif