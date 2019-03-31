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
	UiItem_Image* thumb = nullptr;
	UiItem_Image* bar = nullptr;
	SDL_Rect section;

	//void DoLogicClicked(bool do_slide = false); 
	void DoLogicHovered();
	void DoLogicDragged(); 
	void DoLogicAbandoned(); 

public:
	UiItem_Bar(iPoint position, const SDL_Rect* section, const SDL_Rect* thumb_section, UiItem*const parent/*, TypeBar type = VERTICAL*/);
	~UiItem_Bar() {}
	void Draw(const float& dt);
	float GetBarValue();


private: 
	bool thumbReposition = false; 


};


#endif // UIITEM_BAR_H

