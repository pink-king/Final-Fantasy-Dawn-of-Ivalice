#include "Marche.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1BuffManager.h"
#include "j1Entity.h"
//test buff
#include "j1Input.h"
#include "j1EntityFactory.h"
#include "j1ItemsManager.h"
#include "j1AttackManager.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Scene.h"

Marche::Marche(int posX, int posY): PlayerEntity(posX,posY)
{
	character = characterName::MARCHE;
	name.assign("Marche");

	// TODO: import from xml
	spritesheet = App->tex->Load("textures/characters/marche/Marche_run_WIP.png");
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

	currentAnimation = &run[(int)facingDirection::SE];

	// cooldown data test - TODO: import for each character its base cooldown in ms from xml
	coolDownData.basic.cooldownTime = 0;
	coolDownData.dodge.cooldownTime = 0;
	coolDownData.special1.cooldownTime = 500;
	coolDownData.special2.cooldownTime = 1000;
	coolDownData.ultimate.cooldownTime = 3000;

	previousPos = position;


	// better speed 
	characterBaseSpeed.x /= 1.3f; 
	characterBaseSpeed.y /= 1.3f;

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
			currentAnimation->speed = 10.f;
			InputMovement(dt);
			InputCombat();

			//LOG("player pipos: %f,%f", GetPivotPos().x, GetPivotPos().y);
		}
		else
		{
			currentAnimation->speed = 0.f;
		}
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
			LOG("Launch BASIC");
			coolDownData.basic.timer.Start();
			App->attackManager->AddPropagationAttack(this, App->entityFactory->player->GetCrossHairSubtile(), propagationType::BFS, 10, 7, 40);
		}
		break;
	case combatState::DODGE:
		break;
	case combatState::SPECIAL1:
		if (coolDownData.special1.timer.Read() > coolDownData.special1.cooldownTime)
		{
			coolDownData.special1.timer.Start();

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
			LOG("Launch ULTIMATE");
			coolDownData.ultimate.timer.Start();
			App->attackManager->AddPropagationAttack(this, App->entityFactory->player->GetCrossHairSubtile(), propagationType::BFS, 10, 20, 40);
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
	}
	if (App->input->GetKey(SDL_SCANCODE_5) == 1)
	{
		std::vector<items*>::iterator item = bagObjects.begin();
		App->itemsManager->EquipItem(this, *item);
	}
	if (App->input->GetKey(SDL_SCANCODE_6) == 1)
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

	return true;
}

bool Marche::CleanUp()
{
	return true;
}

//bool Marche::PostUpdate()
//{
//	return true;
//}
