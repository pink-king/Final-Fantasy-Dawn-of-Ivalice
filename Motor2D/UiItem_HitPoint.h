#ifndef UIITEM_HITPOINT_H
#define UIITEM_HITPOINT_H

#include "UiItem.h"
#include <string>
#include "p2Point.h"
#include "SDL_ttf/include/SDL_ttf.h"
#include "j1PerfTimer.h"
#include "j1Entity.h"
#include <cstdint>

#define NUMBER_LIFE 1300
#define TEXT_LIFE 2300
#define WAVE_LIFE 8000

#define middlelifewave 5000
#define middeLife 1000
struct valueInfo {
	std::string string; 
	uint number; 
};

enum lifeState {
	fadeIn,
	Middle,
	fadeOut,
	dead
};

enum variant {
	number,
	text,
	gold,
	wave,
	save,
	levelUp,
};

class UiItem_HitPoint : public UiItem
{
public:
	UiItem_HitPoint(valueInfo valueInfo, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent, variant type);   // HP AND GOLD
	UiItem_HitPoint(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent, variant type = text);   


	void Draw(const float& dt) override;
	void CleanUp(); 
public:
	void updateHitPointPositions();   // TODO: increment all current hitpoins Y position 
	void updateHitPointSizes();   // TODO: switch current hitpoint's states, and change size depending on the state    // use the scaling factor variable inside each hitbox
	void updateHitPointOpacities();  // TODO: switch current hitpoint's states, and change opacity depending on the state  (fadeIn and fadeOut)

protected:
	valueInfo valueInformation; 
	variant numerOrText; 
	SDL_Color color;
	TTF_Font* font = nullptr;
	SDL_Texture* texture = nullptr;
	uint8_t alphaValue = 1; 

public: // public or protected? 

	j1PerfTimer lifeSpan; 
	lifeState returnLifeState();   // return lifeState depending on timer

	// TODO: add scaling factor variable to pass to he blitGui
    
	float scaleFactor = 1.0f; 

	friend class UiItem_HitPointManager; 

	bool to_delete = false;
	bool fromEnemy = false;

};

#endif
