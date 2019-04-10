#include "PlayerEntity.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1PathFinding.h"

PlayerEntity::PlayerEntity(int posX, int posY) : j1Entity(PLAYER, posX , posY, "PlayerParent")
{
	//SetPivot(16, 40);
}

PlayerEntity::~PlayerEntity()
{
}

bool PlayerEntity::Start()
{
	return true;
}

bool PlayerEntity::PreUpdate()
{
	return true;
}

bool PlayerEntity::Update(float dt)
{
	//InputMovement(dt);
	return true;
}

bool PlayerEntity::PostUpdate()
{

	return true;
}

bool PlayerEntity::CleanUp()
{
	return true;
}



bool PlayerEntity::InputMovement(float dt)
{
	bool isMoving = false;

	fPoint previousPosF = position;//GetPivotPos();
	/*iPoint previousPosInt;
	previousPosInt.x = GetPivotPos().x;
	previousPosInt.y = GetPivotPos().y;*/
	/*previousPosInt.x = position.x;
	previousPosInt.y = position.y;*/

	Sint16 xAxis = App->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTX);
	Sint16 yAxis = App->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTY);

	if (xAxis > 0 || xAxis < 0)
	{
		position.x = position.x + (xAxis * 0.003 * characterBaseSpeed.x) * dt; // TODO: GET speed from buff manager
		isMoving = true;
		//LOG("xAxis %i", xAxis); //32767 -32768
	}
	if (yAxis > 0 || yAxis < 0)
	{
		position.y = position.y + (yAxis * 0.003 * characterBaseSpeed.y) * dt; // TODO: GET speed from buff manager
		isMoving = true;
	}

	// keyboard input, only for debug or test purposes at the moment, main gameplay are based on GamePad
	if ((App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) || (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) ||
		(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) || (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT))
	{
		// condition
		isMoving = true;
		// max axis values
		Sint16 max = 32767;
		Sint16 min = -32768;
		// x "axis" input
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			xAxis = min;
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			xAxis = max;
		// y "axis" input
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			yAxis = min;
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			yAxis = max;

		if (xAxis == yAxis || yAxis > 0 && xAxis < 0 || yAxis < 0 && xAxis > 0) // diagonal cases || slow down
		{
			//xAxis = xAxis * 0.5f;
			yAxis = yAxis * 0.5f;
		}
		position.x = position.x + (xAxis * 0.003 * characterBaseSpeed.x) * dt; // TODO: GET speed from buff manager
		position.y = position.y + (yAxis * 0.003 * characterBaseSpeed.y) * dt; // TODO: GET speed from buff manager
	}

	// check individually collisions for each axis movement

	if (isMoving)
	{
		iPoint walkaCheck = App->map->WorldToMap(GetPivotPos().x, GetPivotPos().y);
		if (!App->pathfinding->IsWalkable(walkaCheck)) // if we detect any walkability collision
		{
			// check x
			fPoint tempPos = previousPosF;
			tempPos.x = position.x;
			iPoint checker = App->map->WorldToMap(tempPos.x + pivot.x, tempPos.y + pivot.y);
			if (!App->pathfinding->IsWalkable(checker))
			{
				position.x = previousPosF.x;
			}
			//previousPosF = position;
			tempPos = previousPosF;
			tempPos.y = position.y;
			checker = App->map->WorldToMap(tempPos.x + pivot.x, tempPos.y + pivot.y);
			if (!App->pathfinding->IsWalkable(checker))
			{
				position.y = previousPosF.y;
			}
		}
	}
	

	if (isMoving)// if we get any input, any direction
	{
		// store actual
		float current_cycle_frame = currentAnimation->GetCurrentFloatFrame();
		lastAxisMovAngle = atan2f(yAxis, xAxis);
		currentAnimation = &run[(int)GetPointingDir(lastAxisMovAngle)];
		currentAnimation->SetCurrentFrame(current_cycle_frame);
	}
	else
	{
		currentAnimation = &idle[pointingDir];
	}

	// checks render flip
	CheckRenderFlip();

	//LOG("animation Angle: %f", animationAngle);

	return true;
}

bool PlayerEntity::InputCombat()
{
	combat_state = combatState::IDLE;
	
	if (App->input->GetControllerAxisPulsation(SDL_CONTROLLER_AXIS_TRIGGERLEFT) == KEY_DOWN || 
		App->input->GetControllerAxisPulsation(SDL_CONTROLLER_AXIS_TRIGGERLEFT) ==  KEY_REPEAT)
	{
		if(App->input->GetControllerAxisPulsation(SDL_CONTROLLER_AXIS_TRIGGERLEFT) == KEY_DOWN)
			aiming = true;

		// check ultimate trigger
		if (App->input->GetControllerAxisPulsation(SDL_CONTROLLER_AXIS_TRIGGERRIGHT) == KEY_DOWN)
		{
			combat_state = combatState::ULTIMATE;
			//LOG("ULTIMATE");
		}
		// check basic attack
		if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_X) == KEY_DOWN)
		{
			combat_state = combatState::BASIC;
			LOG("BASIC");
		}
		// check dodge
		if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			combat_state = combatState::DODGE;
			LOG("DODGE");
		}
		// etc
		// code under construction ...
		//LOG("");
	}
	if (App->input->GetControllerAxisPulsation(SDL_CONTROLLER_AXIS_TRIGGERLEFT) == KEY_UP)
	{
		aiming = false;
		return false;
	}

	return true;
}

//combatState PlayerEntity::GetInstantCombatState()

void PlayerEntity::CheckRenderFlip()
{
	if (pointingDir == int(facingDirection::SW) || pointingDir == 4 || pointingDir == 7)
	{
		flip = SDL_FLIP_HORIZONTAL;
	}
	else
		flip = SDL_FLIP_NONE;
}

void PlayerEntity::Draw()
{
	if (spritesheet != nullptr)
	{
		if (currentAnimation != nullptr)
			App->render->Blit(spritesheet, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, flip);
		else
			App->render->Blit(spritesheet, position.x, position.y);
	}
}

int PlayerEntity::GetPointingDir(float angle)
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

float PlayerEntity::GetLastHeadingAngle() const
{
	return lastAxisMovAngle;
}