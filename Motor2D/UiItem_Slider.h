#ifndef UIITEM_SLIDER_H
#define UIITEM_SLIDER_H

#include "UiItem.h"
#include <string>
#include "p2Point.h"
#include "SDL_ttf/include/SDL_ttf.h"


struct _TTF_Font;


class UiItem_Slider : public UiItem
{
public:

	UiItem_Slider(p2Point<int> position, SDL_Rect rect, UiItem*const parent);
	void Draw(const float& dt);


};


#endif // UIITEM_SLIDER_H

