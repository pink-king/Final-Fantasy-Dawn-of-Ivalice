#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#include <list>
#include "UiItem.h"
#include "UiItem_Image.h"
#include "UiItem_Label.h"
#include "UiItem_Checkbox.h"
#include "UiItem_Button.h"
#include "UiItem_Bar.h"
#include "UiItem_Button.h"
#include "UiItem_HitPoint.h"
#include "UiItem_HitPointManager.h"
#include "UiItem_HealthBar.h"
#include "UiItem_CooldownClock.h"
#include "GUI_Definitions.h"
#include "UiItem_Description.h"
#include "UiItem_Inventory.h"

struct labelInfo
{
	std::string labelText;
	SDL_Color labelColor;
	uint labelIndex;
};




/*// - - - - - - - the following two methods are only meant for loading the xml data - - - - - - -
struct coolDownClockData
{
	iPoint position;
	SDL_Rect section;
	std::string type;
};

struct theClocks
{

	coolDownClockData ability1, ability2, ulti, potion;

};

// - - - - - - - the following two methods are meant for checking created clocks according to type and player - - - - - - -

struct createdClocks {
	bool special1 = false;
	bool special2 = false;
	bool ulti = false;
	bool potion = false;
};


struct clockOwners {
	createdClocks Marche, Ritz, Shara;
};

// - - - - - - - - - - - - - this method is meant to contain all clocks for a single player - - - - - - - - - - - - //



struct myClocks {
	UiItem_CooldownClock* special1 = nullptr;
	UiItem_CooldownClock* special2 = nullptr;
	UiItem_CooldownClock* ulti = nullptr;
	UiItem_CooldownClock* potion = nullptr;
};*/

struct theClocks;
struct clockOwners;

class j1Gui : public j1Module
{
public:

	j1Gui();
	~j1Gui();

	//Override funtions from class j1Module
	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	void DoLogicSelected();
	void ApplyTabBetweenSimilar(bool setClicked = false);

	bool PostUpdate();
	bool CleanUp();
	void destroyElement(UiItem*);
	void deleteCurrentDialogs(); 

	// TODO: add destroy element, IMPORTANT!! ---> hitpoint labels are destroyed in hitpoint label manager

	std::list<UiItem*>	ListItemUI;
	UiItem* canvas = nullptr;
	UiItem_Label* AddLabel(std::string text, SDL_Color color, TTF_Font* font, p2Point<int> position, UiItem* const parent, bool type_writer = false);
	UiItem_Image* AddImage(iPoint position, const SDL_Rect* section, UiItem* const parent, bool isPanel = false);
	UiItem_Bar* AddBar(iPoint position, std::string name, const SDL_Rect* section, const SDL_Rect* thumb_section, const SDL_Rect* image_idle, const SDL_Rect* image_hover, UiItem* const parent); // , TypeBar type = VERTICAL);
	UiItem_Button* AddButton(iPoint position, std::string function, const SDL_Rect* idle, UiItem* const parent, const SDL_Rect* click = nullptr, const SDL_Rect* hover = nullptr);
	UiItem* AddEmptyElement(iPoint pos, UiItem* const parent = nullptr);
	UiItem_Checkbox* AddCheckbox(iPoint position, std::string& function, const SDL_Rect* panel_section, const SDL_Rect* box_section, const SDL_Rect* tick_section, labelInfo* labelInfo, UiItem* const parent = nullptr);

	// TODO: AddHitPoint();
	UiItem_HitPoint* AddHitPointLabel(valueInfo valueInfo, SDL_Color color, TTF_Font* font, p2Point<int> position, UiItem* const parent, variant type);
	UiItem_HitPoint* AddHitPointLabel2(std::string text, SDL_Color color, TTF_Font* font, p2Point<int> position, UiItem* const parent, variant type);

	UiItem_HealthBar* AddHealthBar(iPoint position, const SDL_Rect* dynamicSection, const SDL_Rect* damageSection, type variant, UiItem* const parent = nullptr);
	UiItem_HealthBar* AddHealthBarToEnemy(const SDL_Rect* dynamicSection, type variant, j1Entity* deliever, UiItem* const parent = nullptr);
	UiItem_HealthBar* AddHealthBarToBoss(iPoint position, const SDL_Rect* dynamicSection, const SDL_Rect* staticSection, const SDL_Rect* divSection, type variant, uint maxLife, j1Entity* deliever, UiItem* const parent = nullptr); // boss

	UiItem_CooldownClock* AddClock(iPoint position, SDL_Rect* section, std::string type, std::string charName, UiItem* const parent = nullptr);


	UiItem_Description* AddDescriptionToEquipment(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Value, EquipmentStatType variableType, uint level, LootEntity* callback, UiItem* const parent = nullptr, uint price = 0);
	UiItem_Description* AddDescriptionToWeapon(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Attack, float resistance, float cooldown, uint level, LootEntity* callback, UiItem* const parent = nullptr, uint price = 0);
	UiItem_Description* AddDescriptionToPotion(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, std::string effect, iPoint HPandTime, LootEntity* callback, UiItem* const parent = nullptr, uint price = 0);

	UiItem_Image* AddSpecialImage(iPoint position, const SDL_Rect* section, UiItem* const parent, SDL_Texture* newTex = nullptr, UiItem_Description* myDescr = nullptr);
	UiItem_Inventory* AddInventory(UiItem* const parent);

	SDL_Texture* GetAtlas();
	void FadeToScene();
	void ExitGame();
	void SettingsScreen();
	void GoBackToMenu();
	void FpsCap();
	void GoBackToGame();
	void GoBackToStartMenu();
	void GoBackToStartMenuFromDeathWin();

	UiItem_HealthBar* healthBar = nullptr;
	SDL_Texture* lootTexture = nullptr;
private:
	SDL_Texture* atlas = nullptr;

	std::string atlas_file_name;
	bool debug_ = false;
public:
	UiItem* selected_object = nullptr;

public:
	bool resetHoverSwapping = false;
	theClocks allclocksData;
	clockOwners spawnedClocks;
	
	enemyHealthBarInfo enemyLifeBarInfo;
	bossLifeBarInfo bossHealthBarInfo;
	SDL_Texture* hurt_hud_tex = nullptr;
	SDL_Texture* hurt_hud_tex2 = nullptr;
	SDL_Texture* hurt_hud_tex3 = nullptr;

};






#endif

