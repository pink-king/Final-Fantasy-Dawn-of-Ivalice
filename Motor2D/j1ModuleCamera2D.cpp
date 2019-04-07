#include "j1ModuleCamera2D.h"

j1ModuleCamera2D::j1ModuleCamera2D()
{
	name.assign("ModuleCamera2D");
}

j1ModuleCamera2D::~j1ModuleCamera2D()
{
}

bool j1ModuleCamera2D::Awake(pugi::xml_node & node)
{
	return true;
}

bool j1ModuleCamera2D::Start()
{
	return true;
}

bool j1ModuleCamera2D::PreUpdate()
{
	return true;
}

bool j1ModuleCamera2D::Update(float dt)
{
	return true;
}

bool j1ModuleCamera2D::CleanUp()
{
	return true;
}