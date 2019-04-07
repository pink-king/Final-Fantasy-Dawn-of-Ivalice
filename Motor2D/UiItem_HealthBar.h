#ifndef UIITEM_HEALTHBAR_H
#define UIITEM_HEALTHBAR_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1PerfTimer.h"


class UiItem_Image;

enum type
{
	health,
	cooldown,
};

struct damageInfo
{
	bool doDamage = false; 
	uint damageValue = 0; 
};

class UiItem_HealthBar : public UiItem
{

public:
	UiItem_HealthBar(iPoint position, const SDL_Rect* staticSection, const SDL_Rect* dynamicSection, const SDL_Rect* damageSection, type variant, UiItem*const parent);

	void Draw(const float& dt);
	 
	void DamageLogic(); 
	void DamageQuadReset(); 

private: 
	UiItem_Image* staticImage = nullptr;
	UiItem_Image* dynamicImage = nullptr;
	UiItem_Image* damageImage = nullptr;

	uint maxSection = 0; 
	uint lastSection = 0; 
	float conversionFactor = 0.0f; 

	bool recuperating = false; 
	type variantType; 
	j1PerfTimer damageBarTimer; 
	

public:
	damageInfo damageInform; 

};

#endif

