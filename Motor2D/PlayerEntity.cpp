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
	Sint16 xAxis = App->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTX);
	Sint16 yAxis = App->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTY);

	if (xAxis > 0 || xAxis < 0)
		position.x = position.x + (xAxis * 0.005 * characterBaseSpeed.x) * dt; // TODO: GET speed from buff manager
	if (yAxis > 0 || yAxis < 0)
		position.y = position.y + (yAxis * 0.005 * characterBaseSpeed.y) * dt; // TODO: GET speed from buff manager

	float animationAngle = atan2f(yAxis, xAxis);

	LOG("%f", animationAngle);
	if (animationAngle > 0)
	{
		LOG("%f", animationAngle);
		//LOG("%i", xAxis);

		float animDistribution = PI / 4;
		int i = 0;
		for (float portion = 0.0f; portion <= PI; portion += animDistribution)
		{
			if (animDistribution >= portion)
				break;
			LOG("%i", i);
			++i;
		}

		currentAnimation = &run[i];

	}
	/*else
		currentAnimation = &idle[(int)pointingDir];*/

	return true;
}