#include "Ritz.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1BuffManager.h"
#include "j1AttackManager.h"
#include "j1EntityFactory.h"

Ritz::Ritz(int posX, int posY):PlayerEntity(posX,posY)
{
	character = characterName::RITZ;
	name.assign("Ritz");

	// TODO: import from xml
	spritesheet = App->tex->Load("textures/characters/ritz/Ritz_run_WIP.png");
	dash_spritesheet = App->tex->Load("textures/characters/ritz/Ritz_dash_WIP.png");
	entityTex = spritesheet;

	// IDLE
	idle[(int)facingDirection::N].PushBack({ 177,0,45,60 });
	idle[(int)facingDirection::S].PushBack({ 0,0,45,60 });
	idle[(int)facingDirection::E].PushBack({ 90,0,45,60 });
	idle[(int)facingDirection::W].PushBack({ 90,0,45,60 }); // same as E but flipped - 4
	idle[(int)facingDirection::NE].PushBack({ 135,0,45,60 });
	idle[(int)facingDirection::SE].PushBack({ 45,0,45,60 });
	idle[(int)facingDirection::SW].PushBack({ 45,0,45,60 }); // same as SE but flipped - 3
	idle[(int)facingDirection::NW].PushBack({ 135,0,45,60 }); // same as NE but flipped - 7
	// one frame doesnt need speed
	//idle[(int)facingDirection::N].speed = 10.0f;

	// RUN
	run[(int)facingDirection::N].PushBack({ 0,180,45,60 });
	run[(int)facingDirection::N].PushBack({ 45,180,45,60 });
	run[(int)facingDirection::N].PushBack({ 90,180,45,60 });
	run[(int)facingDirection::N].PushBack({ 135,180,45,60 });
	run[(int)facingDirection::N].PushBack({ 180,180,45,60 });
	run[(int)facingDirection::N].PushBack({ 225,180,45,60 });
	run[(int)facingDirection::N].speed = 10.0f;

	run[(int)facingDirection::S].PushBack({ 0,60,45,60 });
	run[(int)facingDirection::S].PushBack({ 45,60,45,60 });
	run[(int)facingDirection::S].PushBack({ 90,60,45,60 });
	run[(int)facingDirection::S].PushBack({ 135,60,45,60 });
	run[(int)facingDirection::S].PushBack({ 180,60,45,60 });
	run[(int)facingDirection::S].PushBack({ 225,60,45,60 });
	run[(int)facingDirection::S].speed = 10.0f;

	run[(int)facingDirection::E].PushBack({ 0,120,45,60 });
	run[(int)facingDirection::E].PushBack({ 45,120,45,60 });
	run[(int)facingDirection::E].PushBack({ 90,120,45,60 });
	run[(int)facingDirection::E].PushBack({ 135,120,45,60 });
	run[(int)facingDirection::E].PushBack({ 180,120,45,60 });
	run[(int)facingDirection::E].PushBack({ 225,120,45,60 });
	run[(int)facingDirection::E].speed = 10.0f;

	run[(int)facingDirection::W].PushBack({ 0,120,45,60 });
	run[(int)facingDirection::W].PushBack({ 45,120,45,60 });
	run[(int)facingDirection::W].PushBack({ 90,120,45,60 });
	run[(int)facingDirection::W].PushBack({ 135,120,45,60 });
	run[(int)facingDirection::W].PushBack({ 180,120,45,60 });
	run[(int)facingDirection::W].PushBack({ 225,120,45,60 });
	run[(int)facingDirection::W].speed = 10.0f;

	run[(int)facingDirection::SE].PushBack({ 0,240,45,60 });
	run[(int)facingDirection::SE].PushBack({ 45,240,45,60 });
	run[(int)facingDirection::SE].PushBack({ 90,240,45,60 });
	run[(int)facingDirection::SE].PushBack({ 135,240,45,60 });
	run[(int)facingDirection::SE].PushBack({ 180,240,45,60 });
	run[(int)facingDirection::SE].PushBack({ 225,240,45,60 });
	run[(int)facingDirection::SE].speed = 10.0f;

	run[(int)facingDirection::SW].PushBack({ 0,240,45,60 });
	run[(int)facingDirection::SW].PushBack({ 45,240,45,60 });
	run[(int)facingDirection::SW].PushBack({ 90,240,45,60 });
	run[(int)facingDirection::SW].PushBack({ 135,240,45,60 });
	run[(int)facingDirection::SW].PushBack({ 180,240,45,60 });
	run[(int)facingDirection::SW].PushBack({ 225,240,45,60 });
	run[(int)facingDirection::SW].speed = 10.0f;

	run[(int)facingDirection::NE].PushBack({ 0,300,45,60 });
	run[(int)facingDirection::NE].PushBack({ 45,300,45,60 });
	run[(int)facingDirection::NE].PushBack({ 90,300,45,60 });
	run[(int)facingDirection::NE].PushBack({ 135,300,45,60 });
	run[(int)facingDirection::NE].PushBack({ 180,300,45,60 });
	run[(int)facingDirection::NE].PushBack({ 225,300,45,60 });
	run[(int)facingDirection::NE].speed = 10.0f;

	run[(int)facingDirection::NW].PushBack({ 0,300,45,60 });
	run[(int)facingDirection::NW].PushBack({ 45,300,45,60 });
	run[(int)facingDirection::NW].PushBack({ 90,300,45,60 });
	run[(int)facingDirection::NW].PushBack({ 135,300,45,60 });
	run[(int)facingDirection::NW].PushBack({ 180,300,45,60 });
	run[(int)facingDirection::NW].PushBack({ 225,300,45,60 });
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

	dashPivotOffset[(int)facingDirection::E][0] = { 67.f,78.f };
	dashPivotOffset[(int)facingDirection::E][1] = { 89.f,80.f };
	dashPivotOffset[(int)facingDirection::E][2] = { 97.f,80.f };
	dashPivotOffset[(int)facingDirection::E][3] = { 101.f,78.f };

	dashPivotOffset[(int)facingDirection::W][0] = { 64.f,78.f };
	dashPivotOffset[(int)facingDirection::W][1] = { 42.f,80.f };
	dashPivotOffset[(int)facingDirection::W][2] = { 32.f,80.f };
	dashPivotOffset[(int)facingDirection::W][3] = { 21.f,78.f };

	dashPivotOffset[(int)facingDirection::S][0] = { 67.f,102.f };
	dashPivotOffset[(int)facingDirection::S][1] = { 65.f,102.f };
	dashPivotOffset[(int)facingDirection::S][2] = { 64.f,102.f };
	dashPivotOffset[(int)facingDirection::S][3] = { 60.f,106.f };

	dashPivotOffset[(int)facingDirection::N][0] = { 64.f,55.f };
	dashPivotOffset[(int)facingDirection::N][1] = { 63.f,53.f };
	dashPivotOffset[(int)facingDirection::N][2] = { 61.f,64.f };
	dashPivotOffset[(int)facingDirection::N][3] = { 61.f,69.f };

	dashPivotOffset[(int)facingDirection::SE][0] = { 69.f,82.f };
	dashPivotOffset[(int)facingDirection::SE][1] = { 75.f,82.f };
	dashPivotOffset[(int)facingDirection::SE][2] = { 82.f,82.f };
	dashPivotOffset[(int)facingDirection::SE][3] = { 78.f,82.f };

	dashPivotOffset[(int)facingDirection::NE][0] = { 69.f,73.f };
	dashPivotOffset[(int)facingDirection::NE][1] = { 87.f,73.f };
	dashPivotOffset[(int)facingDirection::NE][2] = { 96.f,73.f };
	dashPivotOffset[(int)facingDirection::NE][3] = { 96.f,73.f };

	dashPivotOffset[(int)facingDirection::NW][0] = { 64.f,73.f };
	dashPivotOffset[(int)facingDirection::NW][1] = { 44.f,73.f };
	dashPivotOffset[(int)facingDirection::NW][2] = { 35.f,73.f };
	dashPivotOffset[(int)facingDirection::NW][3] = { 26.f,73.f };

	
	dashPivotOffset[(int)facingDirection::SW][0] = { 60.f,82.f };
	dashPivotOffset[(int)facingDirection::SW][1] = { 54.f,82.f };
	dashPivotOffset[(int)facingDirection::SW][2] = { 48.f,82.f };
	dashPivotOffset[(int)facingDirection::SW][3] = { 45.f,82.f };

	dashMaxDistance = 80.f;

	currentAnimation = &run[(int)facingDirection::SE];

	// cooldown data test - TODO: import for each character its base cooldown in ms from xml
	coolDownData.basic.cooldownTime = 0;
	coolDownData.dodge.cooldownTime = 0;
	coolDownData.special1.cooldownTime = 500;
	coolDownData.special2.cooldownTime = 1000;
	coolDownData.ultimate.cooldownTime = 3000;

	previousPos = position;
}

