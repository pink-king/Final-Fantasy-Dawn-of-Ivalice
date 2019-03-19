#ifndef UIITEM_BAR_H
#define UIITEM_BAR_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"

class UiItem_Image;

enum TypeBar :uint
{
	VERTICAL,
	HORIZONTAL,
	MAX
};

class UiItem_Bar :public UiItem
{
protected:
	TypeBar type = HORIZONTAL;
public:
	UiItem_Bar(p2Point<int> position, uint mesure, const SDL_Rect* section, UiItem*const parent, p2Point<int> pivot = { 0,0 }, TypeBar type = HORIZONTAL);
	~UiItem_Bar() {}
	void Draw(const float& dt);
	float GetBarValue();
};


#endif // UIITEM_BAR_H

