#include "PlayerEntity.h"
#include "j1Input.h"
#include "p2Log.h"

PlayerEntity::PlayerEntity() : j1Entity(NO_TYPE, 0,0, "PlayerParent")
{}

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
	Draw(entityTex);
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
		currentAnimation = &run[(int)GetPointingDir(atan2f(yAxis, xAxis))];
	else
		currentAnimation = &idle[pointingDir];

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

int PlayerEntity::GetPointingDir(float angle)
{
	int ret = 0;

	LOG("angle: %f", angle);

	int i = 0;
	if (angle >= 0) // is going right or on bottom semicircle range to left
	{
		// divide the semicircle in 4 portions
		float animDistribution = PI / 4; // each increment between portions

		// iterate between portions to find a match
		for (float portion = 0.0f; portion <= PI; portion += animDistribution) // increment on portion units
		{
			if (portion >= angle) // if the portion is on angle range
			{
				ret = i; // return the increment position matching with enumerator direction animation
						 // TODO: not the best workaround, instead do with std::map
				LOG("bottom semicircle");
				//LOG("portion: %i", i);
				break;
			}
			++i;
		}
	}
	else if (angle <= 0) // animations relatives to upper semicircle
	{
		i = 4; // the next 4 on the enum direction
		float animDistribution = PI / 4; // each increment between portions
		
		for (float portion = 0.0f; portion >= -PI; portion -= animDistribution)
		{
			if (portion <= angle)
			{
				ret = i;
				LOG("upper semicircle");
				//LOG("portion: %i", i);
				break;
			}
			++i;
		}
	}

	pointingDir = i;
	if (pointingDir == 8) // if max direction
		pointingDir = 4; // set to max direction left

	// filter
	ret = pointingDir;
	LOG("portion: %i", pointingDir);

	return ret;
}