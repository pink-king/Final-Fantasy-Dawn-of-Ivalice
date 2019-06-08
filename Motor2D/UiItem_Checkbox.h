#ifndef UIITEM_CHECKBOX_H
#define UIITEM_CHECKBOX_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"

class UiItem_Image;
class UiItem_Label;

struct labelInfo;

class UiItem_Checkbox :public UiItem
{
protected:
	SDL_Rect frames[MAX_STATES];
	UiItem_Image* tick = nullptr;

	SDL_Rect panel_section;   // we will care about the x size. The y size will be calculated procedurally 
	SDL_Rect box_section;
	SDL_Rect tick_section;
	SDL_Rect captureIdleSection;

	void DoLogicClicked(std::string &functionName);   // clicking logic matters the most
	void CleanUp();
public:
	UiItem_Checkbox(iPoint position, std::string& function, std::string& name, const SDL_Rect* idle, UiItem* const parent, const SDL_Rect* click = nullptr, const SDL_Rect* hover = nullptr, const SDL_Rect* tick_Section = nullptr);
	~UiItem_Checkbox();
	void Draw(const float& dt);



};


#endif // UIITEM_CHECKBOX_H