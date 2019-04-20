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
	entityTex = App->tex->Load("textures/spells/Ritz_attacks/Ritz_fx.png");

	anim.PushBack({ 0, 28, 45, 8 });
	anim.PushBack({ 45, 28, 45,8 });
	anim.PushBack({ 90, 28, 45,8 });
	anim.PushBack({ 135, 28, 45, 8 });
	anim.PushBack({ 180, 28, 45, 8 });
	anim.PushBack({ 225, 28, 45, 8 });
	anim.PushBack({ 270, 28, 45, 8 });
	anim.PushBack({ 315, 28, 45, 8 });
	anim.PushBack({ 360, 28, 45, 8 });
	anim.PushBack({ 405, 28, 45, 8 });
	anim.speed = (float)speed;

	currentAnimation = &anim;

	SetPivot(22, 4);
	size.create(45, 8);

	// Important for aiming offset
	SetInitially();
}

FireArrow::~FireArrow()
{
}


bool FireArrow::CleanUp()
{
	if (entityTex != nullptr)
	{
		App->tex->UnLoad(entityTex);
		entityTex = nullptr;
	}

	return true;
}

bool FireArrow::PreUpdate()
{

	if (OnCollisionWithEnemy() && !attacked)
	{
		Pierce();
		attacked = true;
		if (piercedEnemies >= maxPiercedEnemies) {
			to_explode = true;
			App->audio->PlayFx(App->entityFactory->sharaAbility1, 0);
		}
	}

	if (OnCollisionWithWall()) {
		to_explode = true;
		App->audio->PlayFx(App->entityFactory->sharaAbility1, 0);
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
	piercedEnemies++;
}

void FireArrow::Explode()
{
	// Add FX
	// TODO Add fire dmg
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
		damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, 45, 1, 50, true);
	App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
			damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, 20, 7, 40, true);

	App->particles->AddParticle(App->particles->explosion01, position.x, position.y - 20);
	App->camera2D->AddTrauma(0.5F);
	App->input->DoGamePadRumble(0.5F, 200);
	to_delete = true; 
}




