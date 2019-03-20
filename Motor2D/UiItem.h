#ifndef ITEM_UI_H
#define ITEM_UI_H
#include "SDL/include/SDL.h"
#include <list>
#include "p2Point.h"
#include <string>

enum UI_STATES
{
	IDLE,
	HOVER,
	CLICK,
	MAX_STATES,
};

class UiItem
{
protected:
	UiItem* parent = NULL;
public:
	UI_STATES state = IDLE;
	SDL_Rect hitBox = { 0,0,0,0 };
	p2Point<int> pivot = { 0,0 };
	UiItem(const iPoint& pos);
	UiItem(SDL_Rect hitBox, UiItem *const parent, p2Point<int> pivot = { 0,0 });
	~UiItem();
	virtual void Draw(const float&) {};
	void DrawUi(float dt);
};

#endif