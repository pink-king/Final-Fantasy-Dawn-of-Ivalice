#include "Boss_Flower.h"
#include "j1Render.h"
#include "j1ModuleCamera2D.h"
#include "j1EntityFactory.h"
#include "j1PathFinding.h"

FlowerBossEntity::FlowerBossEntity(iPoint position) : j1Entity(FLOWERBOSS, position.x, position.y, "Flower Boss")
{
	boss_spritesheet = App->tex->Load("textures/enemies/enemyFlower_Boss.png");
	debugSubtileTex = App->tex->Load("maps/tile_32x32.png");

	// animations --------
	float animSpeed = 10.f;
	float animSpeedIdle = 5.f;
	float animAttackSpeed = 10.f;
	float animJumpSpeed = 40.0f;
	// idle
	idleAnim[(int)facingDirection::S].PushBack({ 0,320,64,64 });
	idleAnim[(int)facingDirection::S].PushBack({ 64,320,64,64 });
	idleAnim[(int)facingDirection::S].speed = animSpeedIdle;

	idleAnim[(int)facingDirection::SW].PushBack({ 0,384,64,64 });
	idleAnim[(int)facingDirection::SW].PushBack({ 64,384,64,64 });
	idleAnim[(int)facingDirection::SW].speed = animSpeedIdle;

	idleAnim[(int)facingDirection::SE].PushBack({ 0,384,64,64 }); // same as SW but flipped
	idleAnim[(int)facingDirection::SE].PushBack({ 64,384,64,64 });
	idleAnim[(int)facingDirection::SE].speed = animSpeedIdle;

	idleAnim[(int)facingDirection::W].PushBack({ 0,448,64,64 });
	idleAnim[(int)facingDirection::W].PushBack({ 64,448,64,64 });
	idleAnim[(int)facingDirection::W].speed = animSpeedIdle;

	idleAnim[(int)facingDirection::E].PushBack({ 0,448,64,64 }); // same as W but flipped
	idleAnim[(int)facingDirection::E].PushBack({ 64,448,64,64 });
	idleAnim[(int)facingDirection::E].speed = animSpeedIdle;

	idleAnim[(int)facingDirection::NW].PushBack({ 0,512,64,64 });
	idleAnim[(int)facingDirection::NW].PushBack({ 64,512,64,64 });
	idleAnim[(int)facingDirection::NW].speed = animSpeedIdle;

	idleAnim[(int)facingDirection::NE].PushBack({ 0,512,64,64 }); // same as NW but flipped
	idleAnim[(int)facingDirection::NE].PushBack({ 64,512,64,64 });
	idleAnim[(int)facingDirection::NE].speed = animSpeedIdle;

	idleAnim[(int)facingDirection::N].PushBack({ 0,576,64,64 });
	idleAnim[(int)facingDirection::N].PushBack({ 64,576,64,64 });
	idleAnim[(int)facingDirection::N].speed = animSpeedIdle;

	// attack
	attackAnim[(int)facingDirection::S].PushBack({ 0,0,64,64 });
	attackAnim[(int)facingDirection::S].PushBack({ 64,0,64,64 });
	attackAnim[(int)facingDirection::S].PushBack({ 128,0,64,64 });
	attackAnim[(int)facingDirection::S].PushBack({ 192,0,64,64 });
	attackAnim[(int)facingDirection::S].speed = animAttackSpeed;
	attackAnim[(int)facingDirection::S].loop = false;

	attackAnim[(int)facingDirection::SW].PushBack({ 0,64,64,64 });
	attackAnim[(int)facingDirection::SW].PushBack({ 64,64,64,64 });
	attackAnim[(int)facingDirection::SW].PushBack({ 128,64,64,64 });
	attackAnim[(int)facingDirection::SW].PushBack({ 192,64,64,64 });
	attackAnim[(int)facingDirection::SW].speed = animAttackSpeed;
	attackAnim[(int)facingDirection::SW].loop = false;

	attackAnim[(int)facingDirection::SE].PushBack({ 0,64,64,64 }); // same as SW but flipped
	attackAnim[(int)facingDirection::SE].PushBack({ 64,64,64,64 });
	attackAnim[(int)facingDirection::SE].PushBack({ 128,64,64,64 });
	attackAnim[(int)facingDirection::SE].PushBack({ 192,64,64,64 });
	attackAnim[(int)facingDirection::SE].speed = animAttackSpeed;
	attackAnim[(int)facingDirection::SE].loop = false;

	attackAnim[(int)facingDirection::W].PushBack({ 0,128,64,64 });
	attackAnim[(int)facingDirection::W].PushBack({ 64,128,64,64 });
	attackAnim[(int)facingDirection::W].PushBack({ 128,128,64,64 });
	attackAnim[(int)facingDirection::W].PushBack({ 192,128,64,64 });
	attackAnim[(int)facingDirection::W].speed = animAttackSpeed;
	attackAnim[(int)facingDirection::W].loop = false;

	attackAnim[(int)facingDirection::E].PushBack({ 0,128,64,64 }); // same as W but flipped
	attackAnim[(int)facingDirection::E].PushBack({ 64,128,64,64 });
	attackAnim[(int)facingDirection::E].PushBack({ 128,128,64,64 });
	attackAnim[(int)facingDirection::E].PushBack({ 192,128,64,64 });
	attackAnim[(int)facingDirection::E].speed = animAttackSpeed;
	attackAnim[(int)facingDirection::E].loop = false;

	attackAnim[(int)facingDirection::NW].PushBack({ 0,192,64,64 });
	attackAnim[(int)facingDirection::NW].PushBack({ 64,192,64,64 });
	attackAnim[(int)facingDirection::NW].PushBack({ 128,192,64,64 });
	attackAnim[(int)facingDirection::NW].PushBack({ 192,192,64,64 });
	attackAnim[(int)facingDirection::NW].speed = animAttackSpeed;
	attackAnim[(int)facingDirection::NW].loop = false;

	attackAnim[(int)facingDirection::NE].PushBack({ 0,192,64,64 }); // same as NW but flipped
	attackAnim[(int)facingDirection::NE].PushBack({ 64,192,64,64 });
	attackAnim[(int)facingDirection::NE].PushBack({ 128,192,64,64 });
	attackAnim[(int)facingDirection::NE].PushBack({ 192,192,64,64 });
	attackAnim[(int)facingDirection::NE].speed = animAttackSpeed;
	attackAnim[(int)facingDirection::NE].loop = false;

	attackAnim[(int)facingDirection::N].PushBack({ 0,256,64,64 });
	attackAnim[(int)facingDirection::N].PushBack({ 64,256,64,64 });
	attackAnim[(int)facingDirection::N].PushBack({ 128,256,64,64 });
	attackAnim[(int)facingDirection::N].PushBack({ 192,256,64,64 });
	attackAnim[(int)facingDirection::N].speed = animAttackSpeed;
	attackAnim[(int)facingDirection::N].loop = false;

	// jump
	jumpAnim[(int)facingDirection::S].PushBack({ 0,832,64,64 });
	jumpAnim[(int)facingDirection::S].PushBack({ 64,832,64,64 });
	jumpAnim[(int)facingDirection::S].PushBack({ 128,832,64,64 });
	jumpAnim[(int)facingDirection::S].PushBack({ 192,832,64,64 });
	jumpAnim[(int)facingDirection::S].PushBack({ 256,832,64,64 });
	jumpAnim[(int)facingDirection::S].speed = animJumpSpeed;
	jumpAnim[(int)facingDirection::S].loop = false;

	jumpAnim[(int)facingDirection::N].PushBack({ 0,1088,64,64 });
	jumpAnim[(int)facingDirection::N].PushBack({ 64,1088,64,64 });
	jumpAnim[(int)facingDirection::N].PushBack({ 128,1088,64,64 });
	jumpAnim[(int)facingDirection::N].PushBack({ 192,1088,64,64 });
	jumpAnim[(int)facingDirection::N].PushBack({ 256,1088,64,64 });
	jumpAnim[(int)facingDirection::N].speed = animJumpSpeed;
	jumpAnim[(int)facingDirection::N].loop = false;

	jumpAnim[(int)facingDirection::W].PushBack({ 0,960,64,64 });
	jumpAnim[(int)facingDirection::W].PushBack({ 64,960,64,64 });
	jumpAnim[(int)facingDirection::W].PushBack({ 128,960,64,64 });
	jumpAnim[(int)facingDirection::W].PushBack({ 192,960,64,64 });
	jumpAnim[(int)facingDirection::W].PushBack({ 256,960,64,64 });
	jumpAnim[(int)facingDirection::W].speed = animJumpSpeed;
	jumpAnim[(int)facingDirection::W].loop = false;

	jumpAnim[(int)facingDirection::E].PushBack({ 0,960,64,64 }); // same as W but flipped
	jumpAnim[(int)facingDirection::E].PushBack({ 64,960,64,64 });
	jumpAnim[(int)facingDirection::E].PushBack({ 128,960,64,64 });
	jumpAnim[(int)facingDirection::E].PushBack({ 192,960,64,64 });
	jumpAnim[(int)facingDirection::E].PushBack({ 256,960,64,64 });
	jumpAnim[(int)facingDirection::E].speed = animJumpSpeed;
	jumpAnim[(int)facingDirection::E].loop = false;

	jumpAnim[(int)facingDirection::SW].PushBack({ 0,896,64,64 });
	jumpAnim[(int)facingDirection::SW].PushBack({ 64,896,64,64 });
	jumpAnim[(int)facingDirection::SW].PushBack({ 128,896,64,64 });
	jumpAnim[(int)facingDirection::SW].PushBack({ 192,896,64,64 });
	jumpAnim[(int)facingDirection::SW].PushBack({ 256,896,64,64 });
	jumpAnim[(int)facingDirection::SW].speed = animJumpSpeed;
	jumpAnim[(int)facingDirection::SW].loop = false;

	jumpAnim[(int)facingDirection::SE].PushBack({ 0,896,64,64 }); // same as SW but flipped
	jumpAnim[(int)facingDirection::SE].PushBack({ 64,896,64,64 });
	jumpAnim[(int)facingDirection::SE].PushBack({ 128,896,64,64 });
	jumpAnim[(int)facingDirection::SE].PushBack({ 192,896,64,64 });
	jumpAnim[(int)facingDirection::SE].PushBack({ 256,896,64,64 });
	jumpAnim[(int)facingDirection::SE].speed = animJumpSpeed;
	jumpAnim[(int)facingDirection::SE].loop = false;

	jumpAnim[(int)facingDirection::NW].PushBack({ 0,1024,64,64 });
	jumpAnim[(int)facingDirection::NW].PushBack({ 64,1024,64,64 });
	jumpAnim[(int)facingDirection::NW].PushBack({ 128,1024,64,64 });
	jumpAnim[(int)facingDirection::NW].PushBack({ 192,1024,64,64 });
	jumpAnim[(int)facingDirection::NW].PushBack({ 256,1024,64,64 });
	jumpAnim[(int)facingDirection::NW].speed = animJumpSpeed;
	jumpAnim[(int)facingDirection::NW].loop = false;

	jumpAnim[(int)facingDirection::NE].PushBack({ 0,1024,64,64 }); // same as NW but flipped
	jumpAnim[(int)facingDirection::NE].PushBack({ 64,1024,64,64 });
	jumpAnim[(int)facingDirection::NE].PushBack({ 128,1024,64,64 });
	jumpAnim[(int)facingDirection::NE].PushBack({ 192,1024,64,64 });
	jumpAnim[(int)facingDirection::NE].PushBack({ 256,1024,64,64 });
	jumpAnim[(int)facingDirection::NE].speed = animJumpSpeed;
	jumpAnim[(int)facingDirection::NE].loop = false;


	// death animation
	deathAnim.PushBack({ 0,640,64,64 });
	deathAnim.PushBack({ 0,704,64,64 });
	deathAnim.PushBack({ 0,768,64,64 });
	deathAnim.PushBack({ 0,768,64,64 });
	deathAnim.PushBack({ 0,768,64,64 });
	deathAnim.PushBack({ 0,768,64,64 });
	deathAnim.PushBack({ 0,768,64,64 });
	deathAnim.PushBack({ 0,768,64,64 });
	deathAnim.speed = 2.f;
	deathAnim.loop = false;


	// precomputed adjacent tile neighbours pattern
	// for enemy spawn at player position
	// and to switch walkability values around boss (for phase 2,3,4)
	adjacentTileNeighboursPattern[0] = { 0,-1 }; // N
	adjacentTileNeighboursPattern[1] = { 0, 1 }; // S
	adjacentTileNeighboursPattern[2] = { -1,0 }; // W
	adjacentTileNeighboursPattern[3] = { 1,0 }; // E
	adjacentTileNeighboursPattern[4] = { 1,-1 }; // NE
	adjacentTileNeighboursPattern[5] = { -1,-1 }; // NW
	adjacentTileNeighboursPattern[6] = { 1, 1 }; // SE
	adjacentTileNeighboursPattern[7] = { -1,1 }; // SW


	// define values ------
	entityTex = boss_spritesheet;
	currentAnimation = &idleAnim[(int)facingDirection::S];
	SetPivot(32, 56);
	size.create(64, 64);
	this->position -= pivot;

	// timers ----------------
	// phases time
	phase_control_timers.phase1.timer.Start(); // phase1 is the first, start timer here for now
	phase_control_timers.phase1.time = 8000;
	phase_control_timers.phase2.time = 15000; // this time should be +/- the duration of the emitter of rain
	phase_control_timers.phase3.time = 7500;
	phase_control_timers.phase4.time = 10000;
	// attack cadence
	fireball_timer_data.time = 1600; // fireball cadence
	spawnEnemies_timer_data.time = 2000;
	shieldFire_timer_data.time = 800;
	maxEvasion_timer_data.time = 400;

	myState = Boss1State::PHASE1;
}

