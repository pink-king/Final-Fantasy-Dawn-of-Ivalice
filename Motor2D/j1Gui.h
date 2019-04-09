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
#include "PlayerEntityManager.h"
#include "PlayerEntity.h"

struct labelInfo
{
	std::string labelText;
	SDL_Color labelColor;
	uint labelIndex;
};


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
	// TODO: add destroy element, IMPORTANT!! ---> hitpoint labels are destroyed in hitpoint label manager

	std::list<UiItem*>	ListItemUI;
	UiItem* canvas = nullptr;
	UiItem_Label* AddLabel(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent);
	UiItem_Image* AddImage(iPoint position, const SDL_Rect* section, UiItem *const parent, bool isPanel = false);
	UiItem_Bar* AddBar(iPoint position, std::string name, const SDL_Rect* section, const SDL_Rect* thumb_section, UiItem*const parent); // , TypeBar type = VERTICAL);
	UiItem_Button* AddButton(iPoint position, std::string function, const SDL_Rect * idle, UiItem* const parent, const SDL_Rect * click = nullptr, const SDL_Rect * hover = nullptr);
	UiItem* AddEmptyElement(iPoint pos, UiItem * const parent = nullptr);
	UiItem_Checkbox* AddCheckbox(iPoint position, const SDL_Rect* panel_section, const SDL_Rect* box_section, const SDL_Rect* tick_section, labelInfo* labelInfo, UiItem*const parent = nullptr);

	// TODO: AddHitPoint();
	UiItem_HitPoint* AddHitPointLabel(valueInfo valueInfo, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent, variant type);
	UiItem_HitPoint* AddHitPointLabel2(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent, variant type);


	UiItem_HealthBar* AddHealthBar(iPoint position, const SDL_Rect* staticSection, const SDL_Rect* dynamicSection, const SDL_Rect* damageSection, type variant, UiItem*const parent = nullptr);
	UiItem_CooldownClock* AddClock(iPoint position, SDL_Rect* section, std::string type, PlayerEntity* callback = nullptr, UiItem*const parent = nullptr);


	SDL_Texture* GetAtlas();
	void FadeToScene();
	void ExitGame();
	void SettingsScreen();
	void GoBackToMenu();

	UiItem_HealthBar* healthBar = nullptr;

private:
	SDL_Texture * atlas;
	std::string atlas_file_name;
	bool debug_ = false;
	UiItem* selected_object = nullptr;

public:
	bool resetHoverSwapping = false;
	theClocks allclocksData; 

};






#endif