#include "Ritz.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1BuffManager.h"
#include "j1AttackManager.h"
#include "j1EntityFactory.h"
#include "j1PathFinding.h"

Ritz::Ritz(int posX, int posY):PlayerEntity(posX,posY)
{
	character = characterName::RITZ;
	name.assign("Ritz");

	// TODO: import from xml
	// from parent, parent deletes
	spritesheet = App->tex->Load("textures/characters/ritz/Ritz_run_WIP2.png");
	dash_spritesheet = App->tex->Load("textures/characters/ritz/Ritz_dash_WIP.png");
	entityTex = spritesheet;
	// own
	attack1Tex = App->tex->Load("textures/characters/ritz/Ritz_basic_attack_2.png");
	teleportTex = App->tex->Load("textures/characters/ritz/Ritz_teleport_WIP.png");

	float idleAnimSpeed = 7.f;
	// IDLE
	idle[(int)facingDirection::N].PushBack({ 270,240,45,60 });
	idle[(int)facingDirection::N].PushBack({ 315,240,45,60 });
	idle[(int)facingDirection::N].PushBack({ 360,240,45,60 });
	idle[(int)facingDirection::N].PushBack({ 405,240,45,60 });
	idle[(int)facingDirection::N].speed = idleAnimSpeed;

	idle[(int)facingDirection::S].PushBack({ 270,0,45,60 });
	idle[(int)facingDirection::S].PushBack({ 315,0,45,60 });
	idle[(int)facingDirection::S].PushBack({ 360,0,45,60 });
	idle[(int)facingDirection::S].PushBack({ 405,0,45,60 });
	idle[(int)facingDirection::S].speed = idleAnimSpeed;

	idle[(int)facingDirection::E].PushBack({ 270,120,45,60 });
	idle[(int)facingDirection::E].PushBack({ 315,120,45,60 });
	idle[(int)facingDirection::E].PushBack({ 360,120,45,60 });
	idle[(int)facingDirection::E].PushBack({ 405,120,45,60 });
	idle[(int)facingDirection::E].speed = idleAnimSpeed;

	idle[(int)facingDirection::W].PushBack({ 270,120,45,60 });
	idle[(int)facingDirection::W].PushBack({ 315,120,45,60 });
	idle[(int)facingDirection::W].PushBack({ 360,120,45,60 });
	idle[(int)facingDirection::W].PushBack({ 405,120,45,60 });
	idle[(int)facingDirection::W].speed = idleAnimSpeed;

	idle[(int)facingDirection::SE].PushBack({ 270,60,45,60 });
	idle[(int)facingDirection::SE].PushBack({ 315,60,45,60 });
	idle[(int)facingDirection::SE].PushBack({ 360,60,45,60 });
	idle[(int)facingDirection::SE].PushBack({ 405,60,45,60 });
	idle[(int)facingDirection::SE].speed = idleAnimSpeed;

	idle[(int)facingDirection::SW].PushBack({ 270,60,45,60 });
	idle[(int)facingDirection::SW].PushBack({ 315,60,45,60 });
	idle[(int)facingDirection::SW].PushBack({ 360,60,45,60 });
	idle[(int)facingDirection::SW].PushBack({ 405,60,45,60 });
	idle[(int)facingDirection::SW].speed = idleAnimSpeed;

	idle[(int)facingDirection::NE].PushBack({ 270,180,45,60 });
	idle[(int)facingDirection::NE].PushBack({ 315,180,45,60 });
	idle[(int)facingDirection::NE].PushBack({ 360,180,45,60 });
	idle[(int)facingDirection::NE].PushBack({ 405,180,45,60 });
	idle[(int)facingDirection::NE].speed = idleAnimSpeed;

	idle[(int)facingDirection::NW].PushBack({ 270,180,45,60 });
	idle[(int)facingDirection::NW].PushBack({ 315,180,45,60 });
	idle[(int)facingDirection::NW].PushBack({ 360,180,45,60 });
	idle[(int)facingDirection::NW].PushBack({ 405,180,45,60 });
	idle[(int)facingDirection::NW].speed = idleAnimSpeed;

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
	// basic attack pivot offsets
	/*attackPivotOffset[(int)facingDirection::SE] = { 17.f, 50.f };
	attackPivotOffset[(int)facingDirection::SW] = { 29.f, 50.f };
	attackPivotOffset[(int)facingDirection::S] = { 22.f, 50.f };
	attackPivotOffset[(int)facingDirection::N] = { 22.f, 50.f };
	attackPivotOffset[(int)facingDirection::E] = { 9.f, 50.f };
	attackPivotOffset[(int)facingDirection::W] = { 37.f, 50.f };
	attackPivotOffset[(int)facingDirection::NE] = { 18.f, 51.f };
	attackPivotOffset[(int)facingDirection::NW] = { 28.f, 51.f };*/
	// post workaround/new method
	attackPivotOffset[(int)facingDirection::SE] = { 17.f, 49.f };
	attackPivotOffset[(int)facingDirection::SW] = { 29.f, 49.f };
	attackPivotOffset[(int)facingDirection::S] = { 23.f, 49.f };
	attackPivotOffset[(int)facingDirection::N] = { 23.f, 49.f };
	attackPivotOffset[(int)facingDirection::E] = { 11.f, 48.f };
	attackPivotOffset[(int)facingDirection::W] = { 35.f, 48.f };
	attackPivotOffset[(int)facingDirection::NE] = { 17.f, 49.f };
	attackPivotOffset[(int)facingDirection::NW] = { 29.f, 49.f };

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

	currentAnimation = &idle[(int)facingDirection::S];

	tpMaxDistance = 200.f;

	// cooldown data test - TODO: import for each character its base cooldown in ms from xml
	coolDownData.basic.cooldownTime = 0; // basic magic ball
	coolDownData.dodge.cooldownTime = 3000;
	coolDownData.special1.cooldownTime = 5500; // TELEPORT
	coolDownData.special2.cooldownTime = 2000; // Medusa
	coolDownData.ultimate.cooldownTime = 10000; // death circle

	// set timers
	SetCoolDownTimers();

	previousPos = position;
	mySpeedModular = sqrt((characterBaseSpeed.x + characterBaseSpeed.y) * (characterBaseSpeed.x + characterBaseSpeed.y));

}

