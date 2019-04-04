#ifndef _LOOT_ENTITY_H__
#define _LOOT_ENTITY_H__

#include "p2Point.h"
#include "j1Entity.h"
#include "Buff.h"
#include "j1LootSystem.h"


struct SDL_Texture;

enum class LOOT_TYPE
{
	GOLD,
	POTION,
	EQUIPMENT
};

class LootEntity : public j1Entity
{
public:
	LootEntity(int posX, int posY);
	~LootEntity();

	bool PreUpdate();
	
	bool Update(float dt);
	bool PostUpdate();
	//virtual bool CleanUp();

	void Draw();
	LOOT_TYPE loot_type;
	bool enemy_dead = false;
	void GetEnemySubtile(int x, int y);
	SDL_Texture* lootTex;

	int value;
	//Pasar textures i posició de l'enemic, 
	//també pasar les variables del LootSystem
};
#endif