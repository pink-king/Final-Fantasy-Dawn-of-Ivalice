#ifndef UIITEM_HITPOINT_H
#define UIITEM_HITPOINT_H

#include "UiItem.h"
#include <string>
#include "p2Point.h"
#include "SDL_ttf/include/SDL_ttf.h"
#include "j1PerfTimer.h"

struct valueInfo {
	std::string string; 
	uint number; 
};

enum lifeState {
	fadeIn,
	Middle,
	fadeOut
};

class UiItem_HitPoint : public UiItem
{
public:
	UiItem_HitPoint(valueInfo valueInfo, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent);
	void Draw(const float& dt) override;
	void CleanUp(); 

protected:
	valueInfo valueInformation; 
	SDL_Color color;
	TTF_Font* font = nullptr;
	SDL_Texture* texture = nullptr;

public: // public or protected? 

	j1PerfTimer lifeSpan; 
	lifeState returnLifeState();   // return lifeState depending on timer

	// TODO: add scaling factor variable to pass to he blitGui

	friend class UiItem_HitPointManager; 

};

#endif
