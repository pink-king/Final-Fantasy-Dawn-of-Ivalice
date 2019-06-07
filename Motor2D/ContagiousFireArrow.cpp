#include "ContagiousFireArrow.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1EntityFactory.h"
#include "j1AttackManager.h"
#include "j1Render.h"
#include "j1PathFinding.h"
#include "p2Defs.h"
#include "j1ParticlesClassic.h"

ContagiousFireArrow::ContagiousFireArrow(fPoint pos, fPoint destination, uint speed, const j1Entity * owner)
	: Projectile(pos, destination, speed, owner, "ContagiousArrow", PROJECTILE_TYPE::CONTAGIOUS_ARROW)
{

	entityTex = App->entityFactory->arrowsTexture;

	anim.PushBack({ 0,48,64,16 });
	anim.PushBack({ 64,48,64,16 });
	anim.PushBack({ 128,48,64,16 });
	anim.PushBack({ 192,48,64,16 });
	anim.PushBack({ 256,48,64,16 });
	anim.PushBack({ 320,48,64,16 });
	anim.PushBack({ 384,48,64,16 });
	anim.PushBack({ 448,48,64,16 });
	anim.PushBack({ 0,64,64,16 });
	anim.PushBack({ 64,64,64,16 });
	anim.speed = 10.F;

	currentAnimation = &anim;

	SetPivot(32, 8);
	size.create(64, 16);
	App->audio->PlayFx(App->scene->sharaAbility2_shoot, 0);

	timer.Start();

	// Important for aiming offset
	SetInitially();
}

ContagiousFireArrow::~ContagiousFireArrow()
{
	if(wallImpact)
		App->audio->PlayFx(App->scene->sharaAbility2_ImpactsWall, 0);
	LOG("Bye Contagiousarrow!");
}

bool ContagiousFireArrow::PreUpdate()
{

	if (OnCollisionWithEnemy()) {
		
		to_explode = true;
		
		LOG("fire arrow audio played");
	}

	if (OnCollisionWithWall())
	{
		wallImpact = true;
		to_explode = true;
		LOG("contagious wall");
	}
	return true;
}

bool ContagiousFireArrow::Update(float dt)
{
	if (!to_explode) {
		Move(dt);
		SpawnParticles();
	}
	else Explode();

	return true;
}

bool ContagiousFireArrow::Move(float dt)
{
	position += direction * speed * dt;
	return true;
}

bool ContagiousFireArrow::Explode()
{
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
		damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, 40, 1, 60, false);		
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
		damageType::INTIME, ELEMENTAL_TYPE::FIRE_ELEMENT, 40, 5, 60, true);

	App->camera2D->AddTrauma(35.f / 100.f);
	App->input->DoGamePadRumble(0.35f, 100);

	App->audio->PlayFx(App->scene->sharaAbility1, 0);
	App->particles->AddParticle(App->particles->explosion02, GetPivotPos().x - 30, GetPivotPos().y - 30);
	/*App->particles->AddParticle(App->particles->explosion02, GetPivotPos().x - 30, GetPivotPos().y - 30);
		App->particles->AddParticle(App->particles->explosion02, GetPivotPos().x - 45, GetPivotPos().y - 40, { 0, 0 }, 200);
		App->particles->AddParticle(App->particles->explosion02, GetPivotPos().x - 25, GetPivotPos().y - 15, { 0,0 }, 100);*/

	if (!wallImpact) {
		App->particles->AddParticle(App->particles->blood01, GetPivotPos().x - 20, GetPivotPos().y - 30);
		App->particles->AddParticle(App->particles->blood01, GetPivotPos().x - 30, GetPivotPos().y - 35);
		App->particles->AddParticle(App->particles->blood01, GetPivotPos().x - 15, GetPivotPos().y - 20);

	}
	

	to_delete = true;
	return true;
}

void ContagiousFireArrow::SpawnParticles()
{
	if (lastPos.DistanceManhattan(GetPivotPos()) > 40 && timer.Read() > 100)
	{
		App->particles->AddParticle(App->particles->arrowTrail, GetPivotPos().x, GetPivotPos().y, direction.ReturniPoint() * speed, 300u, SDL_FLIP_NONE, angle, App->particles->arrowTrail.pivot.x, App->particles->arrowTrail.pivot.y);
		//timer.Start();
		lastPos = GetPivotPos();

	}
}


