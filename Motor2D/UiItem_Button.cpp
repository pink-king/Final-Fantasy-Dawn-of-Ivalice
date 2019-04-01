
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
	this->idle = idle;
	this->hover = hover;
	this->click = click;
	actual_sprite = *idle;
}

void UiItem_Button::AddFuntion(void(*funtionOnClick)(), bool Down)
{
}

void UiItem_Button::AddFuntion(std::string & string, bool Down)
{
}

void UiItem_Button::Draw(const float &dt)
{
	App->render->BlitGui(App->gui->GetAtlas(), hitBox.x, hitBox.y, &actual_sprite, 0.0F);
}

void UiItem_Button::OnClickUp()
{
}

void UiItem_Button::OnClickDown()
{
}

void UiItem_Button::DoLogicHovered()
{
	actual_sprite = *hover;
}

void UiItem_Button::DoLogicAbandoned()
{
	actual_sprite = *idle;
}
