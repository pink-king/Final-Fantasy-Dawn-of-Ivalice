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
#include "LootEntity.h"
#include "Video.h"
#include "SDL_mixer/include/SDL_mixer.h"

j1Scene::j1Scene() : j1Module()
{
	name.assign("scene");
	state = SceneState::INTRO;
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
	LType = LOOT_TYPE::CONSUMABLE;
	debug = false;

	if (debug_tex == nullptr)
		debug_tex = App->tex->Load("maps/path2.png");

	hit_counter = 0;
	App->pause = false;
	selectUI = App->audio->LoadFx("audio/fx/UI/selectUI.wav");
	acceptUI = App->audio->LoadFx("audio/fx/UI/AcceptUI.wav");
	purchase = App->audio->LoadFx("audio/fx/UI/purchase.wav");
	sell = App->audio->LoadFx("audio/fx/UI/sell.wav");
	switch_page = App->audio->LoadFx("audio/fx/UI/switch_page.wav");
	// Load SFX
	lootGroundSFX = App->audio->LoadFx("audio/fx/loot/lootgrounded.wav");
	potionGroundSFX = App->audio->LoadFx("audio/fx/loot/potion_grounded.wav");
	coinGroundedSFX = App->audio->LoadFx("audio/fx/loot/coinGrounded.wav");
	swapCharSFX = App->audio->LoadFx("audio/fx/Player/swapChar.wav");
	stepSFX = App->audio->LoadFx("audio/fx/Player/footstep-on-stone.wav");
	enemySpawn = App->audio->LoadFx("audio/fx/enemySpawnTest.wav");
	goblinDetection = App->audio->LoadFx("audio/fx/goblin_detection.wav");

	marcheDamaged = App->audio->LoadFx("audio/fx/Player/Marche/Marche_damaged.wav");
	marcheBasic = App->audio->LoadFx("audio/fx/Player/Marche/marche_basic1Grunt.wav");
	marcheBasic2 = App->audio->LoadFx("audio/fx/Player/Marche/marche_basic2Grunt.wav");
	marcheAbility2 = App->audio->LoadFx("audio/fx/Player/Marche/marche_tornado.wav");
	marcheUltimateScream = App->audio->LoadFx("audio/fx/Player/Marche/marcheUltimate.wav");
	marcheAbility1 = App->audio->LoadFx("audio/fx/Player/Marche/marche_ability1.wav");
	marcheEarthShakeSFX = App->audio->LoadFx("audio/fx/Player/Marche/marche_earthShake.wav");
	marcheTornadoExplosion = App->audio->LoadFx("audio/fx/Player/Marche/marche_tornado_explosion.wav");

	RitzDamaged = App->audio->LoadFx("audio/fx/Player/Ritz/Ritz_damaged.wav");
	RitzBasic = App->audio->LoadFx("audio/fx/Player/Ritz/ritz_basic.wav");
	RitzBasicHit = App->audio->LoadFx("audio/fx/Player/Ritz/ritz_basic_hit.wav");
	RitzMedusa = App->audio->LoadFx("audio/fx/Player/Ritz/ritz_medusa.wav");
	RitzAbility2 = App->audio->LoadFx("audio/fx/Player/Ritz/Ritz_Ability2.wav");
	RitzAbility1 = App->audio->LoadFx("audio/fx/Player/Ritz/ritz_teleport.wav");
	RitzUltimate = App->audio->LoadFx("audio/fx/Player/Ritz/ritz_ultimateTest2.wav");


	SharaDamaged = App->audio->LoadFx("audio/fx/Player/Shara/Shara_damaged.wav");
	sharaBasic = App->audio->LoadFx("audio/fx/Player/Shara/sharaBasic.wav");
	basicBodyImp = App->audio->LoadFx("audio/fx/Player/Shara/arrow_impactBody.wav");
	strech_Shoot = App->audio->LoadFx("audio/fx/Player/Shara/strech&shoot.wav");
	SharaUltimateWoosh = App->audio->LoadFx("audio/fx/Player/Shara/Shara_ultimate_woosh.wav");
	emitter_explodeFire = App->audio->LoadFx("audio/fx/Player/Shara/SharaUltimateGrounding.wav");
	sharaAbility1 = App->audio->LoadFx("audio/fx/Player/Shara/shara_ability1.wav");
	sharaBasic_ImpactsWall = App->audio->LoadFx("audio/fx/Player/Shara/SharaBasicImpact.wav");
	sharaAbility1_ImpactsWall = App->audio->LoadFx("audio/fx/Player/Shara/sharaAbility1_impactswall.wav");
	sharaAbility2_ImpactsWall = App->audio->LoadFx("audio/fx/Player/Shara/sharaAbility2_impactswall.wav");
	sharaAbility2_shoot = App->audio->LoadFx("audio/fx/Player/Shara/SharaAbility2Shoot.wav");

	dash = App->audio->LoadFx("audio/fx/Player/dash.wav");

	goblinDamaged = App->audio->LoadFx("audio/fx/goblin_damaged.wav");
	goblinDeath = App->audio->LoadFx("audio/fx/goblin_death.wav");
	goblinLaugh = App->audio->LoadFx("audio/fx/goblin_laugh.wav");

	BombDeathSFX = App->audio->LoadFx("audio/fx/Enemies/bombDeath.wav");
	bombgetHitSFX = App->audio->LoadFx("audio/fx/Enemies/bombgetHit.wav");
	bombExplodeSFX = App->audio->LoadFx("audio/fx/Enemies/bomb_explode.wav");

	golem_deathSFX = App->audio->LoadFx("audio/fx/Enemies/golem/golem_death.wav");
	golem_impactWallSFX = App->audio->LoadFx("audio/fx/Enemies/golem/golem_impactWall.wav");
	golem_spawnSFX = App->audio->LoadFx("audio/fx/Enemies/golem/golem_spawn.wav");
	golem_spawnAttackSFX = App->audio->LoadFx("audio/fx/Enemies/golem/spawn_attack.wav");

	wave_end = App->audio->LoadFx("audio/fx/Enemies/wave/wave_end.wav");
	wave_start = App->audio->LoadFx("audio/fx/Enemies/wave/wave_start.wav");
	wave_respawn = App->audio->LoadFx("audio/fx/Enemies/wave/wave_spawn.wav");

	boss_flower_basic = App->audio->LoadFx("audio/fx/Enemies/boss_flower/BF_basic.wav");
	boss_flower_BasicImpWall = App->audio->LoadFx("audio/fx/Enemies/boss_flower/BF_basicImpWall.wav");
	boss_flower_deathCirc = App->audio->LoadFx("audio/fx/Enemies/boss_flower/BF_deathcircle_SFX.wav");
	boss_flower_death = App->audio->LoadFx("audio/fx/Enemies/boss_flower/BF_dead.wav");

	portal_appear = App->audio->LoadFx("audio/fx/Portal/portal_appear.wav");
	portal_mantain = App->audio->LoadFx("audio/fx/Portal/portal_mantain.wav");
	portal_vanish = App->audio->LoadFx("audio/fx/Portal/portal_vanish.wav");
	portal_travel = App->audio->LoadFx("audio/fx/Portal/portal_travel.wav");

	pickLoot = App->audio->LoadFx("audio/fx/Player/pickLoot.wav");
	pickGold = App->audio->LoadFx("audio/fx/Player/pickGold.wav");
	consumHealPotion = App->audio->LoadFx("audio/fx/Player/consumPotion.wav");
	pickPotion = App->audio->LoadFx("audio/fx/Player/pickPotion.wav");
	
	openInventorySFX = App->audio->LoadFx("audio/fx/UI/open_inventory.wav");
	closeinventorySFX = App->audio->LoadFx("audio/fx/UI/close_inventory.wav");
	open_PauseMenuSFX = App->audio->LoadFx("audio/fx/open_close_pauseMenu.wav");
	enterGameSFX = App->audio->LoadFx("audio/fx/UI/AcceptEnterGame.wav");
	playerDeath = App->audio->LoadFx("audio/fx/States/player_death.wav");
	typeWriterSFX = App->audio->LoadFx("audio/fx/scene/typewriterSFX.wav");
	savedSFX = App->audio->LoadFx("audio/fx/UI/saveGame.wav");
	jar_breakSFX = App->audio->LoadFx("audio/fx/loot/jarbroken.wav");
	OpenChestSFX = App->audio->LoadFx("audio/fx/loot/chestOpen.wav");

	
	if (state == SceneState::LEVEL1)
	{
		iPoint tileSize = { 32,32 };

		App->entityFactory->CreateTrigger(TRIGGER_TYPE::WIN, App->map->MapToWorld(15, 2).x, App->map->MapToWorld(15, 2).y, SceneState::LOBBY, White,3);

		App->entityFactory->CreatePlayer({ -1575, 2150 });
			
		if (!ComeToPortal)
		{
			SDL_Rect waveZone = { 11 * tileSize.x, 5 * tileSize.y, 10 * tileSize.x, 15 * tileSize.y };
			WaveTrigger* waveTrigg = (WaveTrigger*)App->entityFactory->CreateWaveTrigger(iPoint(-14, 511), waveZone, 1);
			waveTrigg->CreateEntryWall(iPoint(16, 22));
			waveTrigg->CreateEntryWall(iPoint(17, 22));
			waveTrigg->CreateExitWall({ 13,3 });
			waveTrigg->CreateExitWall({ 14,3 });
		}

		App->entityFactory->LoadSpawnGroups();

		//App->entityFactory->CreatePlayer({ -209, 650 });
		App->entityFactory->loadEnemies = true;
		App->camera2D->SetCameraPos({ 3575, -3150 });
		//AcceptUISFX_logic = false;
		inGamePanel->enable = true;
		uiMarche->enable = true;
		uiShara->enable = true;
		uiMarchePortrait->enable = true;
		uiSharaPortrait->enable = true;
		uiRitzPortrait->enable = true;
		uiRitz->enable = true;
		settingPanel->enable = false;
		startMenu->enable = false;

		App->audio->PlayFx(enterGameSFX, 0);
		App->audio->PlayMusic("audio/music/FFDI_Theme_14.ogg", -1);
		if (ComeToPortal)
		{
			App->LoadGame("Portal.xml");
			App->entityFactory->player->GetMarche()->position = { portalPos.x,portalPos.y - 32 };
			App->entityFactory->player->GetShara()->position = { portalPos.x,portalPos.y - 32 };
			App->entityFactory->player->GetRitz()->position = { portalPos.x,portalPos.y - 32 };
			App->entityFactory->CreateTrigger(TRIGGER_TYPE::EXITPORTAL, portalPos.x, portalPos.y);

			App->pathfinding->ActivateTile({ 16,22 });
			iPoint wallPoint = App->map->MapToWorld(16, 22);
			wallPoint.y -= 16; // sprite offset
			App->entityFactory->CreateAsset(EnvironmentAssetsTypes::WALL, { wallPoint.x, wallPoint.y }, { 0,384,64,64 });

			App->pathfinding->ActivateTile({ 17,22 });
			wallPoint = App->map->MapToWorld(17, 22);
			wallPoint.y -= 16; // sprite offset
			App->entityFactory->CreateAsset(EnvironmentAssetsTypes::WALL, { wallPoint.x, wallPoint.y }, { 0,384,64,64 });

			ComeToPortal = false;
		}

		if (ComeToDeath || ComeToWin)
		{
			App->LoadGame("save_game.xml");
		}

		if (ComeToWin)
		{
			ComeToDeath = false;
			ComeToWin = false;
		}

		iPoint PosX = App->map->SubTileMapToWorld(88, 192);
		NoWalkableTrigger* introTrigger = (NoWalkableTrigger*)App->entityFactory->CreateTrigger(TRIGGER_TYPE::NOWALKABLE, PosX.x, PosX.y, SceneState::MAX_STATES, Black, 3, false);
		introTrigger->CreateEntryWall(iPoint(44, 98));
		introTrigger->CreateEntryWall(iPoint(45, 98));
	}

	if (state == SceneState::LEVEL2)
	{
		
		App->entityFactory->CreatePlayer({ -820, 3300 });
		
		App->entityFactory->LoadSpawnGroups();

		App->entityFactory->loadEnemies = true;
		App->camera2D->SetCameraPos(1800, -5000);

		iPoint tileSize = { 32,32 };
		
		if (!ComeToPortal)
		{
			SDL_Rect waveZone = { 17 * tileSize.x, 25 * tileSize.y, 8 * tileSize.x, 14 * tileSize.y };
			WaveTrigger* waveTrigg = (WaveTrigger*)App->entityFactory->CreateWaveTrigger(iPoint(App->map->MapToWorld(25, 26).x, App->map->MapToWorld(25, 26).y), waveZone, 2);
			waveTrigg->CreateEntryWall(iPoint(33, 22));
			waveTrigg->CreateEntryWall(iPoint(33, 21));
			waveTrigg->CreateEntryWall(iPoint(33, 20));
			waveTrigg->CreateExitWall({ 19, 46 });
			waveTrigg->CreateExitWall({ 20, 46 });
			waveTrigg->CreateExitWall({ 21, 46 });
			waveTrigg->CreateExitWall({ 22, 46 });

			DialogTriggerVolatile* dialogParent = (DialogTriggerVolatile*)App->entityFactory->CreateDialogTriggerVolatile(App->map->SubTileMapToWorld(118, 68).x, App->map->SubTileMapToWorld(118, 68).y, "PREBOSS", 7, nullptr);
			App->entityFactory->CreateDialogTriggerVolatile(App->map->SubTileMapToWorld(148, 43).x, App->map->SubTileMapToWorld(148, 43).y, "PREBOSS", 7, dialogParent);

		}

		fPoint PosX = fPoint(App->map->MapToWorld(36, 99).x, App->map->MapToWorld(36, 101).y);
		NoWalkableTrigger* bossTrigger = (NoWalkableTrigger*)App->entityFactory->CreateTrigger(TRIGGER_TYPE::NOWALKABLE, PosX.x, PosX.y,SceneState::MAX_STATES,Black,11,true);
		bossTrigger->CreateEntryWall(iPoint(37, 94));
		bossTrigger->CreateEntryWall(iPoint(36, 94));
		bossTrigger->CreateEntryWall(iPoint(35, 94));
		bossTrigger->CreateEntryWall(iPoint(34, 94));

	
		//AcceptUISFX_logic = false;
		inGamePanel->enable = true;
		uiMarche->enable = true;
		uiShara->enable = true;
		uiMarchePortrait->enable = true;
		uiSharaPortrait->enable = true;
		uiRitzPortrait->enable = true;
		uiRitz->enable = true;
		settingPanel->enable = false;
		startMenu->enable = false;

		App->audio->PlayFx(enterGameSFX, 0);
		App->audio->PlayMusic("audio/music/BRPG_Hell_Spawn_FULL_Loop.ogg", -1);


		if (ComeToPortal)
		{
			App->LoadGame("Portal.xml");
			App->entityFactory->player->GetMarche()->position = { portalPos.x,portalPos.y - 32 };
			App->entityFactory->player->GetShara()->position = { portalPos.x,portalPos.y - 32 };
			App->entityFactory->player->GetRitz()->position = { portalPos.x,portalPos.y - 32 };
			App->entityFactory->CreateTrigger(TRIGGER_TYPE::EXITPORTAL, portalPos.x, portalPos.y);

			App->pathfinding->ActivateTile({ 33,22 });
			iPoint wallPoint = App->map->MapToWorld(33, 22);
			wallPoint.y -= 16; // sprite offset
			App->entityFactory->CreateAsset(EnvironmentAssetsTypes::WALL, { wallPoint.x, wallPoint.y }, { 0,873,64,64 });

			App->pathfinding->ActivateTile({ 33,21 });
			wallPoint = App->map->MapToWorld(33, 21);
			wallPoint.y -= 16; // sprite offset
			App->entityFactory->CreateAsset(EnvironmentAssetsTypes::WALL, { wallPoint.x, wallPoint.y }, { 0,873,64,64 });

			App->pathfinding->ActivateTile({ 33,20 });
			wallPoint = App->map->MapToWorld(33, 20);
			wallPoint.y -= 16; // sprite offset
			App->entityFactory->CreateAsset(EnvironmentAssetsTypes::WALL, { wallPoint.x, wallPoint.y }, { 0,873,64,64 });

			ComeToPortal = false;
		}
		if (ComeToDeath || ComeToWin)
		{
			App->LoadGame("save_game.xml");
		}

		if (ComeToWin)
		{
			ComeToDeath = false;
			ComeToWin = false;
		}

	}

	if (state == SceneState::LOBBY)
	{

		open_doorSFX = App->audio->LoadFx("audio/fx/scene/opendoor.wav");
		App->audio->PlayMusic("audio/music/main_hall.ogg", -1);

		App->entityFactory->CreatePlayer({ 115, 240 });

		if (lobbyState == LobbyState::ALLBLOCK)
		{
			doorlvl1 = nullptr;
			firingrange = nullptr;
			doorlvl2 = nullptr;
		}
		
		else if (lobbyState == LobbyState::TALKSTRANGER || lobbyState == LobbyState::PASSLVL1 || lobbyState == LobbyState::PASSLVL2)
		{
	
			doorlvl1 = App->entityFactory->CreateTrigger(TRIGGER_TYPE::DOOR, App->map->SubTileMapToWorld(18, 4).x, App->map->SubTileMapToWorld(18, 4).y, SceneState::LEVEL1, Black, 2, true);
			firingrange = App->entityFactory->CreateTrigger(TRIGGER_TYPE::DOOR, App->map->SubTileMapToWorld(11, 13).x, App->map->SubTileMapToWorld(11, 13).y, SceneState::FIRINGRANGE, Black, 2, false);

			if (lobbyState == LobbyState::PASSLVL1 || lobbyState == LobbyState::PASSLVL2)
				doorlvl2 = App->entityFactory->CreateTrigger(TRIGGER_TYPE::DOOR, App->map->SubTileMapToWorld(26, 4).x, App->map->SubTileMapToWorld(26, 4).y, SceneState::LEVEL2, Black, 2, true);

			if (lobbyState == LobbyState::PASSLVL2 && ComeToPortal)
			{
				ComeToPortal = false;
				App->entityFactory->CreateTrigger(TRIGGER_TYPE::EXITPORTAL, 96, 290);
				App->LoadGame("save_game.xml");
				lobbyState = LobbyState::PASSLVL1;
			}
			else if (ComeToPortal && lobbyState != LobbyState::PASSLVL2)
			{
				ComeToPortal = false;
				App->entityFactory->loadEnemies = false;
				App->LoadGame("Portal.xml");
				App->entityFactory->CreateTrigger(TRIGGER_TYPE::LOBBYPORTAL, 96, 290, previosState, White);
			}

			if (ComeToDeath || ComeToWin)
			{
				App->LoadGame("save_game.xml");
			}

			if (ComeToWin)
			{
				ComeToDeath = false;
				ComeToWin = false;
			}

		}
		App->entityFactory->CreateTrigger(TRIGGER_TYPE::SAVE, 105, 385);

		App->entityFactory->CreateDialogTrigger(App->map->SubTileMapToWorld(11, 21).x, App->map->SubTileMapToWorld(11, 21).y, "VENDOR", { App->map->SubTileMapToWorld(5, 18).x - 5, App->map->SubTileMapToWorld(5, 18).y }, 1);
		strangerDialog = App->entityFactory->CreateDialogTrigger(App->map->SubTileMapToWorld(13, 10).x, App->map->SubTileMapToWorld(13, 10).y, "STRANGER", { App->map->SubTileMapToWorld(8, 6).x + 5, App->map->SubTileMapToWorld(8, 6).y }, 2);


		App->entityFactory->loadEnemies = false;
		inGamePanel->enable = true;
		settingPanel->enable = false;
		startMenu->enable = false;

		// This is when it enters, this is tenebrous, we dont need this in lobby
		//App->audio->PlayFx(enterGameSFX, 0);

		//App->audio->PlayMusic("audio/music/BRPG_Hell_Spawn_FULL_Loop.ogg", -1);

		App->camera2D->SetCameraPos({ 115, 240 });


	}

	if (state == SceneState::FIRINGRANGE)
	{
		App->entityFactory->CreateTrigger(TRIGGER_TYPE::WIN, 340, 700, SceneState::LOBBY, Black,4);    // TODO: adjust trigger to lobby
		App->entityFactory->CreateDialogTrigger(App->map->SubTileMapToWorld(26, 33).x, App->map->SubTileMapToWorld(26, 33).y, "TUTORIAL", { App->map->SubTileMapToWorld(23, 30).x, App->map->SubTileMapToWorld(23, 30).y },2);   // todo: proper position 

		

		App->audio->PlayMusic("audio/music/main_hall.ogg", -1);

		App->entityFactory->CreatePlayer({ 165, 580 });
		
		//AcceptUISFX_logic = false;
		/*App->entityFactory->CreateDialogTrigger(-135, 262, "VENDOR");              // TODO: NPC Tutorial dialog trigger
		App->entityFactory->CreateDialogTrigger(90, 189, "STRANGER");*/

		
		App->entityFactory->CreateEnemy(EnemyType::DUMMY, iPoint(-240.F, 401.F));

		App->entityFactory->loadEnemies = false;
		inGamePanel->enable = true;

		uiMarche->enable = true;
		uiShara->enable = true;
		uiRitz->enable = true;
		settingPanel->enable = false;
		startMenu->enable = false;

		App->audio->PlayFx(enterGameSFX, 0);
		App->camera2D->SetCameraPos({ 115, 240 });

		App->LoadGame("save_game.xml");
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
			LoadCredits(sceneNode);
			LoadInventory(sceneNode);
			LoadDeathScreen(sceneNode);
			LoadWinScreen(sceneNode);
			LoadControls(sceneNode);

			LoadedUi = true;
		}
		startMenu->enable = true;
		uiMarche->enable = false;
		uiShara->enable = false;
		uiRitz->enable = false;
	//	creditsPanel->enable = false;
  //  	controlsPanel->enable = false;
		uiMarchePortrait->enable = false;
		uiSharaPortrait->enable = false;
		uiRitzPortrait->enable = false;
		settingPanel->enable = false;
		inGamePanel->enable = false;
		pausePanel->enable = false;
		inventory->enable = false;
		deathPanel->enable = false;
		winPanel->enable = false;

		App->audio->PlayMusic("audio/music/menu_1.0.ogg"); // Added fade in
	}
	if (state == SceneState::INTRO)
	{
		App->video->PlayVideo("intro/PinkKingGamesIntro.mp4", SceneState::STARTMENU);

		// UI Crashes if not loaded this
		App->gui->resetHoverSwapping = false;
		if (!LoadedUi)
		{
			LoadInGameUi(sceneNode);
			LoadStartMenu(sceneNode);
			LoadPlayerUi(sceneNode);
			LoadSettings(sceneNode);
			LoadPauseSettings(sceneNode);
			LoadCredits(sceneNode);
			LoadInventory(sceneNode);
			LoadDeathScreen(sceneNode);
			LoadWinScreen(sceneNode);
			LoadControls(sceneNode);
			LoadedUi = true;
		}

		startMenu->enable = false;
		uiMarche->enable = false;
		uiShara->enable = false;
		uiRitz->enable = false;
		uiMarchePortrait->enable = false;
		uiSharaPortrait->enable = false;
		uiRitzPortrait->enable = false;
		settingPanel->enable = false;
		inGamePanel->enable = false;
		pausePanel->enable = false;
		inventory->enable = false;
		creditsPanel->enable = false;
		controlsPanel->enable = false;
		deathPanel->enable = false;
		winPanel->enable = false;
	}

	if (state == SceneState::DEATH)
	{
		App->audio->PlayFx(playerDeath, 0);
		App->gui->resetHoverSwapping = false;
		if (inGamePanel->enable)
			inGamePanel->enable = false;
		if (!deathPanel->enable)
			deathPanel->enable = true;
		uiMarche->enable = false;
		uiShara->enable = false;
		uiRitz->enable = false;
		uiMarchePortrait->enable = false;
		uiSharaPortrait->enable = false;
		uiRitzPortrait->enable = false;

		ComeToDeath = true;
		isDeath = false;
	}

	if (state == SceneState::WIN)
	{
		App->video->PlayVideo("intro/creditsVideo.mp4", SceneState::LOBBY);
		//App->audio->PlayMusic("audio/music/win.ogg", -1);

		if (ComeToDeath)
		{
			ComeToDeath = false;
		}

		App->gui->resetHoverSwapping = false;
	

		inGamePanel->enable = false;

		uiMarche->enable = false;
		uiShara->enable = false;
		uiRitz->enable = false;
		uiMarchePortrait->enable = false;
		uiSharaPortrait->enable = false;
		uiRitzPortrait->enable = false;
		
		/*if (!winPanel->enable)
			winPanel->enable = true;*/
		
	}

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

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) //if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
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

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		App->transitionManager->CreateFadeTransition(2.F, true, SceneState::WIN, White);
	}



	//if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)     
	//	App->win->SetScale(2);

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
	// how to get joysticks for correct UI navigation/whatever needed situation examples --------
	if (App->input->GetJoystickPulsation(JOY_STICK_LEFT, JOYSTICK_DIR_UP) == KEY_DOWN)
	{
		//LOG("LEFT JOYSTICK, UP PRESS");
	}
	if (App->input->GetJoystickPulsation(JOY_STICK_LEFT, JOYSTICK_DIR_RIGHT) == KEY_DOWN)
	{
		//LOG("LEFT JOYSTICK, RIGHT PRESS");
	}
	if (App->input->GetJoystickPulsation(JOY_STICK_LEFT, JOYSTICK_DIR_RIGHT) == KEY_UP)
	{
		//LOG("LEFT JOYSTICK, RIGHT UNPRESS");
	}
	if (App->input->GetJoystickPulsation(JOY_STICK_RIGHT, JOYSTICK_DIR_LEFT) == KEY_DOWN)
	{
		//LOG("RIGHT JOYSTICK, LEFT PRESS");
	}
	if (App->input->GetJoystickPulsation(JOY_STICK_RIGHT, JOYSTICK_DIR_UP) == KEY_DOWN)
	{
		//LOG("RIGHT JOYSTICK, UP PRESS");
	}
	if (App->input->GetJoystickPulsation(JOY_STICK_RIGHT, JOYSTICK_DIR_DOWN) == KEY_REPEAT)
	{
		//LOG("RIGHT JOYSTICK, DOWN REPEAT");
	}
	// etc
	// -------------------------------------------------------------------------------------------

	int mx, my;

	App->input->GetMousePosition(mx, my);
	iPoint mousePos = App->render->ScreenToWorld(mx, my);
	//LOG("mousePos: %i,%i", mousePos.x, mousePos.y);
	mousePos = App->map->WorldToMap(mousePos.x, mousePos.y);
	//LOG("mousePosMap: %i,%i", mousePos.x, mousePos.y);
	
	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN && hackerMode && App->entityFactory->active)
	{
		App->entityFactory->RepeatAmountofConsumables(1,{ (float)App->entityFactory->player->position.x, (float)App->entityFactory->player->position.y},OBJECT_TYPE::PHOENIX_TAIL);
		
	}
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN && hackerMode && App->entityFactory->active)
	{
		
		App->entityFactory->CreateLegendariEquipable({ (float)App->entityFactory->player->GetPivotPos().x ,(float)App->entityFactory->player->GetPivotPos().y }, EQUIPABLE_TYPE::SWORD);
		/*App->entityFactory->CreateLegendariEquipable({ (float)App->entityFactory->player->GetPivotPos().x ,(float)App->entityFactory->player->GetPivotPos().y }, EQUIPABLE_TYPE::BOW);
		App->entityFactory->CreateLegendariEquipable({ (float)App->entityFactory->player->GetPivotPos().x ,(float)App->entityFactory->player->GetPivotPos().y }, EQUIPABLE_TYPE::ROD);
		App->entityFactory->CreateLegendariEquipable({ (float)App->entityFactory->player->GetPivotPos().x ,(float)App->entityFactory->player->GetPivotPos().y }, EQUIPABLE_TYPE::ARMOR);
		App->entityFactory->CreateLegendariEquipable({ (float)App->entityFactory->player->GetPivotPos().x ,(float)App->entityFactory->player->GetPivotPos().y }, EQUIPABLE_TYPE::VEST);
		App->entityFactory->CreateLegendariEquipable({ (float)App->entityFactory->player->GetPivotPos().x ,(float)App->entityFactory->player->GetPivotPos().y }, EQUIPABLE_TYPE::MANTLE);*/

	/*	App->entityFactory->RepeatAmountofConsumables(1, { (float)App->entityFactory->player->GetPivotPos().x ,(float)App->entityFactory->player->GetPivotPos().y }, OBJECT_TYPE::GOLD);
		App->entityFactory->RepeatAmountofConsumables(1, { (float)App->entityFactory->player->GetPivotPos().x ,(float)App->entityFactory->player->GetPivotPos().y }, OBJECT_TYPE::POTIONS);
		App->entityFactory->RepeatAmountofConsumables(1, { (float)App->entityFactory->player->GetPivotPos().x ,(float)App->entityFactory->player->GetPivotPos().y }, OBJECT_TYPE::PHOENIX_TAIL);*/
		App->entityFactory->RepeatAmountofEquipable(1, { (float)App->entityFactory->player->GetPivotPos().x, (float)App->entityFactory->player->GetPivotPos().y }, EQUIPABLE_TYPE::SWORD);
	/*	App->entityFactory->RepeatAmountofEquipable(1, { (float)App->entityFactory->player->GetPivotPos().x, (float)App->entityFactory->player->GetPivotPos().y }, EQUIPABLE_TYPE::BOW);
		App->entityFactory->RepeatAmountofEquipable(1, { (float)App->entityFactory->player->GetPivotPos().x, (float)App->entityFactory->player->GetPivotPos().y }, EQUIPABLE_TYPE::ROD);
		App->entityFactory->RepeatAmountofEquipable(1, { (float)App->entityFactory->player->GetPivotPos().x, (float)App->entityFactory->player->GetPivotPos().y }, EQUIPABLE_TYPE::ARMOR);
		App->entityFactory->RepeatAmountofEquipable(1, { (float)App->entityFactory->player->GetPivotPos().x, (float)App->entityFactory->player->GetPivotPos().y }, EQUIPABLE_TYPE::VEST);
		App->entityFactory->RepeatAmountofEquipable(1, { (float)App->entityFactory->player->GetPivotPos().x, (float)App->entityFactory->player->GetPivotPos().y }, EQUIPABLE_TYPE::MANTLE);*/

	
	}
	App->tex->textures;
	// map debug draw grids

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");


	if (App->input->GetKey(SDL_SCANCODE_KP_5) == KEY_DOWN)
	{
		Trigger* trigger = App->entityFactory->CreateTrigger(TRIGGER_TYPE::PORTAL, App->entityFactory->player->position.x, App->entityFactory->player->position.y, SceneState::LOBBY, White);
	}
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
		App->camera2D->camera.y += 1000 * dt;

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
		App->camera2D->camera.y -= 1000 * dt;

	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		App->camera2D->camera.x += 1000 * dt;

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
		App->camera2D->camera.x -= 1000 * dt;




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

	if (App->input->GetKey(SDL_SCANCODE_KP_9) == KEY_DOWN)
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint coords = App->render->ScreenToWorld(x, y);
		App->entityFactory->CreateLoot(coords.x, coords.y);
	}
	if (state == SceneState::STARTMENU)
	{
		result_volume = volume_bar->GetBarValue();
		App->audio->SetVolume(result_volume);
		
		result_fx = fx_bar->GetBarValue();
		App->audio->SetFxVolume(result_fx);
		//settingPanel->enable = false;
	
		if (settingPanel->enable &&  App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			App->gui->GoBackToMenu();
		}
		if (creditsPanel->enable &&  App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			App->gui->GoBackToMenu();
		}
	}

	if (state == SceneState::LOBBY)
	{
		if (App->input->GetKey(SDL_SCANCODE_KP_2) == KEY_DOWN)
		{
			lobbyState = LobbyState::PASSLVL1;
			if (doorlvl1 == nullptr)
				doorlvl1 = App->entityFactory->CreateTrigger(TRIGGER_TYPE::DOOR, App->map->SubTileMapToWorld(18, 4).x, App->map->SubTileMapToWorld(18, 4).y, SceneState::LEVEL1, Black, 2, true);
			if (doorlvl2 == nullptr)
				doorlvl2 = App->entityFactory->CreateTrigger(TRIGGER_TYPE::DOOR, App->map->SubTileMapToWorld(26, 4).x, App->map->SubTileMapToWorld(26, 4).y, SceneState::LEVEL2, Black, 2, true);
			if(firingrange == nullptr)
				firingrange = App->entityFactory->CreateTrigger(TRIGGER_TYPE::DOOR, App->map->SubTileMapToWorld(11, 13).x, App->map->SubTileMapToWorld(11, 13).y, SceneState::FIRINGRANGE, Black, 2, false);
		}
		if (lobbyState == LobbyState::TALKSTRANGER)
		{
			if (doorlvl1 == nullptr)
				doorlvl1 = App->entityFactory->CreateTrigger(TRIGGER_TYPE::DOOR, App->map->SubTileMapToWorld(18, 4).x, App->map->SubTileMapToWorld(18, 4).y, SceneState::LEVEL1, Black, 2, true);
			if (firingrange == nullptr)
				firingrange = App->entityFactory->CreateTrigger(TRIGGER_TYPE::DOOR, App->map->SubTileMapToWorld(11, 13).x, App->map->SubTileMapToWorld(11, 13).y, SceneState::FIRINGRANGE, Black, 2, false);
		}
	}

	if (state == SceneState::LEVEL1 || state == SceneState::LEVEL2 || state == SceneState::LOBBY || state== SceneState::FIRINGRANGE)
	{
		//debug tp
		if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
			DebugTP(SceneState::LOBBY, LvlPart::NO_PART);

		if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
			DebugTP(SceneState::FIRINGRANGE, LvlPart::NO_PART);

		if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
			DebugTP(SceneState::LEVEL1, LvlPart::START);

		if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
			DebugTP(SceneState::LEVEL1, LvlPart::WAVES);

		if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
			DebugTP(SceneState::LEVEL2, LvlPart::START);

		if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
			DebugTP(SceneState::LEVEL2, LvlPart::WAVES);

		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
			DebugTP(SceneState::LEVEL2, LvlPart::BOSS);

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
				if (state != SceneState::LOBBY)
				{
					uiMarche->enable = true;
					uiRitz->enable = false;
					uiShara->enable = false;
				}
				else
				{
					uiMarche->enable = false;
					uiRitz->enable = false;
					uiShara->enable = false;
				}
				uiMarchePortrait->enable = true;
				uiRitzPortrait->enable = false;
				uiSharaPortrait->enable = false;
			}
			if (App->entityFactory->player->selectedCharacterEntity->character == characterName::RITZ && inGamePanel->enable)
			{
				if (state != SceneState::LOBBY)
				{
					uiMarche->enable = false;
					uiRitz->enable = true;
					uiShara->enable = false;
				}
				else
				{
					uiMarche->enable = false;
					uiRitz->enable = false;
					uiShara->enable = false;
				}
				uiMarchePortrait->enable = false;
				uiRitzPortrait->enable = true;
				uiSharaPortrait->enable = false;
			}
			if (App->entityFactory->player->selectedCharacterEntity->character == characterName::SHARA && inGamePanel->enable)
			{
				if (state != SceneState::LOBBY)
				{
					uiMarche->enable = false;
					uiRitz->enable = false;
					uiShara->enable = true;
				}
				else
				{
					uiMarche->enable = false;
					uiRitz->enable = false;
					uiShara->enable = false;
				}
				uiMarchePortrait->enable = false;
				uiRitzPortrait->enable = false;
				uiSharaPortrait->enable = true;
			}
		}

		if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_START) == KEY_DOWN)
		{
			if (!inventory->enable && !App->dialog->isDialogSequenceActive)
			{

				App->pause = !App->pause;
				if (App->pause)
				{
					//Mix_PauseMusic();
					if (!pausePanel->enable)
						App->audio->PlayFx(open_PauseMenuSFX, 0);

					pausePanel->enable = true;
					paused = true;
					App->gui->resetHoverSwapping = false;
					App->entityFactory->player->LockPlayerInput();

					//App->gui->HideInGameEnemyUI(true);
				}
				else
				{
					//Mix_ResumeMusic();
					App->gui->resetHoverSwapping = false;
					App->gui->selected_object->state = IDLE;
					App->gui->selected_object = nullptr;
					
					App->gui->GoBackToGame();

					//App->gui->HideInGameEnemyUI(false);
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

		if (settingPanel->enable &&  App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			App->gui->GoBackToMenu();
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
				inventoryItem->firstTimeSwappedBagVendor = true;
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
				inventoryItem->firstTimeSwappedBagVendor = true;
				inventoryItem->firstTimeSwappedBagLeft3Vendor = false;
				inventoryItem->LoadElements(false, true);
				App->audio->PlayFx(switch_page, 0);
			}

			App->gui->resetHoverSwapping = false;
		}
		////////////
		potion_counterIg = 0;
		phoenix_counterIg = 0;
		for (std::vector<LootEntity*>::iterator iter = App->entityFactory->player->consumables.begin(); iter != App->entityFactory->player->consumables.end(); ++iter)
		{
			// Potions
			if ((*iter)->objectType == OBJECT_TYPE::POTIONS)
			{
				potion_counterIg++;
				str_potionIg = "" + std::to_string(potion_counterIg);
				potionIg_label->ChangeTextureIdle(str_potionIg, NULL, NULL);
			}
			if ((*iter)->objectType == OBJECT_TYPE::PHOENIX_TAIL)
			{
				phoenix_counterIg++;
				str_phoenixIg = "" + std::to_string(phoenix_counterIg);
				phoenixIg_label->ChangeTextureIdle(str_phoenixIg, NULL, NULL);
			}
		}
		if (potion_counterIg == 0)
		{
			str_potionIg = "0";
			potionIg_label->ChangeTextureIdle(str_potionIg, NULL, NULL);
		}
		if (phoenix_counterIg == 0)
		{
			str_phoenixIg = "0";
			phoenixIg_label->ChangeTextureIdle(str_phoenixIg, NULL, NULL);
		}
		if (App->input->GetKey(SDL_SCANCODE_KP_7) == KEY_DOWN)
		{
			if (!swapAbilities)
			{
				for (std::list<UiItem*>::iterator item = App->gui->ListItemUI.begin(); item != App->gui->ListItemUI.end(); ++item)
				{
					if ((*item)->parent == uiMarche || (*item)->parent == uiRitz || (*item)->parent == uiShara)
					{
						if ((!App->gui->spawnedClocks.Marche.dodge || !App->gui->spawnedClocks.Ritz.dodge || !App->gui->spawnedClocks.Shara.dodge) && !dodge)
						{
							App->gui->allclocksData.dodge.position.x += 909;
							dodge = true;
						}
						else if ((!App->gui->spawnedClocks.Marche.special1 || !App->gui->spawnedClocks.Ritz.special1 || !App->gui->spawnedClocks.Shara.special1) && !ability1)
						{
							App->gui->allclocksData.ability1.position.x += 909;
							ability1 = true;
						}
						else if ((!App->gui->spawnedClocks.Marche.special2 || !App->gui->spawnedClocks.Ritz.special2 || !App->gui->spawnedClocks.Shara.special2) && !ability2)
						{
							App->gui->allclocksData.ability2.position.x += 909;
							ability2 = true;
						}
						else if ((!App->gui->spawnedClocks.Marche.ulti || !App->gui->spawnedClocks.Ritz.ulti || !App->gui->spawnedClocks.Shara.ulti) && !ulti)
						{
							App->gui->allclocksData.ulti.position.x += 909;
							ulti = true;
						}

						(*item)->hitBox.x += 909;


					}
					if ((*item)->parent == inGamePanel && (*item)->swapPosition)
					{
						(*item)->hitBox.x -= 1088;
					}



				}
				swapAbilities = true;
				dodge = false;
				ability1 = false;
				ability2 = false;
				ulti = false;
			}
			else
			{
				for (std::list<UiItem*>::iterator item = App->gui->ListItemUI.begin(); item != App->gui->ListItemUI.end(); ++item)
				{
					if ((*item)->parent == uiMarche || (*item)->parent == uiRitz || (*item)->parent == uiShara)
					{
						if ((!App->gui->spawnedClocks.Marche.dodge || !App->gui->spawnedClocks.Ritz.dodge || !App->gui->spawnedClocks.Shara.dodge) && !dodge_rv)
						{
							App->gui->allclocksData.dodge.position.x -= 909;
							dodge_rv = true;
						}
						else if ((!App->gui->spawnedClocks.Marche.special1 || !App->gui->spawnedClocks.Ritz.special1 || !App->gui->spawnedClocks.Shara.special1) && !ability1_rv)
						{
							App->gui->allclocksData.ability1.position.x -= 909;
							ability1_rv = true;
						}
						else if ((!App->gui->spawnedClocks.Marche.special2 || !App->gui->spawnedClocks.Ritz.special2 || !App->gui->spawnedClocks.Shara.special2) && !ability2_rv)
						{
							App->gui->allclocksData.ability2.position.x -= 909;
							ability2_rv = true;
						}
						else if ((!App->gui->spawnedClocks.Marche.ulti || !App->gui->spawnedClocks.Ritz.ulti || !App->gui->spawnedClocks.Shara.ulti) && !ulti_rv)
						{
							App->gui->allclocksData.ulti.position.x -= 909;
							ulti_rv = true;
						}

						(*item)->hitBox.x -= 909;
					}
					if ((*item)->parent == inGamePanel && (*item)->swapPosition)
					{
						(*item)->hitBox.x += 1088;
					}
				}
				swapAbilities = false;
				dodge_rv = false;
				ability1_rv = false;
				ability2_rv = false;
				ulti_rv = false;
			}

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
	if (App->map->active)
		App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->camera2D->camera.x, y - App->camera2D->camera.y);
	iPoint coords = App->render->ScreenToWorld(x, y);

	
	// -------------- Enemies -----------------
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

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		App->entityFactory->CreateEntity(FLOWERBOSS, coords.x, coords.y, "flower_boss");
	}

	// ----------------------------------------


	if (hackerMode)
	{
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

		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		{

			App->entityFactory->CreateArrow(App->entityFactory->player->GetSelectedCharacterEntity()->GetThrowingPos(), fPoint{ (float)coords.x, (float)coords.y }, 100, App->entityFactory->player->GetSelectedCharacterEntity(), PROJECTILE_TYPE::CONTAGIOUS_ARROW);

		}
		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			App->entityFactory->CreateArrow(App->entityFactory->player->GetSelectedCharacterEntity()->GetThrowingPos(), fPoint{ (float)coords.x, (float)coords.y }, 100, App->entityFactory->player->GetSelectedCharacterEntity(), PROJECTILE_TYPE::BASIC_ARROW);
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
			App->transitionManager->CreateFadeTransition(1.F, true, SceneState::DEATH);
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


	UpdateConsumable();
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;
	DecideTexToPulse();
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (exitGame)
		return false;

	if (canExecuteChainAnim)
		executeAnimChain();

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	App->tex->UnLoad(debug_tex);
	debug_tex = nullptr;



	LOG("consumableInfo Size %i", consumableinfo.size());
	consumableinfo.clear();

	LOG("Freeing scene");
	return true;
}

void j1Scene::LoadUiElement(UiItem* parent, pugi::xml_node node)
{

	// images
	for (pugi::xml_node uiNode = node.child("images").child("image"); uiNode; uiNode = uiNode.next_sibling("image"))
	{
		std::string name_image = uiNode.attribute("name").as_string();

		SDL_Rect section = { uiNode.child("section").attribute("x").as_int(), uiNode.child("section").attribute("y").as_int(), uiNode.child("section").attribute("w").as_int(), uiNode.child("section").attribute("h").as_int() };
		iPoint position = { uiNode.child("position").attribute("x").as_int(), uiNode.child("position").attribute("y").as_int() };

		int isTabbable = uiNode.child("flag").attribute("isTabbable").as_int();
		int autorefresh = uiNode.child("flag").attribute("autorefresh").as_int();
		int isSwappable = uiNode.child("flag").attribute("isSwappable").as_int();
		std::string panelName = uiNode.child("flag").attribute("panelName").as_string();

		std::string doNotCreate = uiNode.child("donotdoitflag").attribute("value").as_string();
		std::string lootFlag = uiNode.child("flag").attribute("value").as_string();
		std::string charFlag = uiNode.child("charFlag").attribute("value").as_string();
		if (autorefresh == 1)
		{
			LOG("HHHHHHHHHHH");
		}
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
			MarcheIcon = App->gui->AddImage(position, &section, name_image, nullptr);
		}
		else if (charFlag == "Ritz")
		{
			RitzIcon = App->gui->AddImage(position, &section, name_image, nullptr);
		}
		else if (charFlag == "Shara")
		{
			SharaIcon = App->gui->AddImage(position, &section, name_image, nullptr);
		}
		else if (lootFlag == "dialogueBox")
		{
			dialogueBox = App->gui->AddImage(iPoint(500, 500), &section, name_image, inGamePanel);
			dialogueBox->hide = true;
		}
		else
		{                                  // this is useless now
			

			if (doNotCreate != "true")
			{
				App->gui->AddImage(position, &section, name_image, parent, isSwappable, isTabbable, autorefresh);  // bug: an image is created as panel 
			}
			else
			{

			}

		}

		// enemy skulls
		std::string name = uiNode.attribute("name").as_string();
		if (name == "skullBasic")
		{
			App->gui->enemySkullInfo.baseEnemyRect = section;
		}
		else if (name == "skullStrong")
		{
			App->gui->enemySkullInfo.strongEnemyRect = section;
		}
		else if (name == "skullBoss")
		{
			App->gui->enemySkullInfo.BossEnemyRect = section;
		}

		// character stats icons
		if (name == "upgradeAttack")
		{
			App->gui->upgradeCharacterStatsIconsRects.attack.upgrade = section;
		}
		else if (name == "downgradeAttack")
		{
			App->gui->upgradeCharacterStatsIconsRects.attack.Downgrade = section;
		}
		else if (name == "upgradeDefense")
		{
			App->gui->upgradeCharacterStatsIconsRects.defense.upgrade = section;
		}
		else if (name == "downgradeDefense")
		{
			App->gui->upgradeCharacterStatsIconsRects.defense.Downgrade = section;
		}
		else if (name == "upgradeCooldown")
		{
			App->gui->upgradeCharacterStatsIconsRects.cooldown.upgrade = section;
		}
		else if (name == "downgradeCooldown")
		{
			App->gui->upgradeCharacterStatsIconsRects.cooldown.Downgrade = section;
		}
		else if (name == "upgradeHP")
		{
			App->gui->upgradeCharacterStatsIconsRects.HP.upgrade = section;
		}
		else if (name == "downgradeHP")
		{
			App->gui->upgradeCharacterStatsIconsRects.HP.Downgrade = section;
		}
		else if (name == "upgradeVelocity")
		{
			App->gui->upgradeCharacterStatsIconsRects.velocity.upgrade = section;
		}
		else if (name == "downgradeVelocity")
		{
			App->gui->upgradeCharacterStatsIconsRects.velocity.Downgrade = section;
		}

	}


	//button
	for (pugi::xml_node uiNode = node.child("buttons").child("button"); uiNode; uiNode = uiNode.next_sibling("button"))
	{
		std::string name = uiNode.attribute("name").as_string();
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



		App->gui->AddButton(position, functionPath, name, &sectionIdle, parent, sectionClick, sectionHove);
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

	for (pugi::xml_node uiNode = node.child("checkboxes").child("checkbox"); uiNode; uiNode = uiNode.next_sibling("checkbox"))
	{
		std::string name = uiNode.attribute("name").as_string();
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

		SDL_Rect tick;
		iPoint tick_pos;
		std::string name_tick;
		if (pugi::xml_node tickSecNode = uiNode.child("tickSec"))
		{
			tick = { tickSecNode.attribute("x").as_int(), tickSecNode.attribute("y").as_int(), tickSecNode.attribute("w").as_int(), tickSecNode.attribute("h").as_int() };
			name_tick = "tick";
			tick_pos = { position.x + 14, position.y + 11 };
		}


		
		App->gui->AddCheckbox(position, functionPath, name, &sectionIdle, parent, sectionClick, sectionHove, &tick);
		tick_image = App->gui->AddImage(tick_pos, &tick, name_tick, parent);
	}



	// health bar 

	for (pugi::xml_node uiNode = node.child("healthbars").child("healthbar"); uiNode; uiNode = uiNode.next_sibling("healthbar"))
	{


		SDL_Rect dynamicSection = { uiNode.child("dynamicSection").attribute("x").as_int(), uiNode.child("dynamicSection").attribute("y").as_int(), uiNode.child("dynamicSection").attribute("w").as_int(), uiNode.child("dynamicSection").attribute("h").as_int() };
		iPoint position = { uiNode.child("position").attribute("x").as_int(), uiNode.child("position").attribute("y").as_int() };

		std::string variant = uiNode.child("type").attribute("value").as_string();

		if (variant == "player")
		{
			SDL_Rect staticSection = { uiNode.child("test").attribute("x").as_int(), uiNode.child("test").attribute("y").as_int(), uiNode.child("test").attribute("w").as_int(), uiNode.child("test").attribute("h").as_int() };

			SDL_Rect damageSection = { uiNode.child("damageSection").attribute("x").as_int(), uiNode.child("damageSection").attribute("y").as_int(), uiNode.child("damageSection").attribute("w").as_int(), uiNode.child("damageSection").attribute("h").as_int() };
			
			App->gui->healthBar = App->gui->AddHealthBar(position, &dynamicSection, &damageSection, &staticSection, type::player, inGamePanel);
		}
		else if (variant == "enemy")
		{
			App->gui->enemyLifeBarInfo.dynamicSection = dynamicSection;
		}
		else if (variant == "boss")
		{
			App->gui->bossHealthBarInfo.dynamicSection = dynamicSection;

			SDL_Rect staticSection = { uiNode.child("staticSection").attribute("x").as_int(), uiNode.child("staticSection").attribute("y").as_int(), uiNode.child("staticSection").attribute("w").as_int(), uiNode.child("staticSection").attribute("h").as_int() };
			App->gui->bossHealthBarInfo.staticSection = staticSection;
			SDL_Rect divSection = { uiNode.child("divSection").attribute("x").as_int(), uiNode.child("divSection").attribute("y").as_int(), uiNode.child("divSection").attribute("w").as_int(), uiNode.child("divSection").attribute("h").as_int() };
			App->gui->bossHealthBarInfo.divSection = divSection;
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

		else if (type == "dodge")
		{
			App->gui->allclocksData.dodge.position = position;
			App->gui->allclocksData.dodge.section = section;
			App->gui->allclocksData.dodge.type = type;
		}

	}

}

bool j1Scene::LoadInGameUi(pugi::xml_node& nodeScene)
{
	pugi::xml_node inGameNode = nodeScene.child("InGameUi");
	inGamePanel = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(inGamePanel, inGameNode);
	coins_label = App->gui->AddLabel("0 x", { 255,255,255,255 }, App->font->openSansSemiBold24, { 1080,26 }, inGamePanel);
	coins_label->name = "coinsLabel";
	wave_label = App->gui->AddLabel("", { 255,255,255,255 }, App->font->piecesofEight36, { 1150,107 }, inGamePanel);
	god_label = App->gui->AddLabel("God Mode", { 255,255,255, 150 }, App->font->knightsQuest24, { 1185, 695 }, inGamePanel);
	exp_label = App->gui->AddLabel("LVL 1", { 255,255,255,255 }, App->font->piecesofEight24, { 60,130 }, inGamePanel);
	wave_label->hide = true;
	god_label->hide = true;
	potionIg_label = App->gui->AddLabel("0", { 255,255,255,255 }, App->font->openSansSemiBold24, { 1252,628 }, inGamePanel);
	phoenixIg_label = App->gui->AddLabel("0", { 255,255,255,255 }, App->font->openSansSemiBold24, { 1115,628 }, inGamePanel);
	potionIg_label->swapPosition = true;
	phoenixIg_label->swapPosition = true;
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

	pugi::xml_node marchePNode = nodeScene.child("InGameUiMP");
	uiMarchePortrait = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(uiMarchePortrait, marchePNode);

	pugi::xml_node sharaPNode = nodeScene.child("InGameUiSP");
	uiSharaPortrait = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(uiSharaPortrait, sharaPNode);

	pugi::xml_node ritzPNode = nodeScene.child("InGameUiRP");
	uiRitzPortrait = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(uiRitzPortrait, ritzPNode);

	chain1.PushBack({ 646,1, 54,54 });
	chain1.PushBack({ 571,192, 54,54 });
	chain1.PushBack({ 672,194, 54,54 });
	chain1.PushBack({ 773,194, 54,54 });

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

bool j1Scene::LoadCredits(pugi::xml_node& nodeScene)
{
	pugi::xml_node creditsNode = nodeScene.child("Credits");
	creditsPanel = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(creditsPanel, creditsNode);
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

	// TODO: Add character stats item; 

	characterStatsItem = App->gui->AddCharacterStatsItem(inventoryItem);

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

bool j1Scene::LoadControls(pugi::xml_node& nodeScene)
{
	pugi::xml_node controlsNode = nodeScene.child("Controls");
	controlsPanel = App->gui->AddEmptyElement({ 0,0 });
	LoadUiElement(controlsPanel, controlsNode);
	App->scene->tab_controls = App->gui->AddImage({ 361,92 }, &tabSectionControls, default_string, controlsPanel);

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
	if (App->easing->IsEnabled());
		App->easing->Disable();

	App->audio->UnLoadAudio();
}

void j1Scene::LoadScene(SceneState sceneState)
{
	App->dialog->destroyNPCNameLabels(state);

	UnLoadScene();


	switch (sceneState)
	{
	case SceneState::STARTMENU:
		state = SceneState::STARTMENU;
		break;

	case SceneState::LOBBY:
		state = SceneState::LOBBY;

		if (!App->easing->IsEnabled())
			App->easing->Enable();
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
		if (!App->entityFactory->IsEnabled())
			App->entityFactory->Enable();
		break;

	case SceneState::FIRINGRANGE:
		state = SceneState::FIRINGRANGE;

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
			LoadNewMap("maps/FiringRange.tmx");
		}
		if (!App->easing->IsEnabled())
			App->easing->Enable();
		if (!App->entityFactory->IsEnabled())
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
		if (!App->easing->IsEnabled())
			App->easing->Enable();
		if (!App->entityFactory->IsEnabled())
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
		App->audio->PlayMusic("audio/music/BRPG_Hell_Spawn_FULL_Loop.ogg");
		LoadNewMap("maps/Level2.tmx");//"maps/test_ordering.tmx"))//level1_Block_rev.tmx"))   // ("maps/iso_walk.tmx")
		if (!App->easing->IsEnabled())
			App->easing->Enable();
		if (!App->entityFactory->IsEnabled())
			App->entityFactory->Enable();
		// create player for testing purposes here
		break;

	case SceneState::DEATH:
		state = SceneState::DEATH;
		break;

	case SceneState::WIN:
		state = SceneState::WIN;
		ComeToWin = true;
		break;

	case SceneState::MAX_STATES:
		break;
	default:
		break;
	}

	Start();

	App->dialog->createNPCNameLabels(state);

}



void j1Scene::DebugTP(SceneState const &futureScene, LvlPart const &lvlPart)
{
	if (futureScene != state)
	{
		LoadScene(futureScene);
	}
	switch (futureScene)
	{
	case SceneState::STARTMENU:
		break;
	case SceneState::LEVEL1:
		if (lvlPart == LvlPart::START)
		{
			App->entityFactory->player->GetMarche()->position = { -1575.F, 2150.F };
			App->entityFactory->player->GetShara()->position = { -1575.F, 2150.F };
			App->entityFactory->player->GetRitz()->position = { -1575.F, 2150.F };
		}
		else if (lvlPart == LvlPart::WAVES)
		{
			App->entityFactory->player->GetMarche()->position = (fPoint)App->map->SubTileMapToWorld(30, 44);
			App->entityFactory->player->GetShara()->position = (fPoint)App->map->SubTileMapToWorld(30, 44);
			App->entityFactory->player->GetRitz()->position = (fPoint)App->map->SubTileMapToWorld(30, 44);
		}
		break;
	case SceneState::LEVEL2:
		if (lvlPart == LvlPart::START)
		{
			App->entityFactory->player->GetMarche()->position = { -820, 3300 };
			App->entityFactory->player->GetShara()->position = { -820, 3300 };
			App->entityFactory->player->GetRitz()->position = { -820, 3300 };
		}
		else if (lvlPart == LvlPart::WAVES)
		{
			App->entityFactory->player->GetMarche()->position = (fPoint)App->map->SubTileMapToWorld(70, 42);
			App->entityFactory->player->GetShara()->position = (fPoint)App->map->SubTileMapToWorld(70, 42);
			App->entityFactory->player->GetRitz()->position = (fPoint)App->map->SubTileMapToWorld(70, 42);
		}
		else if (lvlPart == LvlPart::BOSS)
		{
			App->entityFactory->player->GetMarche()->position = (fPoint)App->map->SubTileMapToWorld(70, 182);
			App->entityFactory->player->GetShara()->position = (fPoint)App->map->SubTileMapToWorld(70, 182);
			App->entityFactory->player->GetRitz()->position = (fPoint)App->map->SubTileMapToWorld(70, 182);
		}
		break;
	case SceneState::LOBBY:
		App->entityFactory->player->GetMarche()->position = { 115, 240 };
		App->entityFactory->player->GetShara()->position = { 115, 240 };
		App->entityFactory->player->GetRitz()->position = { 115, 240 };
		break;
	case SceneState::FIRINGRANGE:
		App->entityFactory->player->GetMarche()->position = { 165, 580 };
		App->entityFactory->player->GetShara()->position = { 165, 580 };
		App->entityFactory->player->GetRitz()->position = { 165, 580 };
		break;
	default:
		break;
	}
	App->camera2D->SetCameraPos(App->entityFactory->player->GetMarche()->position.x, App->entityFactory->player->GetMarche()->position.y);
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

				// PREVENT NPC LABEL OVERLAYING
				App->dialog->hideAllNPCLabels(true);
				// character stats panel
				characterStatsItem->generateCharacterStats();
				int a = 0;
				App->entityFactory->player->LockPlayerInput();
			}

			else
			{
				inventoryItem->despawnRubbish();  // don't blame me for the name

				// character stats panel
				characterStatsItem->deGenerateCharacterStats();


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

				// PREVENT NPC LABEL OVERLAYING
				App->dialog->hideAllNPCLabels(false);

				App->entityFactory->player->UnlockPlayerInput();
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
	case 2:
	case 3:

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
	case 4:
	case 5:
	case 6:
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

	case 7:
	case 8:
	case 9:
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
		if (timeindmg.Read() > 3000)
			hit_counter -= 1;
		decreaseAlpha = true;
		//App->entityFactory->alphaTimer.Start();
		break;
	}
	return true;
}

ConsumableStats j1Scene::GetConsumableInfo(LootEntity* consumable)
{
	ConsumableStats consumStats;
	consumStats.position = consumable->position;
	consumStats.initialPos = consumStats.position;
	consumStats.rect = consumable->loot_rect;
	consumStats.actualTime.Start();
	
	LOG("OriginPos X %f", consumStats.position.x);
	LOG("OriginPos X %f", consumStats.position.y);

	consumStats.distance_to_travel.x = App->entityFactory->player->position.x + 50 - consumStats.position.x;//bag screen pos
	consumStats.distance_to_travel.y = App->entityFactory->player->position.y - 50 - consumStats.position.y;
	int a = App->render->camera->x + App->render->camera->w;
	int b = App->render->camera->y + App->render->camera->h;
	LOG("camerapos %i %i", App->render->camera->x, App->render->camera->y);
	LOG("camerapos + W H %i %i ", a, b);
	App->easing->CreateSplineV2(consumStats.position.x, App->entityFactory->player->position.x + 700, 700, EASE,true);
	App->easing->CreateSplineV2(consumStats.position.y, App->entityFactory->player->position.y-600, 700, EASE_IN_OUT_BACK,false);
	return consumStats;
}



void j1Scene::UpdateConsumable()
{
	iPoint mPos = App->render->ScreenToWorld(448, 106);
	int time_to_travel=2000;
//	LOG("mPOS %i %i", App->render->ScreenToWorld(448, 106));
	for (std::list<ConsumableStats>::iterator iter = consumableinfo.begin(); iter != consumableinfo.end(); ++iter)
	{
		
		if ((*iter).actualTime.Read() < 700)
		{
			
			
			//(*iter).position.x -= (*iter).distance_to_travel.x * ((*iter).actualTime.Read()/2000) + (*iter).initialPos.x*0.02; //distance_to_travel * (time_passed / time_to_travel) + initial_position;
			//(*iter).position.y -= (*iter).distance_to_travel.y * ((*iter).actualTime.Read() / 2000) + (*iter).initialPos.y*0.01;

			//(*iter).position.y -= (*iter).distance_to_travel.y * ( (*iter).actualTime.Read() = (*iter).actualTime.Read()/ time_to_travel) + (*iter).initialPos.y * 0.01;
			LOG("valoresX %f", (*iter).position.x);
			LOG("valoresY %f", (*iter).position.y);
			

			App->render->Blit(App->entityFactory->lootItemsTex, (*iter).position.x, (*iter).position.y, &(*iter).rect);
			LOG("");
		}

		else
		{
			consumableinfo.pop_front();
		}
	}
}

bool j1Scene::executeAnimChain()
{
	
		App->render->BlitGui(App->gui->GetAtlas(), 226, 635, &chain1.GetCurrentFrame());
		if (chain1.Finished())
			canExecuteChainAnim = false;
	
	return true;
}
