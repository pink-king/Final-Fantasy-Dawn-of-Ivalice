
#include "Ritz.h"

Ritz::Ritz()
{
	character = characterName::RITZ;
	name.assign("Ritz");

	// TODO: import from xml
	spritesheet = App->tex->Load("textures/characters/ritz/ritz_run_test.png");
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

Ritz::~Ritz()
{
	//if (collider.collider != nullptr)
	//	collider.collider->to_delete = true;
}

bool Ritz::Start()
{
	return true;
}

bool Ritz::PreUpdate()
{
	return true;
}

bool Ritz::Update(float dt)
{
	InputMovement(dt);

	return true;
}

//bool Ritz::PostUpdate()
//{
//	return true;
//}
