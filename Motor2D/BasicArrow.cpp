#include "BasicArrow.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1EntityFactory.h"
#include "j1AttackManager.h"
#include "j1Render.h"
#include "j1PathFinding.h"
#include "p2Defs.h"
#include "j1ParticlesClassic.h"

BasicArrow::BasicArrow(fPoint pos, fPoint destination, uint speed, const j1Entity* owner)
	: Projectile(pos, destination, speed, owner, "Arrow", PROJECTILE_TYPE::BASIC_ARROW)
{
	App->camera2D->AddTrauma(10.F / 100.f);
	App->input->DoGamePadRumble(0.3f, 80);
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

	// Important for aiming offset
	SetInitially();
}

BasicArrow::~BasicArrow()
{
	LOG("Bye arrow!");
}

bool BasicArrow::PreUpdate()
{
	if (OnCollisionWithEnemy()) {
		to_explode = true;
	}

	if (OnCollisionWithWall())	{
		to_delete = true; 
	}
	return true;
}

bool BasicArrow::Update(float dt)
{
	if (!to_explode) {
		Move(dt);
	}
	else Contact();

	return true;
}

bool BasicArrow::Move(float dt)
{
	position += direction * speed * dt;
	return true;
}


bool BasicArrow::Contact()
{
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
		damageType::DIRECT, ELEMENTAL_TYPE::NO_ELEMENT, 30, 1, 20, false);		

	App->camera2D->AddTrauma(35.f / 100.f);
	App->input->DoGamePadRumble(0.35f, 100);

	// TODO Add particles and sfx, blood, slice etc

	to_delete = true;

	return true;
}

bool BasicArrow::CleanUp()
{
	if (entityTex != nullptr)
	{
		App->tex->UnLoad(entityTex);
		entityTex = nullptr;
	}

	return true;
}
