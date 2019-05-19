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
#include "NoWalkableTrigger.h"
#include "WaveTrigger.h"
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

	hit_counter = 0;
	App->pause = false;
	selectUI = App->audio->LoadFx("audio/fx/UI/selectUI.wav");
	acceptUI = App->audio->LoadFx("audio/fx/UI/AcceptUI.wav");
	purchase = App->audio->LoadFx("audio/fx/UI/purchase.wav");
	sell = App->audio->LoadFx("audio/fx/UI/sell.wav");
	switch_page = App->audio->LoadFx("audio/fx/UI/switch_page.wav");

	if (state == SceneState::LEVEL1)
	{
		iPoint tileSize = { 32,32 };
		SDL_Rect waveZone = { 11 * tileSize.x, 5 * tileSize.y, 10 * tileSize.x, 15 * tileSize.y };
		WaveTrigger* waveTrigg = (WaveTrigger*)App->entityFactory->CreateWaveTrigger(iPoint(-14, 511), waveZone, 1); 
		waveTrigg->CreateEntryWall(iPoint(16, 22));
		waveTrigg->CreateEntryWall(iPoint(17, 22));
		waveTrigg->CreateExitWall({ 13,3 });
		waveTrigg->CreateExitWall({ 14,3 });

		App->entityFactory->CreateTrigger(TRIGGER_TYPE::WIN, App->map->MapToWorld(15, 2).x, App->map->MapToWorld(15, 2).y, SceneState::LOBBY, White);

		//App->entityFactory->CreatePlayer({ -1575, 2150 }); Proper Start of the level
		App->entityFactory->CreatePlayer({ -209, 650 });
		//App->entityFactory->CreatePlayer({ 0, 0 });
		App->entityFactory->loadEnemies = true;
		App->camera2D->SetCameraPos({ -(int)App->entityFactory->player->GetPivotPos().x, -(int)App->entityFactory->player->GetPivotPos().y });
		//AcceptUISFX_logic = false;
		inGamePanel->enable = true;
		uiMarche->enable = true;
		uiShara->enable = true;
		uiRitz->enable = true;
		settingPanel->enable = false;
		startMenu->enable = false;

		App->audio->PlayFx(enterGameSFX, 0);
		App->audio->PlayMusic("audio/music/BRPG_Hell_Spawn_FULL_Loop.ogg", -1);

		if (ComeToDeath || ComeToWin)
		{
			App->LoadGame("save_game.xml");
			ComeToDeath = false;
		}
		if (ComeToPortal)
		{
			App->LoadGame("Portal.xml");
			App->entityFactory->player->GetMarche()->position = { portalPos.x,portalPos.y - 32 };
			App->entityFactory->player->GetShara()->position = { portalPos.x,portalPos.y - 32 };
			App->entityFactory->player->GetRitz()->position = { portalPos.x,portalPos.y - 32 };
			App->entityFactory->CreateTrigger(TRIGGER_TYPE::EXITPORTAL, portalPos.x, portalPos.y);
			ComeToPortal = false;
		}
	}

	if (state == SceneState::LEVEL2)
	{
		iPoint tileSize = { 32,32 };
		SDL_Rect waveZone = { 17 * tileSize.x, 25 * tileSize.y, 8 * tileSize.x, 14 * tileSize.y };
		WaveTrigger* waveTrigg = (WaveTrigger*)App->entityFactory->CreateWaveTrigger(iPoint(App->map->MapToWorld(25,26).x, App->map->MapToWorld(25, 26).y), waveZone, 2);
		waveTrigg->CreateEntryWall(iPoint(33, 22));
		waveTrigg->CreateEntryWall(iPoint(33, 21));
		waveTrigg->CreateEntryWall(iPoint(33, 20));
		waveTrigg->CreateExitWall({ 19, 46 });
		waveTrigg->CreateExitWall({ 20, 46 });
		waveTrigg->CreateExitWall({ 21, 46 });
		waveTrigg->CreateExitWall({ 22, 46 });

		fPoint PosX = fPoint(App->map->MapToWorld(36, 99).x, App->map->MapToWorld(36, 101).y);
		NoWalkableTrigger* bossTrigger = (NoWalkableTrigger*)App->entityFactory->CreateTrigger(TRIGGER_TYPE::NOWALKABLE, PosX.x, PosX.y);
		bossTrigger->CreateEntryWall(iPoint(37, 94));
		bossTrigger->CreateEntryWall(iPoint(36, 94));
		bossTrigger->CreateEntryWall(iPoint(35, 94));
		bossTrigger->CreateEntryWall(iPoint(34, 94));

		App->entityFactory->CreatePlayer({ -820,3300 });
		App->entityFactory->loadEnemies = true;
		App->camera2D->SetCameraPos({ -(int)App->entityFactory->player->GetPivotPos().x, -(int)App->entityFactory->player->GetPivotPos().y });
		//AcceptUISFX_logic = false;
		inGamePanel->enable = true;
		uiMarche->enable = true;
		uiShara->enable = true;
		uiRitz->enable = true;
		settingPanel->enable = false;
		startMenu->enable = false;

		App->audio->PlayFx(enterGameSFX, 0);
		App->audio->PlayMusic("audio/music/BRPG_Hell_Spawn_FULL_Loop.ogg", -1);

		if (ComeToDeath || ComeToWin)
		{
			App->LoadGame("save_game.xml");
			ComeToDeath = false;
		}
		if (ComeToPortal)
		{
			App->LoadGame("Portal.xml");
			App->entityFactory->player->GetMarche()->position = { portalPos.x,portalPos.y - 32 };
			App->entityFactory->player->GetShara()->position = { portalPos.x,portalPos.y - 32 };
			App->entityFactory->player->GetRitz()->position = { portalPos.x,portalPos.y - 32 };
			App->entityFactory->CreateTrigger(TRIGGER_TYPE::EXITPORTAL, portalPos.x, portalPos.y);
			ComeToPortal = false;
		}
	}

	if (state == SceneState::LOBBY)
	{

		App->audio->PlayMusic("audio/music/main_hall.ogg",-1);
		App->entityFactory->CreatePlayer({ 115, 240 });

		//AcceptUISFX_logic = false;
		App->entityFactory->CreateDialogTrigger(-135, 262, "VENDOR");
		App->entityFactory->CreateDialogTrigger(90, 189, "STRANGER");
		App->entityFactory->CreateTrigger(TRIGGER_TYPE::SAVE,105, 385);

		App->entityFactory->CreateTrigger(TRIGGER_TYPE::WIN, 250, 180, SceneState::LEVEL1, Black);

		if(ComeToWin)
			door = App->entityFactory->CreateTrigger(TRIGGER_TYPE::WIN, 350, 230, SceneState::LEVEL2, Black);
		App->entityFactory->loadEnemies = false;
		inGamePanel->enable = true;
	
		uiMarche->enable = true;
		uiShara->enable = true;
		uiRitz->enable = true;
		settingPanel->enable = false;
		startMenu->enable = false;

		App->audio->PlayFx(enterGameSFX, 0);
		//App->audio->PlayMusic("audio/music/BRPG_Hell_Spawn_FULL_Loop.ogg", -1);

		if (ComeToPortal)
		{
			
			App->LoadGame("Portal.xml");
			ComeToPortal = false;
			App->entityFactory->CreateTrigger(TRIGGER_TYPE::LOBBYPORTAL, 96, 290, previosState, White);

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
		isDeath = false;
	}

	if (state == SceneState::WIN)
	{
		App->audio->PlayMusic("audio/music/win.ogg", -1);

		if (ComeToDeath)
		{
			ComeToDeath = false;
		}

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


	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
		App->win->SetScale(1);

	if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
		App->win->SetScale(2);

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


	


	//if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)     
	//	App->win->SetScale(2);

	// debug testing subtiles entities empty
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN /*&& hackerMode*/ && App->entityFactory->active)
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

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		Trigger* trigger = App->entityFactory->CreateTrigger(TRIGGER_TYPE::PORTAL, App->entityFactory->player->position.x, App->entityFactory->player->position.y, SceneState::LOBBY, White);

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
	
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);
		App->entityFactory->CreateEntity(FLOWERBOSS, p.x, p.y, "flower_boss");
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
	
	if (state == SceneState::LOBBY)
	{
		if (App->input->GetKey(SDL_SCANCODE_KP_2) == KEY_DOWN)
		{
			if(door == nullptr)
				door = App->entityFactory->CreateTrigger(TRIGGER_TYPE::WIN, 350, 230, SceneState::LEVEL2, Black);
		}
	}
	
	if (state == SceneState::LEVEL1 || state == SceneState::LEVEL2 || state == SceneState::LOBBY)
	{
		//Mix_CloseAudio();
		//if()
		result_volume = volume_bar->GetBarValue();
		App->audio->SetVolume(result_volume);
		result_fx = fx_bar->GetBarValue();
		App->audio->SetFxVolume(result_fx);


		if (App->entityFactory->player->selectedCharacterEntity != nullptr)
		{
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
		}

		if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_START) == KEY_DOWN)
		{
			if (!inventory->enable && !App->dialog->isDialogSequenceActive)
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
		if (pausePanel->enable &&  App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			App->pause = false;
			Mix_ResumeMusic();
			App->gui->resetHoverSwapping = false;
			App->gui->selected_object->state = IDLE;
			App->gui->selected_object = nullptr;
			App->gui->GoBackToGame();

		}

		if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_BACK) == KEY_DOWN) //|| App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
		{
			DoOpenInventory();
		}
		if (inventory->enable &&  App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			DoOpenInventory();
		}
		if (inventory->enable && !inventoryItem->isVendorInventory &&  App->input->GetControllerAxisPulsation(SDL_CONTROLLER_AXIS_TRIGGERRIGHT) == KEY_DOWN && (inventoryItem->swappedBag || inventoryItem->swappedBag2))
		{
			if (!inventoryItem->swappedBag3 && inventoryItem->swappedBag2)
			{
				inventoryItem->swappedBag = false;
				inventoryItem->swappedBag2 = false;
				inventoryItem->swappedBag3 = true;
				inventoryItem->firstTimeSwappedBagLeft = false;
				inventoryItem->firstTimeSwappedBagLeft3 = true;
				inventoryItem->LoadElements();
			}
			if (!inventoryItem->swappedBag2 && inventoryItem->swappedBag)
			{
				inventoryItem->swappedBag = false;
				inventoryItem->swappedBag2 = true;
				inventoryItem->swappedBag3 = false;
				inventoryItem->firstTimeSwappedBag = false;
				inventoryItem->firstTimeSwappedBagLeft = false;
				inventoryItem->firstTimeSwappedBagLeft3 = false;
				inventoryItem->LoadElements();
			}
			App->gui->resetHoverSwapping = false;
		}

		if (inventory->enable && !inventoryItem->isVendorInventory && App->input->GetControllerAxisPulsation(SDL_CONTROLLER_AXIS_TRIGGERLEFT) == KEY_DOWN && (inventoryItem->swappedBag2 || inventoryItem->swappedBag3))
		{

			if (inventoryItem->swappedBag2 && !inventoryItem->swappedBag)
			{
				inventoryItem->swappedBag = true;
				inventoryItem->swappedBag2 = false;
				inventoryItem->swappedBag3 = false;
				inventoryItem->firstTimeSwappedBag = false;
				inventoryItem->firstTimeSwappedBagLeft = true;
				inventoryItem->firstTimeSwappedBagLeft3 = false;
				inventoryItem->LoadElements();
				App->audio->PlayFx(switch_page, 0);
			}

			if (inventoryItem->swappedBag3 && !inventoryItem->swappedBag2)
			{
				inventoryItem->swappedBag = false;
				inventoryItem->swappedBag2 = true;
				inventoryItem->swappedBag3 = false;
				inventoryItem->firstTimeSwappedBag = false;
				inventoryItem->firstTimeSwappedBagLeft3 = false;
				inventoryItem->LoadElements();
				App->audio->PlayFx(switch_page, 0);
			}

			App->gui->resetHoverSwapping = false;
		}
		///////////
		if (inventory->enable && inventoryItem->isVendorInventory &&  App->input->GetControllerAxisPulsation(SDL_CONTROLLER_AXIS_TRIGGERRIGHT) == KEY_DOWN && (inventoryItem->swappedBagVendor || inventoryItem->swappedBag2Vendor))
		{
			if (!inventoryItem->swappedBag3Vendor && inventoryItem->swappedBag2Vendor)
			{
				inventoryItem->swappedBagVendor = false;
				inventoryItem->swappedBag2Vendor = false;
				inventoryItem->swappedBag3Vendor = true;
				inventoryItem->firstTimeSwappedBagLeftVendor = false;
				inventoryItem->firstTimeSwappedBagLeft3Vendor = true;
				inventoryItem->LoadElements(false, true);
				App->audio->PlayFx(switch_page, 0);
			}
			if (!inventoryItem->swappedBag2Vendor && inventoryItem->swappedBagVendor)
			{
				inventoryItem->swappedBagVendor = false;
				inventoryItem->swappedBag2Vendor = true;
				inventoryItem->swappedBag3Vendor = false;
				inventoryItem->firstTimeSwappedBagVendor = false;
				inventoryItem->firstTimeSwappedBagLeftVendor = false;
				inventoryItem->firstTimeSwappedBagLeft3Vendor = false;
				inventoryItem->LoadElements(false, true);
				App->audio->PlayFx(switch_page, 0);
			}
			App->gui->resetHoverSwapping = false;
		}

		if (inventory->enable && inventoryItem->isVendorInventory && App->input->GetControllerAxisPulsation(SDL_CONTROLLER_AXIS_TRIGGERLEFT) == KEY_DOWN && (inventoryItem->swappedBag2Vendor || inventoryItem->swappedBag3Vendor))
		{

			if (inventoryItem->swappedBag2Vendor && !inventoryItem->swappedBagVendor)
			{
				inventoryItem->swappedBagVendor = true;
				inventoryItem->swappedBag2Vendor = false;
				inventoryItem->swappedBag3Vendor = false;
				inventoryItem->firstTimeSwappedBagVendor = false;
				inventoryItem->firstTimeSwappedBagLeftVendor = true;
				inventoryItem->firstTimeSwappedBagLeft3Vendor = false;
				inventoryItem->LoadElements(false, true);
				App->audio->PlayFx(switch_page, 0);
			}

			if (inventoryItem->swappedBag3Vendor && !inventoryItem->swappedBag2Vendor)
			{
				inventoryItem->swappedBagVendor = false;
				inventoryItem->swappedBag2Vendor = true;
				inventoryItem->swappedBag3Vendor = false;
				inventoryItem->firstTimeSwappedBagVendor = false;
				inventoryItem->firstTimeSwappedBagLeft3Vendor = false;
				inventoryItem->LoadElements(false, true);
				App->audio->PlayFx(switch_page, 0);
			}

			App->gui->resetHoverSwapping = false;
		}
		////////////

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
	
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		App->entityFactory->CreateEnemy(EnemyType::ARCHER, { coords.x,coords.y });
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		SDL_Rect zone = { coords.x, coords.y, 30, 30 };
		App->entityFactory->CreateWave(zone, 4, WAVE_TYPE::LEVEL_1);
	}

	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		//App->entityFactory->CreateArrow(App->entityFactory->player->GetSelectedCharacterEntity()->GetThrowingPos(), App->entityFactory->player->position, 100, App->entityFactory->player->GetSelectedCharacterEntity(), PROJECTILE_TYPE::BOSS_EMMITER, 10000u);
		App->entityFactory->CreateBossEmitter(coords.Return_fPoint(), 400u, 200u, App->entityFactory->player, 5000u);
	
	}

	if (hackerMode)
	{
		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		{
		
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
			App->entityFactory->player->gold *= 0.5;
			App->entityFactory->player->str_coin = std::to_string(App->entityFactory->player->gold) + " x";
			App->scene->coins_label->ChangeTextureIdle(App->entityFactory->player->str_coin, NULL, NULL);
			App->SaveGame("save_game.xml");
			isDeath = false;
			ComeToDeath = true;
			App->transitionManager->CreateFadeTransition(1.F,true,SceneState::DEATH);
		}
	}

	App->win->ClearTitle();

	


	if (App->input->GetKey(SDL_SCANCODE_KP_8) == KEY_DOWN)
	{
		App->entityFactory->player->gold += 100000;
		App->entityFactory->player->str_coin = std::to_string(App->entityFactory->player->gold) + " x";
		App->scene->coins_label->ChangeTextureIdle(App->entityFactory->player->str_coin, NULL, NULL);
		App->scene->coins_label_inventory->ChangeTextureIdle(App->entityFactory->player->str_coin, NULL, NULL);

	}


	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;
	DecideTexToPulse();
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
		else if (charFlag == "dialogueBox")
		{
			dialogueBox = App->gui->AddImage(position, &section, nullptr);
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
	wave_label = App->gui->AddLabel("", { 255,255,255,255 }, App->font->piecesofEight36, { 1150,107 }, inGamePanel);
	god_label = App->gui->AddLabel("God Mode", { 255,255,255, 150 }, App->font->openSansBold18, { 1185, 695 }, inGamePanel);
	exp_label = App->gui->AddLabel("LVL 1", { 255,255,255,255 }, App->font->piecesofEight24, { 60,130 }, inGamePanel);
	wave_label->hide = true;
	god_label->hide = true;
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
	coins_label_inventory = App->gui->AddLabel("0 x", { 255,255,255,255 }, App->font->openSansSemiBold24, { 1080,26 }, inventory);

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

		// re set entities data map (create or delete/create if we have a previous one)
		App->entityFactory->CreateEntitiesDataMap(App->map->data.width * 2, App->map->data.height * 2);
	}
}

