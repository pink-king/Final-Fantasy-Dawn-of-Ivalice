#include "MagicBolt.h"
#include "j1ModuleCamera2D.h"
#include "j1Input.h"
#include "j1ParticlesClassic.h"
#include "j1AttackManager.h"
#include "j1EntityFactory.h"
MagicBolt::MagicBolt(fPoint pos, fPoint destination, uint speed, const j1Entity* owner)
	: Projectile(pos, destination, speed, owner, "MagicBolt", PROJECTILE_TYPE::MAGIC_BOLT)
{
	/*App->camera2D->AddTrauma(20.F / 100.f);
	App->input->DoGamePadRumble(0.2F, 100);*/

	entityTex = App->entityFactory->ritzBasicTex;

	// Standard bolt
	/*anim.PushBack({ 0, 28, 45, 8 });
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

	SetPivot(22, 4);
	size.create(45, 8);
	*/


	// Blast Bolt
	anim.PushBack({ 0, 0, 64, 64 });
	anim.PushBack({ 64, 0, 64, 64 });
	anim.PushBack({ 128, 0, 64, 64 });
	anim.PushBack({ 192, 0, 64, 64 });
	anim.PushBack({ 256, 0, 64, 64 });
	anim.PushBack({ 320, 0, 64, 64 });
	anim.PushBack({ 384, 0, 64, 64 });
	anim.PushBack({ 448, 0, 64, 64 });
	anim.PushBack({ 0, 64, 64, 64 });
	anim.PushBack({ 64, 64, 64, 64 });
	anim.PushBack({ 128, 64, 64, 64 });
	anim.PushBack({ 192, 64, 64, 64 });
	anim.PushBack({ 256, 64, 64, 64 });
	anim.PushBack({ 320, 64, 64, 64 });
	anim.PushBack({ 384, 64, 64, 64 });
	anim.PushBack({ 448, 64, 64, 64 });
	anim.speed = (float)speed;
	SetPivot(32, 32);
	size.create(64, 64);

	currentAnimation = &anim;	


	// Important for aiming offset
	SetInitially();
}

MagicBolt::~MagicBolt()
{
}

bool MagicBolt::CleanUp()
{
	App->audio->PlayFx(App->entityFactory->RitzBasicHit, 0);
	/*if (entityTex != nullptr)
	{
		App->tex->UnLoad(entityTex);
		entityTex = nullptr;
	}*/

	return true;
}

bool MagicBolt::PreUpdate()
{
	if (OnCollisionWithEnemy() || OnCollisionWithWall()) {
		to_explode = true;
		
	}

	return true;
}

bool MagicBolt::Update(float dt)
{
	if (!to_explode) {
		Move(dt);
	}
	else Explode();

	return true; 
}

bool MagicBolt::Move(float dt)
{
	position += direction * speed * dt;
	return true;
}

void MagicBolt::Explode()
{
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
		damageType::DIRECT, ELEMENTAL_TYPE::NO_ELEMENT, 30, 7, 50, true);

	App->camera2D->AddTrauma(5.f / 100.f);
	App->input->DoGamePadRumble(0.2f, 300);
	//App->particles->AddParticle(App->particles->blast01, GetPivotPos().x - 48, position.y - 48, { 0,0 }, 0u);
	App->particles->AddParticle(App->particles->blast01, GetPivotPos().x - 44, position.y -20, { 0,0 }, 0u);

	to_delete = true;
}
