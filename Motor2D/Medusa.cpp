#include "Medusa.h"
#include "j1AttackManager.h"
#include "j1Input.h"
#include "j1ModuleCamera2D.h"

Medusa::Medusa(fPoint pos, const j1Entity* owner) : Projectile(pos, { 0,0 }, 0, owner, "Medusa", PROJECTILE_TYPE::MEDUSA)
{
	timer.Start(); 
	centerTimer.Start(); 
	maxRange = 8;
	pulsations = 0;
	radius = 3;
	propSpeed = 120; 
}

Medusa::~Medusa()
{
}

bool Medusa::Update(float dt)
{
	if (!to_explode) {
		if (timer.ReadSec() > 1) 
		{
			Pulsation(); 
		}
		if (centerTimer.Read() > 500)
		{
			CenterPulsation(); 
		}
	}

	return true;
}

bool Medusa::PostUpdate()
{
	if (radius >= maxRange) {	
		to_explode = true;

		if(timer.ReadSec() > 1)
			Explode();
	}

	return true;
}

void Medusa::Pulsation()
{
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS, damageType::INTIME, ELEMENTAL_TYPE::POISON_ELEMENT, 5, radius, propSpeed, true);
	App->input->DoGamePadRumble(0.2F, 500);
	App->camera2D->AddTrauma(10.F / 100);
	radius += 1; 
	timer.Start(); 
}

void Medusa::Explode()
{
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS, damageType::DIRECT, ELEMENTAL_TYPE::POISON_ELEMENT, 60, radius, 50, true);
	App->input->DoGamePadRumble(0.5F, 1000);
	App->camera2D->AddTrauma(40.F / 100);
	to_delete = true; 

}

void Medusa::CenterPulsation()
{
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS, damageType::INTIME, ELEMENTAL_TYPE::POISON_ELEMENT, 15, 3, propSpeed, true);
	App->input->DoGamePadRumble(0.2F, 100);
	App->camera2D->AddTrauma(0.5F / 100);
	centerTimer.Start();
}
