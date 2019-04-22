#include "EarthShaker.h"
#include "j1AttackManager.h"
#include "j1ModuleCamera2D.h"
#include "j1Input.h"
#include "p2Defs.h"
#include "j1EntityFactory.h"
#include "p2Defs.h"
#include "j1EntityFactory.h"
EarthShaker::EarthShaker(fPoint pos, const j1Entity* owner) : Projectile(pos, {0,0}, 0, owner, "EarthShaker", PROJECTILE_TYPE::EARTH_SHAKER)
{
	gen.seed(rd());
	tornados = 5;
	App->audio->PlayFx(App->entityFactory->marcheEarthShakeSFX, 0);

}

EarthShaker::~EarthShaker()
{
}

bool EarthShaker::Update(float dt)
{

	Attack();
	SpawnTornados();
	

	return true;
}

bool EarthShaker::PostUpdate()
{
	to_delete = true;

	return true;
}

void EarthShaker::Attack()
{
	App->attackManager->AddPropagationAttack(owner, App->entityFactory->player->GetSubtilePos(), propagationType::BFS, damageType::DIRECT, ELEMENTAL_TYPE::STONE_ELEMENT, 0, 10, 60, true);
	App->attackManager->AddPropagationAttack(owner, App->entityFactory->player->GetSubtilePos(), propagationType::BFS, damageType::DIRECT, ELEMENTAL_TYPE::NO_ELEMENT, 50, 10, 60, false);

	App->input->DoGamePadRumble(1.F, 900);
	App->camera2D->AddTrauma(1.f);
}

void EarthShaker::SpawnTornados()
{
	std::uniform_real_distribution<float> dis(-1, 1);
	for (uint i = 0; i < tornados; i++)
	{
		fPoint dir;
		dir.x = dis(gen); 
		dir.y = dis(gen); 
		App->entityFactory->CreateArrow(GetPivotPos(), GetPivotPos() + dir, 100, owner, PROJECTILE_TYPE::TORNADO); 
	}

}
