#include "Marche.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1BuffManager.h"
#include "j1Entity.h"
//test buff
#include "j1Input.h"
#include "j1EntityFactory.h"

#include "j1AttackManager.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Scene.h"
#include "Projectile.h"

Marche::Marche(int posX, int posY): PlayerEntity(posX,posY)
{
	character = characterName::MARCHE;
	name.assign("Marche");

	// TODO: import from xml
	spritesheet = App->tex->Load("textures/characters/marche/Marche_run_WIP.png");
	dash_spritesheet = App->tex->Load("textures/characters/marche/Marche_dash_WIP.png");
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

	dash[(int)facingDirection::E].PushBack({ 0,0,95,110 });
	dash[(int)facingDirection::E].PushBack({ 95,0,95,110 });
	dash[(int)facingDirection::E].PushBack({ 190,0, 95,110 });
	dash[(int)facingDirection::E].PushBack({ 285,0, 95,110 });
	dash[(int)facingDirection::E].loop = false;
	dash[(int)facingDirection::E].speed = 16.f;

	dash[(int)facingDirection::W].PushBack({ 0,0,95,110 });
	dash[(int)facingDirection::W].PushBack({ 95,0,95,110 });
	dash[(int)facingDirection::W].PushBack({ 190,0, 95,110 });
	dash[(int)facingDirection::W].PushBack({ 285,0, 95,110 });
	dash[(int)facingDirection::W].loop = false;
	dash[(int)facingDirection::W].speed = 16.0f;

	dash[(int)facingDirection::S].PushBack({ 0,110,95,110 });
	dash[(int)facingDirection::S].PushBack({ 95,110,95,110 });
	dash[(int)facingDirection::S].PushBack({ 190,110, 95,110 });
	dash[(int)facingDirection::S].PushBack({ 285,110, 95,110 });
	dash[(int)facingDirection::S].loop = false;
	dash[(int)facingDirection::S].speed = 16.0f;

	dash[(int)facingDirection::N].PushBack({ 0,220,95,110 });
	dash[(int)facingDirection::N].PushBack({ 95,220,95,110 });
	dash[(int)facingDirection::N].PushBack({ 190,220, 95,110 });
	dash[(int)facingDirection::N].PushBack({ 285,220, 95,110 });
	dash[(int)facingDirection::N].loop = false;
	dash[(int)facingDirection::N].speed = 16.0f;

	dash[(int)facingDirection::SE].PushBack({ 0,330,95,110 });
	dash[(int)facingDirection::SE].PushBack({ 95,330,95,110 });
	dash[(int)facingDirection::SE].PushBack({ 190,330, 95,110 });
	dash[(int)facingDirection::SE].PushBack({ 285,330, 95,110 });
	dash[(int)facingDirection::SE].loop = false;
	dash[(int)facingDirection::SE].speed = 16.0f;

	dash[(int)facingDirection::SW].PushBack({ 0,330,95,110 });
	dash[(int)facingDirection::SW].PushBack({ 95,330,95,110 });
	dash[(int)facingDirection::SW].PushBack({ 190,330, 95,110 });
	dash[(int)facingDirection::SW].PushBack({ 285,330, 95,110 });
	dash[(int)facingDirection::SW].loop = false;
	dash[(int)facingDirection::SW].speed = 16.0f;

	dash[(int)facingDirection::NE].PushBack({ 0,440,95,110 });
	dash[(int)facingDirection::NE].PushBack({ 95,440,95,110 });
	dash[(int)facingDirection::NE].PushBack({ 190,440, 95,110 });
	dash[(int)facingDirection::NE].PushBack({ 285,440, 95,110 });
	dash[(int)facingDirection::NE].loop = false;
	dash[(int)facingDirection::NE].speed = 16.0f;

	dash[(int)facingDirection::NW].PushBack({ 0,440,95,110 });
	dash[(int)facingDirection::NW].PushBack({ 95,440,95,110 });
	dash[(int)facingDirection::NW].PushBack({ 190,440, 95,110 });
	dash[(int)facingDirection::NW].PushBack({ 285,440, 95,110 });
	dash[(int)facingDirection::NW].loop = false;
	dash[(int)facingDirection::NW].speed = 16.0f;

	// TODO: polish this offset positions
	// this offsets relates to its position inside the 95,110 rect pivot position (character foot line)
	dashPivotOffset[(int)facingDirection::E][0] = { 52.f,68.f };
	dashPivotOffset[(int)facingDirection::E][1] = { 60.f,68.f };
	dashPivotOffset[(int)facingDirection::E][2] = { 75.f,68.f };
	dashPivotOffset[(int)facingDirection::E][3] = { 85.f,72.f };
	
	dashPivotOffset[(int)facingDirection::W][0] = { 37.f,68.f };
	dashPivotOffset[(int)facingDirection::W][1] = { 29.f,68.f };
	dashPivotOffset[(int)facingDirection::W][2] = { 15.f,68.f };
	dashPivotOffset[(int)facingDirection::W][3] = { 10.f,68.f };

	dashPivotOffset[(int)facingDirection::S][0] = { 48.f,94.f };
	dashPivotOffset[(int)facingDirection::S][1] = { 48.f,94.f };
	dashPivotOffset[(int)facingDirection::S][2] = { 48.f,101.f };
	dashPivotOffset[(int)facingDirection::S][3] = { 48.f,94.f };

	dashPivotOffset[(int)facingDirection::N][0] = { 48.f,52.f };
	dashPivotOffset[(int)facingDirection::N][1] = { 48.f,52.f };
	dashPivotOffset[(int)facingDirection::N][2] = { 48.f,52.f };
	dashPivotOffset[(int)facingDirection::N][3] = { 48.f,52.f };

	dashPivotOffset[(int)facingDirection::SE][0] = { 48.f,84.f };
	dashPivotOffset[(int)facingDirection::SE][1] = { 58.f,90.f };
	dashPivotOffset[(int)facingDirection::SE][2] = { 67.f,92.f };
	dashPivotOffset[(int)facingDirection::SE][3] = { 73.f,93.f };

	dashPivotOffset[(int)facingDirection::NE][0] = { 48.f,58.f };
	dashPivotOffset[(int)facingDirection::NE][1] = { 64.f,58.f };
	dashPivotOffset[(int)facingDirection::NE][2] = { 70.f,56.f };
	dashPivotOffset[(int)facingDirection::NE][3] = { 73.f,62.f };

	dashPivotOffset[(int)facingDirection::NW][0] = { 48.f,58.f };
	dashPivotOffset[(int)facingDirection::NW][1] = { 32.f,58.f };
	dashPivotOffset[(int)facingDirection::NW][2] = { 26.f,56.f };
	dashPivotOffset[(int)facingDirection::NW][3] = { 24.f,62.f };

	dashPivotOffset[(int)facingDirection::SW][0] = { 48.f,84.f };
	dashPivotOffset[(int)facingDirection::SW][1] = { 40.f,90.f };
	dashPivotOffset[(int)facingDirection::SW][2] = { 28.f,92.f };
	dashPivotOffset[(int)facingDirection::SW][3] = { 23.f,93.f };

	dashMaxDistance = 64.f;

	currentAnimation = &run[(int)facingDirection::W];

	// cooldown data test - TODO: import for each character its base cooldown in ms from xml
	coolDownData.basic.cooldownTime = 0;
	coolDownData.dodge.cooldownTime = 500; // DODGE "COOLDOWN" is limited to finish its "translation" and animation
	coolDownData.special1.cooldownTime = 500;
	coolDownData.special2.cooldownTime = 1000;
	coolDownData.ultimate.cooldownTime = 10000;

	previousPos = position;


	// better speed 
	characterBaseSpeed.x /= 1.3f; 
	characterBaseSpeed.y /= 1.3f;

	//
	//previousFrame = 1; // fake previousFrame to enter on first anim state

}

