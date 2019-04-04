#include "PlayerEntity.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"

PlayerEntity::PlayerEntity(int posX, int posY) : j1Entity(NO_TYPE, posX , posY, "PlayerParent")
{
	//SetPivot(16, 40);
}

PlayerEntity::~PlayerEntity()
{
}

bool PlayerEntity::Start()
{
	// TODO: load from XML 
	life = 100; 

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

	Sint16 xAxis = App->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTX);
	Sint16 yAxis = App->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTY);

	if (xAxis > 0 || xAxis < 0)
	{
		position.x = position.x + (xAxis * 0.003 * characterBaseSpeed.x) * dt; // TODO: GET speed from buff manager
		isMoving = true;
	}
	if (yAxis > 0 || yAxis < 0)
	{
		position.y = position.y + (yAxis * 0.003 * characterBaseSpeed.y) * dt; // TODO: GET speed from buff manager
		isMoving = true;
	}

	if (isMoving)// if we get any input, any direction
	{
		// store actual
		float current_cycle_frame = currentAnimation->GetCurrentFloatFrame();
		currentAnimation = &run[(int)GetPointingDir(atan2f(yAxis, xAxis))];
		currentAnimation->SetCurrentFrame(current_cycle_frame);
	}
	else
	{
		//currentAnimation = &idle[pointingDir]; // Redundant, is repeated in the next function, but more clear this way
		GetInputFromKeyboard(dt);
	}

	//iPoint isoPoint = App->map->WorldToMap(position.x, position.y); // convert world coords to iso map coords

	//position.x = isoPoint.x;
	//position.y = isoPoint.y;

	//LOG("pos x: %f pos y: %f", position.x, position.y);

	

	// checks render flip
	CheckRenderFlip();

	//LOG("animation Angle: %f", animationAngle);

	return true;
}

void PlayerEntity::CheckRenderFlip()
{
	if (pointingDir == 3 || pointingDir == 4 || pointingDir == 7)
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

void PlayerEntity::GetInputFromKeyboard(float dt)
{
	bool up = App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT;
	bool down = App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT;
	bool left = App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT;
	bool right = App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT;

	bool isMoving = false;

	if (up && !down)
	{
		isMoving = true;
		position.y -= characterBaseSpeedKey.y * 100 * dt;
		pointingDir = 6;
	}
	if (down && !up)
	{
		isMoving = true;
		position.y += characterBaseSpeedKey.y * 100 * dt;
		pointingDir = 2;
	}
	if (left && !right)
	{
		isMoving = true;
		position.x -= characterBaseSpeedKey.x * 100 * dt;
		pointingDir = 4;
	}
	if (right && !left)
	{
		isMoving = true;
		position.x += characterBaseSpeedKey.x * 100 * dt;
		pointingDir = 0;
	}

	if (up && right && !left)
	{
		pointingDir = 5;
	}
	if (up && left && !right)
	{
		pointingDir = 7;
	}
	if (down && left  && !right)
	{
		pointingDir = 3;
	}
	if (down && right && !left)
	{
		pointingDir = 1;
	}


	if (isMoving)
	{
		float current_cycle_frame = currentAnimation->GetCurrentFloatFrame();
		currentAnimation = &run[pointingDir];
		currentAnimation->SetCurrentFrame(current_cycle_frame);
	}
	else
		currentAnimation = &idle[pointingDir]; // Redundant, but more clear this way.
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