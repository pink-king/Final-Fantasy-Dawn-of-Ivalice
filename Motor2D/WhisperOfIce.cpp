#include "WhisperOfIce.h"
#include "j1AttackManager.h"
#include "j1ModuleCamera2D.h"
#include "j1Input.h"
#include "j1EntityFactory.h"
#include "j1ParticlesClassic.h"

WhisperOfIce::WhisperOfIce(const fPoint& pos, const fPoint& destination, uint speed, uint lifeTime, const j1Entity * owner) : lifeTime(lifeTime),  
Projectile(pos, destination, speed, owner, "WhisperOfIce", PROJECTILE_TYPE::WHISPER_OF_ICE)
{
	// Provisional animation 
	entityTex = App->entityFactory->iceTornadoTex;
	startAnim.PushBack({ 0, 0, 140, 70 });
	startAnim.PushBack({ 140, 0, 140, 70 });
	startAnim.PushBack({ 280, 0, 140, 70 });
	startAnim.loop = false;
	startAnim.speed = 20.F;

	loopingAnim.PushBack({ 0, 70, 140, 70 });
	loopingAnim.PushBack({ 140, 70, 140, 70 });
	loopingAnim.PushBack({ 280, 70, 140, 70 });
	loopingAnim.PushBack({ 0, 140, 140, 70 });
	loopingAnim.PushBack({ 140, 140, 140, 70 });
	loopingAnim.PushBack({ 280, 140, 140, 70 });
	loopingAnim.speed = 20.F;
	loopingAnim.loop = true;
	
	endingAnim.PushBack({ 0, 210, 140, 70 });
	endingAnim.PushBack({ 136, 210, 140, 70 });
	endingAnim.PushBack({ 280, 210, 140, 70 });
	endingAnim.PushBack({ 0, 280, 140, 70 });
	endingAnim.PushBack({ 140, 280, 140, 70 });
	endingAnim.speed = 20.F;
	endingAnim.loop = false; 

	currentAnimation = &startAnim;
	SetPivot(70, 50);
	size.create(136, 71);

	// Important for aiming offset
	App->audio->PlayFx(App->scene->RitzAbility2, 0);
	SetInitially();
	angle = 0; 
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
		currentAnimation = &endingAnim;  
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
		damageType::INTIME, ELEMENTAL_TYPE::ICE_ELEMENT, 5, 4, 160, true, true);
	App->camera2D->AddTrauma(0.10F);
	App->input->DoGamePadRumble(0.15F, 200);


	int maxParticles = 3; 
	float maxDistance = 40.f;
	float maxDelayTime = 300.f;
	for (int i = 0; i < maxParticles; ++i)
	{
		fPoint instaPos;
		instaPos.x = GetPivotPos().x + (maxDistance * App->camera2D->GetFloatNegOneToOne());
		instaPos.y = GetPivotPos().y + (maxDistance * App->camera2D->GetFloatNegOneToOne());
		App->particles->AddParticle(
			App->particles->snow,
			instaPos.x,
			instaPos.y,
			{ 0,0 },
			App->camera2D->GetFloatNegOneToOne() * maxDelayTime,
			SDL_FLIP_NONE, 0, 32, 32);
	}
}
