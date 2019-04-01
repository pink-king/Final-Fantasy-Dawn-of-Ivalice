#ifndef UIITEM_BUTTON_H
#define UIITEM_BUTTON_H
#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"
#include <string>

class UiItem_Button :public UiItem
{
protected:
	SDL_Rect actual_sprite;
	const SDL_Rect* idle;
	const SDL_Rect* hover;
	const SDL_Rect* click;
	void(*funtionOnClick) () = nullptr;
	void(*funtionOnUp)() = nullptr;



public:
	UiItem_Button(iPoint position, const SDL_Rect * idle, UiItem* const parent, const SDL_Rect * click = nullptr, const SDL_Rect * hover = nullptr);
	void AddFuntion(void(*funtionOnClick) (), bool Down = true);
	void AddFuntion(std::string & string, bool Down = true);

	void Draw(const float&);
	void OnClickUp();
	void OnClickDown();
	void DoLogicHovered();
	void DoLogicAbandoned();
};

#endif