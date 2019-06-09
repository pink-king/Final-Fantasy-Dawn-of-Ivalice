#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Timer.h"
#include <list>
#include "LootEntity.h"
#define MAX_ALPHA 255

struct SDL_Texture;

class UiItem_Image;
class UiItem_Label;
class UiItem_Bar;
class UiItem_Inventory;
class CharacterStats;
class UiItem;
class PlayerEntityManager;
enum class LOOT_TYPE;
enum class OBJECT_TYPE;
class Trigger;

struct ConsumableStats
{
	SDL_Rect rect;
	fPoint position;
	j1Timer actualTime;
	fPoint distance_to_travel;
	fPoint initialPos;
	int counter;
	LOOT_TYPE L_type;
	OBJECT_TYPE objType;
	bool actionParticle=false;
	SDL_Rect auxRect = { 163,873,10,11 };
};
enum class SceneState
{
	STARTMENU,
	LEVEL1,
	LEVEL2,
	DEATH,
	WIN,
	LOBBY,
	FIRINGRANGE,
	INTRO, 
	
	MAX_STATES
};


enum class LobbyState
{
	ALLBLOCK,
	TALKSTRANGER,
	PASSLVL1,
	PASSLVL2,

	OUTGAME
};

enum class LvlPart
{
	START,
	WAVES,
	BOSS,
	NO_PART
};


class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& node);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void DebugTP(SceneState const &futureScene, LvlPart const &lvlPart = LvlPart::NO_PART);

public:
	UiItem* inGamePanel = nullptr;
	UiItem* uiMarche = nullptr;
	UiItem* uiShara = nullptr;
	UiItem* uiRitz = nullptr;
	UiItem* uiMarchePortrait = nullptr;
	UiItem* uiSharaPortrait = nullptr;
	UiItem* uiRitzPortrait = nullptr;
	UiItem* startMenu = nullptr;
	UiItem* settingPanel = nullptr;
	UiItem* pausePanel = nullptr;
	UiItem* creditsPanel = nullptr;
	UiItem* inventory = nullptr;
	UiItem* deathPanel = nullptr;
	UiItem* winPanel = nullptr;
	UiItem* controlsPanel = nullptr;
	UiItem_Label* coins_label = nullptr;
	UiItem_Label* wave_label = nullptr;
	UiItem_Label* exp_label = nullptr;
	UiItem_Label* coins_label_inventory = nullptr;
	UiItem_Label* god_label = nullptr;
	UiItem_Label* potionIg_label = nullptr;
	UiItem_Label* phoenixIg_label = nullptr;
	UiItem_Image* tab_inventory = nullptr;
	UiItem_Image* tab_controls = nullptr;
	UiItem_Image* tick_image = nullptr;
	SDL_Rect tabSectionControls = { 791,1,46,46 };
	std::string default_string = "";
	SDL_Rect lootPanelRect;
	SDL_Rect lootPanelRectNoButton;
	UiItem_Inventory* inventoryItem = nullptr;
	CharacterStats* characterStatsItem = nullptr;

	bool debug = false;
	bool debugSubtiles = false; 
	bool debugColl = false;
	bool hackerMode = false;
	bool ComeToPortal = false;
	bool ComeToDeath = false;
	bool ComeToWin = false;
	bool exitGame = false;

	bool isSaved = false;
	SceneState state = SceneState::INTRO;
	SceneState previosState = SceneState::LOBBY;
	LobbyState lobbyState = LobbyState::ALLBLOCK;
	fPoint portalPos;
	bool isDeath = false;
	bool paused;
	LOOT_TYPE LType;
	//----SFX-----//
	unsigned int openInventorySFX;
	unsigned int closeinventorySFX;
	unsigned int open_PauseMenuSFX;
	unsigned int enterGameSFX;
	unsigned int playerDeath;
	unsigned int selectUI;
	unsigned int acceptUI;
	unsigned int sell;
	unsigned int purchase;
	unsigned int switch_page;

	
	unsigned int lootGroundSFX;
	unsigned int potionGroundSFX;
	unsigned int coinGroundedSFX;
	unsigned int swapCharSFX;
	unsigned int stepSFX;
	unsigned int enemySpawn;
	unsigned int goblinDetection;
	unsigned int marcheDamaged;
	unsigned int marcheBasic;
	unsigned int marcheTornadoExplosion;
	unsigned int marcheBasic2;
	unsigned int RitzDamaged;
	unsigned int RitzBasic;
	unsigned int RitzBasicHit;
	unsigned int RitzMedusa;
	unsigned int RitzAbility2;
	unsigned int RitzAbility1;
	unsigned int RitzUltimate;
	unsigned int sharaBasic;
	unsigned int basicBodyImp;
	unsigned int strech_Shoot;
	unsigned int emitter_explodeFire;
	unsigned int SharaUltimateWoosh;
	unsigned int SharaDamaged;
	unsigned int goblinDamaged;
	unsigned int goblinDeath;
	unsigned int goblinAttack;
	unsigned int goblinLaugh;
	unsigned int marcheUltimateScream;
	unsigned int marcheAbility1;
	unsigned int marcheEarthShakeSFX;
	unsigned int marcheAbility2; //tornado
	unsigned int dash;
	unsigned int sharaAbility1;
	unsigned int sharaAbility2_shoot;
	unsigned int sharaBasic_ImpactsWall;
	unsigned int sharaAbility1_ImpactsWall;
	unsigned int sharaAbility2_ImpactsWall;
	unsigned int BombDeathSFX;
	unsigned int bombgetHitSFX;
	unsigned int bombExplodeSFX;
	unsigned int golem_deathSFX;
	unsigned int golem_impactWallSFX;
	unsigned int golem_spawnSFX;
	unsigned int golem_spawnAttackSFX;

	unsigned int wave_respawn;
	unsigned int wave_start;
	unsigned int wave_end;
	unsigned int boss_flower_deathCirc;
	unsigned int boss_flower_basic;
	unsigned int boss_flower_BasicImpWall;
	unsigned int boss_flower_death; 
	unsigned int portal_appear;
	unsigned int portal_mantain;
	unsigned int portal_vanish;
	unsigned int portal_travel; 
	unsigned int open_doorSFX;
	unsigned int pickLoot;
	unsigned int pickGold;
	unsigned int consumHealPotion;
	unsigned int pickPotion;
	unsigned int typeWriterSFX;
	unsigned int savedSFX;
	unsigned int jar_breakSFX;
	unsigned int OpenChestSFX;
	
