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
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "UiItem_Image.h"
#include "j1AttackManager.h"
#include "j1LootSystem.h"

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
	debug = true;

	if (App->map->Load("maps/iso_walk.tmx"))
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

		// re set entities data map (create or delete/create if we have a previous one)
		App->entityFactory->CreateEntitiesDataMap(App->map->data.width*2, App->map->data.height*2);
	}

	debug_tex = App->tex->Load("maps/path2.png");
	
	// More perspective on the map since the beggining
	App->render->camera.x = 500;

	// create player for testing purposes here
	App->entityFactory->CreatePlayer({ 300,300 });

	/*LoadInGameUi(sceneNode);
	LoadStartMenu(sceneNode);
	LoadPlayerUi(sceneNode);
	inGamePanel->enable = true;
	uiMarche->enable = true;
	uiShara->enable = false;
	uiRitz->enable = false;
	startMenu->enable = false;*/

	//LoadInGameUi(sceneNode);
	//// LoadStartMenu(sceneNode);
	//inGamePanel->enable = true;
	//// startMenu->enable = false;


	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	// debug pathfing ------------------

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);


	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->win->SetScale(1);

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->win->SetScale(2);

	// debug testing subtiles entities empty
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		iPoint entitySubTilePoint = App->render->ScreenToWorld(x, y);
		iPoint clickedTile = entitySubTilePoint;
		clickedTile = App->map->WorldToMap(clickedTile.x, clickedTile.y);
		entitySubTilePoint = App->map->WorldToSubtileMap(entitySubTilePoint.x, entitySubTilePoint.y);

		LOG("clicked tile: %i, %i", clickedTile.x, clickedTile.y);
		LOG("clicked subtile %i,%i", entitySubTilePoint.x, entitySubTilePoint.y);

		if (App->entityFactory->isThisSubtileEmpty(entitySubTilePoint))
			LOG("subtile empty");
		else
			LOG("subtile NOT empty");

		// DEBUG attack propagation!
		App->attackManager->AddPropagationAttack(App->entityFactory->player->GetSelectedCharacterEntity(), { entitySubTilePoint.x,entitySubTilePoint.y }, propagationType::BFS, 10, 20, 40);

	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	// map debug draw grids
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		App->map->ToggleDebugDraw();

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
		App->render->camera.y += 1000 * dt;

	if(App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
		App->render->camera.y -= 1000 * dt;

	if(App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		App->render->camera.x += 1000 * dt;

	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
		App->render->camera.x -= 1000 * dt;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_REPEAT)
		debug = !debug;

	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
	{
		if (inGamePanel->enable)
		{
			inGamePanel->enable = false;
			startMenu->enable = true;
		}
		else
		{
			inGamePanel->enable = true;
			startMenu->enable = false;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
	{
		if (uiMarche->enable)
		{
			uiMarche->enable = false;
			uiShara->enable = true;
			uiRitz->enable = false;
		}
		else if (uiShara->enable)
		{
			uiShara->enable = false;
			uiRitz->enable = true;
			uiMarche->enable = false;
		}
		else if (uiRitz->enable)
		{
			uiRitz->enable = false;
			uiMarche->enable = true;
			uiShara->enable = false;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		App->loot->trigger = true;
	
	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);


	iPoint coords = App->render->ScreenToWorld(x, y);
	static int cont = 0;
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		App->entityFactory->CreateEntity(ENTITY_TYPE::ENEMY_TEST, coords.x, coords.y, "whatever");
		cont++;
	}
	static char title[90];
	sprintf_s(title, 90, " | CURRENTLY THERE ARE %i instantiated ENTITIES", App->entityFactory->entities.size());
	App->win->AddStringToTitle(title);
	//LOG("CURRENTLY THERE ARE %i ENTITES FOLLOWING YOU", App->entityFactory->entities.size());

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
	App->tex->UnLoad(debug_tex); 

	LOG("Freeing scene");
	return true;
}

void j1Scene::LoadUiElement(UiItem*parent, pugi::xml_node node)
{

	// images
	for (pugi::xml_node uiNode = node.child("images").child("image"); uiNode; uiNode = uiNode.next_sibling("image"))
	{
		SDL_Rect section = { uiNode.child("section").attribute("x").as_int(), uiNode.child("section").attribute("y").as_int(), uiNode.child("section").attribute("w").as_int(), uiNode.child("section").attribute("h").as_int() };
		iPoint position = { uiNode.child("position").attribute("x").as_int(), uiNode.child("position").attribute("y").as_int() };
		App->gui->AddImage(position, &section, parent);
	}

	// labels
	for (pugi::xml_node uiNode = node.child("labels").child("label"); uiNode; uiNode = uiNode.next_sibling("label"))
	{
		iPoint position = { uiNode.child("position").attribute("x").as_int(), uiNode.child("position").attribute("y").as_int() };
		std::string text = uiNode.child("text").attribute("value").as_string();
		std::string font = uiNode.child("font").attribute("value").as_string();
		SDL_Color color = { uiNode.child("color").attribute("R").as_uint(),uiNode.child("color").attribute("G").as_uint(),uiNode.child("color").attribute("B").as_uint(),uiNode.child("color").attribute("A").as_uint() };
		
		App->gui->AddLabel(text.data(), color, App->font->sans, position, parent);

	}
	
	// bars 
	for (pugi::xml_node uiNode = node.child("bars").child("bar"); uiNode; uiNode = uiNode.next_sibling("bar"))
	{
		iPoint position = { uiNode.child("position").attribute("x").as_int(), uiNode.child("position").attribute("y").as_int() };
		SDL_Rect section_bar = { uiNode.child("section_bar").attribute("x").as_int(), uiNode.child("section_bar").attribute("y").as_int(), uiNode.child("section_bar").attribute("w").as_int(), uiNode.child("section_bar").attribute("h").as_int() };
		SDL_Rect section_thumb = { uiNode.child("section_thumb").attribute("x").as_int(), uiNode.child("section_thumb").attribute("y").as_int(), uiNode.child("section_thumb").attribute("w").as_int(), uiNode.child("section_thumb").attribute("h").as_int() };
		
		// TODO: spawn thumg according to bar type: vertical or horizontal 
		//std::string type = uiNode.child("type").attribute("value").as_string();

		App->gui->AddBar(position, &section_bar, &section_thumb, nullptr, VERTICAL); // TODO: add parent later 

		// MORE BARS JUST FOR TESTING 

		App->gui->AddBar(iPoint(position.x + 150, position.y), &section_bar, &section_thumb, nullptr, VERTICAL);
		App->gui->AddBar(iPoint(position.x + 300, position.y), &section_bar, &section_thumb, nullptr, VERTICAL);
		App->gui->AddBar(iPoint(position.x + 450, position.y), &section_bar, &section_thumb, nullptr, VERTICAL);
	}


	



}

bool j1Scene::LoadInGameUi(pugi::xml_node & nodeScene)
{
	pugi::xml_node inGameNode = nodeScene.child("InGameUi");
	inGamePanel = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(inGamePanel, inGameNode);
	return true;
}


bool j1Scene::LoadStartMenu(pugi::xml_node & nodeScene)
{
	pugi::xml_node StartMenuNode = nodeScene.child("StartMenu");
	startMenu= App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(startMenu, StartMenuNode);
	return true;
}

bool j1Scene::LoadPlayerUi(pugi::xml_node & nodeScene)
{
	pugi::xml_node marcheNode = nodeScene.child("InGameUiM");
	uiMarche = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(uiMarche, marcheNode);

	pugi::xml_node sharaNode = nodeScene.child("InGameUiS");
	uiShara = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(uiShara, sharaNode);

	pugi::xml_node ritzNode = nodeScene.child("InGameUiR");
	uiRitz = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(uiRitz, ritzNode);

	return true;
}
