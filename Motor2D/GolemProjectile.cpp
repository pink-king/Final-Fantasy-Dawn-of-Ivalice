#include "GolemProjectile.h"
#include "j1EntityFactory.h"
#include "j1ParticlesClassic.h"

GolemProjectile::GolemProjectile(fPoint pos, fPoint destination, uint speed, const j1Entity* owner) : Projectile(pos, destination, speed, owner, "Golem_Arrow", PROJECTILE_TYPE::GOLEM_ARROW)
{
	// Assigning to the same texture as the enemy, it shouldn't be a problem
	entityTex = App->entityFactory->enemyGolemTex;
	anim.PushBack({ 0, 832, 32, 32 });
	anim.PushBack({ 32, 832, 32, 32 });
	anim.PushBack({ 96, 832, 32, 32 });
	anim.speed = 15.F;
	anim.loop = true;

	currentAnimation = &anim;

	SetPivot(16, 16);
	size.create(32, 32);
	// TODO: Add different SFX
	//App->audio->PlayFx(App->entityFactory->sharaBasic);
	App->audio->PlayFx(App->scene->golem_spawnAttackSFX, 0);
	// Important for aiming offset
	SetInitially();
}

GolemProjectile::~GolemProjectile()
{
	App->audio->PlayFx(App->scene->golem_impactWallSFX, 0);
	
}

bool GolemProjectile::PreUpdate()
{
	if (OnCollisionWithPlayer()) {
		to_explode = true;
	}

	if (OnCollisionWithWall()) {
		
		to_delete = true;
		App->particles->AddParticle(App->particles->strike, GetPivotPos().x - 14, GetPivotPos().y - 12);
		// TODO:: Add sfx different from this one
		//App->audio->PlayFx(App->entityFactory->sharaBasic_ImpactsWall, 0);
	}
	return true;
}

bool GolemProjectile::Update(float dt)
{
	if (!to_explode) {
		Move(dt);
	}
	else Contact();

	return true;
}

bool GolemProjectile::Move(float dt)
{
	position += direction * speed * dt;
	return true;
}

bool GolemProjectile::Contact()
{
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
		damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, 15, 1, 20, false);

	// Rumble when player gets hurt? 
	/*App->camera2D->AddTrauma(35.f / 100.f);
	App->input->DoGamePadRumble(0.35f, 100);*/

	App->particles->AddParticle(App->particles->blood02, GetPivotPos().x - 10, GetPivotPos().y - 10);
	App->audio->PlayFx(App->scene->basicBodyImp, 0);

	to_delete = true;

	return true;
}
