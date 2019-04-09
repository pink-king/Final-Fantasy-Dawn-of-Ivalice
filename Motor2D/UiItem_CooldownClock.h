#ifndef UIITEM_COOLDOWNCLOCK_H
#define UIITEM_COOLDOWNCLOCK_H

#include "UiItem.h"
#include "p2Point.h"
#include <cstdint>
#include "j1Timer.h"
#include "PlayerEntity.h"
#include "j1EntityFactory.h"

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
	UiItem_CooldownClock(std::string theType, PlayerEntity* callback, UiItem*const parent);              // add stuff
	void Draw(const float& dt) override;
	void CleanUp();

	void DoLogic(); 
	void CheckState(); 

private: 
	SDL_Texture* texture = nullptr;
	
	
	iPoint position; 
	SDL_Rect section; 


	State theState; 
	toCheck keepAnEye; 
};

#endif

