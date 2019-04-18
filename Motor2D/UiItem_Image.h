#ifndef UIITEM_IMAGE_H
#define UIITEM_IMAGE_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "j1Entity.h"
#include "p2Log.h"

struct SDL_Texture; 

class UiItem_Description;

class UiItem_Image : public UiItem
{

public:
	UiItem_Image(iPoint position, const SDL_Rect* section, UiItem*const parent, bool isPanel = false);
	UiItem_Image(iPoint position, const SDL_Rect* section, UiItem*const parent, SDL_Texture* newTex = nullptr, UiItem_Description* myDescr = nullptr);
	//UiItem_Image(SDL_Rect hitBox, const Animation& section, UiItem*const parent, p2Point<int> pivot = { 0,0 });
	void Draw(const float& dt);
	SDL_Rect section;


private:

	iPoint * ReturnPos()
	{
		iPoint* ret = { 0 };
		ret->x = this->hitBox.x;
		ret->y = this->hitBox.y;
		return ret;

	}

	void SetPos(iPoint pos) {
		this->hitBox.x = pos.x;
		this->hitBox.y = pos.y;
	}

	bool isPanel = false;
	SDL_Rect resizedRect;


	SDL_Texture* newTex = nullptr; 


	UiItem_Description* myDescr = nullptr; 

public: 
	bool hide = false;
	bool printFromLoot = false;   // TODO: use to print the loot description icon from the loot texture


	friend class UiItem_Bar;
	friend class UiItem_Checkbox;
	friend class UiItem_HealthBar;
};


#endif