Ritz::~Ritz()
{}

bool Ritz::Start()
{
	SetPivot(23, 48);
	size.create(45, 60);
	return true;
}

bool Ritz::PreUpdate()
{
	return true;
}

bool Ritz::Update(float dt)
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

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, this, "\0", 1000);
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
			App->audio->PlayFx(App->entityFactory->RitzBasic, 0);
			App->entityFactory->CreateArrow(App->entityFactory->player->GetThrowingPos(), App->entityFactory->player->GetCrossHairPivotPos().Return_fPoint(),
				75, App->entityFactory->player->GetMarche(), PROJECTILE_TYPE::MAGIC_BOLT);
			
			//App->attackManager->AddPropagationAttack(this, App->entityFactory->player->GetCrossHairSubtile(), propagationType::BFS, 10, 7, 40);
			// TODO: Adds a camera shaking based on "x" needed data from attack components
			// same applies when we receive damage
			/*App->camera2D->AddTrauma(10.0f / 100.f);
			App->input->DoGamePadRumble(0.3f, 100);*/

		}
		break;
	case combatState::DODGE:
		if (coolDownData.dodge.timer.Read() > coolDownData.basic.cooldownTime)
		{
			coolDownData.dodge.timer.Start();
			App->audio->PlayFx(App->entityFactory->dash, 0);
		}
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
			coolDownData.ultimate.timer.Start();

			App->entityFactory->CreateArrow(App->entityFactory->player->GetCrossHairPivotPos().Return_fPoint(), { 0,0 }, 0, this, PROJECTILE_TYPE::DEATH_CIRCLE);
			/*App->audio->PlayFx(App->entityFactory->marcheUltimateScream, 0);
			LOG("Launch ULTIMATE");

			App->attackManager->AddPropagationAttack(this, App->entityFactory->player->GetCrossHairSubtile(), propagationType::BFS, 10, 20, 40);
			App->camera2D->AddTrauma(70.0f / 100.f);
			App->input->DoGamePadRumble(0.7f, 400);*/
			



		}
		break;
	}
	case combatState::MAX:
		break;
	default:
		break;
	}

	/*if ((App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_Y) == 1 || App->input->GetKey(SDL_SCANCODE_E) == 1)
		&& !isBurned)
	{
		App->buff->CreateBurned(this, this,1, 15,"inteligence");
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == 1)
	{
		App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, this, "\0", 20);
	}*/
	if (stat.size() != 0)
	{
		if (App->buff->DamageInTime(this))
		{
			App->buff->entitiesTimeDamage.remove(this);
		}
	}

	return true;
}

//bool Ritz::CleanUp()
//{
//	
//	return true;
//}

//bool Ritz::PostUpdate()
//{
//	return true;
//}
