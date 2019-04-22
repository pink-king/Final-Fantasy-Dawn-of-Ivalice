#include "EarthShaker.h"
#include "j1AttackManager.h"
#include "j1ModuleCamera2D.h"
#include "j1Input.h"


EarthShaker::EarthShaker(fPoint pos, const j1Entity* owner) : Projectile(pos, {0,0}, 0, owner, "EarthShaker", PROJECTILE_TYPE::EARTH_SHAKER)
{
}

EarthShaker::~EarthShaker()
{
}

bool EarthShaker::Update(float dt)
{
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
		damageType::DIRECT, ELEMENTAL_TYPE::NO_ELEMENT, 200, 30, 60, true);

	return true;
}

bool EarthShaker::PostUpdate()
{

	return true;
}
