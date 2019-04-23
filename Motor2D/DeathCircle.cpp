#include "DeathCircle.h"
#include "j1AttackManager.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1ModuleCamera2D.h"
#include "j1EntityFactory.h"
#include "j1Map.h"


DeathCircle::DeathCircle(fPoint pos, const j1Entity* owner)
	: Projectile(pos, { 0.F,0.F }, 0u, owner, "DeathCircle", PROJECTILE_TYPE::BASIC_ARROW)
{
	entityTex = App->entityFactory->ritzUltimateTex; 

	anim.PushBack({ 0, 0, 250, 290});
	anim.PushBack({ 250, 0, 250, 290 });
	anim.PushBack({ 500, 0, 250, 290 });
	anim.PushBack({ 750, 0, 250, 290 });
	anim.PushBack({ 0, 290, 250, 290 });
	anim.PushBack({ 250, 290, 250, 290 });
	anim.PushBack({ 500, 290, 250, 290 });
	anim.PushBack({ 750, 290, 250, 290 });
	anim.PushBack({ 0, 580, 250, 290 });
	anim.PushBack({ 250, 580, 250, 290 });
	anim.PushBack({ 500, 580, 250, 290 });
	anim.PushBack({ 750, 580, 250, 290 });
	anim.loop = false;
	anim.speed = 15.F;

	loopAnim.PushBack({ 250, 0, 250, 290 });
	loopAnim.PushBack({ 500, 0, 250, 290 });
	loopAnim.PushBack({ 750, 0, 250, 290 });
	loopAnim.PushBack({ 0, 290, 250, 290 });
	loopAnim.PushBack({ 250, 290, 250, 290 });
	loopAnim.PushBack({ 500, 290, 250, 290 });
	loopAnim.PushBack({ 750, 290, 250, 290 });
	loopAnim.PushBack({ 0, 580, 250, 290 });
	loopAnim.PushBack({ 250, 580, 250, 290 });
	loopAnim.PushBack({ 500, 580, 250, 290 });
	loopAnim.PushBack({ 750, 580, 250, 290 });
	loopAnim.loop = true;
	loopAnim.speed = 15.F;


	totalAnim.PushBack({ 0, 0, 250, 290 });
	totalAnim.PushBack({ 250, 0, 250, 290 });
	totalAnim.PushBack({ 500, 0, 250, 290 });
	totalAnim.PushBack({ 750, 0, 250, 290 });
	//totalAnim.PushBack({ 0, 290, 250, 290 });
	/*totalAnim.PushBack({ 250, 290, 250, 290 });
	totalAnim.PushBack({ 500, 290, 250, 290 });
	totalAnim.PushBack({ 750, 290, 250, 290 });
	totalAnim.PushBack({ 0, 580, 250, 290 });
	totalAnim.PushBack({ 250, 580, 250, 290 });
	totalAnim.PushBack({ 500, 580, 250, 290 });
	totalAnim.PushBack({ 750, 580, 250, 290 });*/
	totalAnim.PushBack({ 0, 870, 250, 290 });
	totalAnim.PushBack({ 250, 870, 250, 290 });
	totalAnim.PushBack({ 500, 870, 250, 290 });
	totalAnim.PushBack({ 750, 870, 250, 290 });
	totalAnim.PushBack({ 0, 1160, 250, 290 });
	totalAnim.PushBack({ 250, 1160, 250, 290 });
	totalAnim.PushBack({ 500, 1160, 250, 290 });
	totalAnim.PushBack({ 750, 1160, 250, 290 });
	totalAnim.speed = 15.F; 
	totalAnim.loop = false; 

	finishAnim.PushBack({ 250, 870, 250, 290 });
	finishAnim.PushBack({ 500, 870, 250, 290 });
	finishAnim.PushBack({ 750, 870, 250, 290 });
	finishAnim.PushBack({ 0, 1160, 250, 290 });
	finishAnim.PushBack({ 250, 1160, 250, 290 });
	finishAnim.PushBack({ 500, 1160, 250, 290 });
	finishAnim.PushBack({ 750, 1160, 250, 290 });
	finishAnim.speed = 10.F;
	finishAnim.loop = false; 
	SetPivot(125, 251);
	size.create(250, 290);

	// To center in subtile (pivot)
	position = App->map->WorldToSubtileMap(position.x, position.y).Return_fPoint(); 
	position = App->map->SubTileMapToWorld(position.x, position.y).Return_fPoint(); // Put in the subtile pos in world coord
	fPoint subtileSize = { 32.F,16.F };	// Add offset of subtile (center it)
	position.x += subtileSize.x *0.5F;
	position.y += subtileSize.y; 
	position -= pivot;	// Print correctly

	lifeTimer.Start();
	propagateTimer.Start();

	currentAnimation = &anim;

	lifeTime = 5u;
	propagationSpeed = 200u;
}

DeathCircle::~DeathCircle()
{
}


bool DeathCircle::PreUpdate()
{
	if (lifeTimer.ReadSec() > lifeTime) 
	{
		to_explode = true; 

		if (!deathTimerStarted) 
		{
			deathTimer.Start(); 
			deathTimerStarted = true; 
			currentAnimation = &finishAnim;
		}
	}

	return true;
}

bool DeathCircle::Update(float dt)
{
	if (anim.Finished() && currentAnimation != &finishAnim && currentAnimation!=&totalAnim) {
		currentAnimation = &loopAnim;
	}
	if (!to_explode && propagateTimer.Read() > propagationSpeed)	{
		Propagate();
		propagateTimer.Start();
	}
	
	return true;
}


bool DeathCircle::PostUpdate()
{
	if (to_explode) {

		if (!madeFinisher && currentAnimation->Finished()) 
		{
			currentAnimation = &totalAnim;
			App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
			damageType::DIRECT, ELEMENTAL_TYPE::ICE_ELEMENT, 70, 8, 30, true);
			App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
				damageType::INTIME, ELEMENTAL_TYPE::ICE_ELEMENT, 50, 10, 30, true,true);
			App->camera2D->AddTrauma(0.8F);
			App->input->DoGamePadRumble(0.6F, 800);
			madeFinisher = true;
			propagateTimer.Start(); // One last time so it doesnt die while the expansion
		}

		if (madeFinisher && totalAnim.Finished())	// So it doesnt die instantly 
			to_delete = true;
		
	}

	return true;
}


void DeathCircle::Propagate() const
{
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
		damageType::DIRECT, ELEMENTAL_TYPE::ICE_ELEMENT, 10, 5, 40, false);
	App->camera2D->AddTrauma(0.15F);
	App->input->DoGamePadRumble(0.2F, 300);
}
