#include "EntityArrow.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1EntityFactory.h"
#include "j1AttackManager.h"
#include "j1Render.h"
#include "j1PathFinding.h"
#include "p2Defs.h"
#include "j1ParticlesClassic.h"

EntityArrow::EntityArrow(fPoint pos, fPoint destination, uint speed, const j1Entity* owner) 
	: Projectile(pos, destination, speed, owner, "Arrow")
{
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

	SetInitially();
}

EntityArrow::~EntityArrow()
{
	LOG("Bye arrow!");
}

bool EntityArrow::PreUpdate()
{
	if (OnCollisionWithEnemy() || OnCollisionWithWall()) {
		to_explode = true;
	}
	
	return true;
}

bool EntityArrow::Update(float dt)
{
	if (!to_explode) {
		Move(dt);
	}
	else Explode(); 

	return true;
}

bool EntityArrow::Move(float dt)
{
	position += direction * speed * dt; 
	return true;
}


bool EntityArrow::Explode()
{
	App->attackManager->AddPropagationAttack(owner, imOnSubtile, propagationType::BFS, 5, 7, 50);

	App->camera2D->AddTrauma(35.f / 100.f);
	App->input->DoGamePadRumble(0.35f, 100);
	App->particles->AddParticle(App->particles->explosion01, position.x, position.y - 20, { 0,0 }, 0u);
	// Just to show off
	App->particles->AddParticle(App->particles->explosion01, position.x + 10, position.y - 10, { 0,0 }, 100u);
	App->particles->AddParticle(App->particles->explosion01, position.x - 10, position.y - 30, { 0,0 }, 50u);

	to_delete = true; 

	return true;
}

bool EntityArrow::CleanUp()
{
	if (entityTex != nullptr)
	{
		App->tex->UnLoad(entityTex);
		entityTex = nullptr;
	}

	return true;
}

void EntityArrow::Draw()
{
	iPoint subTilePos = GetSubtilePos();
	subTilePos = App->map->SubTileMapToWorld(subTilePos.x, subTilePos.y);
	App->render->Blit(debugSubtile, subTilePos.x, subTilePos.y, NULL);

	if (entityTex != nullptr)
	{
		if (currentAnimation != nullptr)
			App->render->Blit(entityTex, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, SDL_FLIP_NONE, 1.0F, angle, pivot.x * 2, pivot.y * 2);
		else
			App->render->Blit(entityTex, position.x, position.y, &drawAtlasRect, 1.0F, SDL_FLIP_NONE, 1.0F, angle, pivot.x, pivot.y);
	}	
}


