#ifndef UIITEM_IMAGE_H
#define UIITEM_IMAGE_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"

class UiItem_Image : public UiItem
{

public:
	UiItem_Image(SDL_Rect hitBox, const SDL_Rect* section, UiItem*const parent, p2Point<int> pivot = { 0,0 });
	//UiItem_Image(SDL_Rect hitBox, const Animation& section, UiItem*const parent, p2Point<int> pivot = { 0,0 });
	void Draw(const float& dt);
};

#endif
