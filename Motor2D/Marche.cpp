
#include "Marche.h"
#include "j1Textures.h"

Marche::Marche()
{
	character = characterName::MARCHE;
	name.assign("Marche");

	// TODO: import from xml
	spritesheet = App->tex->Load("textures/characters/marche/marche_run_test.png");
	entityTex = spritesheet;

	// RUN

	run[(int)facingDirection::N].PushBack({ 0,196,32,45 });
	run[(int)facingDirection::N].PushBack({ 33,196,32,45 });
	run[(int)facingDirection::N].PushBack({ 64,196,32,45 });
	run[(int)facingDirection::N].PushBack({ 95,196,32,45 });
	run[(int)facingDirection::N].PushBack({ 128,196,32,45 });
	run[(int)facingDirection::N].PushBack({ 160,196,32,45 });
	run[(int)facingDirection::N].speed = 10.0f;

	run[(int)facingDirection::S].PushBack({ 0,74,32,45 });
	run[(int)facingDirection::S].PushBack({ 33,74,32,45 });
	run[(int)facingDirection::S].PushBack({ 64,74,32,45 });
	run[(int)facingDirection::S].PushBack({ 95,74,32,45 });
	run[(int)facingDirection::S].PushBack({ 128,74,32,45 });
	run[(int)facingDirection::S].PushBack({ 160,74,32,45 });
	run[(int)facingDirection::S].speed = 10.0f;

	run[(int)facingDirection::E].PushBack({ 0,136,32,40 });
	run[(int)facingDirection::E].PushBack({ 33,136,32,40 });
	run[(int)facingDirection::E].PushBack({ 64,136,32,40 });
	run[(int)facingDirection::E].PushBack({ 95,136,32,40 });
	run[(int)facingDirection::E].PushBack({ 128,136,32,40 });
	run[(int)facingDirection::E].PushBack({ 160,136,32,40 });
	run[(int)facingDirection::E].speed = 10.0f;

	run[(int)facingDirection::SE].PushBack({ 0,261,32,45 });
	run[(int)facingDirection::SE].PushBack({ 33,261,32,45 });
	run[(int)facingDirection::SE].PushBack({ 64,261,32,45 });
	run[(int)facingDirection::SE].PushBack({ 95,261,32,45 });
	run[(int)facingDirection::SE].PushBack({ 128,261,32,45 });
	run[(int)facingDirection::SE].PushBack({ 160,261,32,45 });
	run[(int)facingDirection::SE].speed = 10.0f;

	run[(int)facingDirection::NE].PushBack({ 0,327,32,45 });
	run[(int)facingDirection::NE].PushBack({ 33,327,32,45 });
	run[(int)facingDirection::NE].PushBack({ 64,327,32,45 });
	run[(int)facingDirection::NE].PushBack({ 95,327,32,45 });
	run[(int)facingDirection::NE].PushBack({ 128,327,32,45 });
	run[(int)facingDirection::NE].PushBack({ 160,327,32,45 });
	run[(int)facingDirection::NE].speed = 10.0f;

	currentAnimation = &run[(int)facingDirection::E];
}

Marche::~Marche()
{
	//if (collider.collider != nullptr)
	//	collider.collider->to_delete = true;
}

bool Marche::Start()
{
	return true;
}

bool Marche::PreUpdate()
{
	return true;
}

bool Marche::Update(float dt)
{
	InputMovement(dt);

	return true;
}

//bool Marche::PostUpdate()
//{
//	return true;
//}
