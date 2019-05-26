#ifndef UIITEM_HEALTHBAR_H
#define UIITEM_HEALTHBAR_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1PerfTimer.h"
// #include "j1Entity.h"

#define bossBarSeparations 4

class UiItem_Image;
class UiItem_Label; 
class j1Entity;

enum type
{
	player,
	enemy,
	boss
};



struct damageInfo
{
	bool doDamage = false;
	uint damageValue = 0;
};

class UiItem_HealthBar : public UiItem
{

public:
	UiItem_HealthBar(iPoint position, const SDL_Rect* dynamicSection, const SDL_Rect* damageSection, type variant, UiItem* const parent); // player
	UiItem_HealthBar(iPoint position,  const SDL_Rect* dynamicSection, const SDL_Rect* staticSection, const SDL_Rect* divSection, type variant, uint maxLife, j1Entity* deliever, UiItem* const parent); // boss
	UiItem_HealthBar(const SDL_Rect* dynamicSection, type variant, UiItem* const parent, j1Entity* deliever); // enemies
	~UiItem_HealthBar();

	void Draw(const float& dt);

	void DamageLogic();
	void DamageQuadReset();
	void doDamageToBoss(uint lifeToSubstract);
	void UpdatePos();

	void CleanUp();


	void ShowBossBarWhenDialogIsOver(); 

public:
	UiItem_Image* dynamicImage = nullptr;
	UiItem_Image* damageImage = nullptr;
	UiItem_Image* staticImage = nullptr;
	UiItem_Image* divisionImage = nullptr;
	UiItem_Label* nameOnTop = nullptr; 

private:

	uint maxSection = 0;
	uint lastSection = 0;
	float conversionFactor = 0.0f;

	bool recuperating = false;
	type variantType;
	j1PerfTimer damageBarTimer;


	iPoint offsetFromEnemy;
	uint enemyMaxLife;
	bool startShowing = false;


	iPoint playerBarOffset = { 28,11 };

	uint remainingBossSeparations = 4; 
	uint bossSeparationWidth = 0; 

public:
	damageInfo damageInform;
	j1Entity* deliever = nullptr;

   
};

#endif