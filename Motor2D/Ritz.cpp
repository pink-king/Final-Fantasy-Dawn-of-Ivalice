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
	// from parent, parent deletes
	spritesheet = App->tex->Load("textures/characters/ritz/Ritz_run_WIP.png");
	dash_spritesheet = App->tex->Load("textures/characters/ritz/Ritz_dash_WIP.png");
	entityTex = spritesheet;
	// own
	attack1Tex = App->tex->Load("textures/characters/ritz/Ritz_basic_attack_2.png");
	teleportTex = App->tex->Load("textures/characters/ritz/Ritz_teleport_WIP.png");

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

	// BASIC ATTACK ANIMATIONS - exclusive of char class
	// ----------------------------------------------------
	attack1[(int)facingDirection::E].PushBack({ 0,360,45,60 });
	attack1[(int)facingDirection::E].PushBack({ 45,360,45,60 });
	attack1[(int)facingDirection::E].PushBack({ 90,360,45,60 });
	attack1[(int)facingDirection::E].PushBack({ 135,360,45,60 });
	attack1[(int)facingDirection::E].PushBack({ 180,360,45,60 });
	attack1[(int)facingDirection::E].PushBack({ 225,360,45,60 });
	attack1[(int)facingDirection::E].PushBack({ 270,360,45,60 });
	attack1[(int)facingDirection::E].PushBack({ 315,360,45,60 });
	attack1[(int)facingDirection::E].PushBack({ 360,360,45,60 });
	attack1[(int)facingDirection::E].PushBack({ 405,360,45,60 });
	attack1[(int)facingDirection::E].PushBack({ 450,360,45,60 });
	attack1[(int)facingDirection::E].PushBack({ 0,420,45,60 });
	attack1[(int)facingDirection::E].PushBack({ 45,420,45,60 });
	attack1[(int)facingDirection::E].PushBack({ 90,420,45,60 });
	attack1[(int)facingDirection::E].PushBack({ 135,420,45,60 });
	attack1[(int)facingDirection::E].PushBack({ 180,420,45,60 });
	// ------
	attack1[(int)facingDirection::W].PushBack({ 0,360,45,60 });
	attack1[(int)facingDirection::W].PushBack({ 45,360,45,60 });
	attack1[(int)facingDirection::W].PushBack({ 90,360,45,60 });
	attack1[(int)facingDirection::W].PushBack({ 135,360,45,60 });
	attack1[(int)facingDirection::W].PushBack({ 180,360,45,60 });
	attack1[(int)facingDirection::W].PushBack({ 225,360,45,60 });
	attack1[(int)facingDirection::W].PushBack({ 270,360,45,60 });
	attack1[(int)facingDirection::W].PushBack({ 315,360,45,60 });
	attack1[(int)facingDirection::W].PushBack({ 360,360,45,60 });
	attack1[(int)facingDirection::W].PushBack({ 405,360,45,60 });
	attack1[(int)facingDirection::W].PushBack({ 450,360,45,60 });
	attack1[(int)facingDirection::W].PushBack({ 0,420,45,60 });
	attack1[(int)facingDirection::W].PushBack({ 45,420,45,60 });
	attack1[(int)facingDirection::W].PushBack({ 90,420,45,60 });
	attack1[(int)facingDirection::W].PushBack({ 135,420,45,60 });
	attack1[(int)facingDirection::W].PushBack({ 180,420,45,60 });
	// ----------------------------------------------------------
	attack1[(int)facingDirection::N].PushBack({ 0,240,45,60 });
	attack1[(int)facingDirection::N].PushBack({ 45,240,45,60 });
	attack1[(int)facingDirection::N].PushBack({ 90,240,45,60 });
	attack1[(int)facingDirection::N].PushBack({ 135,240,45,60 });
	attack1[(int)facingDirection::N].PushBack({ 180,240,45,60 });
	attack1[(int)facingDirection::N].PushBack({ 225,240,45,60 });
	attack1[(int)facingDirection::N].PushBack({ 270,240,45,60 });
	attack1[(int)facingDirection::N].PushBack({ 315,240,45,60 });
	attack1[(int)facingDirection::N].PushBack({ 360,240,45,60 });
	attack1[(int)facingDirection::N].PushBack({ 405,240,45,60 });
	attack1[(int)facingDirection::N].PushBack({ 450,240,45,60 });
	attack1[(int)facingDirection::N].PushBack({ 0,300,45,60 });
	attack1[(int)facingDirection::N].PushBack({ 45,300,45,60 });
	attack1[(int)facingDirection::N].PushBack({ 90,300,45,60 });
	attack1[(int)facingDirection::N].PushBack({ 135,300,45,60 });
	attack1[(int)facingDirection::N].PushBack({ 180,300,45,60 });
	// -----------------------------------------------------------
	attack1[(int)facingDirection::S].PushBack({ 0, 120,45,60 });
	attack1[(int)facingDirection::S].PushBack({ 45, 120,45,60 });
	attack1[(int)facingDirection::S].PushBack({ 90, 120,45,60 });
	attack1[(int)facingDirection::S].PushBack({ 135, 120,45,60 });
	attack1[(int)facingDirection::S].PushBack({ 180, 120,45,60 });
	attack1[(int)facingDirection::S].PushBack({ 225, 120,45,60 });
	attack1[(int)facingDirection::S].PushBack({ 270, 120,45,60 });
	attack1[(int)facingDirection::S].PushBack({ 315, 120,45,60 });
	attack1[(int)facingDirection::S].PushBack({ 360, 120,45,60 });
	attack1[(int)facingDirection::S].PushBack({ 405, 120,45,60 });
	attack1[(int)facingDirection::S].PushBack({ 0, 180,45,60 });
	attack1[(int)facingDirection::S].PushBack({ 45, 180,45,60 });
	attack1[(int)facingDirection::S].PushBack({ 90, 180,45,60 });
	attack1[(int)facingDirection::S].PushBack({ 135, 180,45,60 });
	attack1[(int)facingDirection::S].PushBack({ 180, 180,45,60 });
	// ----------------------------------------------------------
	attack1[(int)facingDirection::SE].PushBack({ 0,0,45,60 });
	attack1[(int)facingDirection::SE].PushBack({ 45,0,45,60 });
	attack1[(int)facingDirection::SE].PushBack({ 90,0,45,60 });
	attack1[(int)facingDirection::SE].PushBack({ 135,0,45,60 });
	attack1[(int)facingDirection::SE].PushBack({ 180,0,45,60 });
	attack1[(int)facingDirection::SE].PushBack({ 225,0,45,60 });
	attack1[(int)facingDirection::SE].PushBack({ 270,0,45,60 });
	attack1[(int)facingDirection::SE].PushBack({ 315,0,45,60 });
	attack1[(int)facingDirection::SE].PushBack({ 360,0,45,60 });
	attack1[(int)facingDirection::SE].PushBack({ 405,0,45,60 });
	attack1[(int)facingDirection::SE].PushBack({ 0,60,45,60 });
	attack1[(int)facingDirection::SE].PushBack({ 45,60,45,60 });
	attack1[(int)facingDirection::SE].PushBack({ 90,60,45,60 });
	attack1[(int)facingDirection::SE].PushBack({ 135,60,45,60 });
	attack1[(int)facingDirection::SE].PushBack({ 180,60,45,60 });
	// --------
	attack1[(int)facingDirection::SW].PushBack({ 0,0,45,60 });
	attack1[(int)facingDirection::SW].PushBack({ 45,0,45,60 });
	attack1[(int)facingDirection::SW].PushBack({ 90,0,45,60 });
	attack1[(int)facingDirection::SW].PushBack({ 135,0,45,60 });
	attack1[(int)facingDirection::SW].PushBack({ 180,0,45,60 });
	attack1[(int)facingDirection::SW].PushBack({ 225,0,45,60 });
	attack1[(int)facingDirection::SW].PushBack({ 270,0,45,60 });
	attack1[(int)facingDirection::SW].PushBack({ 315,0,45,60 });
	attack1[(int)facingDirection::SW].PushBack({ 360,0,45,60 });
	attack1[(int)facingDirection::SW].PushBack({ 405,0,45,60 });
	attack1[(int)facingDirection::SW].PushBack({ 0,60,45,60 });
	attack1[(int)facingDirection::SW].PushBack({ 45,60,45,60 });
	attack1[(int)facingDirection::SW].PushBack({ 90,60,45,60 });
	attack1[(int)facingDirection::SW].PushBack({ 135,60,45,60 });
	attack1[(int)facingDirection::SW].PushBack({ 180,60,45,60 });
	// ----------------------------------------------------------
	attack1[(int)facingDirection::NE].PushBack({ 0,480,45,60 });
	attack1[(int)facingDirection::NE].PushBack({ 45,480,45,60 });
	attack1[(int)facingDirection::NE].PushBack({ 90,480,45,60 });
	attack1[(int)facingDirection::NE].PushBack({ 135,480,45,60 });
	attack1[(int)facingDirection::NE].PushBack({ 180,480,45,60 });
	attack1[(int)facingDirection::NE].PushBack({ 225,480,45,60 });
	attack1[(int)facingDirection::NE].PushBack({ 270,480,45,60 });
	attack1[(int)facingDirection::NE].PushBack({ 315,480,45,60 });
	attack1[(int)facingDirection::NE].PushBack({ 360,480,45,60 });
	attack1[(int)facingDirection::NE].PushBack({ 405,480,45,60 });
	attack1[(int)facingDirection::NE].PushBack({ 0,540,45,60 });
	attack1[(int)facingDirection::NE].PushBack({ 45,540,45,60 });
	attack1[(int)facingDirection::NE].PushBack({ 90,540,45,60 });
	attack1[(int)facingDirection::NE].PushBack({ 135,540,45,60 });
	attack1[(int)facingDirection::NE].PushBack({ 180,540,45,60 });
	// ------------
	attack1[(int)facingDirection::NW].PushBack({ 0,480,45,60 });
	attack1[(int)facingDirection::NW].PushBack({ 45,480,45,60 });
	attack1[(int)facingDirection::NW].PushBack({ 90,480,45,60 });
	attack1[(int)facingDirection::NW].PushBack({ 135,480,45,60 });
	attack1[(int)facingDirection::NW].PushBack({ 180,480,45,60 });
	attack1[(int)facingDirection::NW].PushBack({ 225,480,45,60 });
	attack1[(int)facingDirection::NW].PushBack({ 270,480,45,60 });
	attack1[(int)facingDirection::NW].PushBack({ 315,480,45,60 });
	attack1[(int)facingDirection::NW].PushBack({ 360,480,45,60 });
	attack1[(int)facingDirection::NW].PushBack({ 405,480,45,60 });
	attack1[(int)facingDirection::NW].PushBack({ 0,540,45,60 });
	attack1[(int)facingDirection::NW].PushBack({ 45,540,45,60 });
	attack1[(int)facingDirection::NW].PushBack({ 90,540,45,60 });
	attack1[(int)facingDirection::NW].PushBack({ 135,540,45,60 });
	attack1[(int)facingDirection::NW].PushBack({ 180,540,45,60 });
	// -------------------------------------------------------------
	for (int i = 0; i < (int)facingDirection::MAX; ++i)
	{
		attack1[i].speed = 50.f;
		attack1[i].loop = false;
	}

	// TELEPORT
	// -------------------------------------------------------------
	tpAnim[(int)facingDirection::E].PushBack({ 0,240,45,60 });
	tpAnim[(int)facingDirection::E].PushBack({ 45,240,45,60 });
	tpAnim[(int)facingDirection::E].PushBack({ 90,240,45,60 });
	tpAnim[(int)facingDirection::E].PushBack({ 135,240,45,60 });
	tpAnim[(int)facingDirection::E].PushBack({ 180,240,45,60 });
	tpAnim[(int)facingDirection::E].PushBack({ 225,240,45,60 });
	tpAnim[(int)facingDirection::E].PushBack({ 270,240,45,60 });
	tpAnim[(int)facingDirection::E].PushBack({ 315,240,45,60 });
	tpAnim[(int)facingDirection::E].PushBack({ 360,240,45,60 });
	tpAnim[(int)facingDirection::E].PushBack({ 405,240,45,60 });
	tpAnim[(int)facingDirection::E].PushBack({ 0,300,45,60 });
	tpAnim[(int)facingDirection::E].PushBack({ 45,300,45,60 });
	tpAnim[(int)facingDirection::E].PushBack({ 90,300,45,60 });
	tpAnim[(int)facingDirection::E].PushBack({ 135,300,45,60 });
	// ---------
	tpAnim[(int)facingDirection::W].PushBack({ 0,240,45,60 });
	tpAnim[(int)facingDirection::W].PushBack({ 45,240,45,60 });
	tpAnim[(int)facingDirection::W].PushBack({ 90,240,45,60 });
	tpAnim[(int)facingDirection::W].PushBack({ 135,240,45,60 });
	tpAnim[(int)facingDirection::W].PushBack({ 180,240,45,60 });
	tpAnim[(int)facingDirection::W].PushBack({ 225,240,45,60 });
	tpAnim[(int)facingDirection::W].PushBack({ 270,240,45,60 });
	tpAnim[(int)facingDirection::W].PushBack({ 315,240,45,60 });
	tpAnim[(int)facingDirection::W].PushBack({ 360,240,45,60 });
	tpAnim[(int)facingDirection::W].PushBack({ 405,240,45,60 });
	tpAnim[(int)facingDirection::W].PushBack({ 0,300,45,60 });
	tpAnim[(int)facingDirection::W].PushBack({ 45,300,45,60 });
	tpAnim[(int)facingDirection::W].PushBack({ 90,300,45,60 });
	tpAnim[(int)facingDirection::W].PushBack({ 135,300,45,60 });
	// --------------------------------------------------------------
	tpAnim[(int)facingDirection::N].PushBack({ 0,360,45,60 });
	tpAnim[(int)facingDirection::N].PushBack({ 45,360,45,60 });
	tpAnim[(int)facingDirection::N].PushBack({ 90,360,45,60 });
	tpAnim[(int)facingDirection::N].PushBack({ 135,360,45,60 });
	tpAnim[(int)facingDirection::N].PushBack({ 180,360,45,60 });
	tpAnim[(int)facingDirection::N].PushBack({ 225,360,45,60 });
	tpAnim[(int)facingDirection::N].PushBack({ 270,360,45,60 });
	tpAnim[(int)facingDirection::N].PushBack({ 315,360,45,60 });
	tpAnim[(int)facingDirection::N].PushBack({ 360,360,45,60 });
	tpAnim[(int)facingDirection::N].PushBack({ 405,360,45,60 });
	tpAnim[(int)facingDirection::N].PushBack({ 0,420,45,60 });
	tpAnim[(int)facingDirection::N].PushBack({ 45,420,45,60 });
	tpAnim[(int)facingDirection::N].PushBack({ 90,420,45,60 });
	tpAnim[(int)facingDirection::N].PushBack({ 135,420,45,60 });
	// -------------------------------------------------------------
	tpAnim[(int)facingDirection::S].PushBack({ 0,480,45,60 });
	tpAnim[(int)facingDirection::S].PushBack({ 45,480,45,60 });
	tpAnim[(int)facingDirection::S].PushBack({ 90,480,45,60 });
	tpAnim[(int)facingDirection::S].PushBack({ 135,480,45,60 });
	tpAnim[(int)facingDirection::S].PushBack({ 180,480,45,60 });
	tpAnim[(int)facingDirection::S].PushBack({ 225,480,45,60 });
	tpAnim[(int)facingDirection::S].PushBack({ 270,480,45,60 });
	tpAnim[(int)facingDirection::S].PushBack({ 315,480,45,60 });
	tpAnim[(int)facingDirection::S].PushBack({ 360,480,45,60 });
	tpAnim[(int)facingDirection::S].PushBack({ 405,480,45,60 });
	tpAnim[(int)facingDirection::S].PushBack({ 0,540,45,60 });
	tpAnim[(int)facingDirection::S].PushBack({ 45,540,45,60 });
	tpAnim[(int)facingDirection::S].PushBack({ 90,540,45,60 });
	tpAnim[(int)facingDirection::S].PushBack({ 135,540,45,60 });
	// --------------------------------------------------------------
	tpAnim[(int)facingDirection::NE].PushBack({ 0,120,45,60 });
	tpAnim[(int)facingDirection::NE].PushBack({ 45,120,45,60 });
	tpAnim[(int)facingDirection::NE].PushBack({ 90,120,45,60 });
	tpAnim[(int)facingDirection::NE].PushBack({ 135,120,45,60 });
	tpAnim[(int)facingDirection::NE].PushBack({ 180,120,45,60 });
	tpAnim[(int)facingDirection::NE].PushBack({ 225,120,45,60 });
	tpAnim[(int)facingDirection::NE].PushBack({ 270,120,45,60 });
	tpAnim[(int)facingDirection::NE].PushBack({ 315,120,45,60 });
	tpAnim[(int)facingDirection::NE].PushBack({ 360,120,45,60 });
	tpAnim[(int)facingDirection::NE].PushBack({ 405,120,45,60 });
	tpAnim[(int)facingDirection::NE].PushBack({ 0,180,45,60 });
	tpAnim[(int)facingDirection::NE].PushBack({ 45,180,45,60 });
	tpAnim[(int)facingDirection::NE].PushBack({ 90,180,45,60 });
	tpAnim[(int)facingDirection::NE].PushBack({ 135,180,45,60 });
	// ----------
	tpAnim[(int)facingDirection::NW].PushBack({ 0,120,45,60 });
	tpAnim[(int)facingDirection::NW].PushBack({ 45,120,45,60 });
	tpAnim[(int)facingDirection::NW].PushBack({ 90,120,45,60 });
	tpAnim[(int)facingDirection::NW].PushBack({ 135,120,45,60 });
	tpAnim[(int)facingDirection::NW].PushBack({ 180,120,45,60 });
	tpAnim[(int)facingDirection::NW].PushBack({ 225,120,45,60 });
	tpAnim[(int)facingDirection::NW].PushBack({ 270,120,45,60 });
	tpAnim[(int)facingDirection::NW].PushBack({ 315,120,45,60 });
	tpAnim[(int)facingDirection::NW].PushBack({ 360,120,45,60 });
	tpAnim[(int)facingDirection::NW].PushBack({ 405,120,45,60 });
	tpAnim[(int)facingDirection::NW].PushBack({ 0,180,45,60 });
	tpAnim[(int)facingDirection::NW].PushBack({ 45,180,45,60 });
	tpAnim[(int)facingDirection::NW].PushBack({ 90,180,45,60 });
	tpAnim[(int)facingDirection::NW].PushBack({ 135,180,45,60 });
	// --------------------------------------------------------------
	tpAnim[(int)facingDirection::SE].PushBack({ 0,0,45,60 });
	tpAnim[(int)facingDirection::SE].PushBack({ 45,0,45,60 });
	tpAnim[(int)facingDirection::SE].PushBack({ 90,0,45,60 });
	tpAnim[(int)facingDirection::SE].PushBack({ 135,0,45,60 });
	tpAnim[(int)facingDirection::SE].PushBack({ 180,0,45,60 });
	tpAnim[(int)facingDirection::SE].PushBack({ 225,0,45,60 });
	tpAnim[(int)facingDirection::SE].PushBack({ 270,0,45,60 });
	tpAnim[(int)facingDirection::SE].PushBack({ 315,0,45,60 });
	tpAnim[(int)facingDirection::SE].PushBack({ 360,0,45,60 });
	tpAnim[(int)facingDirection::SE].PushBack({ 405,0,45,60 });
	tpAnim[(int)facingDirection::SE].PushBack({ 0,60,45,60 });
	tpAnim[(int)facingDirection::SE].PushBack({ 45,60,45,60 });
	tpAnim[(int)facingDirection::SE].PushBack({ 90,60,45,60 });
	tpAnim[(int)facingDirection::SE].PushBack({ 135,60,45,60 });
	// ------------
	tpAnim[(int)facingDirection::SW].PushBack({ 0,0,45,60 });
	tpAnim[(int)facingDirection::SW].PushBack({ 45,0,45,60 });
	tpAnim[(int)facingDirection::SW].PushBack({ 90,0,45,60 });
	tpAnim[(int)facingDirection::SW].PushBack({ 135,0,45,60 });
	tpAnim[(int)facingDirection::SW].PushBack({ 180,0,45,60 });
	tpAnim[(int)facingDirection::SW].PushBack({ 225,0,45,60 });
	tpAnim[(int)facingDirection::SW].PushBack({ 270,0,45,60 });
	tpAnim[(int)facingDirection::SW].PushBack({ 315,0,45,60 });
	tpAnim[(int)facingDirection::SW].PushBack({ 360,0,45,60 });
	tpAnim[(int)facingDirection::SW].PushBack({ 405,0,45,60 });
	tpAnim[(int)facingDirection::SW].PushBack({ 0,60,45,60 });
	tpAnim[(int)facingDirection::SW].PushBack({ 45,60,45,60 });
	tpAnim[(int)facingDirection::SW].PushBack({ 90,60,45,60 });
	tpAnim[(int)facingDirection::SW].PushBack({ 135,60,45,60 });
	// -----------------------------------------------------------
	for (int i = 0; i < (int)facingDirection::MAX; ++i)
	{
		tpAnim[i].loop = false;
		tpAnim[i].speed = 45.f;
	}
	// ------------------------------------------------------------

	currentAnimation = &run[(int)facingDirection::SE];

	tpMaxDistance = 200.f;

	// cooldown data test - TODO: import for each character its base cooldown in ms from xml
	coolDownData.basic.cooldownTime = 0; // basic magic ball
	coolDownData.dodge.cooldownTime = 0;
	coolDownData.special1.cooldownTime = 1500; // TELEPORT
	coolDownData.special2.cooldownTime = 1000; // Medusa
	coolDownData.ultimate.cooldownTime = 3000; // death circle

	previousPos = position;
}

