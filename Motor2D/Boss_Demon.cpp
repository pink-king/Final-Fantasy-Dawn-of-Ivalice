#include "Boss_Demon.h"
#include "j1Render.h"
#include "j1ModuleCamera2D.h"
#include "j1EntityFactory.h"

DemonBossEntity::DemonBossEntity(iPoint position) : j1Entity(DEMONBOSS, position.x, position.y , "Demon Boss") 
{
	boss_spritesheet = App->tex->Load("textures/enemies/demon_boss_v1.png");

	// animations --------
	float animSpeed = 4.f;
	// walk
	walkCycleAnim[(int)facingDirection::N].PushBack({ 800,0,200,150 });
	walkCycleAnim[(int)facingDirection::N].PushBack({ 800,150,200,150 });
	walkCycleAnim[(int)facingDirection::N].PushBack({ 800,300,200,150 });
	walkCycleAnim[(int)facingDirection::N].PushBack({ 800,450,200,150 });
	walkCycleAnim[(int)facingDirection::N].PushBack({ 800,300,200,150 });
	walkCycleAnim[(int)facingDirection::N].PushBack({ 800,150,200,150 });
	walkCycleAnim[(int)facingDirection::N].speed = animSpeed;

	walkCycleAnim[(int)facingDirection::S].PushBack({ 0,0,200,150 });
	walkCycleAnim[(int)facingDirection::S].PushBack({ 0,150,200,150 });
	walkCycleAnim[(int)facingDirection::S].PushBack({ 0,300,200,150 });
	walkCycleAnim[(int)facingDirection::S].PushBack({ 0,450,200,150 });
	walkCycleAnim[(int)facingDirection::S].PushBack({ 0,300,200,150 });
	walkCycleAnim[(int)facingDirection::S].PushBack({ 0,150,200,150 });
	walkCycleAnim[(int)facingDirection::S].speed = animSpeed;

	walkCycleAnim[(int)facingDirection::E].PushBack({ 1200,0,200,150 });
	walkCycleAnim[(int)facingDirection::E].PushBack({ 1200,150,200,150 });
	walkCycleAnim[(int)facingDirection::E].PushBack({ 1200,300,200,150 });
	walkCycleAnim[(int)facingDirection::E].PushBack({ 1200,450,200,150 });
	walkCycleAnim[(int)facingDirection::E].PushBack({ 1200,300,200,150 });
	walkCycleAnim[(int)facingDirection::E].PushBack({ 1200,150,200,150 });
	walkCycleAnim[(int)facingDirection::E].speed = animSpeed;

	walkCycleAnim[(int)facingDirection::W].PushBack({ 400,0,200,150 });
	walkCycleAnim[(int)facingDirection::W].PushBack({ 400,150,200,150 });
	walkCycleAnim[(int)facingDirection::W].PushBack({ 400,300,200,150 });
	walkCycleAnim[(int)facingDirection::W].PushBack({ 400,450,200,150 });
	walkCycleAnim[(int)facingDirection::W].PushBack({ 400,300,200,150 });
	walkCycleAnim[(int)facingDirection::W].PushBack({ 400,150,200,150 });
	walkCycleAnim[(int)facingDirection::W].speed = animSpeed;

	walkCycleAnim[(int)facingDirection::SW].PushBack({ 200,0,200,150 });
	walkCycleAnim[(int)facingDirection::SW].PushBack({ 200,150,200,150 });
	walkCycleAnim[(int)facingDirection::SW].PushBack({ 200,300,200,150 });
	walkCycleAnim[(int)facingDirection::SW].PushBack({ 200,450,200,150 });
	walkCycleAnim[(int)facingDirection::SW].PushBack({ 200,300,200,150 });
	walkCycleAnim[(int)facingDirection::SW].PushBack({ 200,150,200,150 });
	walkCycleAnim[(int)facingDirection::SW].speed = animSpeed;

	walkCycleAnim[(int)facingDirection::NW].PushBack({ 600,0,200,150 });
	walkCycleAnim[(int)facingDirection::NW].PushBack({ 600,150,200,150 });
	walkCycleAnim[(int)facingDirection::NW].PushBack({ 600,300,200,150 });
	walkCycleAnim[(int)facingDirection::NW].PushBack({ 600,450,200,150 });
	walkCycleAnim[(int)facingDirection::NW].PushBack({ 600,300,200,150 });
	walkCycleAnim[(int)facingDirection::NW].PushBack({ 600,150,200,150 });
	walkCycleAnim[(int)facingDirection::NW].speed = animSpeed;

	walkCycleAnim[(int)facingDirection::NE].PushBack({ 1000,0,200,150 });
	walkCycleAnim[(int)facingDirection::NE].PushBack({ 1000,150,200,150 });
	walkCycleAnim[(int)facingDirection::NE].PushBack({ 1000,300,200,150 });
	walkCycleAnim[(int)facingDirection::NE].PushBack({ 1000,450,200,150 });
	walkCycleAnim[(int)facingDirection::NE].PushBack({ 1000,300,200,150 });
	walkCycleAnim[(int)facingDirection::NE].PushBack({ 1000,150,200,150 });
	walkCycleAnim[(int)facingDirection::NE].speed = animSpeed;

	walkCycleAnim[(int)facingDirection::SE].PushBack({ 1400,0,200,150 });
	walkCycleAnim[(int)facingDirection::SE].PushBack({ 1400,150,200,150 });
	walkCycleAnim[(int)facingDirection::SE].PushBack({ 1400,300,200,150 });
	walkCycleAnim[(int)facingDirection::SE].PushBack({ 1400,450,200,150 });
	walkCycleAnim[(int)facingDirection::SE].PushBack({ 1400,300,200,150 });
	walkCycleAnim[(int)facingDirection::SE].PushBack({ 1400,150,200,150 });
	walkCycleAnim[(int)facingDirection::SE].speed = animSpeed;

	// define values ------
	entityTex = boss_spritesheet;
	currentAnimation = &walkCycleAnim[(int)facingDirection::S];
	SetPivot(100, 135);
	size.create(200, 150);
	this->position -= pivot;
}

