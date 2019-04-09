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


class UiItem_CooldownClock : public UiItem
{
public:
	UiItem_CooldownClock(iPoint position, const SDL_Rect* section, UiItem*const parent);              // add stuff
	void Draw(const float& dt) override;
	void CleanUp();

	void DoLogic(); 


	void CheckState(); 

private: 
	SDL_Texture* texture = nullptr;
	SDL_Rect section; 


	State theState; 

};

#endif