Ritz::~Ritz()
{
	App->tex->UnLoad(attack1Tex);
	attack1Tex = nullptr;
	App->tex->UnLoad(teleportTex);
	teleportTex = nullptr;

	memset(attack1, 0, sizeof(attack1));
	memset(tpAnim, 0, sizeof(tpAnim));
	memset(dashPivotOffset, 0, sizeof(dashPivotOffset));


	if (!App->cleaningUp)
	{
		if (App->gui->spawnedClocks.Ritz.special1)
		{
			myUIClocks.special1->to_delete = true;
			App->gui->spawnedClocks.Ritz.special1 = false;
		}
		if (App->gui->spawnedClocks.Ritz.special2)
		{
			myUIClocks.special2->to_delete = true;
			App->gui->spawnedClocks.Ritz.special2 = false;
		}
		if (App->gui->spawnedClocks.Ritz.ulti)
		{
			myUIClocks.ulti->to_delete = true;
			App->gui->spawnedClocks.Ritz.ulti = false;
		}
	}





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

	// control linked projectile
	if (CheckMyLastProjectile())
		ControlMyOwnProjectile();

	if (!isParalize)
	{
		if (inputReady)
		{
			InputMovement(dt);
			if (App->scene->state != SceneState::LOBBY)
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

				if (combat_state == combatState::DODGE)
					coolDownData.dodge.timer.Start();
			}
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
				App->audio->PlayFx(App->scene->RitzBasic, 0);
			}

			// TODO: Adds a camera shaking based on "x" needed data from attack components
			// same applies when we receive damage
			/*App->camera2D->AddTrauma(10.0f / 100.f);
			App->input->DoGamePadRumble(0.3f, 100);*/

		}
		if (!inputReady)
		{
			// wait until the required casting frame, and we are done
			if ((int)currentAnimation->GetCurrentFloatFrame() >= 8)
			{
				// Launch attack
				App->entityFactory->CreateArrow(GetThrowingPos(), GetShotDirection(),
					100, App->entityFactory->player->GetRitz(), PROJECTILE_TYPE::MAGIC_BOLT);
				/*App->entityFactory->CreateArrow(GetThrowingPos(), GetShotDirection(),
					100, App->entityFactory->player->GetRitz(), PROJECTILE_TYPE::MAGIC_BOLT);*/

				// change combat state to idle
				combat_state = combatState::IDLE;
			/*	App->camera2D->AddTrauma(40.0f / 100.f);
				App->input->DoGamePadRumble(0.4f, 100);*/
				// restart timer
				coolDownData.basic.timer.Start();
			}

			// pivots
			//reposition pos
			transference_pivot = attackPivotOffset[pointingDir];
			transference_pivot -= pivot;
		}
		break;
	}
	case combatState::DODGE:
	{
		if (!inputReady)
		{
			//reposition pos
			transference_pivot = dashPivotOffset[pointingDir][(int)currentAnimation->GetCurrentFloatFrame()];
			transference_pivot -= pivot;
			position = App->camera2D->lerp(position, dashDestinationPos, dt * currentAnimation->speed);
		}

		if (!App->gui->spawnedClocks.Ritz.dodge)
		{

			myUIClocks.dodge = App->gui->AddClock(App->gui->allclocksData.dodge.position, &App->gui->allclocksData.dodge.section, "dodge", "Ritz", App->scene->uiRitz);
			App->gui->spawnedClocks.Ritz.dodge = true;
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
				App->audio->PlayFx(App->scene->RitzAbility1, 0);
			}

			// TODO: Adds a camera shaking based on "x" needed data from attack components
			// same applies when we receive damage
			App->camera2D->AddTrauma(20.0f / 100.f);
			App->input->DoGamePadRumble(0.4f, 100);

			// add gui clock

			if (!App->gui->spawnedClocks.Ritz.special1)
			{

				myUIClocks.special1 = App->gui->AddClock(App->gui->allclocksData.ability1.position, &App->gui->allclocksData.ability1.section, "special1", "Ritz", App->scene->uiRitz);
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
			//App->audio->PlayFx(App->entityFactory->RitzAbility2, 0);
			lastProjectile = dynamic_cast<Projectile*>(App->entityFactory->CreateArrow(GetThrowingPos(), GetShotDirection(), 70, this, PROJECTILE_TYPE::WHISPER_OF_ICE, 5000u));
			
			/*lastProjectile = dynamic_cast<Projectile*>(App->entityFactory->CreateArrow(GetThrowingPos(), GetShotDirection(),
				100, App->entityFactory->player->GetRitz(), PROJECTILE_TYPE::MAGIC_BOLT));*/
			// add gui clock

			if (!App->gui->spawnedClocks.Ritz.special2)
			{

				myUIClocks.special2 = App->gui->AddClock(App->gui->allclocksData.ability2.position, &App->gui->allclocksData.ability2.section, "special2", "Ritz", App->scene->uiRitz);

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
			App->audio->PlayFx(App->scene->RitzUltimate, 0);

			App->entityFactory->CreateArrow(App->entityFactory->player->GetCrossHairPivotPos().Return_fPoint(), { 0,0 }, 0, this, PROJECTILE_TYPE::DEATH_CIRCLE);
			coolDownData.ultimate.timer.Start();


			if (!App->gui->spawnedClocks.Ritz.ulti)
			{

				myUIClocks.ulti = App->gui->AddClock(App->gui->allclocksData.ulti.position, &App->gui->allclocksData.ulti.section, "ulti", "Ritz", App->scene->uiRitz);


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
	/*if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_DOWN && App->scene->inGamePanel->enable && !App->scene->inventory->enable)
	{
		std::vector<LootEntity*>::iterator item = App->entityFactory->player->consumables.begin();
		if (item != App->entityFactory->player->consumables.end())
			App->entityFactory->player->ConsumConsumable(*item, this);
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

bool Ritz::SetStopperState() // disable user player input and sets the facing dir to crosshair angle, set textures and animations
{
	bool ret = true;

	inputReady = false; // deactivate user input
	// checks the direction of aiming
	
	if (aiming)
	{
		iPoint targetDirection;
		fPoint targetPos;
		targetDirection = App->entityFactory->player->GetCrossHairPivotPos();
		targetPos.x = targetDirection.x - GetPivotPos().x;
		targetPos.y = targetDirection.y - GetPivotPos().y;
		targetPos.Normalize();
		// sets new pointing dir
		lastAxisMovAngle = atan2f(targetPos.y, targetPos.x);
	}

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
	
	// if destination is not walkable, go backwards till find a walkable pos and adds offset (between player pivot to collider radius)
	// TODO: improve this going backwards on tile cells, we need to "plot" a line to grid between current pos to destination pos and checks this cells
	// and only if the first destination is invalid
	fPoint directionVector = { cosf(lastAxisMovAngle), sinf(lastAxisMovAngle) };
	directionVector.Normalize();
	fPoint checker;
	int distMultiplier = (int)tpMaxDistance;

	for (; distMultiplier >= 0; --distMultiplier)
	{
		checker = GetPivotPos() + directionVector * distMultiplier;
		if (App->pathfinding->IsWalkable(App->map->WorldToMap(checker.x, checker.y)))
		{
			LOG("researched VALID");
			break;
		}
	}

	int playerVolumeOffset = 10;
	int maxOffset = distMultiplier - playerVolumeOffset;
	if (maxOffset <= 0) maxOffset = 0;
	ret = position + directionVector * maxOffset;

	return ret;
}

bool Ritz::CheckMyLastProjectile()
{
	bool ret = false;

	if (lastProjectile != nullptr)
	{
		if(lastProjectile->to_delete)
		{
			lastProjectile = nullptr;
		}
		else	
			ret = true;
	}

	return ret;
}

void Ritz::ControlMyOwnProjectile()
{
	Sint16 xAxis = App->input->GetControllerAxis(SDL_CONTROLLER_AXIS_RIGHTX);
	Sint16 yAxis = App->input->GetControllerAxis(SDL_CONTROLLER_AXIS_RIGHTY);

	if (xAxis > 0 || xAxis < 0 || yAxis > 0 || yAxis < 0)
	{
		 lastProjectile->SetNewDirection({ (float)xAxis, (float)yAxis });
	}

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


