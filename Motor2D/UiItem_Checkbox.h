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

	UiItem_Image * panel = nullptr;
	UiItem_Image* box = nullptr;
	UiItem_Label* label = nullptr;
	UiItem_Image* tick = nullptr;

	SDL_Rect panel_section;   // we will care about the x size. The y size will be calculated procedurally 
	SDL_Rect box_section;
	SDL_Rect tick_section;
	SDL_Rect captureIdleSection;

	void DoLogicClicked(std::string &functionName);   // clicking logic matters the most
	void DoLogicHovered();
	void DoLogicAbandoned();
	void CleanUp();
public:
	UiItem_Checkbox(iPoint position, std::string& function, std::string& name, const SDL_Rect* panel_section, const SDL_Rect* box_section, const SDL_Rect* tick_section, labelInfo* labelInfo, UiItem* const parent = nullptr);
	~UiItem_Checkbox();
	void Draw(const float& dt);



};


#endif // UIITEM_CHECKBOX_H