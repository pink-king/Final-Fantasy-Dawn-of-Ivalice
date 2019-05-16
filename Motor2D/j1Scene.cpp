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
#include "UiItem_Bar.h"
#include "j1AttackManager.h"
#include "j1ModuleCamera2D.h"
#include "UiItem_HitPointManager.h"
#include "UiItem_HealthBar.h"
#include "j1BuffManager.h"
#include "UiItem_CooldownClock.h"
#include "GUI_Definitions.h"
#include "Projectile.h"
#include "j1DialogSystem.h"
#include "j1TransitionManager.h"
#include "SDL_mixer/include/SDL_mixer.h"

j1Scene::j1Scene() : j1Module()
{
	name.assign("scene");
	state = SceneState::STARTMENU;
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
	debug = false;
	
	if(debug_tex == nullptr)
		debug_tex = App->tex->Load("maps/path2.png");

	App->pause = false;
	selectUI = App->audio->LoadFx("audio/fx/UI/selectUI.wav");
	acceptUI = App->audio->LoadFx("audio/fx/UI/AcceptUI.wav");
	if (state == SceneState::LEVEL1)
	{
		//AcceptUISFX_logic = false;
		inGamePanel->enable = true;
		uiMarche->enable = true;
		uiShara->enable = true;
		uiRitz->enable = true;
		settingPanel->enable = false;
		startMenu->enable = false;

		App->audio->PlayFx(enterGameSFX, 0);
		App->audio->PlayMusic("audio/music/BRPG_Hell_Spawn_FULL_Loop.ogg", -1);

		if (ComeToDeath)
		{
			App->LoadGame("save_game.xml");
			ComeToDeath = false;
		}

	}
	if (state == SceneState::STARTMENU)
	{
		App->gui->resetHoverSwapping = false;
		if (!LoadedUi)
		{
			LoadInGameUi(sceneNode);
			LoadStartMenu(sceneNode);
			LoadPlayerUi(sceneNode);
			LoadSettings(sceneNode);
			LoadPauseSettings(sceneNode);
			LoadInventory(sceneNode);
			LoadDeathScreen(sceneNode);
			LoadWinScreen(sceneNode);
			LoadedUi = true;
		}
		startMenu->enable = true;
		uiMarche->enable = false;
		uiShara->enable = false;
		uiRitz->enable = false;
		settingPanel->enable = false;
		inGamePanel->enable = false;
		pausePanel->enable = false;
		inventory->enable = false;
		deathPanel->enable = false;
		winPanel->enable = false;

		App->audio->PlayMusic("audio/music/menu_1.0.ogg", -1);
	}

	if (state == SceneState::DEATH)
	{
		App->audio->PlayFx(playerDeath, 0);
		App->gui->resetHoverSwapping = false;
		if (inGamePanel->enable)
		inGamePanel->enable = false;
		if (!deathPanel->enable)
		deathPanel->enable = true;

		ComeToDeath = true;
	}

	if (state == SceneState::WIN)
	{
		App->gui->resetHoverSwapping = false;
		if (inGamePanel->enable)
			inGamePanel->enable = false;
		if (!winPanel->enable)
			winPanel->enable = true;
	}

		openInventorySFX = App->audio->LoadFx("audio/fx/UI/open_inventory.wav");
		closeinventorySFX = App->audio->LoadFx("audio/fx/UI/close_inventory.wav");
		open_PauseMenuSFX = App->audio->LoadFx("audio/fx/open_close_pauseMenu.wav");
		enterGameSFX = App->audio->LoadFx("audio/fx/UI/AcceptEnterGame.wav");
		playerDeath = App->audio->LoadFx("audio/fx/States/player_death.wav");
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


	/*if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->win->SetScale(1);

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->win->SetScale(2);*/

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->map->ToggleDebugDraw();
		debug = !debug;
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		debugSubtiles = !debugSubtiles;
	}
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		debugColl = !debugColl;
	}
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		hackerMode = !hackerMode;
	}
	// FAKE KEYS FOR TESTING 

	/*if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		App->win->SetScale(1);

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)     
		App->win->SetScale(2);*/

	// debug testing subtiles entities empty
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && hackerMode && App->entityFactory->active)
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

		 //DEBUG attack propagation!
		App->attackManager->AddPropagationAttack(
			App->entityFactory->player->GetSelectedCharacterEntity(), // from entity
			{ entitySubTilePoint.x,entitySubTilePoint.y }, // impact position, (on subtilemap units)
			propagationType::BFS, // propagation expansion type
			damageType::INTIME,	// damage type: direct/in time
			ELEMENTAL_TYPE::ICE_ELEMENT, // if the attack has any extra elemental dmg type (if the attack is dmgType=direct, the elemental probability of dmg is calculated by the buff manager)
			100, // base attack damage
			13, // radius (on subtile units)
			60, // propagation speed, in ms (time between steps)
			true); // if this attack instantate particles of the elemental type while propagating
		App->camera2D->AddTrauma(0.7f);
	}
	
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	int mx, my;
	
	App->input->GetMousePosition(mx, my);
	iPoint mousePos = App->render->ScreenToWorld(mx, my);
	//LOG("mousePos: %i,%i", mousePos.x, mousePos.y);
	mousePos = App->map->WorldToMap(mousePos.x, mousePos.y);
	//LOG("mousePosMap: %i,%i", mousePos.x, mousePos.y);
	
	App->tex->textures;
	// map debug draw grids
	// map debug draw grids

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
	{
		App->LoadGame("Portal.xml");
		ComeToPortal = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		App->SaveGame("Portal.xml");
		ComeToPortal = true;
	}
	if(App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
		App->camera2D->camera.y += 1000 * dt;

	if(App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
		App->camera2D->camera.y -= 1000 * dt;

	if(App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		App->camera2D->camera.x += 1000 * dt;

	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
		App->camera2D->camera.x -= 1000 * dt;

	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		App->gui->AddLabel("Hola buenos dias Carlos", { 255,255,255,255 }, App->font->openSansBold36, { 300,200 }, inGamePanel, true);
	}
	
	

	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
	{
		if (state == SceneState::LEVEL1)
		{
			App->gui->resetHoverSwapping = false;
			state = SceneState::STARTMENU;
			startMenu->enable = true;

		}
		else
		{
			
			state = SceneState::LEVEL1;
			
		}

	}

	
	if (state == SceneState::STARTMENU)
	{
		result_volume = volume_bar->GetBarValue();
		App->audio->SetVolume(result_volume);
		result_fx = fx_bar->GetBarValue();
		App->audio->SetFxVolume(result_fx);
		//settingPanel->enable = false;
	}
	
	
	if (state == SceneState::LEVEL1)
	{
		//Mix_CloseAudio();
		//if()
		


		//settingPanel->enable = false;
		if (App->entityFactory->player->selectedCharacterEntity->character == characterName::MARCHE && inGamePanel->enable)
		{
			//LOG("marche");
			uiMarche->enable = true;
			uiRitz->enable = false;
			uiShara->enable = false;
		}
		if (App->entityFactory->player->selectedCharacterEntity->character == characterName::RITZ && inGamePanel->enable)
		{
			//LOG("marche");
			uiMarche->enable = false;
			uiRitz->enable = true;
			uiShara->enable = false;
		}
		if (App->entityFactory->player->selectedCharacterEntity->character == characterName::SHARA && inGamePanel->enable)
		{
			//LOG("marche");
			uiMarche->enable = false;
			uiRitz->enable = false;
			uiShara->enable = true;
		}

		if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_START) == KEY_DOWN)
		{
			if (!inventory->enable)
			{

				App->pause = !App->pause;
				if (App->pause)
				{
					Mix_PauseMusic();
					if (!pausePanel->enable)
						App->audio->PlayFx(open_PauseMenuSFX, 0);

					pausePanel->enable = true;
					paused = true;
					App->gui->resetHoverSwapping = false;
				}
				else
				{
					Mix_ResumeMusic();
					App->gui->resetHoverSwapping = false;
					App->gui->selected_object->state = IDLE;
					App->gui->selected_object = nullptr;
					App->gui->GoBackToGame();
				}
			}
		}

		if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_BACK) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
		{
			DoOpenInventory();
		}
	
	}
	 
	//if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
	//{
	//	App->entityFactory->player->life -= 20;
	//	App->gui->healthBar->damageInform.doDamage = true;
	//	App->gui->healthBar->damageInform.damageValue = 20;
	//}

	//if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)    // player uses health potion !!
	//{

	//	App->entityFactory->player->selectedCharacterEntity->life += 30;
	//	App->gui->healthBar->damageInform.damageValue = -30;
	//}
	//


	/*if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		App->loot->trigger = true;*/
	if(App->map->active)
		App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->camera2D->camera.x, y - App->camera2D->camera.y);


	iPoint coords = App->render->ScreenToWorld(x, y);

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		j1Entity* ent; 
		ent = App->entityFactory->CreateEnemy(EnemyType::TEST, { coords.x,coords.y }); 

	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		j1Entity* ent;
		ent = App->entityFactory->CreateEnemy(EnemyType::BOMB, { coords.x,coords.y });

	}
	
	if (hackerMode)
	{
		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		{
			/*std::vector<EnemyType> typeVec; 
			typeVec.reserve(2);
			typeVec.push_back(EnemyType::BOMB);
			typeVec.push_back(EnemyType::TEST);
			App->entityFactory->CreateEnemiesGroup(typeVec, SDL_Rect{ coords.x, coords.y, 150, 150}, 2, 6);*/
			App->entityFactory->CreateArrow(App->entityFactory->player->GetSelectedCharacterEntity()->GetThrowingPos(), fPoint{ (float)coords.x, (float)coords.y }, 100, App->entityFactory->player->GetSelectedCharacterEntity(), PROJECTILE_TYPE::CONTAGIOUS_ARROW);

		}
		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			App->entityFactory->CreateArrow(App->entityFactory->player->GetSelectedCharacterEntity()->GetThrowingPos(), fPoint{ (float)coords.x, (float)coords.y }, 100, App->entityFactory->player->GetSelectedCharacterEntity(),PROJECTILE_TYPE::BASIC_ARROW);
		}

		if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
		{
			App->entityFactory->CreateArrow(App->entityFactory->player->GetSelectedCharacterEntity()->GetThrowingPos(), fPoint{ (float)coords.x, (float)coords.y }, 100, App->entityFactory->player->GetSelectedCharacterEntity(), PROJECTILE_TYPE::MAGIC_BOLT);
		}

		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			App->entityFactory->CreateArrow(fPoint{ (float)coords.x, (float)coords.y }, { 0,0 }, 0, App->entityFactory->player->GetSelectedCharacterEntity(), PROJECTILE_TYPE::MEDUSA);
		}

		if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		{
			App->entityFactory->CreateArrow(fPoint{ (float)coords.x, (float)coords.y }, { 0,0 }, 0, App->entityFactory->player->GetSelectedCharacterEntity(), PROJECTILE_TYPE::DEATH_CIRCLE);
		}
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			App->entityFactory->CreateArrow(App->entityFactory->player->GetSelectedCharacterEntity()->GetThrowingPos(), fPoint{ (float)coords.x, (float)coords.y }, 170, App->entityFactory->player->GetSelectedCharacterEntity(), PROJECTILE_TYPE::FIRE_ARROW);
		}
	}
	
	if (App->entityFactory->player != nullptr && (state == SceneState::LEVEL1 || state == SceneState::LEVEL2))
	{
		if (isDeath)
		{
			App->entityFactory->player->exp = 0;
			App->SaveGame("save_game.xml");
			App->entityFactory->player->life = App->entityFactory->player->maxLife;
			isDeath = false;
			App->transitionManager->CreateFadeTransition(1.F,true,SceneState::DEATH);
		}
	}

	App->win->ClearTitle();

	//	App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::FIRE_ELEMENT, ROL::DEFENCE_ROL, en, "\0", 21);
	//	App->buff->CreateBurned(App->entityFactory->player->selectedCharacterEntity, en, 21, 10, "burn");
	//}
	//if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)		// Spawn unanimate dummy
	//{
	//	Enemy* en = App->entityFactory->CreateEnemy(EnemyType::TEST, { coords.x,coords.y });

	//	App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::POISON_ELEMENT, ROL::DEFENCE_ROL, en, "\0", 21);
	//	App->buff->CreatePoision(App->entityFactory->player->selectedCharacterEntity, en, 21, 10, "poison");
	//}

	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;
	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (exitGame)
		return false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	App->tex->UnLoad(debug_tex); 
	debug_tex = nullptr;
	

	LOG("Freeing scene");
	return true;
}

