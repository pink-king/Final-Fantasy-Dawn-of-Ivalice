#include "Emmiter.h"
#include "j1EntityFactory.h"
#include "j1Render.h"

Emmiter::Emmiter(fPoint pos, const j1Entity * owner)
	:Projectile(pos, { 0.F,0.F }, 0u, owner, "EmmiterArrows", PROJECTILE_TYPE::EMMITER_ARROWS)
{
	entityTex = App->tex->Load("textures/spells/Ritz_ultimate/deathCircleTest.png");

	anim.PushBack({ 0,0, 300, 225 });
	SetPivot(150, 112);
	size.create(300, 225);

	position -= pivot;

	engine.seed(rd());

	lifeTimer.Start();
	createArrowsTimer.Start();
	dieTimer.Start();
	currentAnimation = &anim;

	rang.x = 500;
	rang.y = 300;

	lifeTime = 4u;
	createArrowsSpeed = 100u;
	dieTime = 6u;

	constantHeigth = App->render->camera->h;
	App->audio->PlayFx(App->entityFactory->strech_Shoot, 0);
}

Emmiter::~Emmiter()
{

}

bool Emmiter::PreUpdate()
{
	if (lifeTimer.ReadSec() > lifeTime)
	{
		to_explode = true;
	}

	return true;
}

bool Emmiter::Update(float dt)
{
	if (!to_explode)
	{
		if (createArrowsTimer.Read() > createArrowsSpeed)
		{
			CreateArrow();
			createArrowsTimer.Start();
		}
	}
	return true;
}

bool Emmiter::PostUpdate()
{
	if (to_explode)
	{
		if (dieTime < dieTimer.ReadSec())
		{
			to_delete = true;
		}
		
	}
	return true;
}

void Emmiter::CreateArrow() 
{
	float posX = RandomValue(-rang.x * 10, rang.x * 10);
	posX += position.x + size.x / 2;

	float posY = RandomValue(-rang.y * 10, rang.y * 10);
	posY += position.y + size.y / 2;

	App->entityFactory->CreateArrow({position.x + size.x/2 , position.y + size.y / 2 - 350}, { posX, posY }, 100, App->entityFactory->player->GetShara(), PROJECTILE_TYPE::EMMITER_ARROWS,4);

}

float Emmiter::RandomValue(int min, int max)
{
	std::uniform_int_distribution<int> range(-rang.x, rang.x);
	return range(rd)*0.1;

}
