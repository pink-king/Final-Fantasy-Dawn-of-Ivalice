#include "Boss_Flower.h"
#include "j1Render.h"
#include "j1ModuleCamera2D.h"
#include "j1EntityFactory.h"
#include "j1PathFinding.h"

FlowerBossEntity::FlowerBossEntity(iPoint position) : j1Entity(FLOWERBOSS, position.x, position.y, "Flower Boss")
{
	boss_spritesheet = App->tex->Load("textures/enemies/enemyFlower_Boss.png");

	// animations --------
	float animSpeed = 10.f;
	float animSpeedIdle = 5.f;
	float animAttackSpeed = 10.f;
	// idle
	idleAnim[(int)facingDirection::S].PushBack({ 0,320,64,64 });
	idleAnim[(int)facingDirection::S].PushBack({ 64,320,64,64 });
	idleAnim[(int)facingDirection::S].speed = animSpeedIdle;
	idleAnim[(int)facingDirection::S].loop = true;

	idleAnim[(int)facingDirection::SW].PushBack({ 0,384,64,64 });
	idleAnim[(int)facingDirection::SW].PushBack({ 64,384,64,64 });
	idleAnim[(int)facingDirection::SW].speed = animSpeedIdle;
	idleAnim[(int)facingDirection::SW].loop = true;

	idleAnim[(int)facingDirection::SE].PushBack({ 0,384,64,64 }); // same as SW but flipped
	idleAnim[(int)facingDirection::SE].PushBack({ 64,384,64,64 });
	idleAnim[(int)facingDirection::SE].speed = animSpeedIdle;
	idleAnim[(int)facingDirection::SE].loop = true;

	idleAnim[(int)facingDirection::W].PushBack({ 0,448,64,64 });
	idleAnim[(int)facingDirection::W].PushBack({ 64,448,64,64 });
	idleAnim[(int)facingDirection::W].speed = animSpeedIdle;
	idleAnim[(int)facingDirection::W].loop = true;

	idleAnim[(int)facingDirection::E].PushBack({ 0,448,64,64 }); // same as W but flipped
	idleAnim[(int)facingDirection::E].PushBack({ 64,448,64,64 });
	idleAnim[(int)facingDirection::E].speed = animSpeedIdle;
	idleAnim[(int)facingDirection::E].loop = true;

	idleAnim[(int)facingDirection::NW].PushBack({ 0,512,64,64 });
	idleAnim[(int)facingDirection::NW].PushBack({ 64,512,64,64 });
	idleAnim[(int)facingDirection::NW].speed = animSpeedIdle;
	idleAnim[(int)facingDirection::NW].loop = true;

	idleAnim[(int)facingDirection::NE].PushBack({ 0,512,64,64 }); // same as NW but flipped
	idleAnim[(int)facingDirection::NE].PushBack({ 64,512,64,64 });
	idleAnim[(int)facingDirection::NE].speed = animSpeedIdle;
	idleAnim[(int)facingDirection::NE].loop = true;

	idleAnim[(int)facingDirection::N].PushBack({ 0,576,64,64 });
	idleAnim[(int)facingDirection::N].PushBack({ 64,576,64,64 });
	idleAnim[(int)facingDirection::N].speed = animSpeedIdle;
	idleAnim[(int)facingDirection::N].loop = true;


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
	jumpAnim[(int)facingDirection::S].speed = animSpeed;
	jumpAnim[(int)facingDirection::S].loop = false;


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
	phase_control_timers.phase2.time = 15000;
	phase_control_timers.phase3.time = 7500;
	phase_control_timers.phase4.time = 10000;
	// attack cadence
	fireball_timer_data.time = 1600; // fireball cadence
	spawnEnemies_timer_data.time = 2000;

	myState = Boss1State::PHASE1;
}

FlowerBossEntity::~FlowerBossEntity() {}

//bool DemonBossEntity::Start()
//{
//	return true;
//}

bool FlowerBossEntity::PreUpdate()
{
	return true;
}

bool FlowerBossEntity::Update(float dt)
{

	PhaseManager(dt);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		Phase3Logic();
	}

	//LOG("tilepos:%i,%i", imOnTile.x, imOnTile.y);

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
		if (phase_control_timers.phase1.timer.Read() >= phase_control_timers.phase1.time && !doingAttack)
		{
			// changes phase
			LOG("phase change");

			if (patternsCounter % 2 == 0)
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

			if (life <= maxLife * 0.5f)
			{
				myState = Boss1State::PHASE4;
				phase_control_timers.phase4.timer.Start();
			}

			++patternsCounter;
			break; // exit case
		}

		if(fireball_timer_data.timer.Read() >= fireball_timer_data.time && !doingAttack)
		{
			// changes animation to attack one and set the flag attacking
			doingAttack = true;
			LookToPlayer(attackAnim);
		}
		else if(!doingAttack)
		{
			LookToPlayer(idleAnim);
		}

		// checks if the attack animation finishes and do logic
		if (currentAnimation->GetCurrentFloatFrame() >= 2.f && !launchedBall)
		{
			LOG("attack");
			launchedBall = true;

			// TODO: get the direction vector from the boss last axis (current angle), instead of the realtime player pivot
			App->entityFactory->CreateArrow(GetThrowingPos(), App->entityFactory->player->GetPivotPos(), 130, this, PROJECTILE_TYPE::ENEMY_ARROW);
			App->camera2D->AddTrauma(0.2f);
		}

		if (currentAnimation->Finished() && launchedBall)
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

			break;
		}

		// instantiate shield
		if (!shieldActive)
			ActiveShield();

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

		// instantiate shield
		if (!shieldActive)
			ActiveShield();

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

		// instantiate shield
		if (!shieldActive)
			ActiveShield();

		Phase2Logic();
		Phase3Logic();

		LookToPlayer(idleAnim);

		break;
	}
	case Boss1State::DEATH:
		break;
	case Boss1State::MAX:
		break;
	default:
		break;
	}

	
	CheckRenderFlip();

}

void FlowerBossEntity::Phase2Logic() // spawn poison rain
{
	
}

void FlowerBossEntity::Phase3Logic() // spawn enemies around player neighbour positions
{
	if (spawnEnemies_timer_data.timer.Read() >= spawnEnemies_timer_data.time)
	{
		// if the enemy life is less than x, spawn archer too
		// TODO: test 
		std::vector<EnemyType> enemyTypesVec;
		enemyTypesVec.push_back(EnemyType::TEST);

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
	for (int i = 0; i < NUM_NEIGH_PATTERN; ++i)
	{
		iPoint tileToBlock = imOnTile + adjacentTileNeighboursPattern[i];
		App->pathfinding->ActivateTile(tileToBlock);
		LOG("Blocking Tile: %i,%i", tileToBlock.x, tileToBlock.y);
	}

	shieldActive = true;
}

void FlowerBossEntity::DesactiveShield()
{
	for (int i = 0; i < NUM_NEIGH_PATTERN; ++i)
	{
		iPoint tileToBlock = imOnTile + adjacentTileNeighboursPattern[i];
		App->pathfinding->DeactivateTile(tileToBlock);
		LOG("Unblocking Tile: %i,%i", tileToBlock.x, tileToBlock.y);
	}

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