private:
	SDL_Texture* debug_tex = nullptr;
	
	pugi::xml_node sceneNode;
	bool LoadedUi = false;
	UiItem_Bar* volume_bar = nullptr;
	UiItem_Bar* fx_bar = nullptr;
	float result_volume = 0.0f;
	float result_fx = 0.0f;
	SDL_Rect inventory_transparency = { 0,0,1280,720 };
public:

	void LoadNewMap(const char* mapName);
	void UnLoadScene();
	void LoadScene(SceneState sceneState);

private:
	void LoadUiElement(UiItem*parent, pugi::xml_node node);
	bool LoadInGameUi(pugi::xml_node& nodeScene);
	bool LoadStartMenu(pugi::xml_node& nodeScene);
	bool LoadPlayerUi(pugi::xml_node& nodeScene);
	bool LoadSettings(pugi::xml_node& nodeScene);
	bool LoadPauseSettings(pugi::xml_node& nodeScene);
	bool LoadCredits(pugi::xml_node& nodeScene);
	bool LoadInventory(pugi::xml_node& nodeScene);
	bool LoadDeathScreen(pugi::xml_node& nodeScene);
	bool LoadWinScreen(pugi::xml_node& nodeScene);
	bool LoadControls(pugi::xml_node& nodeScene);

	PlayerEntityManager* player_selected = nullptr;


public: 
	void DoOpenInventory(bool onlyEquipped = false, bool isVendor = false); 

	float AlphaDecrease(float alphavalue, int counter);
	float AlphaIncrease(float alphavalue, int counter);
	bool DecideTexToPulse();
	ConsumableStats GetConsumableInfo(LootEntity* consumable);
	void UpdateConsumable();
	std::list<ConsumableStats> consumableinfo;
public:
	float hudAlphavalue[3];
	int hit_counter;
	int previous_counter;
	bool decreaseAlpha = false;
	Trigger* doorlvl1 = nullptr;
	Trigger* doorlvl2 = nullptr;
	Trigger* firingrange = nullptr;
	Trigger* strangerDialog = nullptr;
	j1Timer timeindmg;
	UiItem_Image* MarcheIcon = nullptr;
	UiItem_Image* SharaIcon = nullptr;
	UiItem_Image* RitzIcon = nullptr;
	UiItem_Image* dialogueBox = nullptr;
	std::string str_potionIg;
	std::string str_phoenixIg;
	int potion_counterIg = 0;
	int phoenix_counterIg = 0;
	bool swapAbilities = false;
	bool dodge = false;
	bool ability1 = false;
	bool ability2 = false;
	bool ulti = false;

	bool dodge_rv = false;
	bool ability1_rv = false;
	bool ability2_rv = false;
	bool ulti_rv = false;
	Animation chain1;
	bool executeAnimChain();
	bool canExecuteChainAnim = false;
};

#endif // __j1SCENE_H__