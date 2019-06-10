#include "EnemyDummy.h"
#include "j1BuffManager.h"

EnemyDummy::EnemyDummy(const iPoint & pos) : Enemy(pos, 0, 0, 0, 0, 0, true, ENTITY_TYPE::ENEMY_DUMMY, "Scarecrow")
{
	entityTex = App->tex->Load("textures/map_props/shop/shop_props.png");

	//idle.PushBack({ 129, 64, 64, 64 });
	//idle.PushBack({ 146, 68, 27, 56 }); // Armor
	idle.PushBack({ 270, 70, 35, 52 }); // ScareCrow
	idle.loop = false; 
	currentAnimation = &idle;

	//SetPivot(13, 46); // Armor
	SetPivot(17, 45); // ScareCrow
	size.create(35, 52);

	position -= pivot; 

	life = maxLife;

	// re-align all scarecrow instantiated without flip direction
	flip = SDL_RendererFlip::SDL_FLIP_NONE;
}

bool EnemyDummy::CleanUp()
{
	App->tex->UnLoad(entityTex); 
	entityTex = nullptr; 

	std::list<entityStat*>::iterator item = stat.begin();
	for (; item != stat.end(); )
	{
		delete *item;
		*item = nullptr;
		item = stat.erase(item);

	}
	stat.clear();



	return true;
}

EnemyDummy::~EnemyDummy()
{
}

bool EnemyDummy::Update(float dt)
{

	// Some kind of damage animation?
	if (stat.size() != 0 && !App->pause)
	{
		if (App->buff->DamageInTime(this))
		{
			App->buff->entitiesTimeDamage.remove(this);
		}
	}

	return true;
}

bool EnemyDummy::PostUpdate()
{
	if (life < maxLife)
		life += (maxLife - life) * 0.1F; // Restores 10 % of missing health 

	return true;
}


