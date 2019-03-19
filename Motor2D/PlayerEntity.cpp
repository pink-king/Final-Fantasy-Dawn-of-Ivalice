#include "PlayerEntity.h"

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
	return true;
}

bool PlayerEntity::PostUpdate()
{
	Draw(entityTex);
	return true;
}