void j1Scene::LoadUiElement(UiItem* parent, pugi::xml_node node)
{

	// images
	for (pugi::xml_node uiNode = node.child("images").child("image"); uiNode; uiNode = uiNode.next_sibling("image"))
	{
		SDL_Rect section = { uiNode.child("section").attribute("x").as_int(), uiNode.child("section").attribute("y").as_int(), uiNode.child("section").attribute("w").as_int(), uiNode.child("section").attribute("h").as_int() };
		iPoint position = { uiNode.child("position").attribute("x").as_int(), uiNode.child("position").attribute("y").as_int() };

		int isPanel = uiNode.child("flag").attribute("isPanel").as_int();
		std::string panelName = uiNode.child("flag").attribute("panelName").as_string();


		std::string lootFlag = uiNode.child("flag").attribute("value").as_string();
		std::string charFlag = uiNode.child("charFlag").attribute("value").as_string();
		if (lootFlag == "loot")
		{

			lootPanelRect = section;
		}
		else if (lootFlag == "lootNoButton")
		{
			lootPanelRectNoButton = section;
		}
		else if (charFlag == "Marche")
		{
			MarcheIcon = App->gui->AddImage(position, &section, nullptr);
		}
		else if (charFlag == "Ritz")
		{
			RitzIcon = App->gui->AddImage(position, &section, nullptr);
		}
		else if (charFlag == "Shara")
		{
			SharaIcon = App->gui->AddImage(position, &section, nullptr);
		}
		else
		{                                  // this is useless now
			if (isPanel != 1)
			{
				App->gui->AddImage(position, &section, parent, isPanel);  // bug: an image is created as panel 
			}
			else
			{

			}

		}




	}


	//button
	for (pugi::xml_node uiNode = node.child("buttons").child("button"); uiNode; uiNode = uiNode.next_sibling("button"))
	{
		std::string functionPath = uiNode.attribute("function").as_string();
		SDL_Rect sectionIdle = { uiNode.child("idleSec").attribute("x").as_int(), uiNode.child("idleSec").attribute("y").as_int(), uiNode.child("idleSec").attribute("w").as_int(), uiNode.child("idleSec").attribute("h").as_int() };
		iPoint position = { uiNode.child("position").attribute("x").as_int(), uiNode.child("position").attribute("y").as_int() };

		pugi::xml_node hoverSecNode = uiNode.child("hoverSec");
		SDL_Rect* sectionHove = nullptr;
		if (hoverSecNode)
		{
			SDL_Rect hover = { hoverSecNode.attribute("x").as_int(), hoverSecNode.attribute("y").as_int(), hoverSecNode.attribute("w").as_int(), hoverSecNode.attribute("h").as_int() };
			sectionHove = &hover;
		}

		SDL_Rect* sectionClick = nullptr;
		if (pugi::xml_node clickSecNode = uiNode.child("clickSec"))
		{
			SDL_Rect click = { clickSecNode.attribute("x").as_int(), clickSecNode.attribute("y").as_int(), clickSecNode.attribute("w").as_int(), clickSecNode.attribute("h").as_int() };
			sectionClick = &click;
		}



		App->gui->AddButton(position, functionPath, &sectionIdle, parent, sectionClick, sectionHove);
	}

	// labels
	for (pugi::xml_node uiNode = node.child("labels").child("label"); uiNode; uiNode = uiNode.next_sibling("label"))
	{
		iPoint position = { uiNode.child("position").attribute("x").as_int(), uiNode.child("position").attribute("y").as_int() };
		std::string text = uiNode.child("text").attribute("value").as_string();
		std::string font = uiNode.child("font").attribute("value").as_string();
		SDL_Color color = { uiNode.child("color").attribute("R").as_uint(),uiNode.child("color").attribute("G").as_uint(),uiNode.child("color").attribute("B").as_uint(),uiNode.child("color").attribute("A").as_uint() };
		const char* path = uiNode.child("path").attribute("p").as_string();
		uint size = uiNode.child("size").attribute("s").as_int();
		

		App->gui->AddLabel(text.data(), color, App->font->Load(path, size), position, parent);

	}

	// bars 
	for (pugi::xml_node uiNode = node.child("panelBars").child("panelBar"); uiNode; uiNode = uiNode.next_sibling("panelBar"))
	{

		std::string name = uiNode.attribute("name").as_string();
		iPoint position = { uiNode.child("position").attribute("x").as_int(), uiNode.child("position").attribute("y").as_int() };
		SDL_Rect section_bar = { uiNode.child("section_bar").attribute("x").as_int(), uiNode.child("section_bar").attribute("y").as_int(), uiNode.child("section_bar").attribute("w").as_int(), uiNode.child("section_bar").attribute("h").as_int() };
		SDL_Rect section_thumb = { uiNode.child("section_thumb").attribute("x").as_int(), uiNode.child("section_thumb").attribute("y").as_int(), uiNode.child("section_thumb").attribute("w").as_int(), uiNode.child("section_thumb").attribute("h").as_int() };
		SDL_Rect image_idle = { uiNode.child("image_idle").attribute("x").as_int(), uiNode.child("image_idle").attribute("y").as_int(), uiNode.child("image_idle").attribute("w").as_int(), uiNode.child("image_idle").attribute("h").as_int() };
		SDL_Rect image_hover = { uiNode.child("image_hover").attribute("x").as_int(), uiNode.child("image_hover").attribute("y").as_int(), uiNode.child("image_hover").attribute("w").as_int(), uiNode.child("image_hover").attribute("h").as_int() };
		UiItem_Bar* slider_volume = App->gui->AddBar(position, name, &section_bar, &section_thumb, &image_idle, &image_hover, parent);
		if (name == "volumeSlider")
		{
			volume_bar = slider_volume;
		}
		if (name == "fxSlider")
		{
			fx_bar = slider_volume;
		}
	}


	// checkboxes

	for (pugi::xml_node uiNode = node.child("PanelCheckboxes").child("PanelCheckbox"); uiNode; uiNode = uiNode.next_sibling("PanelCheckbox"))
	{
		std::string functionPath = uiNode.attribute("function").as_string();
		iPoint panelPosition = { uiNode.child("panelPosition").attribute("x").as_int(), uiNode.child("panelPosition").attribute("y").as_int() };
		SDL_Rect panelSection = { uiNode.child("panelSection").attribute("x").as_int(), uiNode.child("panelSection").attribute("y").as_int(), uiNode.child("panelSection").attribute("w").as_int(), uiNode.child("panelSection").attribute("h").as_int() };
		SDL_Rect boxSection = { uiNode.child("boxSection").attribute("x").as_int(), uiNode.child("boxSection").attribute("y").as_int(), uiNode.child("boxSection").attribute("w").as_int(), uiNode.child("boxSection").attribute("h").as_int() };
		SDL_Rect tickSection = { uiNode.child("tickSection").attribute("x").as_int(), uiNode.child("tickSection").attribute("y").as_int(), uiNode.child("tickSection").attribute("w").as_int(), uiNode.child("tickSection").attribute("h").as_int() };


		// LABEL INSIDE CHECKBOX 
		pugi::xml_node uiNode3 = uiNode.child("checkboxLabels").child("checkboxLabel");

		std::string text = uiNode3.child("text").attribute("value").as_string();
		uint fontIndex = uiNode3.child("font").attribute("value").as_uint();
		uint fontSize = uiNode3.child("font").attribute("size").as_uint();
		SDL_Color color = { uiNode3.child("color").attribute("R").as_uint(),uiNode3.child("color").attribute("G").as_uint(),uiNode3.child("color").attribute("B").as_uint(),uiNode3.child("color").attribute("A").as_uint() };


		labelInfo labelInfo = {
			text,
			color ,
			fontIndex,
		};

		App->gui->AddCheckbox(panelPosition, functionPath, &panelSection, &boxSection, &tickSection, &labelInfo, parent);

	}



	// health bar 

	for (pugi::xml_node uiNode = node.child("healthbars").child("healthbar"); uiNode; uiNode = uiNode.next_sibling("healthbar"))
	{


		SDL_Rect dynamicSection = { uiNode.child("dynamicSection").attribute("x").as_int(), uiNode.child("dynamicSection").attribute("y").as_int(), uiNode.child("dynamicSection").attribute("w").as_int(), uiNode.child("dynamicSection").attribute("h").as_int() };
		iPoint position = { uiNode.child("position").attribute("x").as_int(), uiNode.child("position").attribute("y").as_int() };

		std::string variant = uiNode.child("type").attribute("value").as_string();

		if (variant == "player")
		{
			SDL_Rect damageSection = { uiNode.child("damageSection").attribute("x").as_int(), uiNode.child("damageSection").attribute("y").as_int(), uiNode.child("damageSection").attribute("w").as_int(), uiNode.child("damageSection").attribute("h").as_int() };
			App->gui->healthBar = App->gui->AddHealthBar(position, &dynamicSection, &damageSection, type::player, inGamePanel);
		}
		else if (variant == "enemy")
		{
			App->gui->enemyLifeBarInfo.dynamicSection = dynamicSection;
		}

	}

	// cooldown clocks    

	for (pugi::xml_node uiNode = node.child("clocks").child("clock"); uiNode; uiNode = uiNode.next_sibling("clock"))
	{
		SDL_Rect section = { uiNode.child("section").attribute("x").as_int(), uiNode.child("section").attribute("y").as_int(), uiNode.child("section").attribute("w").as_int(), uiNode.child("section").attribute("h").as_int() };
		iPoint position = { uiNode.child("position").attribute("x").as_int(), uiNode.child("position").attribute("y").as_int() };

		std::string type = uiNode.child("type").attribute("value").as_string();


		if (type == "ability1")
		{
			App->gui->allclocksData.ability1.position = position;
			App->gui->allclocksData.ability1.section = section;
			App->gui->allclocksData.ability1.type = type;
		}
		else if (type == "ability2")
		{

			App->gui->allclocksData.ability2.position = position;
			App->gui->allclocksData.ability2.section = section;
			App->gui->allclocksData.ability2.type = type;
		}
		else if (type == "ulti")
		{

			App->gui->allclocksData.ulti.position = position;
			App->gui->allclocksData.ulti.section = section;
			App->gui->allclocksData.ulti.type = type;
		}

		else if (type == "potion")
		{

		}

	}

}

