#ifndef UIITEM_IMAGE_H
#define UIITEM_IMAGE_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "p2Log.h"

class UiItem_Image : public UiItem
{

public:
	UiItem_Image(iPoint position, const SDL_Rect* section, UiItem*const parent);
	//UiItem_Image(SDL_Rect hitBox, const Animation& section, UiItem*const parent, p2Point<int> pivot = { 0,0 });
	void Draw(const float& dt);
	SDL_Rect section;

	
private: 

	SDL_Rect* ReturnPos()
	{
		return  &(this->hitBox); 
    }

	void SetPos(iPoint pos) {
		this->hitBox.x = pos.x; 
		this->hitBox.y = pos.y;
	}

	bool hide = false; 

public: 
	friend class UiItem_Bar; 
	friend class UiItem_Checkbox;

};

#endif
