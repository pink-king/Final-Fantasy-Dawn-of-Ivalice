#include "Boss_Demon.h"
#include "j1Render.h"

DemonBossEntity::DemonBossEntity(iPoint position) : j1Entity(DEMONBOSS, position.x, position.y , "Demon Boss") 
{
	boss_spritesheet = App->tex->Load("textures/enemies/demon_boss_v1.png");

	// animations
	walkCycleAnim[0].PushBack({ 0,0,200,150 });


	// define values ------
	entityTex = boss_spritesheet;
	currentAnimation = &walkCycleAnim[0];
	SetPivot(100, 135);
	size.create(200, 150);
	this->position -= pivot;
}

DemonBossEntity::~DemonBossEntity() {}

//bool DemonBossEntity::Start()
//{
//	return true;
//}

bool DemonBossEntity::PreUpdate()
{
	return true;
}

bool DemonBossEntity::Update(float dt)
{
	return true;
}

bool DemonBossEntity::PostUpdate()
{
	return true;
}

bool DemonBossEntity::CleanUp()
{
	return true;
}

void DemonBossEntity::Draw()
{
	App->render->Blit(entityTex, position.x, position.y, &currentAnimation->GetCurrentFrame());
}