#include "EnemyTest.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"

EnemyTest::EnemyTest(iPoint position) : j1Entity(NO_TYPE, position.x, position.y, "PIM")
{
	name.assign("Test");

	// TODO: import from xml
	entityTex = App->tex->Load("textures/enemies/GoblinEnemy.png");

	idle.PushBack({ 7,34,13,36 });

	currentAnimation = &idle;
	SetPivot(6, 32);
	size.create(13,36);
}

EnemyTest::~EnemyTest()
{
	//if (collider.collider != nullptr)
	//	collider.collider->to_delete = true;
}

bool EnemyTest::Start()
{
	
	return true;
}

bool EnemyTest::PreUpdate()
{
	return true;
}

bool EnemyTest::Update(float dt)
{
	static char title[30];
	sprintf_s(title, 30, " | Tiles Rendered: %f", life);
	App->win->SetTitle(title);
	return true;
}

bool EnemyTest::PostUpdate()
{

	return true;
}

bool EnemyTest::CleanUp()
{

	return true;
}

void EnemyTest::Draw()
{
	if (entityTex != nullptr)
	{
		if (currentAnimation != nullptr)
			App->render->Blit(entityTex, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, flip);
		else
			App->render->Blit(entityTex, position.x, position.y);
	}
}
