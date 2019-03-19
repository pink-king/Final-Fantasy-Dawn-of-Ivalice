#include "EnemyTest.h"
#include "j1Textures.h"

EnemyTest::EnemyTest(iPoint position) : j1Entity(NO_TYPE, position.x, position.y, "PEM")
{
	name.assign("Test");

	// TODO: import from xml
	entityTex = App->tex->Load("textures/enemies/enemy_test.png");

	idle.PushBack({ 0,0,32,32 });

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
