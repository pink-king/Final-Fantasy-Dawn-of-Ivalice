#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"


struct SDL_Texture;

class UiItem_Image;
class UiItem_Label;
class UiItem_Bar;
class UiItem_Inventory;
class UiItem;
class PlayerEntityManager;


enum class SceneState
{
	STARTMENU,
	LEVEL1,
	LEVEL2,
	DEATH,
	WIN,
	LOBBY,
	MAX_STATES
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

public:
	UiItem* inGamePanel = nullptr;
	UiItem* uiMarche = nullptr;
	UiItem* uiShara = nullptr;
	UiItem* uiRitz = nullptr;
	UiItem* startMenu = nullptr;
	UiItem* settingPanel = nullptr;
	UiItem* pausePanel = nullptr;
	UiItem* inventory = nullptr;
	UiItem* deathPanel = nullptr;
	UiItem* winPanel = nullptr;
	UiItem_Label* coins_label = nullptr;
	UiItem_Label* coins_label_inventory = nullptr;
	UiItem_Image* tab_inventory = nullptr;
	SDL_Rect lootPanelRect;
	SDL_Rect lootPanelRectNoButton;
	UiItem_Inventory* inventoryItem = nullptr;
	
	bool debug = false;
	bool debugSubtiles = false; 
	bool debugColl = false;
	bool hackerMode = true;
	bool ComeToPortal = false;
	bool ComeToDeath = false;

	bool exitGame = false;
	SceneState state = SceneState::STARTMENU;
	SceneState previosState = SceneState::LOBBY;
	fPoint portalPos;
	bool isDeath = false;
	bool paused;
	unsigned int openInventorySFX;
	unsigned int closeinventorySFX;
	unsigned int open_PauseMenuSFX;
	unsigned int enterGameSFX;
	unsigned int playerDeath;
	unsigned int selectUI;
	unsigned int acceptUI;

	
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
	bool LoadInventory(pugi::xml_node& nodeScene);
	bool LoadDeathScreen(pugi::xml_node& nodeScene);
	bool LoadWinScreen(pugi::xml_node& nodeScene);
	
	PlayerEntityManager* player_selected = nullptr;

	

public: 
	void DoOpenInventory(bool onlyEquipped = false, bool isVendor = false); 



public:
	UiItem_Image* MarcheIcon = nullptr;
	UiItem_Image* SharaIcon = nullptr;
	UiItem_Image* RitzIcon = nullptr;
	
};

#endif // __j1SCENE_H__