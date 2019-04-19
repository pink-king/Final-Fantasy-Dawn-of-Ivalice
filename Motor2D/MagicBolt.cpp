#include "MagicBolt.h"
#include "j1ModuleCamera2D.h"
#include "j1Input.h"
#include "j1ParticlesClassic.h"
#include "j1AttackManager.h"

MagicBolt::MagicBolt(fPoint pos, fPoint destination, uint speed, const j1Entity* owner)
	: Projectile(pos, destination, speed, owner, "MagicBolt", PROJECTILE_TYPE::MAGIC_BOLT)
{
	App->camera2D->AddTrauma(8.F / 100.f);
	App->input->DoGamePadRumble(0.3F, 100);

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

MagicBolt::~MagicBolt()
{
}

bool MagicBolt::CleanUp()
{
	if (entityTex != nullptr)
	{
		App->tex->UnLoad(entityTex);
		entityTex = nullptr;
	}

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
	App->attackManager->AddPropagationAttack(owner, imOnSubtile, propagationType::BFS, 5, 7, 50);

	App->camera2D->AddTrauma(35.f / 100.f);
	App->input->DoGamePadRumble(0.35f, 100);
	App->particles->AddParticle(App->particles->explosion01, position.x, position.y - 20, { 0,0 }, 0u);
	// Just to show off
	App->particles->AddParticle(App->particles->explosion01, position.x + 10, position.y - 10, { 0,0 }, 100u);
	App->particles->AddParticle(App->particles->explosion01, position.x - 10, position.y - 30, { 0,0 }, 50u);

	to_delete = true;
}
