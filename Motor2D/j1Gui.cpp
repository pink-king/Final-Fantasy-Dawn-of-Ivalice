#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Map.h"


j1Gui::j1Gui() : j1Module() 
{
}


j1Gui::~j1Gui()
{
}

bool j1Gui::Awake(pugi::xml_node& conf)
{
	return true;
}

bool j1Gui::Start()
{
	return true;
}

bool j1Gui::Update(float dt)
{
	return true;
}

bool j1Gui::PostUpdate()
{
	return true;
}

bool j1Gui::CleanUp()
{
	return true;
}
