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

enum GUI_TYPES
{
	BAR,
	BUTTON,
	IMAGE,
	LABEL,
	CHECKBOX,
	HITPOINT,
	HEALTHBAR,
	CLOCK,
	DESCRIPTION,
	INVENTORY,
	UNKNOWN
};

class UiItem
{
protected:
	// UiItem* parent = NULL;
public:
	UiItem* parent = nullptr;

	UI_STATES state = IDLE;
	GUI_TYPES guiType = UNKNOWN;
	std::string name_object;
	SDL_Rect hitBox = { 0,0,0,0 };
	std::string function;
	// p2Point<int> pivot = { 0,0 };
	uint mouseButtonDown = 0;
	UiItem(const iPoint& pos, UiItem* const parent);
	UiItem(const iPoint& pos, std::string& function, UiItem* const parent);
	UiItem(UiItem* const parent);
	//UiItem(SDL_Rect hitBox, UiItem *const parent, p2Point<int> pivot = { 0,0 });
	~UiItem();
	virtual void Draw(const float&) {};
	//virtual bool Update(float dt){};

	void DrawUi(float dt);

	void Draw_Cursor(float dt);
	bool slidable = false;
	bool tabbed = false;
	bool tabbable = false;
	bool isNPCLabel = false;
	bool enable = true;
	bool hide = false;
	UiItem* iFriend = nullptr;
	bool to_delete = false;
	bool useCamera = true;
	bool selected = false;
	int scaleFactor;
	iPoint textureDimensions;
	iPoint prevTextDimension;
	bool first_hitbox = false;
	bool isDialog = false; 
	bool swapPosition = false;
	bool changeTexture = false; 
	bool hasBeenSpawned = false; 
	bool isCharacterStat = false; 

	uint dialogPos = 666; 

	virtual void DoLogicClicked() {};
	virtual void DoLogicClicked(std::string& functionName) {};
	virtual void DoLogicHovered() {};
	virtual void DoLogicAbandoned() {};
	virtual void CleanUp() {};
};

#endif