FlowerBossEntity::~FlowerBossEntity() 
{
	App->entityFactory->DeleteEntityFromSubtile(this);
	if (rainEmitter != nullptr)
		rainEmitter->to_delete = true;

	DesactiveShield();
}


bool FlowerBossEntity::PreUpdate()
{
	previousLife = life;

	return true;
}

bool FlowerBossEntity::Update(float dt)
{

	PhaseManager(dt);

	if (to_die)
		myState = Boss1State::DEATH;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		Phase3Logic();
	}

	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
	{
		life = 40.f;
	}
	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		shootedPoisonRainEmitter = false;
		Phase2Logic();
	}

	//LOG("tilepos:%i,%i", imOnTile.x, imOnTile.y);
	//LOG("life:%f", life);

	return true;
}

void FlowerBossEntity::PhaseManager(float dt)
{
	
	switch (myState)
	{
	case Boss1State::NOTHING:
	{
		LookToPlayer(idleAnim);
		break;
	}
	case Boss1State::PHASE1: // normal basic fireball pattern state
	{
		static bool launchedBall = false;

		// checks timer and attack
		if (phase_control_timers.phase1.timer.Read() >= phase_control_timers.phase1.time && !doingAttack && !evading)
		{
			// changes phase
			LOG("phase change");

			if (life <= maxLife * 0.5f)
			{
				myState = Boss1State::PHASE4;
				phase_control_timers.phase4.timer.Start();
			}
			else if (patternsCounter % 2 == 0)
			{
				myState = Boss1State::PHASE2;
				LOG("phase2");
				phase_control_timers.phase2.timer.Start();
			}
			else
			{
				myState = Boss1State::PHASE3;
				LOG("phase3");
				phase_control_timers.phase3.timer.Start();
			}

			++patternsCounter;
			break; // exit case
		}

		// --------------------------------------------------------

		if(fireball_timer_data.timer.Read() >= fireball_timer_data.time && !doingAttack && !evading)
		{
			// changes animation to attack one and set the flag attacking
			doingAttack = true;
			LookToPlayer(attackAnim);
		}
		else if(!doingAttack && !evading)
		{
			LookToPlayer(idleAnim);
		}

		// checks if the attack animation finishes and do logic
		if (currentAnimation->GetCurrentFloatFrame() >= 2.f && !launchedBall && !evading && doingAttack)
		{
			LOG("attack");
			launchedBall = true;

			// TODO: get the direction vector from the boss last axis (current angle), instead of the realtime player pivot
			App->entityFactory->CreateArrow(GetThrowingPos(), App->entityFactory->player->GetPivotPos(), 130, this, PROJECTILE_TYPE::ENEMY_ARROW);
			App->camera2D->AddTrauma(0.2f);
		}

		if (currentAnimation->Finished() && launchedBall && !evading && doingAttack)
		{
			for (int i = 0; i < 8; ++i)
				attackAnim[i].Reset();
			doingAttack = false;
			launchedBall = false;
			fireball_timer_data.timer.Start();
		}

		// TODO: improve how the cadence and the evasion probability are calculated
		if (life <= maxLife * 0.5f)
		{

		}

		// evasion of perimeter attacks
		// while not on doingAttack "state"
		if (!doingAttack && !evading)
		{
			bool isProjectileOnPerimeter = IsAttackOnTilePerimeter();
			
			if (isProjectileOnPerimeter)
			{
				evading = true;
				currentAnimation = &jumpAnim[pointingDir];
				LOG("evading");
				if (App->entityFactory->DeleteEntityFromSubtile(this))
				{
					LOG("del true");
				}
				else
					LOG("del false");

				maxEvasion_timer_data.timer.Start();
			}

		}

		if (evading)
		{
			bool isAttackOnPerimeter = IsAttackOnTilePerimeter();

			if (currentAnimation->Finished() && maxEvasion_timer_data.timer.Read() >= maxEvasion_timer_data.time && !isAttackOnPerimeter)
			{
				currentAnimation->Reset();
				evading = false;
				// resets timers
				fireball_timer_data.timer.Start();
				App->entityFactory->AssignEntityToSubtilePos(this, imOnSubtile);

				LOG("finished anim evasion");
			}
			else if (isAttackOnPerimeter)
			{
				LookToPlayer(jumpAnim);
				maxEvasion_timer_data.timer.Start();
			}
		}
		
		break;
	}
	case Boss1State::PHASE2:
	{
		if (phase_control_timers.phase2.timer.Read() >= phase_control_timers.phase2.time)
		{
			myState = Boss1State::PHASE1;
			LOG("returning from phase2 to phase1");
			phase_control_timers.phase1.timer.Start();
			fireball_timer_data.timer.Start();

			// erase shield walkability
			DesactiveShield();

			// reset shooted emitter
			shootedPoisonRainEmitter = false;

			// unlink emitter
			rainEmitter = nullptr;

			break;
		}

		ShieldLogic();
		Phase2Logic();

		LookToPlayer(idleAnim);

		break;
	}
	case Boss1State::PHASE3:
	{
		if (phase_control_timers.phase3.timer.Read() >= phase_control_timers.phase3.time)
		{
			myState = Boss1State::PHASE1;
			LOG("returning from phase3 to phase1");
			phase_control_timers.phase1.timer.Start();
			fireball_timer_data.timer.Start();

			// erase shield walkability
			DesactiveShield();

			break;
		}

		ShieldLogic();
		Phase3Logic();

		LookToPlayer(idleAnim);

		break;
	}
	case Boss1State::PHASE4:
	{
		if (phase_control_timers.phase4.timer.Read() >= phase_control_timers.phase4.time)
		{
			myState = Boss1State::PHASE1;
			LOG("returning from phase4 to phase1");
			phase_control_timers.phase1.timer.Start();
			fireball_timer_data.timer.Start();

			// erase shield walkability
			DesactiveShield();

			break;
		}

		
		ShieldLogic();
		Phase2Logic();
		Phase3Logic();

		LookToPlayer(idleAnim);

		break;
	}
	case Boss1State::DEATH:
	{
		currentAnimation = &deathAnim;

		if (currentAnimation->Finished())
			to_delete = true;

		break;
	}
	case Boss1State::MAX:
		break;
	default:
		break;
	}

	
	CheckRenderFlip();

}

