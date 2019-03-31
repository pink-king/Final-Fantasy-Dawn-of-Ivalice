
#include "UiItem_Button.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "p2Point.h"
#include "j1Scene.h"
#include "j1Map.h"
#include <assert.h>



UiItem_Button::UiItem_Button(iPoint position, const SDL_Rect * idle, UiItem * const parent, const SDL_Rect * click, const SDL_Rect * hover) : UiItem(position, parent)
{
	assert(parent != nullptr);
	frames[IDLE] = *idle;

	if (click)
		frames[CLICK] = *click;
	else
		frames[CLICK] = *idle;

	if (hover)
		frames[HOVER] = *hover;
	else
		frames[HOVER] = *idle;
}

void UiItem_Button::AddFuntion(void(*funtionOnClick)(), bool Down)
{
}

void UiItem_Button::AddFuntion(std::string & string, bool Down)
{
}

void UiItem_Button::Draw(const float &dt)
{
	App->render->BlitGui(App->gui->GetAtlas(), hitBox.x, hitBox.y, &frames[state], 0.0F);
}

void UiItem_Button::OnClickUp()
{
}

void UiItem_Button::OnClickDown()
{
}