bool j1Scene::LoadInGameUi(pugi::xml_node& nodeScene)
{
	pugi::xml_node inGameNode = nodeScene.child("InGameUi");
	inGamePanel = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(inGamePanel, inGameNode);
	coins_label = App->gui->AddLabel("0 x", { 255,255,255,255 }, App->font->openSansSemiBold24, { 1080,26 }, inGamePanel);
	return true;
}


bool j1Scene::LoadStartMenu(pugi::xml_node& nodeScene)
{
	pugi::xml_node StartMenuNode = nodeScene.child("StartMenu");
	startMenu = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(startMenu, StartMenuNode);
	return true;
}

bool j1Scene::LoadPlayerUi(pugi::xml_node& nodeScene)
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

bool j1Scene::LoadSettings(pugi::xml_node& nodeScene)
{
	pugi::xml_node settingNode = nodeScene.child("Settings");
	settingPanel = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(settingPanel, settingNode);
	return true;
}

bool j1Scene::LoadPauseSettings(pugi::xml_node& nodeScene)
{
	pugi::xml_node settingPauseNode = nodeScene.child("SettingsInGame");
	pausePanel = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(pausePanel, settingPauseNode);
	return true;
}

bool j1Scene::LoadInventory(pugi::xml_node& nodeScene)
{
	pugi::xml_node inventoryNode = nodeScene.child("Inventory");
	inventory = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(inventory, inventoryNode);
	inventoryItem = App->gui->AddInventory(inventory);

	MarcheIcon->parent = inventoryItem;     // now assign the parent to the inventory icon
	SharaIcon->parent = inventoryItem;
	RitzIcon->parent = inventoryItem;

	MarcheIcon->hide = true;
	SharaIcon->hide = true;
	RitzIcon->hide = true;

	return true;
}

