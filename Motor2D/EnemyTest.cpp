#include "EnemyTest.h"
#include "j1Textures.h"

EnemyTest::EnemyTest(iPoint position) : j1Entity(NO_TYPE, position.x, position.y, "PEM")
{
	name.assign("Test");

	// TODO: import from xml
	entityTex = App->tex->Load("textures/enemies/GoblinEnemy.png");

	idle.PushBack({ 7,34,13,36 });

	currentAnimation = &idle;

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

	return true;
}

bool EnemyTest::PostUpdate()
{
	Draw(entityTex);
	return true;
}
