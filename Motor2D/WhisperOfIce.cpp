#include "WhisperOfIce.h"
#include "j1AttackManager.h"
#include "j1ModuleCamera2D.h"
#include "j1Input.h"
#include "j1EntityFactory.h"

WhisperOfIce::WhisperOfIce(const fPoint& pos, const fPoint& destination, uint speed, uint lifeTime, const j1Entity * owner) : lifeTime(lifeTime),  
Projectile(pos, destination, speed, owner, "WhisperOfIce", PROJECTILE_TYPE::WHISPER_OF_ICE)
{
	// Provisional animation 
	entityTex = App->entityFactory->enemyGolemTex;
	anim.PushBack({ 0, 832, 32, 32 });
	anim.PushBack({ 32, 832, 32, 32 });
	anim.PushBack({ 96, 832, 32, 32 });
	anim.speed = 15.F;
	anim.loop = true;
	currentAnimation = &anim; 


	SetPivot(16, 16);
	size.create(32, 32);

	// Important for aiming offset
	SetInitially();
}

WhisperOfIce::~WhisperOfIce()
{
	// TODO: Add SFX
}

bool WhisperOfIce::PreUpdate()
{
	if (timer.Read() > lifeTime || OnCollisionWithWall())
		to_explode = true; 

	return false;
}

bool WhisperOfIce::Update(float dt)
{
	if (!to_explode)
	{
		Move(dt);
		if (propagationTimer.Read() >= propagationSpeed)
		{
			Propagate();
			propagationTimer.Start();
		}
	}

	if (startAnim.Finished() && currentAnimation != &endingAnim)
		currentAnimation = &loopingAnim; 

	if (to_explode)
	{
		//currentAnimation = &endingAnim;  // 
		to_delete = true; 
	}


	return true;
}

bool WhisperOfIce::PostUpdate()
{
	if (endingAnim.Finished())
		to_delete = true;

	return true;
}

bool WhisperOfIce::Move(float dt)
{
	position += direction * speed * dt; 
	return true;
}

void WhisperOfIce::Propagate()
{
	// TODO: Add SFX

	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
		damageType::INTIME, ELEMENTAL_TYPE::ICE_ELEMENT, 1, 4, 160, true, true);
	App->camera2D->AddTrauma(0.08F);
	App->input->DoGamePadRumble(0.1F, 200);
}
