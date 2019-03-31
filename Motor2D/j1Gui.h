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



struct labelInfo
{
	std::string labelText;
	SDL_Color labelColor;
	uint labelIndex;
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

	std::list<UiItem*>	ListItemUI;
	UiItem* canvas = nullptr;
	UiItem_Label* AddLabel(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent);
	UiItem_Image* AddImage(iPoint position, const SDL_Rect* section, UiItem *const parent, bool isPanel = false);
	UiItem_Bar* AddBar(iPoint position, const SDL_Rect* section, const SDL_Rect* thumb_section, UiItem*const parent); // , TypeBar type = VERTICAL);
	UiItem_Button* AddButton(iPoint position, const SDL_Rect * idle, UiItem* const parent, const SDL_Rect * click = nullptr, const SDL_Rect * hover = nullptr);
	UiItem* AddEmptyElement(iPoint pos, UiItem * const parent = nullptr);
	UiItem_Checkbox* AddCheckbox(iPoint position, const SDL_Rect* panel_section, const SDL_Rect* box_section, const SDL_Rect* tick_section, labelInfo* labelInfo, UiItem*const parent = nullptr);
	
	SDL_Texture* GetAtlas();

private:
	SDL_Texture * atlas;
	std::string atlas_file_name;
	bool debug_ = false;  
	UiItem* selected_object = nullptr;


	bool resetHoverSwapping = false; 
};

#endif