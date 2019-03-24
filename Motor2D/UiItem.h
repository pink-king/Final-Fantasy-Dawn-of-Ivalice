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
	DRAG,
	MAX_STATES,
};

enum GUI_TYPES
{
	BAR,
	BUTTON,
	IMAGE,
	LABEL,
	UNKNOWN
};

class UiItem
{
protected:
	// UiItem* parent = NULL;
public:
	UiItem* parent = NULL;

	UI_STATES state = IDLE;
	GUI_TYPES guiType = UNKNOWN; 
	SDL_Rect hitBox = { 0,0,0,0 };
	// p2Point<int> pivot = { 0,0 };
	uint mouseButtonDown = 0;
	UiItem(const iPoint& pos, UiItem *const parent);
	//UiItem(SDL_Rect hitBox, UiItem *const parent, p2Point<int> pivot = { 0,0 });
	~UiItem();
	virtual void Draw(const float&) {};
	void DrawUi(float dt);
	bool slidable = false; 
	bool tabbed = false; 
	bool enable = true;


	virtual void DoLogicClicked(bool slidable = false) {}; // TODO: Function pointers
	virtual void DoLogicHovered(bool slidable = false) {}; // TODO: Function pointers
	virtual void DoLogicDragged(bool slidable = false) {}; // TODO: Function pointers
	virtual void DoLogicAbandoned(bool slidable = false) {}; // TODO: Function pointers
};

#endif