bool j1Scene::LoadDeathScreen(pugi::xml_node& nodeScene)
{
	pugi::xml_node deathNode = nodeScene.child("DeathScreen");
	deathPanel = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(deathPanel, deathNode);
	return true;
}
bool j1Scene::LoadWinScreen(pugi::xml_node& nodeScene)
{
	pugi::xml_node winNode = nodeScene.child("WinScreen");
	winPanel = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(winPanel, winNode);
	return true;
}

void j1Scene::LoadNewMap(const char* mapName)
{
	if (App->map->Load(mapName))
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

		App->entityFactory->CreatePlayer({ -1575, 2150 }); //  {300,300}
		// re set entities data map (create or delete/create if we have a previous one)
		App->entityFactory->CreateEntitiesDataMap(App->map->data.width * 2, App->map->data.height * 2);
	}
}

void j1Scene::UnLoadScene()
{
	App->map->Disable();
	App->attackManager->Disable();
	App->entityFactory->Disable();
	App->pathfinding->Disable();
	App->buff->Disable();
	App->camera2D->Disable();

	App->audio->UnLoadAudio();

}

void j1Scene::LoadScene(SceneState sceneState)
{
	UnLoadScene();

	switch (sceneState)
	{
	case SceneState::STARTMENU:
		state = SceneState::STARTMENU;
		break;

	case SceneState::LEVEL1:

		state = SceneState::LEVEL1;
		App->attackManager->Enable();
		App->pathfinding->Enable();
		App->camera2D->Enable();
		App->buff->Enable();
		App->map->active = true;
		LoadNewMap("maps/Level1_Final_Borders_Faked.tmx");//"maps/test_ordering.tmx"))//level1_Block_rev.tmx"))   // ("maps/iso_walk.tmx")
		App->entityFactory->Enable();
		// create player for testing purposes here
		//App->entityFactory->CreatePlayer({ -1563, 1000 });
		break;

	case SceneState::LEVEL2:
		break;

	case SceneState::DEATH:
		state = SceneState::DEATH;
		if (!App->camera2D->IsEnabled())
		App->camera2D->Enable();
		break;

	case SceneState::WIN:
		state = SceneState::WIN;
		if (!App->camera2D->IsEnabled())
		App->camera2D->Enable();
		break;

	case SceneState::MAX_STATES:
		break;
	default:
		break;
	}

	Start();
}



void j1Scene::DoOpenInventory(bool onlyEquipped, bool isVendor)
{
	
		if (!pausePanel->enable)
		{
			App->pause = !App->pause;
			if (App->pause && !pausePanel->enable)
			{
				if (!App->dialog->isDialogInScreen)  // dont open if inventory in vendor dialog sequence active, open it later
				{
					inventory->enable = true;
					App->gui->resetHoverSwapping = false;

					inventoryItem->LoadElements(onlyEquipped, isVendor);
					App->audio->PlayFx(openInventorySFX, 0);
				}
				
			}

			else
			{
				App->audio->PlayFx(closeinventorySFX, 0);
				inventory->enable = false;


				if (App->dialog->isDialogSequenceActive)
				{
					App->dialog->spawnDialoguesAfterInventory(); 
				}
			}

		}

}
