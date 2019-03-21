#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#include <list>
#include "UiItem.h"
#include "UiItem_Image.h"
#include "UiItem_Label.h"
/*#include "UiItem_Button.h"*/
#include "UiItem_Bar.h"

class j1Gui : public j1Module
{
public:

	j1Gui();
	~j1Gui();

	//Override funtions from class j1Module
	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	void SearchandSelectClicked(); 
	void ResolveChildren(UiItem*); 
	void DoLogicSelected(); 
	void ApplyTabBetweenSimilar(bool setClicked = false); 

	bool PostUpdate();
	bool CleanUp();

	std::list<UiItem*>	ListItemUI;
	UiItem* canvas = nullptr;
	UiItem_Label* AddLabel(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position);
	UiItem_Image* AddImage(iPoint position, const SDL_Rect* section, UiItem *const parent);
	UiItem_Bar* AddBar(iPoint position, const SDL_Rect* section, const SDL_Rect* thumb_section, UiItem*const parent, TypeBar type = VERTICAL);
	UiItem* AddEmptyElement(iPoint pos, UiItem * const parent = nullptr);

	SDL_Texture* GetAtlas();

private:
	SDL_Texture * atlas;
	std::string atlas_file_name;
	bool debug_ = false;  
	UiItem* selected_object = nullptr;

	bool resetHoverSwapping = false; 
};

#endif