#include "UiItem.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"

UiItem::UiItem(const iPoint & pos) 
{
	hitBox.x = pos.x;
	hitBox.y = pos.y;
}

UiItem::UiItem(SDL_Rect hitBox, UiItem * const parent, p2Point<int> pivot) :pivot(pivot), parent(parent)
{

}

UiItem::~UiItem()
{
}

void UiItem::DrawUi(float dt)
{
	
	for (std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin(); iter!= App->gui->ListItemUI.end(); ++iter)
	{
		(*iter)->Draw(dt);
	}
}