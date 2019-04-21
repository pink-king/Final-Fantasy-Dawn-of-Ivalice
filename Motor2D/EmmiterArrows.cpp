#include "EmmiterArrows.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1EntityFactory.h"
#include "j1AttackManager.h"
#include "j1Render.h"
#include "j1PathFinding.h"
#include "p2Defs.h"
#include "j1ParticlesClassic.h"

EmmiterArrows::EmmiterArrows(fPoint pos, fPoint destination, uint speed, const j1Entity* owner, uint timeLife)
	: Projectile(pos, destination, speed, owner, "Arrow", PROJECTILE_TYPE::EMMITER_ARROWS), timeLife(timeLife)
{
	// TODO SFX arrow throwing

	entityTex = App->tex->Load("textures/spells/Shara_ultimate/shara_ultimate_WIP.png");
	
	fall.PushBack({ 0, 0, 170, 310 });
	fall.speed = 5.5f;
	fall.loop = true;

	anim.PushBack({ 0, 0, 170, 398 });
	anim.PushBack({ 170, 0, 170, 398 });
	anim.PushBack({ 340, 0, 170, 398 });
	anim.PushBack({ 510, 0, 170, 398 });
	anim.PushBack({ 680, 0, 170, 398 });
	anim.PushBack({ 850, 0, 170, 398 });
	anim.PushBack({ 0, 429, 170, 398 });
	anim.PushBack({ 170, 429, 170, 398 });
	anim.speed = 10.5f;
	anim.loop = false;
	currentAnimation = &fall;

	SetPivot(85, 378);
	size.create(170, 398);

	damage = true;
	App->audio->PlayFx(App->entityFactory->SharaUltimateWoosh, 0);
	// Important for aiming offset
	position -= pivot;
	SetNewDirection(destination);
	angle = 0;
}

EmmiterArrows::~EmmiterArrows()
{
	LOG("Bye arrow!");
}

bool EmmiterArrows::PreUpdate()
{

	
	return true;
}

bool EmmiterArrows::Update(float dt)
{
	if (to_explode) {
		to_delete = true;
	}
	else {
		Move(dt);
	}
	return true;
}

bool EmmiterArrows::Move(float dt)
{
	if (position.y + size.y <= destination.y)
	{
		position += direction * speed * dt;
		willExplode = true;
	}
	else
	{
		if (willExplode)
		{
			App->audio->PlayFx(App->entityFactory->emitter_explodeFire, 0);
			willExplode = false;
		}
		if (damage)
		{
			currentAnimation = &anim;
			App->attackManager->AddPropagationAttack(App->entityFactory->player->GetShara(), GetSubtilePos(), propagationType::BFS,
				damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, 30, 4, 120, false);
			timeLifeTimer.Start();
			damage = false;
		}
		
		
		if (timeLife < timeLifeTimer.ReadSec())
			to_explode = true;
		
	}
	return true;
}


bool EmmiterArrows::CleanUp()
{
	if (entityTex != nullptr)
	{
		App->tex->UnLoad(entityTex);
		entityTex = nullptr;
	}

	return true;
}
