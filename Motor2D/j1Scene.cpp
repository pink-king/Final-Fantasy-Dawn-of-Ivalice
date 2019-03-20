#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1EntityFactory.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "UiItem_Image.h"


j1Scene::j1Scene() : j1Module()
{
	name.assign("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& node)
{
	LOG("Loading Scene");
	bool ret = true;
	sceneNode = node;
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load("maps/iso_walk.tmx");

	debug_tex = App->tex->Load("maps/path2.png");

	// create player for testing purposes here
	App->entityFactory->CreatePlayer({ 300,300 });
	LoadStartMenu(sceneNode);
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{


	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_REPEAT)
		App->transition = !App->transition;

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);


	iPoint coords = App->render->ScreenToWorld(x, y);

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		App->entityFactory->CreateEntity(ENTITY_TYPE::ENEMY_TEST, coords.x, coords.y,  "whatever");
	
	
	//App->win->SetTitle(App->title.data());


	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	return true;
}

void j1Scene::LoadUiElement(pugi::xml_node node)
{
	// images
	for (pugi::xml_node uiNode = node.child("images").child("image"); uiNode; uiNode = uiNode.next_sibling("image"))
	{
		SDL_Rect section = { uiNode.child("section").attribute("x").as_int(), uiNode.child("section").attribute("y").as_int(), uiNode.child("section").attribute("w").as_int(), uiNode.child("section").attribute("h").as_int() };
		iPoint position = { uiNode.child("position").attribute("x").as_int(), uiNode.child("position").attribute("y").as_int() };
		App->gui->AddImage(position, &section);
	}

	// labels
	for (pugi::xml_node uiNode = node.child("labels").child("label"); uiNode; uiNode = uiNode.next_sibling("label"))
	{
		iPoint position = { uiNode.child("position").attribute("x").as_int(), uiNode.child("position").attribute("y").as_int() };
		std::string  text = uiNode.child("text").attribute("value").as_string();
		std::string  font = uiNode.child("font").attribute("value").as_string();
		SDL_Color color = { uiNode.child("color").attribute("R").as_uint(),uiNode.child("color").attribute("G").as_uint(),uiNode.child("color").attribute("B").as_uint(),uiNode.child("color").attribute("A").as_uint() };
		
		App->gui->AddLabel(text.data(), color, App->font->default, position);

	}
	
	
}

bool j1Scene::LoadStartMenu(pugi::xml_node & nodeScene)
{
	pugi::xml_node startMenuNode = nodeScene.child("StartMenu");

	LoadUiElement(startMenuNode);
	return true;
}