Marche::~Marche()
{}

bool Marche::Start()
{
	SetPivot(23, 48);
	size.create(45,60);
	return true;
}

bool Marche::PreUpdate()
{
	return true;
}

bool Marche::Update(float dt)
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
		
		//LOG("transpivot: %f,%f:", transference_pivot.x, transference_pivot.y);
	/*}
	else
	{
		position = previousPos;
	}*/

	// CHECK COMBAT STATE
	switch (combat_state)
	{
	case combatState::IDLE:
		
		break;
	case combatState::BASIC:
		
		if (coolDownData.basic.timer.Read() > coolDownData.basic.cooldownTime)
		{
			App->audio->PlayFx(App->entityFactory->marcheBasic, 0);
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

		if (coolDownData.dodge.timer.Read() > coolDownData.dodge.cooldownTime)
		{
			//App->audio->PlayFx(App->entityFactory->dash, 0);
			coolDownData.dodge.timer.Start();
			dodgedTest = true;
				
		}
		else 
		{
			
			dodged = false;
		}
			break;
	case combatState::SPECIAL1:
		if (coolDownData.special1.timer.Read() > coolDownData.special1.cooldownTime)
		{
			coolDownData.special1.timer.Start();
			App->audio->PlayFx(App->entityFactory->marcheAbility1, 0);
			App->entityFactory->CreateArrow(App->entityFactory->player->GetSelectedCharacterEntity()->GetThrowingPos(), App->entityFactory->player->GetCrossHairPivotPos().Return_fPoint(), 75, App->entityFactory->player->GetMarche(),PROJECTILE_TYPE::BASIC_ARROW);
			// add gui clock

			if (!App->gui->spawnedClocks.Marche.special1)
			{

				myUIClocks.special1 = App->gui->AddClock(App->gui->allclocksData.ability1.position, &App->gui->allclocksData.ability1.section, "special1", "Marche", App->scene->inGamePanel);

				App->gui->spawnedClocks.Marche.special1 = true;
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

			App->audio->PlayFx(App->entityFactory->marcheAbility2, 0);

			App->entityFactory->CreateArrow(App->entityFactory->player->GetSelectedCharacterEntity()->GetThrowingPos(), App->entityFactory->player->GetCrossHairPivotPos().Return_fPoint(), 75, App->entityFactory->player->GetMarche(), PROJECTILE_TYPE::CONTAGIOUS_ARROW);

			// add gui clock

			if (!App->gui->spawnedClocks.Marche.special2)
			{

				myUIClocks.special2 = App->gui->AddClock(App->gui->allclocksData.ability2.position, &App->gui->allclocksData.ability2.section, "special2", "Marche", App->scene->inGamePanel);

				App->gui->spawnedClocks.Marche.special2 = true;
			}
			else
			{
				myUIClocks.special2->Restart();
			}


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
			UlitMarche();

			coolDownData.ultimate.timer.Start();
		
		}
		break;
	}
	case combatState::MAX:
		break;
	default:
		break;
	}

	//test buff
	/*j1Entity* j1 = nullptr;
	if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_A) == 1 || App->input->GetKey(SDL_SCANCODE_Q) == 1)
	{
		
		App->buff->DirectAttack(this, this, 10, ELEMENTAL_TYPE::NORMAL_ELEMENT);
		
	}
	if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_B) == 1 || App->input->GetKey(SDL_SCANCODE_W) == 1)
	{
		App->buff->DirectAttack(this, this, 15, ELEMENTAL_TYPE::POISON_ELEMENT);
	}
	if ((App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_Y) == 1 || App->input->GetKey(SDL_SCANCODE_E) == 1)
		&& !isBurned)
	{
		App->buff->CreateBurned(this, this,5,2);
	}
	if ((App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_X) == 1 || App->input->GetKey(SDL_SCANCODE_R) == 1)
		&& !isParalize)
	{
		App->buff->CreateParalize(this, this, 10);
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == 1)
	{
		App->itemsManager->CreateItem("superSword", OBJECT_TYPE::WEAPON_OBJECT, this);
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == 1)
	{
		App->itemsManager->RemoveItem(this,"superSword");
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == 1)
	{
		std::vector<items*>::iterator item = equipedObjects.begin();
		App->itemsManager->DesequipItem(this, *item);
	}*/
	if (App->input->GetKey(SDL_SCANCODE_3) == 1)
	{
		std::vector<LootEntity*>::iterator item = App->entityFactory->player->bagObjects.begin();
		if(item != App->entityFactory->player->bagObjects.end())
			App->entityFactory->player->EquipItem(*item);
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == 1)
	{
		std::vector<LootEntity*>::iterator item = App->entityFactory->player->equipedObjects.begin();
		if (item != App->entityFactory->player->equipedObjects.end())
			App->entityFactory->player->DesequipItem(*item);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_A) == 1)
	{
		std::vector<LootEntity*>::iterator item = App->entityFactory->player->consumables.begin();
		if (item != App->entityFactory->player->consumables.end())
			App->entityFactory->player->ConsumConsumable(*item,this);
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == 1)
	{
		App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, this, "\0", 100);
	}

	if (App->input->GetKey(SDL_SCANCODE_B) == 1)
	{
		App->buff->CreatePoision(this, this, 5, 5, "a");
	}

	/*if (App->input->GetKey(SDL_SCANCODE_6) == 1)
	{
		App->itemsManager->CreateItem("sword2", OBJECT_TYPE::WEAPON_OBJECT, this);
	}*/
	//if (App->input->GetKey(SDL_SCANCODE_3) == 1)
	//{
	//	
	//}
	//if (App->input->GetKey(SDL_SCANCODE_4) == 1)
	//{
	//	
	//}

	if (stat.size() != 0)
	{
		if (App->buff->DamageInTime(this))
		{
			App->buff->entitiesTimeDamage.remove(this);
		}	
	}
	if (App->entityFactory->player->stat.size() != 0)
	{
		if (App->buff->DamageInTime(App->entityFactory->player))
		{
			App->buff->entitiesTimeDamage.remove(App->entityFactory->player);
		}
	}
	return true;
}

