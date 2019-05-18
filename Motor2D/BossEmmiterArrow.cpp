#include "BossEmmiterArrow.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1EntityFactory.h"
#include "j1AttackManager.h"
#include "j1Render.h"
#include "j1PathFinding.h"
#include "p2Defs.h"
#include "j1ParticlesClassic.h"

BossEmmiterArrow::BossEmmiterArrow(fPoint pos, fPoint destination, uint speed, const j1Entity* owner, uint timeLife)
	: Projectile(pos, destination, speed, owner, "Arrow", PROJECTILE_TYPE::EMMITER_ARROWS), timeLife(timeLife)
{
	// TODO SFX arrow throwing
	SetPivot(132, 12);
	size.create(64, 16);

	damage = true;
	App->audio->PlayFx(App->entityFactory->SharaUltimateWoosh, 0);
	// Important for aiming offset
	SetInitially();
	
	this->speed += 200;

	iPoint newDest = App->map->WorldToSubtileMap(destination.x, destination.y);
	newDest = App->map->SubTileMapToWorld(newDest.x, newDest.y);
	iPoint drawRectified;
	drawRectified.x = newDest.x + 20;
	drawRectified.y = newDest.y - 90;

	// flip particles pseudo randomly
	SDL_RendererFlip renderFlip = SDL_RendererFlip::SDL_FLIP_NONE;

	if (rand() % 2 == 0)
	{
		renderFlip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	}

	iPoint fireBlastPivot = { 85, 390 };
	drawRectified -= fireBlastPivot;
	App->particles->AddParticle(App->particles->PoisonBlast, drawRectified.x, drawRectified.y, { 0,0 }, 0u, renderFlip);
}

BossEmmiterArrow::~BossEmmiterArrow()
{
	LOG("Bye arrow!");
}

bool BossEmmiterArrow::PreUpdate()
{


	return true;
}

bool BossEmmiterArrow::Update(float dt)
{
	if (to_explode) {
		to_delete = true;
	}
	else {
		Move(dt);
	}
	return true;
}

bool BossEmmiterArrow::Move(float dt)
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
			iPoint newCords = App->map->SubTileMapToWorld(GetSubtilePos().x, GetSubtilePos().y);
			App->attackManager->AddPropagationAttack(owner, App->map->WorldToSubtileMap(newCords.x, newCords.y - 90), propagationType::BFS,
				damageType::DIRECT, ELEMENTAL_TYPE::POISON_ELEMENT, 30, 6, 120, false);
			App->attackManager->AddPropagationAttack(owner, App->map->WorldToSubtileMap(newCords.x, newCords.y - 90), propagationType::BFS,
				damageType::INTIME, ELEMENTAL_TYPE::POISON_ELEMENT, 30, 6, 200, false);
			timeLifeTimer.Start();
			damage = false;


			App->camera2D->AddTrauma(6.5f / 100.f);
			App->input->DoGamePadRumble(0.2f, 100);
		}

		if (timeLife < timeLifeTimer.ReadSec())
			to_explode = true;

	}
	return true;
}


bool BossEmmiterArrow::CleanUp()
{
	return true;
}

