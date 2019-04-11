#ifndef UIITEM_COOLDOWNCLOCK_H
#define UIITEM_COOLDOWNCLOCK_H

#include "UiItem.h"
#include "p2Point.h"
#include <cstdint>
#include "j1Timer.h"


enum State {
	stop,
	run,
	await    
};

struct toCheck {

	std::string character; 
	std::string ability; 

};



class UiItem_CooldownClock : public UiItem
{
public:
	UiItem_CooldownClock(iPoint position, const SDL_Rect* section, std::string type, std::string charName, UiItem*const parent);              // add stuff
	void Draw(const float& dt) override;
	void Restart();    // once created, the clock loops, it does not destroy itself
	void DoLogic(); 
	void CheckState(); 

private: 

	
	SDL_Rect section; 
	uint maxHeight; 
	uint LastHeight; 
	uint heightDiff; 
	iPoint position; 
	std::string type; 

	State theState; 
	toCheck keepAnEye; 
	bool hide = false; 

	uint8_t alphaValue = 56;

};

#endif

