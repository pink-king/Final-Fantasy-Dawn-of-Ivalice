
#include "Shara.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1BuffManager.h"
#include "j1AttackManager.h"
#include "j1EntityFactory.h"

Shara::Shara(int posX, int posY):PlayerEntity(posX,posY)
{
	character = characterName::SHARA;
	name.assign("Shara");

	// TODO: import from xml
	spritesheet = App->tex->Load("textures/characters/shara/Shara_run_WIP.png");
	dash_spritesheet = App->tex->Load("textures/characters/shara/Shara_dash_WIP.png");
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

	dashMaxDistance = 120.f;


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
	fPoint pivotPos = GetPivotPos();
	//LOG("%f,%f", pivot.x, pivot.y);
	iPoint onTilePos = App->map->WorldToMap(pivotPos.x, pivotPos.y);
	//LOG("Player pos: %f,%f | Tile pos: %i,%i",position.x, position.y, onTilePos.x, onTilePos.y);
	/*if (App->pathfinding->IsWalkable(onTilePos))
	{
		previousPos = position;*/


	if (!isParalize)
	{
		if (inputReady)
		{
			InputMovement(dt);
			InputCombat();
		}
		if (!inputReady) // dash, or animations that needs control of its finish state
		{	// TODO: do switch combat state if this get more complexity

			//reposition pos
			transference_pivot = dashPivotOffset[pointingDir][(int)currentAnimation->GetCurrentFloatFrame()];
			transference_pivot -= pivot;

			if (currentAnimation->Finished())
			{
				currentAnimation->Reset();
				entityTex = spritesheet;
				currentAnimation = &idle[pointingDir];
				inputReady = true;
				transference_pivot = { 0,0 };
			}

			position = App->camera2D->lerp(position, dashDestinationPos, dt * currentAnimation->speed);

		}
	}
	else
	{
		currentAnimation->speed = 0.f;
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
			//App->attackManager->AddPropagationAttack(this, App->entityFactory->player->GetCrossHairSubtile(), propagationType::BFS, 10, 7, 40);
			// TODO: Adds a camera shaking based on "x" needed data from attack components
			// same applies when we receive damage
			App->camera2D->AddTrauma(10.0f / 100.f);
			App->input->DoGamePadRumble(0.3f, 100);

		}
		break;
	case combatState::DODGE:
		break;
	case combatState::SPECIAL1:
		if (coolDownData.special1.timer.Read() > coolDownData.special1.cooldownTime)
		{
			coolDownData.special1.timer.Start();

			//App->entityFactory->CreateArrow(App->entityFactory->player->GetSelectedCharacterEntity()->GetThrowingPos(), App->entityFactory->player->GetCrossHairPivotPos().Return_fPoint(), 75, App->entityFactory->player->GetMarche());
			App->camera2D->AddTrauma(20.f / 100.f);
			App->input->DoGamePadRumble(0.3f, 100);
			// add gui clock

			/*if (!App->gui->spawnedClocks.Ritz.special1)
			{

				myUIClocks.special1 = App->gui->AddClock(App->gui->allclocksData.ability1.position, &App->gui->allclocksData.ability1.section, "special1", "Ritz", App->scene->inGamePanel);

				App->gui->spawnedClocks.Ritz.special1 = true;
			}
			else
			{
				myUIClocks.special1->Restart();
			}*/


		}
		break;

	case combatState::SPECIAL2:
		if (coolDownData.special2.timer.Read() > coolDownData.special2.cooldownTime)
		{
			coolDownData.special2.timer.Start();

			//App->audio->PlayFx(App->entityFactory->ritzAbility2, 0);

			// add gui clock

			/*if (!App->gui->spawnedClocks.Ritz.special2)
			{

				myUIClocks.special2 = App->gui->AddClock(App->gui->allclocksData.ability2.position, &App->gui->allclocksData.ability2.section, "special2", "Ritz", App->scene->inGamePanel);

				App->gui->spawnedClocks.Ritz.special2 = true;
			}
			else
			{
				myUIClocks.special2->Restart();
			}*/


		}
		break;

	case combatState::ULTIMATE:
	{
		if (coolDownData.ultimate.timer.Read() > coolDownData.ultimate.cooldownTime)
		{
			/*App->audio->PlayFx(App->entityFactory->marcheUltimateScream, 0);
			LOG("Launch ULTIMATE");

			App->attackManager->AddPropagationAttack(this, App->entityFactory->player->GetCrossHairSubtile(), propagationType::BFS, 10, 20, 40);
			App->camera2D->AddTrauma(70.0f / 100.f);
			App->input->DoGamePadRumble(0.7f, 400);*/



			coolDownData.ultimate.timer.Start();

		}
		break;
	}
	case combatState::MAX:
		break;
	default:
		break;
	}

	return true;
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
