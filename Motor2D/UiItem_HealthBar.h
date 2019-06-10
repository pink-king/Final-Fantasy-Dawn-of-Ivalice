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
class j1Entity;
class UiItem_Label;

enum type
{
	player,
	enemy,
	boss,
	experience
};



struct damageInfo
{
	bool doDamage = false;
	uint damageValue = 0;
};

class UiItem_HealthBar : public UiItem
{

public:
	UiItem_HealthBar(iPoint position, const SDL_Rect* dynamicSection, const SDL_Rect* damageSection, const SDL_Rect* staticSection, type variant, UiItem* const parent); // player
	UiItem_HealthBar(iPoint position, const SDL_Rect* dynamicSection, const SDL_Rect* divisionSection, const SDL_Rect* staticSection, type variant, bool experience, UiItem* const parent); // experience
	UiItem_HealthBar(iPoint position, const SDL_Rect* dynamicSection, const SDL_Rect* staticSection, const SDL_Rect* divSection, type variant, uint maxLife, j1Entity* deliever, UiItem* const parent); // boss
	UiItem_HealthBar(const SDL_Rect* dynamicSection, const SDL_Rect* staticSection, type variant, UiItem* const parent, j1Entity* deliever); // enemies
	~UiItem_HealthBar();

	void Draw(const float& dt);

	void DamageLogic();
	void DamageQuadReset();
	void doDamageToBoss(uint lifeToSubstract);
	void UpdatePos();
	void RecalculateSection(); 

	void CleanUp();
	void ShowBossBarWhenDialogIsOver();

public:
	UiItem_Image* dynamicImage = nullptr;
	UiItem_Image* damageImage = nullptr;
	UiItem_Image* staticImage = nullptr;

	//boss
	UiItem_Image* divisionImage = nullptr;
	UiItem_Label* nameOnTop = nullptr;

	// boss and enemies
	UiItem_Image* skull = nullptr;
	iPoint skullOffset = iPoint(-25, 0);
	std::string name = "";


	// player
	UiItem_Label* NameLabel = nullptr; 
	UiItem_Label* TotalLabel = nullptr;

private:

	uint maxSection = 0;
	uint lastSection = 0;
	float conversionFactor = 0.0f;

	bool recuperating = false;

	j1PerfTimer damageBarTimer;


	iPoint offsetFromEnemy;
	uint enemyMaxLife;

public: 
	bool startShowing = false;
	bool spawnedSkull = false;
	type variantType;
private: 

	iPoint playerBarOffset = { 28,11 };

	uint remainingBossSeparations = 4;
	uint bossSeparationWidth = 0;

public:
	damageInfo damageInform;
	j1Entity* deliever = nullptr;

	
};

#endif