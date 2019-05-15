
#include "Shara.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1BuffManager.h"
#include "j1AttackManager.h"
#include "j1EntityFactory.h"
#include "j1ParticlesClassic.h"

Shara::Shara(int posX, int posY):PlayerEntity(posX,posY)
{
	character = characterName::SHARA;
	name.assign("Shara");

	// TODO: import from xml
	spritesheet = App->tex->Load("textures/characters/shara/Shara_run_WIP.png");
	dash_spritesheet = App->tex->Load("textures/characters/shara/Shara_dash_WIP.png");
	ultiCastTex = App->tex->Load("textures/characters/shara/Shara_ultimate_animation.png");
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
	run[(int)facingDirection::E].PushBack({ 275,120,55,60 });
	run[(int)facingDirection::E].speed = 10.0f;

	run[(int)facingDirection::W].PushBack({ 0,120,55,60 });
	run[(int)facingDirection::W].PushBack({ 55,120,55,60 });
	run[(int)facingDirection::W].PushBack({ 110,120,55,60 });
	run[(int)facingDirection::W].PushBack({ 165,120,55,60 });
	run[(int)facingDirection::W].PushBack({ 220,120,55,60 });
	run[(int)facingDirection::W].PushBack({ 275,120,55,60 });
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

	// DASH

	dash[(int)facingDirection::E].PushBack({ 0,0,130,130 });
	dash[(int)facingDirection::E].PushBack({ 130,0,130,130 });
	dash[(int)facingDirection::E].PushBack({ 260,0, 130,130 });
	dash[(int)facingDirection::E].PushBack({ 390,0, 130,130 });
	dash[(int)facingDirection::E].loop = false;
	dash[(int)facingDirection::E].speed = 16.f;

	dash[(int)facingDirection::S].PushBack({ 0,130,130,130 });
	dash[(int)facingDirection::S].PushBack({ 130,130,130,130 });
	dash[(int)facingDirection::S].PushBack({ 260,130, 130,130 });
	dash[(int)facingDirection::S].PushBack({ 390,130, 130,130 });
	dash[(int)facingDirection::S].loop = false;
	dash[(int)facingDirection::S].speed = 16.0f;

	dash[(int)facingDirection::N].PushBack({ 0,260,130,130 });
	dash[(int)facingDirection::N].PushBack({ 130,260,130,130 });
	dash[(int)facingDirection::N].PushBack({ 260,260, 130,130 });
	dash[(int)facingDirection::N].PushBack({ 390,260, 130,130 });
	dash[(int)facingDirection::N].loop = false;
	dash[(int)facingDirection::N].speed = 16.0f;

	dash[(int)facingDirection::W].PushBack({ 0,0,130,130 });
	dash[(int)facingDirection::W].PushBack({ 130,0,130,130 });
	dash[(int)facingDirection::W].PushBack({ 260,0, 130,130 });
	dash[(int)facingDirection::W].PushBack({ 390,0, 130,130 });
	dash[(int)facingDirection::W].loop = false;
	dash[(int)facingDirection::W].speed = 16.0f;

	dash[(int)facingDirection::SE].PushBack({ 0,390,130,130 });
	dash[(int)facingDirection::SE].PushBack({ 130,390,130,130 });
	dash[(int)facingDirection::SE].PushBack({ 260,390, 130,130 });
	dash[(int)facingDirection::SE].PushBack({ 390,390, 130,130 });
	dash[(int)facingDirection::SE].loop = false;
	dash[(int)facingDirection::SE].speed = 16.0f;

	dash[(int)facingDirection::SW].PushBack({ 0,390,130,130 });
	dash[(int)facingDirection::SW].PushBack({ 130,390,130,130 });
	dash[(int)facingDirection::SW].PushBack({ 260,390, 130,130 });
	dash[(int)facingDirection::SW].PushBack({ 390,390, 130,130 });
	dash[(int)facingDirection::SW].loop = false;
	dash[(int)facingDirection::SW].speed = 16.0f;

	dash[(int)facingDirection::NE].PushBack({ 0,520,130,130 });
	dash[(int)facingDirection::NE].PushBack({ 130,520,130,130 });
	dash[(int)facingDirection::NE].PushBack({ 260,520, 130,130 });
	dash[(int)facingDirection::NE].PushBack({ 390,520, 130,130 });
	dash[(int)facingDirection::NE].loop = false;
	dash[(int)facingDirection::NE].speed = 16.0f;

	dash[(int)facingDirection::NW].PushBack({ 0,520,130,130 });
	dash[(int)facingDirection::NW].PushBack({ 130,520,130,130 });
	dash[(int)facingDirection::NW].PushBack({ 260,520, 130,130 });
	dash[(int)facingDirection::NW].PushBack({ 390,520, 130,130 });
	dash[(int)facingDirection::NW].loop = false;
	dash[(int)facingDirection::NW].speed = 16.0f;

	dashPivotOffset[(int)facingDirection::E][0] = { 66.f,76.f };
	dashPivotOffset[(int)facingDirection::E][1] = { 90.f,76.f };
	dashPivotOffset[(int)facingDirection::E][2] = { 99.f,72.f };
	dashPivotOffset[(int)facingDirection::E][3] = { 103.f,74.f };

	dashPivotOffset[(int)facingDirection::W][0] = { 66.f,76.f };
	dashPivotOffset[(int)facingDirection::W][1] = { 42.f,76.f };
	dashPivotOffset[(int)facingDirection::W][2] = { 32.f,72.f };
	dashPivotOffset[(int)facingDirection::W][3] = { 21.f,74.f };

	dashPivotOffset[(int)facingDirection::S][0] = { 61.f,90.f };
	dashPivotOffset[(int)facingDirection::S][1] = { 63.f,90.f };
	dashPivotOffset[(int)facingDirection::S][2] = { 65.f,99.f };
	dashPivotOffset[(int)facingDirection::S][3] = { 62.f,109.f };

	dashPivotOffset[(int)facingDirection::N][0] = { 61.f,68.f };
	dashPivotOffset[(int)facingDirection::N][1] = { 60.f,60.f };
	dashPivotOffset[(int)facingDirection::N][2] = { 62.f,54.f };
	dashPivotOffset[(int)facingDirection::N][3] = { 61.f,53.f };

	dashPivotOffset[(int)facingDirection::SE][0] = { 70.f,85.f };
	dashPivotOffset[(int)facingDirection::SE][1] = { 75.f,88.f };
	dashPivotOffset[(int)facingDirection::SE][2] = { 80.f,87.f };
	dashPivotOffset[(int)facingDirection::SE][3] = { 83.f,91.f };

	dashPivotOffset[(int)facingDirection::NE][0] = { 61.f,74.f }; 
	dashPivotOffset[(int)facingDirection::NE][1] = { 86.f,69.f };
	dashPivotOffset[(int)facingDirection::NE][2] = { 89.f,65.f };
	dashPivotOffset[(int)facingDirection::NE][3] = { 95.f,62.f };

	dashPivotOffset[(int)facingDirection::NW][0] = { 70.f,74.f };
	dashPivotOffset[(int)facingDirection::NW][1] = { 48.f,69.f };
	dashPivotOffset[(int)facingDirection::NW][2] = { 42.f,65.f };
	dashPivotOffset[(int)facingDirection::NW][3] = { 28.f,62.f };


	dashPivotOffset[(int)facingDirection::SW][0] = { 60.f,85.f };
	dashPivotOffset[(int)facingDirection::SW][1] = { 55.f,88.f };
	dashPivotOffset[(int)facingDirection::SW][2] = { 50.f,87.f };
	dashPivotOffset[(int)facingDirection::SW][3] = { 40.f,91.f };

	// ULTIMATE shot animation

	ultiCastAnim[(int)facingDirection::S].PushBack({ 0, 0, 45, 70 });
	ultiCastAnim[(int)facingDirection::S].PushBack({ 45, 0, 45, 70 });
	ultiCastAnim[(int)facingDirection::S].PushBack({ 90, 0, 45, 70 });
	ultiCastAnim[(int)facingDirection::S].PushBack({ 135, 0, 45, 70 });
	ultiCastAnim[(int)facingDirection::S].PushBack({ 180, 0, 45, 70 });
	ultiCastAnim[(int)facingDirection::S].PushBack({ 225, 0, 45, 70 });
	ultiCastAnim[(int)facingDirection::S].PushBack({ 270, 0, 45, 70 });
	ultiCastAnim[(int)facingDirection::S].PushBack({ 315, 0, 45, 70 });
	ultiCastAnim[(int)facingDirection::S].PushBack({ 360, 0, 45, 70 });
	ultiCastAnim[(int)facingDirection::S].PushBack({ 405, 0, 45, 70 });
	ultiCastAnim[(int)facingDirection::S].speed = 15.F;
	ultiCastAnim[(int)facingDirection::S].loop = false; 
	
	ultiCastAnim[(int)facingDirection::SE].PushBack({ 0, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SE].PushBack({ 45, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SE].PushBack({ 90, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SE].PushBack({ 135, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SE].PushBack({ 180, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SE].PushBack({ 225, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SE].PushBack({ 270, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SE].PushBack({ 315, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SE].PushBack({ 360, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SE].PushBack({ 405, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SE].speed = 15.F;
	ultiCastAnim[(int)facingDirection::SE].loop = false;
	
	ultiCastAnim[(int)facingDirection::SW].PushBack({ 0, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SW].PushBack({ 45, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SW].PushBack({ 90, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SW].PushBack({ 135, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SW].PushBack({ 180, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SW].PushBack({ 225, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SW].PushBack({ 270, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SW].PushBack({ 315, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SW].PushBack({ 360, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SW].PushBack({ 405, 70, 45, 70 });
	ultiCastAnim[(int)facingDirection::SW].speed = 15.F;
	ultiCastAnim[(int)facingDirection::SW].loop = false;
	
	ultiCastAnim[(int)facingDirection::E].PushBack({ 0, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::E].PushBack({ 45, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::E].PushBack({ 90, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::E].PushBack({ 135, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::E].PushBack({ 180, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::E].PushBack({ 225, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::E].PushBack({ 270, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::E].PushBack({ 315, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::E].PushBack({ 360, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::E].PushBack({ 405, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::E].speed = 15.F;
	ultiCastAnim[(int)facingDirection::E].loop = false;
	
	ultiCastAnim[(int)facingDirection::W].PushBack({ 0, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::W].PushBack({ 45, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::W].PushBack({ 90, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::W].PushBack({ 135, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::W].PushBack({ 180, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::W].PushBack({ 225, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::W].PushBack({ 270, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::W].PushBack({ 315, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::W].PushBack({ 360, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::W].PushBack({ 405, 140, 45, 70 });
	ultiCastAnim[(int)facingDirection::W].speed = 15.F;
	ultiCastAnim[(int)facingDirection::W].loop = false;
	
	ultiCastAnim[(int)facingDirection::N].PushBack({ 0, 210, 45, 70 });
	ultiCastAnim[(int)facingDirection::N].PushBack({ 45, 210, 45, 70 });
	ultiCastAnim[(int)facingDirection::N].PushBack({ 90, 210, 45, 70 });
	ultiCastAnim[(int)facingDirection::N].PushBack({ 135, 210, 45, 70 });
	ultiCastAnim[(int)facingDirection::N].PushBack({ 180, 210, 45, 70 });
	ultiCastAnim[(int)facingDirection::N].PushBack({ 225, 210, 45, 70 });
	ultiCastAnim[(int)facingDirection::N].PushBack({ 270, 210, 45, 70 });
	ultiCastAnim[(int)facingDirection::N].PushBack({ 315, 210, 45, 70 });
	ultiCastAnim[(int)facingDirection::N].PushBack({ 360, 210, 45, 70 });
	ultiCastAnim[(int)facingDirection::N].PushBack({ 405, 210, 45, 70 });
	ultiCastAnim[(int)facingDirection::N].speed = 15.F;
	ultiCastAnim[(int)facingDirection::N].loop = false;
	
	ultiCastAnim[(int)facingDirection::NE].PushBack({ 0, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NE].PushBack({ 45, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NE].PushBack({ 90, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NE].PushBack({ 135, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NE].PushBack({ 180, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NE].PushBack({ 225, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NE].PushBack({ 270, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NE].PushBack({ 315, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NE].PushBack({ 360, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NE].PushBack({ 405, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NE].speed = 15.F;
	ultiCastAnim[(int)facingDirection::NE].loop = false;
	
	ultiCastAnim[(int)facingDirection::NW].PushBack({ 0, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NW].PushBack({ 45, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NW].PushBack({ 90, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NW].PushBack({ 135, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NW].PushBack({ 180, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NW].PushBack({ 225, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NW].PushBack({ 270, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NW].PushBack({ 315, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NW].PushBack({ 360, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NW].PushBack({ 405, 280, 45, 70 });
	ultiCastAnim[(int)facingDirection::NW].speed = 15.F;
	ultiCastAnim[(int)facingDirection::NW].loop = false;

	dashMaxDistance = 120.f;

	currentAnimation = &run[(int)facingDirection::SE];

	// cooldown data test - TODO: import for each character its base cooldown in ms from xml
	coolDownData.basic.cooldownTime = 0;
	coolDownData.dodge.cooldownTime = 0;
	coolDownData.special1.cooldownTime = 500;
	coolDownData.special2.cooldownTime = 1000;
	coolDownData.ultimate.cooldownTime = 3000;
	maxEmitterDelayTime = 1000;

	previousPos = position;
}

Shara::~Shara()
{
	App->tex->UnLoad(ultiCastTex);
	ultiCastTex = nullptr;
	if (!App->cleaningUp)
	{
		if (App->gui->spawnedClocks.Shara.special1)
		{
			myUIClocks.special1->to_delete = true;
			App->gui->spawnedClocks.Shara.special1 = false;
		}
		if (App->gui->spawnedClocks.Shara.special2)
		{
			myUIClocks.special2->to_delete = true;
			App->gui->spawnedClocks.Shara.special2 = false;
		}
		if (App->gui->spawnedClocks.Shara.ulti)
		{
			myUIClocks.ulti->to_delete = true;
			App->gui->spawnedClocks.Shara.ulti = false;
		}
	}

	memset(ultiCastAnim, 0, sizeof(ultiCastAnim));
	memset(dashPivotOffset, 0, sizeof(dashPivotOffset));
}

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
	fPoint pivotPos = GetPivotPos();
	//LOG("%f,%f", pivot.x, pivot.y);
	iPoint onTilePos = App->map->WorldToMap(pivotPos.x, pivotPos.y);
	//LOG("Player pos: %f,%f | Tile pos: %i,%i",position.x, position.y, onTilePos.x, onTilePos.y);
	
	if (!isParalize)
	{
		if (inputReady)
		{
			InputMovement(dt);
			InputCombat();
		}
		if (!inputReady) // dash, or animations that needs control of its finish state
		{
			if (currentAnimation->Finished())
			{
				currentAnimation->Reset();
				entityTex = spritesheet;
				currentAnimation = &idle[pointingDir];
				inputReady = true;
				transference_pivot = { 0,0 };
			}
		}
	}

	// Check if we have a emitter to instantiate from ultimate 
	// TODO: add emitter delayer on emitter itself
	if (instantiateEmitter)
	{
		if (ultiEmitterTimerDelayer.Read() >= maxEmitterDelayTime)
		{
			App->entityFactory->CreateArrow(App->entityFactory->player->GetCrossHairPivotPos().Return_fPoint(), { 0,0 }, 0, this, PROJECTILE_TYPE::EMMITER);
			instantiateEmitter = false;
		}
	}
	// ---------------------------------------------------------

	if (stat.size() != 0)
	{
		if (App->buff->DamageInTime(this))
		{
			App->buff->entitiesTimeDamage.remove(this);
		}
	}

	if (App->entityFactory->pushEF)
	{

		DoPushback();
		/*blink = true;
		alphaTimer.Start();*/
		App->entityFactory->pushEF = false;
		LOG("log from marche update()");
		App->render->SetTextureColor(entityTex, 255, 0, 0);

	}
	// CHECK COMBAT STATE
	switch (combat_state)
	{
	case combatState::IDLE:

		break;
	case combatState::BASIC:

		if (coolDownData.basic.timer.Read() > coolDownData.basic.cooldownTime)
		{
			//App->audio->PlayFx(App->entityFactory->ritzBasic, 0);
			LOG("Launch BASIC");
			coolDownData.basic.timer.Start();
			App->entityFactory->CreateArrow(GetThrowingPos(), App->entityFactory->player->GetCrossHairPivotPos().Return_fPoint(), 100, this, PROJECTILE_TYPE::BASIC_ARROW);

			//App->attackManager->AddPropagationAttack(this, App->entityFactory->player->GetCrossHairSubtile(), propagationType::BFS, 10, 7, 40);
			// TODO: Adds a camera shaking based on "x" needed data from attack components
			// same applies when we receive damage
			App->camera2D->AddTrauma(10.0f / 100.f);
			App->input->DoGamePadRumble(0.3f, 100);

		}
		break;
	case combatState::DODGE:
	{
		if (coolDownData.dodge.timer.Read() > coolDownData.basic.cooldownTime)
		{
			coolDownData.dodge.timer.Start();
			//App->audio->PlayFx(App->entityFactory->dash, 0);
		}
		if (!inputReady)
		{
			//reposition pos
			transference_pivot = dashPivotOffset[pointingDir][(int)currentAnimation->GetCurrentFloatFrame()];
			transference_pivot -= pivot;
			position = App->camera2D->lerp(position, dashDestinationPos, dt * currentAnimation->speed);
		}
		break;
	}
	case combatState::SPECIAL1:
		if (coolDownData.special1.timer.Read() > coolDownData.special1.cooldownTime)
		{
			coolDownData.special1.timer.Start();
			App->audio->PlayFx(App->entityFactory->sharaBasic);
			App->entityFactory->CreateArrow(GetThrowingPos(), App->entityFactory->player->GetCrossHairPivotPos().Return_fPoint(), 100, this, PROJECTILE_TYPE::FIRE_ARROW);

			//App->entityFactory->CreateArrow(App->entityFactory->player->GetSelectedCharacterEntity()->GetThrowingPos(), App->entityFactory->player->GetCrossHairPivotPos().Return_fPoint(), 75, App->entityFactory->player->GetMarche());
			//App->camera2D->AddTrauma(20.f / 100.f);
			//App->input->DoGamePadRumble(0.3f, 100);
			// add gui clock

			if (!App->gui->spawnedClocks.Shara.special1)
			{

				myUIClocks.special1 = App->gui->AddClock(App->gui->allclocksData.ability1.position, &App->gui->allclocksData.ability1.section, "special1", "Shara", App->scene->inGamePanel);

				App->gui->spawnedClocks.Shara.special1 = true;
			}
			else
			{
				myUIClocks.special1->Restart();
			}


		}
		break;

	case combatState::SPECIAL2:
		if (coolDownData.special2.timer.Read() > coolDownData.special2.cooldownTime)
		{
			coolDownData.special2.timer.Start();
			App->audio->PlayFx(App->entityFactory->sharaAbility1);
			App->entityFactory->CreateArrow(GetThrowingPos(), App->entityFactory->player->GetCrossHairPivotPos().Return_fPoint(), 120, this, PROJECTILE_TYPE::CONTAGIOUS_ARROW);

			//App->audio->PlayFx(App->entityFactory->ritzAbility2, 0);

			// add gui clock

			if (!App->gui->spawnedClocks.Shara.special2)
			{

				myUIClocks.special2 = App->gui->AddClock(App->gui->allclocksData.ability2.position, &App->gui->allclocksData.ability2.section, "special2", "Shara", App->scene->inGamePanel);

				App->gui->spawnedClocks.Shara.special2 = true;
			}
			else
			{
				myUIClocks.special2->Restart();
			}


		}
		break;

	case combatState::ULTIMATE:
	{
		if (coolDownData.ultimate.timer.Read() > coolDownData.ultimate.cooldownTime && inputReady)
		{
			LOG("Launch ULTIMATE"); // this basic requieres no input while casting
			if (ultiCastTex != nullptr)
			{
				SetStopperState();
				//App->audio->PlayFx(App->entityFactory->RitzBasic, 0);
			}

			// TODO: Adds a camera shaking based on "x" needed data from attack components
			// same applies when we receive damage
			App->camera2D->AddTrauma(.74f);
			App->input->DoGamePadRumble(0.3f, 100);

			// CLOCKS
			if (!App->gui->spawnedClocks.Shara.ulti)
			{

				myUIClocks.ulti = App->gui->AddClock(App->gui->allclocksData.ulti.position, &App->gui->allclocksData.ulti.section, "ulti", "Shara", App->scene->inGamePanel);

				App->gui->spawnedClocks.Shara.ulti = true;
			}
			else
			{
				myUIClocks.ulti->Restart();
			}

		}
		if (!inputReady)
		{
			// wait until the required casting frame, and we are done
			if ((int)currentAnimation->GetCurrentFloatFrame() >= 8) // shot frame
			{
				// prepares emitter instantiation
				instantiateEmitter = true;
				// launch particle
				App->particles->AddParticle(App->particles->arrow, GetPivotPos().x, GetPivotPos().y, { 0, -800 }, 0u, SDL_FLIP_NONE, 270, 32, 8);
				// change combat state to idle
				combat_state = combatState::IDLE;
				App->camera2D->AddTrauma(40.0f / 100.f);
				App->input->DoGamePadRumble(0.4f, 100);
				// restart timer
				coolDownData.ultimate.timer.Start();
			}
		}
		break;



		/*if (coolDownData.ultimate.timer.Read() > coolDownData.ultimate.cooldownTime)
		{
			coolDownData.ultimate.timer.Start();


			if (!App->gui->spawnedClocks.Shara.ulti)
			{

				myUIClocks.ulti = App->gui->AddClock(App->gui->allclocksData.ulti.position, &App->gui->allclocksData.ulti.section, "ulti", "Shara", App->scene->inGamePanel);

				App->gui->spawnedClocks.Shara.ulti = true;
			}
			else
			{
				myUIClocks.ulti->Restart();
			}
			App->entityFactory->CreateArrow(App->entityFactory->player->GetCrossHairPivotPos().Return_fPoint(), { 0,0 }, 0, this, PROJECTILE_TYPE::EMMITER);


		}*/
		
		break;
	}
	case combatState::MAX:
		break;
	default:
		break;
	}

	if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_DOWN && App->scene->inGamePanel->enable && !App->scene->inventory->enable)
	{
		std::vector<LootEntity*>::iterator item = App->entityFactory->player->consumables.begin();
		if (item != App->entityFactory->player->consumables.end())
			App->entityFactory->player->ConsumConsumable(*item, this);
	}

	return true;
}

// TODO: think this and port to parent class
// used the same maybe on all characters

bool Shara::SetStopperState() // disable user player input and sets the facing dir to crosshair angle, set textures and animations
{
	bool ret = true;

	inputReady = false; // deactivate user input
	// checks the direction of aiming
	iPoint targetDirection = App->entityFactory->player->GetCrossHairPivotPos();
	fPoint targetPos;
	targetPos.x = targetDirection.x - GetPivotPos().x;
	targetPos.y = targetDirection.y - GetPivotPos().y;
	targetPos.Normalize();
	// sets new pointing dir
	lastAxisMovAngle = atan2f(targetPos.y, targetPos.x);
	pointingDir = GetPointingDir(lastAxisMovAngle);
	// updates renderflip if we need
	CheckRenderFlip();
	// links animation and textures
	switch (combat_state)
	{
	case combatState::IDLE:
		break;
	case combatState::BASIC:
	{
		/*currentAnimation = &attack1[pointingDir];
		entityTex = attack1Tex;*/
		break;
	}
	case combatState::DODGE: // common on all three player subclasses, this behaviour pertains to player Entity
		break;
	case combatState::SPECIAL1:
		break;
	case combatState::SPECIAL2:
		break;
	case combatState::ULTIMATE:
	{
		App->audio->PlayFx(App->entityFactory->strech_Shoot, 0);
		currentAnimation = &ultiCastAnim[pointingDir];
		entityTex = ultiCastTex;
		break;
	}
	case combatState::MAX:
		break;
	default:
		ret = false;
		break;
	}

	return ret;
}

//bool Shara::CleanUp()
//{
//	
//	return true;
//}

//bool Shara::PostUpdate()
//{
//	return true;
//}

bool Shara::Load(pugi::xml_node &node)
{
	pugi::xml_node nodeSpeed = node.child("speed");

	characterBaseSpeed.x = nodeSpeed.attribute("speedx").as_float();
	characterBaseSpeed.y = nodeSpeed.attribute("speedy").as_float();

	return true;
}

bool Shara::Save(pugi::xml_node &node) const
{
	pugi::xml_node nodeSpeed = node.append_child("Shara").append_child("speed");

	nodeSpeed.append_attribute("speedx") = characterBaseSpeed.x;
	nodeSpeed.append_attribute("speedy") = characterBaseSpeed.y;

	return true;
}