#include "j1SceneUi.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1EntityFactory.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "UiItem_Image.h"

j1SceneUi::j1SceneUi()
{
	name.assign("sceneUi");
}

j1SceneUi::~j1SceneUi()
{
}

bool j1SceneUi::Awake(pugi::xml_node & node)
{
	return true;
}

bool j1SceneUi::Start()
{
	return true;
}

bool j1SceneUi::PreUpdate()
{
	return true;
}

bool j1SceneUi::Update(float dt)
{
	return true;
}

bool j1SceneUi::PostUpdate()
{
	return true;
}

bool j1SceneUi::CleanUp()
{
	return true;
}