void Marche::UlitMarche()
{
	App->buff->TemporalBuff(this, BUFF_TYPE::MULTIPLICATIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, 1, 5);
	App->buff->TemporalBuff(App->entityFactory->player->GetRitz(), BUFF_TYPE::MULTIPLICATIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, 1, 5);
	App->buff->TemporalBuff(App->entityFactory->player->GetShara(), BUFF_TYPE::MULTIPLICATIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, 1, 5);
	App->buff->TemporalBuff(this, BUFF_TYPE::MULTIPLICATIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, 1, 5);
	App->buff->TemporalBuff(App->entityFactory->player->GetRitz(), BUFF_TYPE::MULTIPLICATIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, 1, 5);
	App->buff->TemporalBuff(App->entityFactory->player->GetShara(), BUFF_TYPE::MULTIPLICATIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, 1, 5);
	App->buff->TemporalBuff(this, BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, 0.9F, 5);
	App->buff->TemporalBuff(App->entityFactory->player->GetRitz(), BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, 0.9F, 5);
	App->buff->TemporalBuff(App->entityFactory->player->GetShara(), BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, 0.9F, 5);
	App->buff->TemporalBuff(this, BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, 100, 5);
}

//bool Marche::CleanUp()
//{
//	return true;
//}

//bool Marche::PostUpdate()
//{
//	return true;
//}
