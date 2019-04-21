#include "FireArrow.h"
#include "j1AttackManager.h"
#include "j1ModuleCamera2D.h"
#include "j1Input.h"
#include "j1ParticlesClassic.h"
#include "j1EntityFactory.h"

FireArrow::FireArrow(fPoint pos, fPoint destination, uint speed, const j1Entity * owner) : Projectile(pos, destination, speed, owner, "Firearrow", PROJECTILE_TYPE::FIRE_ARROW)
{
	App->camera2D->AddTrauma(20.F / 100.f);
	App->input->DoGamePadRumble(0.3f, 100);

	//TODO Change sprite
	entityTex = App->entityFactory->arrowsTexture;

	anim.PushBack({ 0,112,64,16 });
	anim.PushBack({ 64,112,64,16 });
	anim.PushBack({ 128,112,64,16 });
	anim.PushBack({ 192,112,64,16 });
	anim.PushBack({ 256,112,64,16 });
	anim.PushBack({ 320,112,64,16 });
	anim.PushBack({ 384,112,64,16 });
	anim.PushBack({ 448,112,64,16 });
	anim.PushBack({ 0,128,64,16 });
	anim.PushBack({ 64,128,64,16 });
	anim.speed = 10.F;
	currentAnimation = &anim;

	SetPivot(32, 8);
	size.create(64, 16);

	// Important for aiming offset
	SetInitially();
}

FireArrow::~FireArrow()
{
	App->audio->PlayFx(App->entityFactory->sharaAbility1, 0);
	if(hitwall)
		App->audio->PlayFx(App->entityFactory->sharaAbility1, 0);

}


bool FireArrow::PreUpdate()
{

	if (OnCollisionWithEnemy() && !attacked)
	{
		Pierce();
		attacked = true;
		if (piercedEnemies >= maxPiercedEnemies) {
			to_explode = true;
			
		}
	}

	if (OnCollisionWithWall()) {
		to_explode = true;
		hitwall = true;
	}

	if (changedSubtile)
	{
		attacked = false;
		LOG("Changed Subtile");
	}

	return true;
}

bool FireArrow::Update(float dt)
{
	if (!to_explode) {
		Move(dt);
	}
	else Explode(); 
	

	return true;
}

bool FireArrow::Move(float dt)
{
	position += direction * speed * dt; 
	return true;
}

void FireArrow::Pierce()
{
	// Add FX
	// TODO Add particles of blood and fire dmg
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
		damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, 35, 1, 40, true);
	App->camera2D->AddTrauma(0.2F);
	App->input->DoGamePadRumble(0.3F, 30);
	App->particles->AddParticle(App->particles->blood02, GetPivotPos().x - 10, GetPivotPos().y - 10);
	piercedEnemies++;
}

void FireArrow::Explode()
{
	// Add FX
	// TODO Add fire dmg
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
		damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, 45, 1, 50, true);
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
			damageType::INTIME, ELEMENTAL_TYPE::FIRE_ELEMENT, 20, 7, 80, true);

	if (!hitwall) {
		App->particles->AddParticle(App->particles->blood01, GetPivotPos().x - 20, GetPivotPos().y - 30);
	}
	App->particles->AddParticle(App->particles->explosion01, position.x, position.y - 20);
	App->camera2D->AddTrauma(0.5F);
	App->input->DoGamePadRumble(0.5F, 200);
	to_delete = true; 
}