void FlowerBossEntity::ShieldLogic()
{
	// instantiate shield
	if (!shieldActive)
		ActiveShield();
	else
		DoShieldLogic();
}

void FlowerBossEntity::DoShieldLogic()
{
	if (shieldFire_timer_data.timer.Read() >= shieldFire_timer_data.time)
	{
		App->attackManager->AddPropagationAttack(
			this, // from entity
			{ GetSubtilePos().x,GetSubtilePos().y }, // impact position, (on subtilemap units)
			propagationType::BFS, // propagation expansion type
			damageType::DIRECT,	// damage type: direct/in time
			ELEMENTAL_TYPE::FIRE_ELEMENT, // if the attack has any extra elemental dmg type (if the attack is dmgType=direct, the elemental probability of dmg is calculated by the buff manager)
			10, // base attack damage
			7, // radius (on subtile units)
			60, // propagation speed, in ms (time between steps)
			true); // if this attack instantate particles of the elemental type while propagation

		App->camera2D->AddTrauma(0.2f);

		shieldFire_timer_data.timer.Start();
	}
}

void FlowerBossEntity::Phase2Logic() // spawn poison rain
{
	if (!shootedPoisonRainEmitter)
	{
		LOG("shooting emitter");
		shootedPoisonRainEmitter = true;
		uint spawnRatio = uint(life * 2.5f);
		uint radius = 200u;
		// SHOOT
		rainEmitter = App->entityFactory->CreateBossEmitter(
			GetPivotPos(), // projected position
			radius, // radius, world coords
			spawnRatio, // spawn ratio, ms
			App->entityFactory->player, // owner
			phase_control_timers.phase2.time); // duration, ms
	}
}

