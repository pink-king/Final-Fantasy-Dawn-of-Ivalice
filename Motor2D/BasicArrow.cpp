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

	entityTex = App->entityFactory->arrowsTexture;
	anim.PushBack({ 15, 20, 26, 7});
	currentAnimation = &anim;

	SetPivot(13, 4);
	size.create(32, 8);
	App->audio->PlayFx(App->entityFactory->sharaBasic);

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
		BodyContact = true;
	}

	if (OnCollisionWithWall())	{
		App->audio->PlayFx(App->entityFactory->basicWallImp, 0);
		to_delete = true; 
		WallContact = true;
		
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
	if (BodyContact)
		App->audio->PlayFx(App->entityFactory->basicBodyImp, 0);

	

	// TODO Add particles and sfx, blood, slice etc

	to_delete = true;

	return true;
}

