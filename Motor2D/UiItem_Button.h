#ifndef UIITEM_BUTTON_H
#define UIITEM_BUTTON_H
#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"
#include <string>

class UiItem_Button :public UiItem
{
protected:
	SDL_Rect frames[MAX_STATES];

public:
	UiItem_Button(iPoint position, std::string &function, const SDL_Rect * idle, UiItem* const parent, const SDL_Rect * click = nullptr, const SDL_Rect * hover = nullptr);
	void AddFuntion(std::string & string);
	void DoLogicClicked(std::string &functionName);
	void Draw(const float&);
	

};

#endif