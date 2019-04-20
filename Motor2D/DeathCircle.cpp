#include "DeathCircle.h"
#include "j1AttackManager.h"
#include "j1Textures.h"

DeathCircle::DeathCircle(fPoint pos, const j1Entity* owner)
	: Projectile(pos, { 0.F,0.F }, 0u, owner, "DeathCircle", PROJECTILE_TYPE::BASIC_ARROW)
{
	// Please change this ASAP 
	entityTex = App->tex->Load("textures/spells/Ritz_ultimate/deathCircleTest.png");
	
	anim.PushBack({ 0,0, 300, 225 });
	SetPivot(150, 112);
	size.create(300, 225);

	position -= pivot;

	lifeTimer.Start();
	propagateTimer.Start();

	currentAnimation = &anim;

	lifeTime = 4u;
	propagationSpeed = 300u;
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
			propagateTimer.Start(); // One last time to calculate death time
			deathTimerStarted = true; 
		}
	}

	return true;
}

bool DeathCircle::Update(float dt)
{
	if (!to_explode && propagateTimer.Read() > propagationSpeed)	{
		Propagate();
		propagateTimer.Start();
	}
	
	return true;
}


bool DeathCircle::PostUpdate()
{
	if (to_explode) {
		
		if (propagateTimer.Read() >= 500)	// So it doesnt die instantly 
			to_delete = true;

		if (!madeFinisher) 
		{
			App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
			damageType::DIRECT, ELEMENTAL_TYPE::POISON_ELEMENT, 50, 8, 40, true);
			madeFinisher = true;
		}
		
		
	}

	return true;
}


void DeathCircle::Propagate() const
{
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
		damageType::DIRECT, ELEMENTAL_TYPE::POISON_ELEMENT, 10, 8, 40, true);
}