void FlowerBossEntity::Phase3Logic() // spawn enemies around player neighbour positions
{
	if (spawnEnemies_timer_data.timer.Read() >= spawnEnemies_timer_data.time)
	{
		// if the enemy life is less than x, spawn archer too
		// TODO: test 
		std::vector<EnemyType> enemyTypesVec;

		if (patternsCounter <= 2)
		{
			enemyTypesVec.push_back(EnemyType::TEST);
			LOG("only basic enemy");
		}
		else if (patternsCounter <= 4)
		{
			enemyTypesVec.push_back(EnemyType::TEST);
			enemyTypesVec.push_back(EnemyType::ARCHER);
			LOG("spawn basic enemy and archer");
		}
		else
		{
			enemyTypesVec.push_back(EnemyType::ARCHER);
			enemyTypesVec.push_back(EnemyType::BOMB);
			enemyTypesVec.push_back(EnemyType::TEST);
			LOG("spawn archer,basic and bomb");
		}
		

		for (int i = 0; i < NUM_NEIGH_PATTERN; ++i)
		{
			iPoint  tilePos = App->entityFactory->player->GetTilePos() + adjacentTileNeighboursPattern[i];

			if (!App->pathfinding->IsWalkable(tilePos))
				continue;

			iPoint tileSize = { 32,32 };
			SDL_Rect spawnTileRect = { tilePos.x * tileSize.x, tilePos.y * tileSize.y, tileSize.x, tileSize.y };
			
			App->entityFactory->CreateEnemiesGroup(enemyTypesVec, spawnTileRect, 1, 1);
		}

		// restart timer
		spawnEnemies_timer_data.timer.Start();
	}

}