Ritz::~Ritz()
{
	App->tex->UnLoad(attack1Tex);
	App->tex->UnLoad(teleportTex);
	//App->tex->UnLoad();
}

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

	/*if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, this, "\0", 1000);*/
	// CHECK COMBAT STATE
	switch (combat_state)
	{
	case combatState::IDLE:

		break;
	case combatState::BASIC:
	{
		if (coolDownData.basic.timer.Read() > coolDownData.basic.cooldownTime && inputReady)
		{
			LOG("Launch BASIC"); // this basic requieres no input while casting
			if (attack1Tex != nullptr)
			{
				SetStopperState();
				App->audio->PlayFx(App->entityFactory->RitzBasic, 0);
			}

			// TODO: Adds a camera shaking based on "x" needed data from attack components
			// same applies when we receive damage
			App->camera2D->AddTrauma(10.0f / 100.f);
			App->input->DoGamePadRumble(0.3f, 100);

		}
		if (!inputReady)
		{
			// wait until the required casting frame, and we are done
			if ((int)currentAnimation->GetCurrentFloatFrame() >= 8)
			{
				// Launch attack
				App->entityFactory->CreateArrow(GetThrowingPos(), App->entityFactory->player->GetCrossHairPivotPos().Return_fPoint(),
					100, App->entityFactory->player->GetRitz(), PROJECTILE_TYPE::MAGIC_BOLT);

				// change combat state to idle
				combat_state = combatState::IDLE;
				App->camera2D->AddTrauma(40.0f / 100.f);
				App->input->DoGamePadRumble(0.4f, 100);
				// restart timer
				coolDownData.basic.timer.Start();
			}
		}
		break;
	}
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
	{
		static fPoint tpPos;

		if (coolDownData.special1.timer.Read() > coolDownData.special1.cooldownTime)
		{
			coolDownData.special1.timer.Start();

			if (teleportTex != nullptr)
			{
				SetStopperState();
				tpPos = GetTeleportPos();
				App->audio->PlayFx(App->entityFactory->RitzAbility1, 0);
			}

			// TODO: Adds a camera shaking based on "x" needed data from attack components
			// same applies when we receive damage
			App->camera2D->AddTrauma(20.0f / 100.f);
			App->input->DoGamePadRumble(0.4f, 100);

			// add gui clock

			if (!App->gui->spawnedClocks.Ritz.special1)
			{

				myUIClocks.special1 = App->gui->AddClock(App->gui->allclocksData.ability1.position, &App->gui->allclocksData.ability1.section, "special1", "Ritz", App->scene->inGamePanel);

				App->gui->spawnedClocks.Ritz.special1 = true;
			}
			else
			{
				myUIClocks.special1->Restart();
			}


		}
		if (!inputReady)
		{
			if ((int)currentAnimation->GetCurrentFloatFrame() >= currentAnimation->GetSize() - 1) // trick
			{
				LOG("last frame");
				position = tpPos;
				currentAnimation->Reset();
				combat_state = combatState::IDLE;
			}

		}
		break;
	}
	case combatState::SPECIAL2:
		if (coolDownData.special2.timer.Read() > coolDownData.special2.cooldownTime)
		{
			coolDownData.special2.timer.Start();
			App->audio->PlayFx(App->entityFactory->RitzAbility2, 0);
			App->entityFactory->CreateArrow(App->entityFactory->player->GetCrossHairPivotPos().Return_fPoint(), { 0,0 }, 0, this, PROJECTILE_TYPE::MEDUSA);

			// add gui clock

			if (!App->gui->spawnedClocks.Ritz.special2)
			{

				myUIClocks.special2 = App->gui->AddClock(App->gui->allclocksData.ability2.position, &App->gui->allclocksData.ability2.section, "special2", "Ritz", App->scene->inGamePanel);

				App->gui->spawnedClocks.Ritz.special2 = true;
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
			coolDownData.ultimate.timer.Start();
			App->audio->PlayFx(App->entityFactory->RitzUltimate, 0);

			App->entityFactory->CreateArrow(App->entityFactory->player->GetCrossHairPivotPos().Return_fPoint(), { 0,0 }, 0, this, PROJECTILE_TYPE::DEATH_CIRCLE);
			coolDownData.ultimate.timer.Start();


			if (!App->gui->spawnedClocks.Ritz.ulti)
			{

				myUIClocks.ulti = App->gui->AddClock(App->gui->allclocksData.ulti.position, &App->gui->allclocksData.ulti.section, "ulti", "Ritz", App->scene->inGamePanel);
			

				App->gui->spawnedClocks.Ritz.ulti = true;
			}
			else
			{
				myUIClocks.ulti->Restart();
			}

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
	if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_DOWN && App->scene->inGamePanel->enable && !App->scene->inventory->enable)
	{
		std::vector<LootEntity*>::iterator item = App->entityFactory->player->consumables.begin();
		if (item != App->entityFactory->player->consumables.end())
			App->entityFactory->player->ConsumConsumable(*item, this);
	}

	if (stat.size() != 0)
	{
		if (App->buff->DamageInTime(this))
		{
			App->buff->entitiesTimeDamage.remove(this);
		}
	}

	return true;
}

bool Ritz::SetStopperState() // disable user player input and sets the facing dir to crosshair angle, set textures and animations
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
	float angle = atan2f(targetPos.y, targetPos.x);
	pointingDir = GetPointingDir(angle);
	// updates renderflip if we need
	CheckRenderFlip();
	// links animation and textures
	switch (combat_state)
	{
	case combatState::IDLE:
		break;
	case combatState::BASIC:
	{
		currentAnimation = &attack1[pointingDir];
		entityTex = attack1Tex;
		break;
	}
	case combatState::DODGE: // common on all three player subclasses, this behaviour pertains to player Entity
		break;
	case combatState::SPECIAL1:
	{
		currentAnimation = &tpAnim[pointingDir];
		entityTex = teleportTex;
		break;
	}
	case combatState::SPECIAL2:
		break;
	case combatState::ULTIMATE:
		break;
	case combatState::MAX:
		break;
	default:
		ret = false;
		break;
	}

	return ret;
}

fPoint Ritz::GetTeleportPos()
{
	fPoint ret;
	// TODO: rework how to get the heading vector, this calcs are needed previously and do de same
	// checks the direction of aiming
	iPoint targetPos = App->entityFactory->player->GetCrossHairPivotPos();
	fPoint targetDirection;
	targetDirection.x = targetPos.x - GetPivotPos().x;
	targetDirection.y = targetPos.y - GetPivotPos().y;
	targetDirection.Normalize();
	
	ret = position + targetDirection * tpMaxDistance;

	return ret;
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
