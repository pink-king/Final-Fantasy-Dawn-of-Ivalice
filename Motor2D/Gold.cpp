#include "Gold.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1BuffManager.h"
#include "j1Entity.h"
#include "j1EntityFactory.h"


Gold::Gold(int posx, int posy, int value) :LootEntity(posx, posy)
{
	loot_type = LOOT_TYPE::GOLD;

	lootTex = App->tex->Load("textures/loot/Gold.png");
	entityTex = lootTex;

	 value = App->loot->goldearned;

	 // gold_pos = position;
}

Gold::~Gold()
{
}

bool Gold::Update(float dt)
{
	//SetPivot(16, 32);

	return true;
}

bool Gold::CleanUp()
{
	return true;
}

