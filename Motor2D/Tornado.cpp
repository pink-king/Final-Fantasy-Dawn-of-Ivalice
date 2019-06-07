#include "Tornado.h"
#include "j1EntityFactory.h"
#include "j1AttackManager.h"

Tornado::Tornado(fPoint pos, fPoint destination, uint speed, const j1Entity * owner)
	: Projectile(pos, destination , speed, owner, "Tornado", PROJECTILE_TYPE::BASIC_ARROW)
{
	entityTex = App->entityFactory->marcheTornadoTex;
	anim.PushBack({ 0, 0, 140, 70 });
	anim.PushBack({ 140, 0, 140, 70 });
	anim.PushBack({ 280, 0, 140, 70 });
	anim.PushBack({ 0, 70, 140, 70 });
	anim.PushBack({ 140, 70, 140, 70 });
	anim.PushBack({ 280, 70, 140, 70 });
	anim.PushBack({ 0, 140, 140, 70 });
	anim.PushBack({ 140, 140, 140, 70 });
	anim.PushBack({ 280, 140, 140, 70 });
	anim.PushBack({ 0, 210, 140, 70 });
	anim.PushBack({ 136, 210, 140, 70 });
	anim.PushBack({ 280, 210, 140, 70 });
	anim.PushBack({ 0, 280, 140, 70 });
	anim.PushBack({ 140, 280, 140, 70 });
	anim.loop = false;
	anim.speed = 35.F;

	finishAnim.PushBack({ 280, 140, 140, 70 });
	finishAnim.PushBack({ 0, 210, 140, 70 });
	finishAnim.PushBack({ 136, 210, 140, 70 });
	finishAnim.PushBack({ 280, 210, 140, 70 });
	finishAnim.PushBack({ 0, 280, 140, 70 });
	finishAnim.PushBack({ 140, 280, 140, 70 });
	finishAnim.loop = false;
	finishAnim.speed = 25.F;

	loopAnim.PushBack({ 0, 0, 140, 70 });
	loopAnim.PushBack({ 140, 0, 140, 70 });
	loopAnim.PushBack({ 280, 0, 140, 70 });
	loopAnim.PushBack({ 0, 70, 140, 70 });
	loopAnim.PushBack({ 140, 70, 140, 70 });
	loopAnim.PushBack({ 280, 70, 140, 70 });
	loopAnim.PushBack({ 0, 140, 140, 70 });
	loopAnim.PushBack({ 140, 140, 140, 70 });
	loopAnim.loop = true;
	loopAnim.speed = 35.F;

	currentAnimation = &loopAnim;

	SetPivot(70,50);
	size.create(136, 71);
	App->audio->PlayFx(App->scene->marcheAbility2); // TODO: ADJUST SFX VOL

	LifeTimer.Start();
	AttackTimer.Start();

	AttackTime = 300;
	lifeTime =2u;
	// Important for aiming offset
	SetInitially();
	angle = 0;

	isMoving = true;
}

Tornado::~Tornado()
{
	App->audio->PlayFx(App->scene->marcheTornadoExplosion,0);
}

bool Tornado::PreUpdate()
{
	if (OnCollisionWithWall()) {
		isMoving = false;
		finishTornado = true;
		//App->particles->AddParticle(App->particles->strike, GetPivotPos().x - 14, GetPivotPos().y - 12);
	}

	if (finishAnim.Finished() == true)
	{
		to_delete = true;
		App->camera2D->AddTrauma(10.f / 100.f);
		App->input->DoGamePadRumble(0.25f, 100);
	}
	
	return true;
}

bool Tornado::Update(float dt)
{
	if (isMoving)
	{
		Move(dt);
		Attack();
	}

	if (finishTornado)
	{
		currentAnimation = &finishAnim;
	}

	if (lifeTime < LifeTimer.ReadSec())
	{
		finishTornado = true;
	}
	return true;
}

bool Tornado::Move(float dt)
{
	position += direction * speed * dt;
	return true;
}

bool Tornado::Attack()
{
	if (AttackTimer.Read() > AttackTime)
	{
		App->camera2D->AddTrauma(0.05F);
		App->input->DoGamePadRumble(0.3F, 10);

		App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
			damageType::DIRECT, ELEMENTAL_TYPE::NO_ELEMENT, 15, 5, 20, false);
		App->attackManager->AddPropagationAttack(owner, GetSubtilePos(), propagationType::BFS,
			damageType::DIRECT, ELEMENTAL_TYPE::STONE_ELEMENT, 0, 5, 200, true);
		AttackTimer.Start();
	}
	return true;
}
