#ifndef UIITEM_HITPOINT_H
#define UIITEM_HITPOINT_H

#include "UiItem.h"
#include <string>
#include "p2Point.h"
#include "SDL_ttf/include/SDL_ttf.h"
#include "j1PerfTimer.h"
#include <cstdint>

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

class UiItem_HitPoint : public UiItem
{
public:
	UiItem_HitPoint(valueInfo valueInfo, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent);
	void Draw(const float& dt) override;
	void CleanUp(); 
public:
	void updateHitPointPositions();   // TODO: increment all current hitpoins Y position 
	void updateHitPointSizes();   // TODO: switch current hitpoint's states, and change size depending on the state    // use the scaling factor variable inside each hitbox
	void updateHitPointOpacities();  // TODO: switch current hitpoint's states, and change opacity depending on the state  (fadeIn and fadeOut)

protected:
	valueInfo valueInformation; 
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

};

#endif