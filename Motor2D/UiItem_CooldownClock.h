#ifndef UIITEM_COOLDOWNCLOCK_H
#define UIITEM_COOLDOWNCLOCK_H

#include "UiItem.h"
#include "p2Point.h"
#include <cstdint>

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



	State ReturnState(); 

private: 
	SDL_Texture* texture = nullptr;
	SDL_Rect section; 

public: 

	friend class UiItem_CooldownClockManager;

	bool to_delete = false;
};

#endif