void FlowerBossEntity::ActiveShield()
{
	/*for (int i = 0; i < NUM_NEIGH_PATTERN; ++i)
	{
		iPoint tileToBlock = imOnTile + adjacentTileNeighboursPattern[i];
		App->pathfinding->ActivateTile(tileToBlock);
		LOG("Blocking Tile: %i,%i", tileToBlock.x, tileToBlock.y);
	}*/

	App->pathfinding->ActivateTile(imOnTile);

	shieldActive = true;
}

void FlowerBossEntity::DesactiveShield()
{
	/*for (int i = 0; i < NUM_NEIGH_PATTERN; ++i)
	{
		iPoint tileToBlock = imOnTile + adjacentTileNeighboursPattern[i];
		App->pathfinding->DeactivateTile(tileToBlock);
		LOG("Unblocking Tile: %i,%i", tileToBlock.x, tileToBlock.y);
	}*/
	App->pathfinding->DeactivateTile(imOnTile);

	shieldActive = false;
}

void FlowerBossEntity::LookToPlayer(Animation* desiredAnim) // looks to player and updates current animation frame correctly
{
	fPoint dirVec = (App->entityFactory->player->GetPivotPos() - pivot) - position;
	dirVec.Normalize();

	float angle = atan2f(dirVec.y, dirVec.x);

	// TODO: check this, doesnt work properly when player go from bottomleft quadrant to up left quadrant (move ocurrs instantly)
	// to do a smooth rotation we need some special checks
	// check the shortest path

	if (lastAngle < -3.0f && angle > 3.0f)
	{
		currentAngle += M_PI * 2.0f;
	}
	else if (lastAngle > 3.0f && angle < -3.0f)
	{
		currentAngle -= M_PI * 2.0f;
	}

	lastAngle = angle;

	// constant smooth speed
	/*const float RotationBlendFactor = 0.2f;
	currentAngle = angle * RotationBlendFactor + currentAngle * (1.0f - RotationBlendFactor);*/

	float lerpFactor = 2.6f;
	// lerped speed
	currentAngle = lerpAngle(currentAngle, angle, App->GetDt() * lerpFactor);

	// updates current animation
	pointingDir = GetPointingDir(currentAngle);

	float prevFrame = currentAnimation->GetCurrentFloatFrame();
	currentAnimation = &desiredAnim[pointingDir];
	currentAnimation->SetCurrentFrame(prevFrame);

	/*LOG("current angle: %f", currentAngle);
	LOG("last angle: %f", lastAngle);
	LOG("pointingdir: %i", pointingDir);
	LOG("dirVec: %f, %f", dirVec.x, dirVec.y);*/

}

