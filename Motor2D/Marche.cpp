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
#include "j1ParticlesClassic.h"

Marche::Marche(int posX, int posY): PlayerEntity(posX,posY)
{
	character = characterName::MARCHE;
	name.assign("Marche");

	// TODO: import from xml
	spritesheet = App->tex->Load("textures/characters/marche/Marche_run_WIP.png");
	dash_spritesheet = App->tex->Load("textures/characters/marche/Marche_dash_WIP.png");

	whirlwindTex = App->tex->Load("textures/characters/marche/Marche_tornado_bodySpin_WIP.png");
	whirlwindFireTex = App->tex->Load("textures/spells/Marche_attacks/Marche_tornado_twisterSpinx2.png");
	basicAttackTex = App->tex->Load("textures/characters/marche/marche_blocking_basic_attack.png");
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

	// --------------------------------------------------------------------

	// WHIRLWIND : TODO, needs correct order of pushing
	int indexAnim[(int)facingDirection::MAX] = { 13,13,14,14,13,14,14,13 };
	for (int y = 0; y <= (int)facingDirection::MAX; ++y)
	{ // 60,65
		for (int x = 0; x < indexAnim[y]; ++x)
			whirlwindAnim[y].PushBack({ x * 60, y * 65, 60,65 });

		whirlwindAnim[y].speed = 20.f;
		whirlwindAnim[y].loop = false;
	}
	// --------------
	// whirlwind fire anim
	// all time loop
	whirlwindFireAnim.PushBack({ 0,0, 180,80 });
	whirlwindFireAnim.PushBack({ 180,0, 180,80 });
	whirlwindFireAnim.PushBack({ 360,0, 180,80 });
	whirlwindFireAnim.PushBack({ 0,80, 180,80 });
	//whirlwindFireAnim.PushBack({ 180,80, 180,80 });
	//whirlwindFireAnim.PushBack({ 360,80, 180,80 });
	//whirlwindFireAnim.PushBack({ 0,160, 180,80 });
	//whirlwindFireAnim.PushBack({ 180,160, 180,80 });
	//whirlwindFireAnim.PushBack({ 360,160, 180,80 });
	//whirlwindFireAnim.PushBack({ 0,240, 180,80 });
	whirlwindFireAnim.speed = 10.f;
	whirlwindFireAnim.loop = true;
	// decay
	//whirlwindFireAnim.PushBack({ 180,240, 180,80 });
	//whirlwindFireAnim.PushBack({ 360,240, 180,80 });
	//whirlwindFireAnim.PushBack({ 0,320, 180,80 });
	//whirlwindFireAnim.PushBack({ 180,320, 180,80 });

	whirlwindFireAnimExitLoop.PushBack({ 180,240, 180,80 });
	whirlwindFireAnimExitLoop.PushBack({ 360,240, 180,80 });
	whirlwindFireAnimExitLoop.PushBack({ 0,320, 180,80 });
	whirlwindFireAnimExitLoop.PushBack({ 180,320, 180,80 });
	whirlwindFireAnimExitLoop.speed = 10.f;
	whirlwindFireAnimExitLoop.loop = false;


	// ------------------------------------------------------------------

	// BASIC ATTACK  -------
	float attackAnimSpeed = 20.f;
	// ---------------------------
	basicAttackAnim[(int)facingDirection::E][0].PushBack({ 0,150,60,75 });
	basicAttackAnim[(int)facingDirection::E][0].PushBack({ 60,150,60,75 });
	basicAttackAnim[(int)facingDirection::E][0].PushBack({ 120,150, 60,75 });
	basicAttackAnim[(int)facingDirection::E][0].PushBack({ 180,150, 60,75 }); // first hit, 4 frame
	basicAttackAnim[(int)facingDirection::E][0].PushBack({ 240,150, 60,75 });
	basicAttackAnim[(int)facingDirection::E][0].PushBack({ 300,150, 60,75 });
	basicAttackAnim[(int)facingDirection::E][0].PushBack({ 360,150, 60,75 });
	basicAttackAnim[(int)facingDirection::E][0].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::E][0].loop = false;
	// -----
	basicAttackAnim[(int)facingDirection::E][1].PushBack({ 360,150, 60,75 });
	basicAttackAnim[(int)facingDirection::E][1].PushBack({ 420,150, 60,75 }); // HIT2, 2 frame
	basicAttackAnim[(int)facingDirection::E][1].PushBack({ 480,150, 60,75 });
	basicAttackAnim[(int)facingDirection::E][1].PushBack({ 540,150, 60,75 });
	basicAttackAnim[(int)facingDirection::E][1].PushBack({ 600,150, 60,75 });
	basicAttackAnim[(int)facingDirection::E][1].PushBack({ 660,150, 60,75 });
	basicAttackAnim[(int)facingDirection::E][1].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::E][1].loop = false;
	// ------
	basicAttackAnim[(int)facingDirection::W][0].PushBack({ 0,150,60,75 });
	basicAttackAnim[(int)facingDirection::W][0].PushBack({ 60,150,60,75 });
	basicAttackAnim[(int)facingDirection::W][0].PushBack({ 120,150, 60,75 });
	basicAttackAnim[(int)facingDirection::W][0].PushBack({ 180,150, 60,75 }); // first hit, 4 frame
	basicAttackAnim[(int)facingDirection::W][0].PushBack({ 240,150, 60,75 });
	basicAttackAnim[(int)facingDirection::W][0].PushBack({ 300,150, 60,75 });
	basicAttackAnim[(int)facingDirection::W][0].PushBack({ 360,150, 60,75 });
	basicAttackAnim[(int)facingDirection::W][0].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::W][0].loop = false;
	// -----							  
	basicAttackAnim[(int)facingDirection::W][1].PushBack({ 360,150, 60,75 });
	basicAttackAnim[(int)facingDirection::W][1].PushBack({ 420,150, 60,75 }); // HIT2, 2 frame
	basicAttackAnim[(int)facingDirection::W][1].PushBack({ 480,150, 60,75 });
	basicAttackAnim[(int)facingDirection::W][1].PushBack({ 540,150, 60,75 });
	basicAttackAnim[(int)facingDirection::W][1].PushBack({ 600,150, 60,75 });
	basicAttackAnim[(int)facingDirection::W][1].PushBack({ 660,150, 60,75 });
	basicAttackAnim[(int)facingDirection::W][1].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::W][1].loop = false;
	// ---------------------------
	basicAttackAnim[(int)facingDirection::S][0].PushBack({ 0,75, 60,75 });
	basicAttackAnim[(int)facingDirection::S][0].PushBack({ 60,75, 60,75 });
	basicAttackAnim[(int)facingDirection::S][0].PushBack({ 120,75, 60,75 });
	basicAttackAnim[(int)facingDirection::S][0].PushBack({ 180,75, 60,75 }); // first hit, 4 frame
	basicAttackAnim[(int)facingDirection::S][0].PushBack({ 240,75, 60,75 });
	basicAttackAnim[(int)facingDirection::S][0].PushBack({ 300,75, 60,75 });
	basicAttackAnim[(int)facingDirection::S][0].PushBack({ 360,75, 60,75 });
	basicAttackAnim[(int)facingDirection::S][0].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::S][0].loop = false;
	// -----
	basicAttackAnim[(int)facingDirection::S][1].PushBack({ 360,75, 60,75 });
	basicAttackAnim[(int)facingDirection::S][1].PushBack({ 420,75, 60,75 }); // HIT2, 2 frame
	basicAttackAnim[(int)facingDirection::S][1].PushBack({ 480,75, 60,75 });
	basicAttackAnim[(int)facingDirection::S][1].PushBack({ 540,75, 60,75 });
	basicAttackAnim[(int)facingDirection::S][1].PushBack({ 600,75, 60,75 });
	basicAttackAnim[(int)facingDirection::S][1].PushBack({ 660,75, 60,75 });
	basicAttackAnim[(int)facingDirection::S][1].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::S][1].loop = false;
	// ---------------------------
	basicAttackAnim[(int)facingDirection::N][0].PushBack({ 0, 300, 60, 75 });
	basicAttackAnim[(int)facingDirection::N][0].PushBack({ 60, 300, 60, 75 });
	basicAttackAnim[(int)facingDirection::N][0].PushBack({ 120, 300, 60, 75 });
	basicAttackAnim[(int)facingDirection::N][0].PushBack({ 180, 300, 60, 75 }); // first hit, 4 frame
	basicAttackAnim[(int)facingDirection::N][0].PushBack({ 240, 300, 60, 75 });
	basicAttackAnim[(int)facingDirection::N][0].PushBack({ 300, 300, 60, 75 });
	basicAttackAnim[(int)facingDirection::N][0].PushBack({ 360, 300, 60, 75 });
	basicAttackAnim[(int)facingDirection::N][0].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::N][0].loop = false;
	// -----
	basicAttackAnim[(int)facingDirection::N][1].PushBack({ 360,300, 60,75 });
	basicAttackAnim[(int)facingDirection::N][1].PushBack({ 420,300, 60,75 }); // HIT2, 2 frame
	basicAttackAnim[(int)facingDirection::N][1].PushBack({ 480,300, 60,75 });
	basicAttackAnim[(int)facingDirection::N][1].PushBack({ 540,300, 60,75 });
	basicAttackAnim[(int)facingDirection::N][1].PushBack({ 600,300, 60,75 });
	basicAttackAnim[(int)facingDirection::N][1].PushBack({ 660,300, 60,75 });
	basicAttackAnim[(int)facingDirection::N][1].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::N][1].loop = false;
	// -----------------------------
	basicAttackAnim[(int)facingDirection::SE][0].PushBack({ 0,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SE][0].PushBack({ 60,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SE][0].PushBack({ 120,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SE][0].PushBack({ 180,0, 60,75 }); // first hit, 4 frame
	basicAttackAnim[(int)facingDirection::SE][0].PushBack({ 240,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SE][0].PushBack({ 300,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SE][0].PushBack({ 360,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SE][0].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::SE][0].loop = false;
	// -----
	basicAttackAnim[(int)facingDirection::SE][1].PushBack({ 360,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SE][1].PushBack({ 420,0, 60,75 }); // HIT2, 2 frame
	basicAttackAnim[(int)facingDirection::SE][1].PushBack({ 480,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SE][1].PushBack({ 540,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SE][1].PushBack({ 600,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SE][1].PushBack({ 660,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SE][1].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::SE][1].loop = false;
	// --------------
	basicAttackAnim[(int)facingDirection::SW][0].PushBack({ 0,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SW][0].PushBack({ 60,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SW][0].PushBack({ 120,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SW][0].PushBack({ 180,0, 60,75 }); // first hit, 4 frame
	basicAttackAnim[(int)facingDirection::SW][0].PushBack({ 240,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SW][0].PushBack({ 300,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SW][0].PushBack({ 360,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SW][0].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::SW][0].loop = false;
	// -----							  
	basicAttackAnim[(int)facingDirection::SW][1].PushBack({ 360,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SW][1].PushBack({ 420,0, 60,75 }); // HIT2, 2 frame
	basicAttackAnim[(int)facingDirection::SW][1].PushBack({ 480,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SW][1].PushBack({ 540,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SW][1].PushBack({ 600,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SW][1].PushBack({ 660,0, 60,75 });
	basicAttackAnim[(int)facingDirection::SW][1].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::SW][1].loop = false;
	// ------------------------------------------
	basicAttackAnim[(int)facingDirection::NE][0].PushBack({ 0, 225,60,75 });
	basicAttackAnim[(int)facingDirection::NE][0].PushBack({ 60, 225,60,75 });
	basicAttackAnim[(int)facingDirection::NE][0].PushBack({ 120, 225,60,75 });
	basicAttackAnim[(int)facingDirection::NE][0].PushBack({ 180, 225,60,75 });
	basicAttackAnim[(int)facingDirection::NE][0].PushBack({ 240, 225,60,75 });
	basicAttackAnim[(int)facingDirection::NE][0].PushBack({ 300, 225,60,75 });
	basicAttackAnim[(int)facingDirection::NE][0].PushBack({ 360, 225,60,75 });
	basicAttackAnim[(int)facingDirection::NE][0].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::NE][0].loop = false;
	// -----
	basicAttackAnim[(int)facingDirection::NE][1].PushBack({ 360,225, 60,75 });
	basicAttackAnim[(int)facingDirection::NE][1].PushBack({ 420,225, 60,75 }); // HIT2, 2 frame
	basicAttackAnim[(int)facingDirection::NE][1].PushBack({ 480,225, 60,75 });
	basicAttackAnim[(int)facingDirection::NE][1].PushBack({ 540,225, 60,75 });
	basicAttackAnim[(int)facingDirection::NE][1].PushBack({ 600,225, 60,75 });
	basicAttackAnim[(int)facingDirection::NE][1].PushBack({ 660,225, 60,75 });
	basicAttackAnim[(int)facingDirection::NE][1].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::NE][1].loop = false;
	// ---------------
	basicAttackAnim[(int)facingDirection::NW][0].PushBack({ 0, 225,60,75 });
	basicAttackAnim[(int)facingDirection::NW][0].PushBack({ 60, 225,60,75 });
	basicAttackAnim[(int)facingDirection::NW][0].PushBack({ 120, 225,60,75 });
	basicAttackAnim[(int)facingDirection::NW][0].PushBack({ 180, 225,60,75 });
	basicAttackAnim[(int)facingDirection::NW][0].PushBack({ 240, 225,60,75 });
	basicAttackAnim[(int)facingDirection::NW][0].PushBack({ 300, 225,60,75 });
	basicAttackAnim[(int)facingDirection::NW][0].PushBack({ 360, 225,60,75 });
	basicAttackAnim[(int)facingDirection::NW][0].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::NW][0].loop = false;
	// -----							  
	basicAttackAnim[(int)facingDirection::NW][1].PushBack({ 360,225, 60,75 });
	basicAttackAnim[(int)facingDirection::NW][1].PushBack({ 420,225, 60,75 }); // HIT2, 2 frame
	basicAttackAnim[(int)facingDirection::NW][1].PushBack({ 480,225, 60,75 });
	basicAttackAnim[(int)facingDirection::NW][1].PushBack({ 540,225, 60,75 });
	basicAttackAnim[(int)facingDirection::NW][1].PushBack({ 600,225, 60,75 });
	basicAttackAnim[(int)facingDirection::NW][1].PushBack({ 660,225, 60,75 });
	basicAttackAnim[(int)facingDirection::NW][1].speed = attackAnimSpeed;
	basicAttackAnim[(int)facingDirection::NW][1].loop = false;

	// ------------------------------------------------------------------


	currentAnimation = &run[(int)facingDirection::W];

	// cooldown data test - TODO: import for each character its base cooldown in ms from xml
	coolDownData.basic.cooldownTime = 0;
	coolDownData.dodge.cooldownTime = 500; // DODGE "COOLDOWN" is limited to finish its "translation" and animation
	coolDownData.special1.cooldownTime = 500;
	coolDownData.special2.cooldownTime = 1500;
	coolDownData.ultimate.cooldownTime = 1;
	// starts timers
	coolDownData.basic.timer.Start();
	coolDownData.dodge.timer.Start();
	coolDownData.special1.timer.Start();
	coolDownData.special2.timer.Start();
	coolDownData.ultimate.timer.Start();

	previousPos = position;


	// better speed 
	characterBaseSpeed.x /= 1.3f; 
	characterBaseSpeed.y /= 1.3f;

	//
	//previousFrame = 1; // fake previousFrame to enter on first anim state

	dashMaxDistance = 64.f;
	whirlwindMaxCastingTime = 3500;
	whirlwindCadence = 150;
	maxTornadoDistanceMultiplier = 30.f; // multiplicate the direction vector
	basicAttackPulsationMaxTime = 600; // the time between the player can or not encadenate the second part of the basic attack animation
											// second hit is more powerfull too
	baseDamage = 40; // base damage for basic attack / other attacks that need the basic dmg value
	ultiTime = 5;
}

Marche::~Marche()
{
	App->tex->UnLoad(whirlwindTex);
	App->tex->UnLoad(whirlwindFireTex);
	App->tex->UnLoad(basicAttackTex);


	for (std::list<Buff*>::iterator iter = ulti_buffs.begin(); iter != ulti_buffs.end(); ++iter)
	{
		ulti_buffs.remove(*iter);
	}
	ulti_buffs.clear();
}

bool Marche::Start()
{
	SetPivot(23, 48);
	size.create(45,60);

	combat_state = combatState::IDLE;

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
		
	

	// CHECK COMBAT STATE
	switch (combat_state)
	{
	case combatState::IDLE:
		
		break;
	case combatState::BASIC:
		
	{
		static int attackType;

		if (coolDownData.basic.timer.Read() > coolDownData.basic.cooldownTime && inputReady)
		{
			LOG("Launch BASIC");
			inputReady = false;
			coolDownData.basic.timer.Start();

			if (basicAttackPulsationTimer.Read() > basicAttackPulsationMaxTime)
			{
				basicAttackPulsationTimer.Start();
				App->audio->PlayFx(App->entityFactory->marcheBasic, 0);

				LOG("ATTACK1");
				attackType = 0;
			}
			else
			{
				App->audio->PlayFx(App->entityFactory->marcheBasic2, 0);

				LOG("ATTACK2");
				attackType = 1;
			}
			currentAnimation = &basicAttackAnim[pointingDir][attackType];
			entityTex = basicAttackTex;

			//App->audio->PlayFx(App->entityFactory->marcheBasic, 0);

			//App->attackManager->AddPropagationAttack(this, App->entityFactory->player->GetCrossHairSubtile(), propagationType::BFS, 10, 7, 40);
			// TODO: Adds a camera shaking based on "x" needed data from attack components
			// same applies when we receive damage
			App->camera2D->AddTrauma(10.0f / 100.f);
			App->input->DoGamePadRumble(0.3f, 100);

		}

		if (!inputReady)
		{
			// check type of attack and hit frame of each and instantiate the feel
			if (attackType == 0)
			{
				if ((int)currentAnimation->GetCurrentFloatFrame() >= 4.f) // hit frame of 0 type
				{
					App->attackManager->AddPropagationAttack(
						(j1Entity*)this,
						App->entityFactory->player->GetSubtilePos(),
						propagationType::BFS,
						damageType::DIRECT,
						ELEMENTAL_TYPE::NO_ELEMENT,
						baseDamage,
						2, 30, false);

					// FINAL RUMBLE AND SHAKE
					App->camera2D->AddTrauma(0.45f);
					App->input->DoGamePadRumble(0.4f, 300);

					// force change state
					combat_state = combatState::IDLE;
				}
			}
			else
			{
				App->attackManager->AddPropagationAttack(
					(j1Entity*)this,
					App->entityFactory->player->GetSubtilePos(),
					propagationType::BFS,
					damageType::DIRECT,
					ELEMENTAL_TYPE::NO_ELEMENT,
					baseDamage * 2, // TODO: PASS parameter basedamage from attackdata from attack manager to float
					3, 20, false);

				// FINAL RUMBLE AND SHAKE
				App->camera2D->AddTrauma(0.65f);
				App->input->DoGamePadRumble(0.8f, 300);

				// force change state
				combat_state = combatState::IDLE;
			}

		}
		break;
	}
	case combatState::DODGE:

		if (coolDownData.dodge.timer.Read() > coolDownData.basic.cooldownTime)
		{
			coolDownData.dodge.timer.Start();
				
		}
		if (!inputReady)
		{
			//reposition pos
			transference_pivot = dashPivotOffset[pointingDir][(int)currentAnimation->GetCurrentFloatFrame()];
			transference_pivot -= pivot;
			position = App->camera2D->lerp(position, dashDestinationPos, dt * currentAnimation->speed);
		}
			break;
	case combatState::SPECIAL1:
		if (coolDownData.special1.timer.Read() > coolDownData.special1.cooldownTime)
		{
			coolDownData.special1.timer.Start();
			//App->audio->PlayFx(App->entityFactory->marcheAbility1, 0);
			
			App->entityFactory->CreateArrow(App->entityFactory->player->GetSelectedCharacterEntity()->GetPivotPos(), { 0,0 }, 0, this, PROJECTILE_TYPE::EARTH_SHAKER);
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
		static int startFacingDirIndex;

		if (coolDownData.special2.timer.Read() > coolDownData.special2.cooldownTime)
		{
			inputReady = false;
			currentAnimation = &whirlwindAnim[pointingDir];
			entityTex = whirlwindTex;
			startFacingDirIndex = pointingDir;
			whirlwindFirePos = position;

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

			if (!inputReady)
			{
				//checks animation state
				if ((int)currentAnimation->GetCurrentFloatFrame() >= 4 && !doingWhirlwind)
				{
					whirlwindTotalCastingTimer.Start();
					doingWhirlwind = true;
				}
				if (doingWhirlwind)
				{
					if (whirlwindTotalCastingTimer.Read() > whirlwindMaxCastingTime)
					{
						// hability timer restart
						coolDownData.special2.timer.Start();
						// changes animation to real pointing direction, and decelerates
						float fft = currentAnimation->GetCurrentFloatFrame(); // float frame temp
						// resets current whirlwind froming anim
						currentAnimation->Reset();
						// changes to final target direction
						currentAnimation = &whirlwindAnim[GetPointingDir(lastAxisMovAngle)];
						currentAnimation->SetCurrentFrame(fft);
						combat_state = combatState::IDLE;
						doingWhirlwind = false;
						// DO LAST ATTACK -----
						App->attackManager->AddPropagationAttack(
							(j1Entity*)this,
							App->entityFactory->player->GetSubtilePos(),
							propagationType::BFS,
							damageType::DIRECT,
							ELEMENTAL_TYPE::FIRE_ELEMENT,
							whirlwindFinalBoomDMG,
							5, 30, true);

						// FINAL RUMBLE AND SHAKE
						App->camera2D->AddTrauma(0.7f);
						App->input->DoGamePadRumble(0.7f, 300);

						// instantiate final explosion particles
						int maxParticles = 13; // TODO: RANDOM between x/y
						float maxDistance = 60.f;
						float maxDelayTime = 700.f;
						for (int i = 0; i < maxParticles; ++i)
						{
							fPoint instaPos;
							instaPos.x = GetPivotPos().x + (maxDistance * App->camera2D->GetFloatNegOneToOne());
							instaPos.y = GetPivotPos().y + (maxDistance * App->camera2D->GetFloatNegOneToOne());
							App->particles->AddParticle(
								App->particles->burn01,
								instaPos.x,
								instaPos.y,
								{ 0,0 },
								App->camera2D->GetFloatNegOneToOne() * maxDelayTime,
								SDL_FLIP_NONE, 0, 48, 24, 0.65F);
						}


					}
					else
					{
						// stores current frame, direction etc
						float currentFFrame = currentAnimation->GetCurrentFloatFrame();
						// checks input, needed to move on whirlwinding
						InputMovement(dt);
						// rechange current animation and state
						combat_state = combatState::SPECIAL2;
						currentAnimation = &whirlwindAnim[startFacingDirIndex];
						currentAnimation->SetCurrentFrame(currentFFrame);
						// checks animation to loop between casting time
						if ((int)currentAnimation->GetCurrentFloatFrame() >= 9)
						{
							currentAnimation->SetCurrentFrame(5.f);
						}

						// do attack
						if (whirlwindAttackTimer.Read() > whirlwindCadence)
						{
							whirlwindAttackTimer.Start();
							App->attackManager->AddPropagationAttack(
								(j1Entity*)this,
								App->entityFactory->player->GetSubtilePos(),
								propagationType::BFS,
								damageType::DIRECT,
								ELEMENTAL_TYPE::FIRE_ELEMENT,
								whirlwindWhileLoopDMG,
								3, 30, true);

							// camera shake and rumble
							App->camera2D->AddTrauma(0.3f);
							App->input->DoGamePadRumble(0.4f, whirlwindCadence* 0.9f);

							// instantiate or not a random tornado
							if (rand() % 2 == 0)
							{
								LOG("tornado extra");
								// get offset position
								fPoint offset;
								offset.x = GetPivotPos().x + (maxTornadoDistanceMultiplier * App->camera2D->GetFloatNegOneToOne());
								offset.y = GetPivotPos().y + (maxTornadoDistanceMultiplier * App->camera2D->GetFloatNegOneToOne());

								App->entityFactory->CreateArrow(GetPivotPos(), offset, 75, this, PROJECTILE_TYPE::TORNADO);

							}
							else
							{
								LOG("NOT TORNADO");
							}
						}
						
					}

				}

			}
		}
		break;

	case combatState::ULTIMATE:
	
		if (coolDownData.ultimate.timer.Read() > coolDownData.ultimate.cooldownTime)
		{
			App->audio->PlayFx(App->entityFactory->marcheUltimateScream, 0);

			/*App->audio->PlayFx(App->entityFactory->marcheUltimateScream, 0);
			LOG("Launch ULTIMATE");

			App->attackManager->AddPropagationAttack(this, App->entityFactory->player->GetCrossHairSubtile(), propagationType::BFS, 10, 20, 40);
			App->camera2D->AddTrauma(70.0f / 100.f);
			App->input->DoGamePadRumble(0.7f, 400);*/
			if (!App->gui->spawnedClocks.Marche.ulti)
			{
				
				myUIClocks.ulti = App->gui->AddClock(App->gui->allclocksData.ulti.position, &App->gui->allclocksData.ulti.section, "ulti", "Marche", App->scene->inGamePanel);

				App->gui->spawnedClocks.Marche.ulti = true;
			}
			else
			{
				myUIClocks.ulti->Restart();
			}
			activeUlti = true;

			coolDownData.ultimate.timer.Start();

		}
		break;
	
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

	if (App->input->GetKey(SDL_SCANCODE_S) == 1)
	{
		App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, App->entityFactory->player->GetMarche(), "\0", 100);
	}

	if (App->input->GetKey(SDL_SCANCODE_B) == 1)
	{
		App->buff->CreatePoision(this, this, 5, 5, "a", false);
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

	
	return true;
}

void Marche::UlitMarche()
{
	if (!startUlti)
	{
		ulti_buffs.push_back(App->buff->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, App->entityFactory->player->GetMarche(), "stat", 1));
		ulti_buffs.push_back(App->buff->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, App->entityFactory->player->GetShara(), "stat", 1));
		ulti_buffs.push_back(App->buff->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, App->entityFactory->player->GetRitz(),"stat", 1));
		ulti_buffs.push_back(App->buff->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, App->entityFactory->player->GetMarche(), "stat", 1));
		ulti_buffs.push_back(App->buff->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, App->entityFactory->player->GetShara(), "stat", 1));
		ulti_buffs.push_back(App->buff->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, App->entityFactory->player->GetRitz(), "stat", 1));
		ulti_buffs.push_back(App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, App->entityFactory->player->GetMarche(), "stat", 0.8f));
		ulti_buffs.push_back(App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, App->entityFactory->player->GetShara(), "stat", 0.8f));
		ulti_buffs.push_back(App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, App->entityFactory->player->GetRitz(), "stat", 0.8f));
		ulti_buffs.push_back(App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::NO_ELEMENT, ROL::HEALTH, App->entityFactory->player->GetRitz(), "stat", 100));
		
		ultiTimer.Start();
		startUlti = true;
	}
	if (ultiTime < ultiTimer.ReadSec() && startUlti)
	{
		for (std::list<Buff*>::iterator iter = ulti_buffs.begin(); iter != ulti_buffs.end();++iter)
		{
			App->buff->DeleteBuff(*iter);

			ulti_buffs.remove(*iter);
		}
		ulti_buffs.clear();

		activeUlti = false;
		startUlti = false;
	}


}

//bool Marche::CleanUp()
//{
//	return true;
//}

bool Marche::PostUpdate()
{
	// draw fire whirlwind if we need
	if (doingWhirlwind)
	{
		// checks next exit loop anim and resets
		if (whirlwindFireAnimExitLoop.Finished())
		{
			whirlwindFireAnimExitLoop.Reset();
		}
		whirlwindFirePos = App->camera2D->lerp(whirlwindFirePos, position, App->GetDt() * 10.5f);
		// TODO: improve hardcoded positions to place anim on correct pos
		App->render->Blit(whirlwindFireTex, whirlwindFirePos.x - 63, whirlwindFirePos.y - 10, &whirlwindFireAnim.GetCurrentFrame());
		LOG("blitpos:%f,%f", whirlwindFirePos.x, whirlwindFirePos.y);
	}
	else
	{
		// checks 
		if (!whirlwindFireAnimExitLoop.Finished())
		{
			App->render->Blit(whirlwindFireTex, position.x - 63, position.y - 10, &whirlwindFireAnimExitLoop.GetCurrentFrame());
		}
	}

	return true;
}