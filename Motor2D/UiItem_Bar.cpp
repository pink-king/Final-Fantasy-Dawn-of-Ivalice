#include "UiItem_Bar.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "UiItem.h"
#include "j1Scene.h"
#include "UiItem_Image.h"
#include "Brofiler/Brofiler.h"
#include "p2Point.h"

UiItem_Bar::UiItem_Bar(iPoint pos, uint mesure, const SDL_Rect * section, UiItem * const parent, p2Point<int> pivot, TypeBar type) : UiItem(pos, parent)
{
}

void UiItem_Bar::Draw(const float & dt)
{
}

float UiItem_Bar::GetBarValue()
{
	return 0.0f;
}