float FlowerBossEntity::lerpAngle(float a1, float a2, float t)
{
	return a1 * (1 - t) + a2 * t;
}

void FlowerBossEntity::CheckRenderFlip()
{
	if (pointingDir == int(facingDirection::SE) || pointingDir == int(facingDirection::E) || pointingDir == int(facingDirection::NE))
	{
		flip = SDL_FLIP_HORIZONTAL;
	}
	else
		flip = SDL_FLIP_NONE;
}



bool FlowerBossEntity::PostUpdate()
{
	if(previousLife != life)
		LOG("life:%f", life);

	if (!evading)
	{
		// subtiles
		iPoint subTilePos = GetSubtilePos();
		subTilePos = App->map->SubTileMapToWorld(subTilePos.x, subTilePos.y);
		App->render->Blit(debugSubtileTex, subTilePos.x, subTilePos.y, NULL);
	}

	return true;
}

bool FlowerBossEntity::CleanUp()
{
	return true;
}

void FlowerBossEntity::Draw()
{
	App->render->Blit(entityTex, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0f, flip);
}

int FlowerBossEntity::GetPointingDir(float angle)
{

	int numAnims = 8;
	//LOG("angle: %f", angle);
	// divide the semicircle in 4 portions
	float animDistribution = PI / (numAnims * 0.5f); // each increment between portions //two semicircles

	int i = 0;
	if (angle >= 0) // is going right or on bottom semicircle range to left
	{
		// iterate between portions to find a match
		for (float portion = animDistribution * 0.5f; portion <= PI; portion += animDistribution) // increment on portion units
		{
			if (portion >= angle) // if the portion is on angle range
			{
				// return the increment position matching with enumerator direction animation
				// TODO: not the best workaround, instead do with std::map
				/*LOG("bottom semicircle");
				LOG("portion: %i", i);*/
				break;
			}
			++i;
		}
	}
	else if (angle <= 0) // animations relatives to upper semicircle
	{
		i = 0; // the next 4 on the enum direction

		for (float portion = -animDistribution * 0.5f; portion >= -PI; portion -= animDistribution)
		{
			if (i == 1) i = numAnims * 0.5f + 1;
			if (portion <= angle)
			{
				/*LOG("upper semicircle");
				LOG("portion: %i", i);*/
				break;
			}
			++i;
		}
	}

	pointingDir = i;
	if (pointingDir == numAnims) // if max direction
		pointingDir = numAnims - 1; // set to prev

	//LOG("portion: %i", pointingDir);

	return pointingDir;
}