void j1Scene::UnLoadScene()
{
	if (App->map->IsEnabled())
		App->map->Disable();
	if (App->attackManager->IsEnabled())
		App->attackManager->Disable();
	if (App->entityFactory->IsEnabled())
		App->entityFactory->Disable();
	if (App->pathfinding->IsEnabled())
		App->pathfinding->Disable();
	if (App->buff->IsEnabled())
		App->buff->Disable();
	if (App->camera2D->IsEnabled())
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

	case SceneState::LOBBY:
		state = SceneState::LOBBY;
		if (!App->attackManager->IsEnabled())
			App->attackManager->Enable();
		if (!App->pathfinding->IsEnabled())
			App->pathfinding->Enable();
		if (!App->camera2D->IsEnabled())
			App->camera2D->Enable();
		if (!App->buff->IsEnabled())
			App->buff->Enable();
		if (!App->map->IsEnabled())
		{
			App->map->active = true;
			LoadNewMap("maps/mainhall.tmx");//"maps/test_ordering.tmx"))//level1_Block_rev.tmx"))   // ("maps/iso_walk.tmx")
		}
		App->entityFactory->Enable();
		break;
	case SceneState::LEVEL1:

		state = SceneState::LEVEL1;
		App->attackManager->Enable();
		App->pathfinding->Enable();
		App->camera2D->Enable();
		App->buff->Enable();
		App->map->active = true;
		LoadNewMap("maps/Level1_Final_Borders_Faked.tmx");
		//LoadNewMap("maps/Level2.tmx");
		App->entityFactory->Enable();
		// create player for testing purposes here
		//App->entityFactory->CreatePlayer({ -1563, 1000 });
		//App->entityFactory->CreateTrigger(TRIGGER_TYPE::WIN, 336, 264, SceneState::WIN, White);

		break;

	case SceneState::LEVEL2:
		state = SceneState::LEVEL2;
		App->attackManager->Enable();
		App->pathfinding->Enable();
		App->camera2D->Enable();
		App->buff->Enable();
		App->map->active = true;
		LoadNewMap("maps/Level2.tmx");//"maps/test_ordering.tmx"))//level1_Block_rev.tmx"))   // ("maps/iso_walk.tmx")
		App->entityFactory->Enable();
		// create player for testing purposes here
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
		ComeToWin = true;

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
			if (!App->dialog->isDialogInScreen)  // dont open if inventory in vendor dialog sequence active, open it later
			{
				App->pause = !App->pause;
				if (App->pause && !pausePanel->enable)
				{

					inventory->enable = true;
					App->gui->resetHoverSwapping = false;

					inventoryItem->LoadElements(onlyEquipped, isVendor);
					App->audio->PlayFx(openInventorySFX, 0);


				}

				else
				{
					App->audio->PlayFx(closeinventorySFX, 0);
					inventory->enable = false;
					inventoryItem->swappedBag = true;
					inventoryItem->swappedBag2 = false;
					inventoryItem->swappedBag3 = false;
					inventoryItem->swappedBagVendor = true;
					inventoryItem->swappedBag2Vendor = false;
					inventoryItem->swappedBag3Vendor = false;


					if (App->dialog->isDialogSequenceActive)
					{
						App->dialog->spawnDialoguesAfterInventory();
					}
				}
			}
		}

}



