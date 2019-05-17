#include "EnemyProjectile.h"
#include "j1EntityFactory.h"
#include "j1ParticlesClassic.h"

EnemyProjectile::EnemyProjectile(fPoint pos, fPoint destination, uint speed, const j1Entity* owner) : Projectile(pos, destination, speed, owner, "Enemy_Arrow", PROJECTILE_TYPE::BASIC_ARROW)
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

	// Important for aiming offset
	SetInitially();
}

EnemyProjectile::~EnemyProjectile()
{	
}

bool EnemyProjectile::PreUpdate()
{
	if (OnCollisionWithPlayer()) {
		to_explode = true;
	}

	if (OnCollisionWithWall()) {

		to_delete = true;
		App->particles->AddParticle(App->particles->strike, GetPivotPos().x - 14, GetPivotPos().y - 12);
		// TODO:: Add different sfx
		App->audio->PlayFx(App->entityFactory->sharaBasic_ImpactsWall, 0);
	}
	return true;
}

bool EnemyProjectile::Update(float dt)
{
	if (!to_explode) {
		Move(dt);
	}
	else Contact();

	return true;
}

bool EnemyProjectile::Move(float dt)
{
	position += direction * speed * dt;
	return true;
}

bool EnemyProjectile::Contact()
{
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
		damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, 5, 1, 20, false);

	// Rumble when player gets hurt? 
	/*App->camera2D->AddTrauma(35.f / 100.f);
	App->input->DoGamePadRumble(0.35f, 100);*/

	App->particles->AddParticle(App->particles->blood02, GetPivotPos().x - 10, GetPivotPos().y - 10);
	App->audio->PlayFx(App->entityFactory->basicBodyImp, 0);

	to_delete = true;

	return true;
}