DemonBossEntity::~DemonBossEntity() {}

//bool DemonBossEntity::Start()
//{
//	return true;
//}

bool DemonBossEntity::PreUpdate()
{
	return true;
}

bool DemonBossEntity::Update(float dt)
{
	FollowPlayer(dt);

	return true;
}

void DemonBossEntity::FollowPlayer(float dt) // testing
{
	fPoint velocity = position - App->camera2D->lerp(position, App->entityFactory->player->GetPivotPos() - pivot, dt * 0.5f);

	float angle = atan2f(-velocity.y, -velocity.x);

	int prePointingDir = pointingDir;
	pointingDir = GetPointingDir(angle);
	if (prePointingDir != pointingDir)
	{
		float prevFrame = currentAnimation->GetCurrentFloatFrame();
		currentAnimation = &walkCycleAnim[pointingDir];
		currentAnimation->SetCurrentFrame(prevFrame);
	}

	LOG("vel: %f,%f", velocity.x, velocity.y);
	LOG("pointing dir: %i", pointingDir);
	LOG("angle: %f", angle);
	position -= velocity;
}

bool DemonBossEntity::PostUpdate()
{
	return true;
}

bool DemonBossEntity::CleanUp()
{
	return true;
}

void DemonBossEntity::Draw()
{
	App->render->Blit(entityTex, position.x, position.y, &currentAnimation->GetCurrentFrame());
}

int DemonBossEntity::GetPointingDir(float angle)
{

	int numAnims = 8;
	//LOG("angle: %f", angle);
									 // divide the semicircle in 4 portions
	float animDistribution = PI / (numAnims * 0.5f); // each increment between portions //two semicircles

	int i = 0;
	if (angle >= 0) // is going right or on bottom semicircle range to left
	{
		// iterate between portions to find a match
		for (float portion = animDistribution * 0.5f; portion <= PI; portion += animDistribution) // increment on portion units
		{
			if (portion >= angle) // if the portion is on angle range
			{
				// return the increment position matching with enumerator direction animation
				// TODO: not the best workaround, instead do with std::map
				/*LOG("bottom semicircle");
				LOG("portion: %i", i);*/
				break;
			}
			++i;
		}
	}
	else if (angle <= 0) // animations relatives to upper semicircle
	{
		i = 0; // the next 4 on the enum direction

		for (float portion = -animDistribution * 0.5f; portion >= -PI; portion -= animDistribution)
		{
			if (i == 1) i = numAnims * 0.5f + 1;
			if (portion <= angle)
			{
				/*LOG("upper semicircle");
				LOG("portion: %i", i);*/
				break;
			}
			++i;
		}
	}

	pointingDir = i;
	if (pointingDir == numAnims) // if max direction
		pointingDir = numAnims - 1; // set to prev

	//LOG("portion: %i", pointingDir);

	return pointingDir;
}