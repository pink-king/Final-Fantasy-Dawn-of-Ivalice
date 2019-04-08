
#include "Shara.h"

Shara::Shara(int posX, int posY):PlayerEntity(posX,posY)
{
	character = characterName::SHARA;
	name.assign("Shara");

	// TODO: import from xml
	spritesheet = App->tex->Load("textures/characters/shara/Shara_run_WIP.png");
	entityTex = spritesheet;

	// TODO: improve this from xml and with for loop, just for testing now
	// IDLE
	idle[(int)facingDirection::N].PushBack({ 222,0,55,60 });
	idle[(int)facingDirection::S].PushBack({ 0,0,55,60 });
	idle[(int)facingDirection::E].PushBack({ 110,0,55,60 });
	idle[(int)facingDirection::W].PushBack({ 110,0,55,60 }); // same as E but flipped - 4
	idle[(int)facingDirection::NE].PushBack({ 165,0,55,60 });
	idle[(int)facingDirection::SE].PushBack({ 55,0,55,60 });
	idle[(int)facingDirection::SW].PushBack({ 55,0,55,60 }); // same as SE but flipped - 3
	idle[(int)facingDirection::NW].PushBack({ 165,0,55,60 }); // same as NE but flipped - 7
	// one frame doesnt need speed
	//idle[(int)facingDirection::N].speed = 10.0f;

	// RUN
	run[(int)facingDirection::N].PushBack({ 0,180,55,60 });
	run[(int)facingDirection::N].PushBack({ 55,180,55,60 });
	run[(int)facingDirection::N].PushBack({ 110,180,55,60 });
	run[(int)facingDirection::N].PushBack({ 165,180,55,60 });
	run[(int)facingDirection::N].PushBack({ 220,180,55,60 });
	run[(int)facingDirection::N].PushBack({ 275,180,55,60 });
	run[(int)facingDirection::N].speed = 10.0f;

	run[(int)facingDirection::S].PushBack({ 0,60,55,60 });
	run[(int)facingDirection::S].PushBack({ 55,60,55,60 });
	run[(int)facingDirection::S].PushBack({ 110,60,55,60 });
	run[(int)facingDirection::S].PushBack({ 165,60,55,60 });
	run[(int)facingDirection::S].PushBack({ 220,60,55,60 });
	run[(int)facingDirection::S].PushBack({ 275,60,55,60 });
	run[(int)facingDirection::S].speed = 10.0f;

	run[(int)facingDirection::E].PushBack({ 0,120,55,60 });
	run[(int)facingDirection::E].PushBack({ 55,120,55,60 });
	run[(int)facingDirection::E].PushBack({ 110,120,55,60 });
	run[(int)facingDirection::E].PushBack({ 165,120,55,60 });
	run[(int)facingDirection::E].PushBack({ 220,120,55,60 });
	run[(int)facingDirection::E].PushBack({ 275,120,45,60 });
	run[(int)facingDirection::E].speed = 10.0f;

	run[(int)facingDirection::W].PushBack({ 0,120,55,60 });
	run[(int)facingDirection::W].PushBack({ 55,120,55,60 });
	run[(int)facingDirection::W].PushBack({ 110,120,55,60 });
	run[(int)facingDirection::W].PushBack({ 165,120,55,60 });
	run[(int)facingDirection::W].PushBack({ 220,120,55,60 });
	run[(int)facingDirection::W].PushBack({ 275,120,45,60 });
	run[(int)facingDirection::W].speed = 10.0f;

	run[(int)facingDirection::SE].PushBack({ 0,240,55,60 });
	run[(int)facingDirection::SE].PushBack({ 55,240,55,60 });
	run[(int)facingDirection::SE].PushBack({ 110,240,55,60 });
	run[(int)facingDirection::SE].PushBack({ 165,240,55,60 });
	run[(int)facingDirection::SE].PushBack({ 220,240,55,60 });
	run[(int)facingDirection::SE].PushBack({ 275,240,55,60 });
	run[(int)facingDirection::SE].speed = 10.0f;

	run[(int)facingDirection::SW].PushBack({ 0,240,55,60 });
	run[(int)facingDirection::SW].PushBack({ 55,240,55,60 });
	run[(int)facingDirection::SW].PushBack({ 110,240,55,60 });
	run[(int)facingDirection::SW].PushBack({ 165,240,55,60 });
	run[(int)facingDirection::SW].PushBack({ 220,240,55,60 });
	run[(int)facingDirection::SW].PushBack({ 275,240,55,60 });
	run[(int)facingDirection::SW].speed = 10.0f;

	run[(int)facingDirection::NE].PushBack({ 0,300,55,60 });
	run[(int)facingDirection::NE].PushBack({ 55,300,55,60 });
	run[(int)facingDirection::NE].PushBack({ 110,300,55,60 });
	run[(int)facingDirection::NE].PushBack({ 165,300,55,60 });
	run[(int)facingDirection::NE].PushBack({ 220,300,55,60 });
	run[(int)facingDirection::NE].PushBack({ 275,300,55,60 });
	run[(int)facingDirection::NE].speed = 10.0f;

	run[(int)facingDirection::NW].PushBack({ 0,300,55,60 });
	run[(int)facingDirection::NW].PushBack({ 55,300,55,60 });
	run[(int)facingDirection::NW].PushBack({ 110,300,55,60 });
	run[(int)facingDirection::NW].PushBack({ 165,300,55,60 });
	run[(int)facingDirection::NW].PushBack({ 220,300,55,60 });
	run[(int)facingDirection::NW].PushBack({ 275,300,55,60 });
	run[(int)facingDirection::NW].speed = 10.0f;

	currentAnimation = &run[(int)facingDirection::SE];

	// cooldown data test - TODO: import for each character its base cooldown in ms from xml
	coolDownData.basic.cooldownTime = 0;
	coolDownData.dodge.cooldownTime = 0;
	coolDownData.special1.cooldownTime = 500;
	coolDownData.special2.cooldownTime = 1000;
	coolDownData.ultimate.cooldownTime = 3000;

	previousPos = position;
}

Shara::~Shara()
{}

bool Shara::Start()
{
	SetPivot(23, 48);
	size.create(55,60);
	return true;
}

bool Shara::PreUpdate()
{
	return true;
}

bool Shara::Update(float dt)
{
	InputMovement(dt);
	InputCombat();

	return true;
}

bool Shara::CleanUp()
{
	std::vector<items*>::iterator iter = bagObjects.begin();
	for (; iter != bagObjects.end(); ++iter)
	{
		delete *iter;
		*iter = nullptr;
	}
	bagObjects.clear();

	std::vector<items*>::iterator iter2 = equipedObjects.begin();
	for (; iter2 != equipedObjects.end(); ++iter2)
	{
		delete *iter2;
		*iter2 = nullptr;
	}
	equipedObjects.clear();


	return true;
}

//bool Shara::PostUpdate()
//{
//	return true;
//}
