#include "EnemyProjectile.h"
#include "j1EntityFactory.h"
#include "j1ParticlesClassic.h"
#include "j1Window.h"

EnemyProjectile::EnemyProjectile(fPoint pos, fPoint destination, uint speed, const j1Entity* owner) : Projectile(pos, destination, speed, owner, "Enemy_Arrow", PROJECTILE_TYPE::ENEMY_ARROW)
{
	// Assigning to the same texture as the enemy, it shouldn't be a problem
	entityTex = App->entityFactory->enemyGolemTex;
	anim.PushBack({ 0, 832, 32, 32 });
	anim.PushBack({ 32, 832, 32, 32 });
	anim.PushBack({ 96, 832, 32, 32 });
	anim.speed = 15.F;
	anim.loop = true; 

	currentAnimation = &anim;

	//SetPivot(16, 16);		// Original pivot pos
	SetPivot(24, 24);		 // x 1.5 as the sprite is rescaleted 1.5x
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
		App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
			damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, 5, 3, 20, true);
		//App->particles->AddParticle(App->particles->strike, GetPivotPos().x - 14, GetPivotPos().y - 12);
		App->particles->AddParticle(App->particles->fire02, GetPivotPos().x - 45, GetPivotPos().y - 65, { 0,0 }, 200);

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
		damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, 5, 3, 20, true);

	// Rumble when player gets hurt? 
	/*App->camera2D->AddTrauma(35.f / 100.f);
	App->input->DoGamePadRumble(0.35f, 100);*/
	App->particles->AddParticle(App->particles->fire02, GetPivotPos().x - 45, GetPivotPos().y - 65, { 0,0 }, 200);

	App->particles->AddParticle(App->particles->blood02, GetPivotPos().x - 10, GetPivotPos().y - 10);
	App->audio->PlayFx(App->entityFactory->basicBodyImp, 0);

	to_delete = true;

	return true;
}

void EnemyProjectile::Draw()
{
	if (App->scene->debugSubtiles == true)
	{
		iPoint subTilePos = GetSubtilePos();
		subTilePos = App->map->SubTileMapToWorld(subTilePos.x, subTilePos.y);
		App->render->Blit(debugSubtile, subTilePos.x, subTilePos.y, NULL);
	}

	if (entityTex != nullptr)
	{
		if (currentAnimation != nullptr)	// Re-scaled the sprite, 1.5x bigger, if changed, change the pivot pos aswell
			App->render->Blit(entityTex, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, SDL_FLIP_NONE, 1.5F, angle, pivot.x * App->win->GetScale(), pivot.y * App->win->GetScale());
		else
			App->render->Blit(entityTex, position.x, position.y, &drawAtlasRect, 1.0F, SDL_FLIP_NONE, 1.0F, angle, pivot.x, pivot.y);
	}
}

