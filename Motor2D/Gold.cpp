#include "Gold.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1BuffManager.h"
#include "j1Entity.h"
#include "j1EntityFactory.h"
#include "j1Render.h"


Gold::Gold(int posx, int posy, int value) :LootEntity(posx, posy)
{
	loot_type = LOOT_TYPE::GOLD;

	lootTex = App->tex->Load("textures/loot/Gold.png");
	entityTex = lootTex;

	 value = App->loot->goldearned;

	 SetPivot(0, 0);
	 //size.create(32, 32);
	 rect_test = { 0,0,32,32 };
	 // gold_pos = position;
}

Gold::~Gold()
{
}

bool Gold::Update(float dt)
{
	App->render->Blit(lootTex, position.x, position.y, &rect_test, 1.0f);
	
	return true;
}

bool Gold::CleanUp()
{

	return true;
}

