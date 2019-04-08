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
	EQUIPABLE
	
};



class LootEntity : public j1Entity
{

public:
	LootEntity(int posX, int posY);
	~LootEntity();

	bool Update(float dt);
	std::string GetName();
	LOOT_TYPE GetType();
	int CheckPlayerPos(PlayerEntity*);
	void SetEnemyPos();
	void ChooseEntity();
	void Draw();
	int GetRandomValue(int min, int max);
	
public:
	
	int randomvalue;
	iPoint loot_pos;
	LOOT_TYPE loot_type;
	std::string lootname;
	PlayerEntity* player;
	EnemyTest* enemy;
	SDL_Rect loot_rect;
	SDL_Texture* goldTex;

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