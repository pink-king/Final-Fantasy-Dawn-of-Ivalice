#ifndef UIITEM_HEALTHBAR_H
#define UIITEM_HEALTHBAR_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "p2Log.h"


class UiItem_Image;

class UiItem_HealthBar : public UiItem
{

public:
	UiItem_HealthBar(iPoint position, const SDL_Rect* staticSection, const SDL_Rect* dynamicSection, UiItem*const parent);

	void Draw(const float& dt);
	 

private: 
	UiItem_Image* staticImage = nullptr;
	UiItem_Image* dynamicImage = nullptr;

	uint maxSection = 0; 
	float conversionFactor = 0.0f; 
};

#endif

