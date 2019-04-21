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

	entityTex = App->tex->Load("textures/spells/Ritz_attacks/Ritz_fx.png");

	anim.PushBack({ 0, 28, 45, 8 });
	anim.PushBack({ 45, 28, 45,8 });
	anim.PushBack({ 90, 28, 45,8 });
	anim.PushBack({ 135, 28, 45, 8 });
	anim.PushBack({ 180, 28, 45, 8 });
	anim.PushBack({ 225, 28, 45, 8 });
	anim.PushBack({ 270, 28, 45, 8 });
	anim.PushBack({ 315, 28, 45, 8 });
	anim.PushBack({ 360, 28, 45, 8 });
	anim.PushBack({ 405, 28, 45, 8 });
	anim.speed = (float)speed;

	currentAnimation = &anim;

	SetPivot(22, 4);
	size.create(45, 8);
	App->audio->PlayFx(App->entityFactory->SharaUltimateWoosh, 0);
	// Important for aiming offset
	SetInitially();
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



		App->attackManager->AddPropagationAttack(App->entityFactory->player->GetShara(), GetSubtilePos(), propagationType::BFS,
			damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, 30, 2, 20, true);
	}
	else {
		Move(dt);
	}
	
		
	

	return true;
}

bool EmmiterArrows::Move(float dt)
{
	if (position.y <= destination.y)
	{
		position += direction * speed * dt;
	}
	else
	{
		if (damage)
		{
			App->attackManager->AddPropagationAttack(App->entityFactory->player->GetShara(), GetSubtilePos(), propagationType::BFS,
				damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, 10, 1, 20, false);
			timeLifeTimer.Start();
			damage = false;
		}
		
		if (timeLife < timeLifeTimer.ReadSec())
		{
			to_explode = true;
			if (to_explode && willExplode)
			{
				willExplode = false;
				App->audio->PlayFx(App->entityFactory->emitter_explodeFire, 0);
			}
		}
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
