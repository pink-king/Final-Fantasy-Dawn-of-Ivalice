#ifndef UIITEM_BAR_H
#define UIITEM_BAR_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"

class UiItem_Image;

enum TypeBar :uint
{
	VERTICAL,
	HORIZONTAL,
	MAX
};

class UiItem_Bar :public UiItem
{
protected:
	TypeBar type = HORIZONTAL;
	UiItem_Image* image_bar = nullptr;
	UiItem_Image* bar = nullptr;
	SDL_Rect section;
	SDL_Rect image_idle;
	SDL_Rect image_hover;
	std::string name;
	//void DoLogicClicked(bool do_slide = false); 
	void DoLogicHovered();
	void DoLogicDragged();
	void DoLogicAbandoned();
	void CleanUp();

public:
	UiItem_Bar(iPoint position, std::string name, const SDL_Rect* section, const SDL_Rect* thumb_section, const SDL_Rect* image_idle, const SDL_Rect* image_hover, UiItem*const parent/*, TypeBar type = VERTICAL*/);
	~UiItem_Bar();
	void Draw(const float& dt);
	float GetBarValue();

public:
	UiItem_Image* thumb = nullptr;

private:
	bool thumbReposition = false;


};

#endif // UIITEM_BAR_H

