#include "UiItem.h"
#include "j1App.h"
#include "j1Render.h"

UiItem::UiItem(const iPoint & pos, UiItem * const parent) : parent(parent)
{

}

UiItem::UiItem(SDL_Rect hitBox, UiItem * const parent, p2Point<int> pivot) :pivot(pivot), parent(parent)
{

}

UiItem::~UiItem()
{
}