bool FlowerBossEntity::IsAttackOnTilePerimeter()
{
	bool ret = false;

	iPoint subtileNeighbours[4];
	subtileNeighbours[0] = { 0,0 };
	subtileNeighbours[1] = { 1,0 };
	subtileNeighbours[2] = { 0,1 };
	subtileNeighbours[3] = { 1,1 };

	// check every neighbour subtile for attack
	for (int i = 0; i < NUM_NEIGH_PATTERN; ++i && !ret)
	{
		iPoint tileToExplore = adjacentTileNeighboursPattern[i] + imOnTile;

		iPoint firstSubtileDivision = tileToExplore * 2; // subtiles are two times the size of the tile

		for (int n = 0; n < 4; ++n && !ret)
		{
			iPoint subtileCheck = { firstSubtileDivision.x + subtileNeighbours[n].x, firstSubtileDivision.y + subtileNeighbours[n].y };

			if (!App->entityFactory->isThisSubtileEmpty(subtileCheck))
			{
				std::vector<j1Entity*>* checkVector = App->entityFactory->GetSubtileEntityVectorAt(subtileCheck);

				std::vector<j1Entity*>::iterator iter;

				if (checkVector != nullptr)
					iter = checkVector->begin();
				else
					continue;

				for (; iter != checkVector->end(); ++iter)
				{
					if ((*iter)->type == ENTITY_TYPE::PROJECTILE)
					{
						const j1Entity* linkedEntity = dynamic_cast<Projectile*>(*iter)->GetOwnerEntity();

						if (linkedEntity != nullptr)
						{
							if (linkedEntity->type == ENTITY_TYPE::PLAYER)
							{
								/*LOG("detected player attack on perimeter");
								LOG("pertaints to tile area: %i,%i", tileToExplore.x, tileToExplore.y);
								LOG("subtile: %i, %i", subtileCheck.x, subtileCheck.y);*/

								ret = true;

								break;
							}
						}
					}
				}
			}
		}
	}


	return ret;
}