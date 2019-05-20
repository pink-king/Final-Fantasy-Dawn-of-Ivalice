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
	App->camera2D->AddTrauma(5.F / 100.F);
	App->input->DoGamePadRumble(0.1f, 80);
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
	if (wallCol)
		App->audio->PlayFx(App->entityFactory->sharaBasic_ImpactsWall, 0);
	LOG("Bye arrow!");

}

bool BasicArrow::PreUpdate()
{
	if (OnCollisionWithEnemy()) {
		to_explode = true;
	}

	if (OnCollisionWithWall())	{
		
		to_delete = true; 
		App->camera2D->AddTrauma(5.F / 100.F);
		App->input->DoGamePadRumble(0.1f, 80);
		App->particles->AddParticle(App->particles->strike, GetPivotPos().x - 14, GetPivotPos().y - 12);
		wallCol = true;
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

	App->camera2D->AddTrauma(0.5F / 100.F);
	App->input->DoGamePadRumble(0.4f, 50);

	

	// TODO Add sfx, blood, slice etc
	App->particles->AddParticle(App->particles->blood02, GetPivotPos().x - 10, GetPivotPos().y - 10);
	App->audio->PlayFx(App->entityFactory->basicBodyImp, 0);

	to_delete = true;

	return true;
}

