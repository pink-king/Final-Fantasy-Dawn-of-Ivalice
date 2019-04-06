#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class UiItem_Image;
class UiItem_Label;
class UiItem;
class PlayerEntityManager;

enum class SceneState
{
	STARTMENU,
	GAME,
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
	UiItem * inGamePanel = nullptr;
	UiItem * uiMarche = nullptr;
	UiItem * uiShara = nullptr;
	UiItem * uiRitz = nullptr;
	UiItem * startMenu = nullptr;
	UiItem * settingPanel = nullptr;
	// UiItem_Image* testPanel = nullptr; 
	bool debug = false;
	bool exitGame = false;
	SceneState state = SceneState::STARTMENU;

private:
	SDL_Texture* debug_tex;
	pugi::xml_node sceneNode;
	bool LoadedUi = false;
private:
	void LoadUiElement(UiItem*parent, pugi::xml_node node);
	bool LoadInGameUi(pugi::xml_node& nodeScene);
	bool LoadStartMenu(pugi::xml_node& nodeScene);
	bool LoadPlayerUi(pugi::xml_node& nodeScene);
	bool LoadSettings(pugi::xml_node& nodeScene);

	PlayerEntityManager* player_selected = nullptr;
};

#endif // __j1SCENE_H__