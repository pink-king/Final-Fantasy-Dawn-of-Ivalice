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

	// Important for aiming offset
	SetInitially();
}

ContagiousFireArrow::~ContagiousFireArrow()
{
	LOG("Bye Contagiousarrow!");
}

bool ContagiousFireArrow::PreUpdate()
{
	if (OnCollisionWithEnemy() || OnCollisionWithWall()) {
		
		to_explode = true;
		App->audio->PlayFx(App->entityFactory->sharaAbility1);
		LOG("fire arrow audio played");
	}

	return true;
}

bool ContagiousFireArrow::Update(float dt)
{
	if (!to_explode) {
		Move(dt);
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
		damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, 10, 1, 60, false);		
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
		damageType::INTIME, ELEMENTAL_TYPE::FIRE_ELEMENT, 10, 5, 60, true);

	App->camera2D->AddTrauma(35.f / 100.f);
	App->input->DoGamePadRumble(0.35f, 100);

	to_delete = true;
	return true;
}

void ContagiousFireArrow::Draw()
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