float j1Scene::AlphaIncrease(float alphavalue, int counter)
{
	if (alphavalue < MAX_ALPHA)
	{
		alphavalue += 10;
		if (alphavalue > MAX_ALPHA)
			alphavalue = MAX_ALPHA;
	}

	else if (alphavalue >= MAX_ALPHA)
	{
		decreaseAlpha = true;
	}

	hudAlphavalue[counter] = alphavalue;
	return hudAlphavalue[counter];
}
float j1Scene::AlphaDecrease(float alphavalue, int counter)
{

	if (alphavalue > 0)
	{
		if (previous_counter > counter && counter == 0)
			alphavalue -= 8;
		else if (counter == 1 && previous_counter > counter)
			alphavalue -= 5;

		else alphavalue -= 3;

		if (alphavalue < 0)
			alphavalue = 0;
	}

	else if (alphavalue <= 0)
		hit_counter -= 1;

	hudAlphavalue[counter] = alphavalue;

	return hudAlphavalue[counter];
}

bool j1Scene::DecideTexToPulse()
{
	switch (App->scene->hit_counter)
	{
	case 0:

		App->render->SetTextureAlpha(App->gui->hurt_hud_tex, 0);
		App->render->SetTextureAlpha(App->gui->hurt_hud_tex2, 0);
		App->render->SetTextureAlpha(App->gui->hurt_hud_tex3, 0);
		break;

	case 1:
		if (!App->scene->decreaseAlpha)
		{
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex, AlphaIncrease(hudAlphavalue[0], 0));
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex2, 0);
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex3, 0);
		}

		else
		{
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex, AlphaDecrease(hudAlphavalue[0], 0));
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex2, 0);
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex3, 0);
		}
		break;

	case 2:
		if (!App->scene->decreaseAlpha)
		{
			hudAlphavalue[0] = 255;
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex, hudAlphavalue[0]);
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex2, AlphaIncrease(hudAlphavalue[1], 1));
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex3, 0);
		}
		else
		{
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex, 255);
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex2, AlphaDecrease(hudAlphavalue[1], 1));
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex3, 0);
		}
		break;

	case 3:
		if (!App->scene->decreaseAlpha)
		{
			hudAlphavalue[0] = hudAlphavalue[1] = 255;

			App->render->SetTextureAlpha(App->gui->hurt_hud_tex, hudAlphavalue[0]);
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex2, hudAlphavalue[1]);
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex3, AlphaIncrease(hudAlphavalue[2], 2));
		}

		else
		{
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex, 255);
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex2, 255);
			App->render->SetTextureAlpha(App->gui->hurt_hud_tex3, AlphaDecrease(hudAlphavalue[2], 2));
		}
		break;


	default:
		App->render->SetTextureAlpha(App->gui->hurt_hud_tex, 255);
		App->render->SetTextureAlpha(App->gui->hurt_hud_tex2, 255);
		App->render->SetTextureAlpha(App->gui->hurt_hud_tex3, 255);
		if(timeindmg.Read() > 3000)
			hit_counter -= 1;
		decreaseAlpha = true;
		//App->entityFactory->alphaTimer.Start();
		break;
	}
	return true;
}