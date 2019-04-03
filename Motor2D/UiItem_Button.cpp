
#include "UiItem_Button.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "p2Point.h"
#include "j1Scene.h"
#include "j1Map.h"

UiItem_Button::UiItem_Button(SDL_Rect hitBox, const SDL_Rect * idle, UiItem * const parent, const SDL_Rect * click, const SDL_Rect * hover, p2Point<int> pivot) : UiItem(pivot, parent)
{
}

void UiItem_Button::AddFuntion(void(*funtionOnClick)(), bool Down)
{
}

void UiItem_Button::AddFuntion(std::string & string, bool Down)
{
}

void UiItem_Button::Draw(const float &)
{
}

void UiItem_Button::OnClickUp()
{
}

void UiItem_Button::OnClickDown()
{
}
