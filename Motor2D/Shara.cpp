
#include "Shara.h"

Shara::Shara()
{
	character = characterName::SHARA;
	name.assign("Shara");

	// TODO: import from xml
	spritesheet = App->tex->Load("textures/characters/shara/shara_run_test.png");
	entityTex = spritesheet;

	run[(int)facingDirection::E].PushBack({ 0,136,32,40 });
	run[(int)facingDirection::E].PushBack({ 33,136,32,40 });
	run[(int)facingDirection::E].PushBack({ 64,136,32,40 });
	run[(int)facingDirection::E].PushBack({ 95,136,32,40 });
	run[(int)facingDirection::E].PushBack({ 128,136,32,40 });
	run[(int)facingDirection::E].PushBack({ 160,136,32,40 });
	run[(int)facingDirection::E].speed = 10.0f;

	currentAnimation = &run[(int)facingDirection::E];
}

Shara::~Shara()
{
	//if (collider.collider != nullptr)
	//	collider.collider->to_delete = true;
}

bool Shara::Start()
{
	return true;
}

bool Shara::PreUpdate()
{
	return true;
}

bool Shara::Update(float dt)
{
	return true;
}

//bool Shara::PostUpdate()
//{
//	return true;
//}
