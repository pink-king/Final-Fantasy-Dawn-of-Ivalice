
#include "UiItem_Button.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "p2Point.h"
#include "j1Scene.h"
#include "j1Map.h"
#include <assert.h>


UiItem_Button::UiItem_Button(iPoint position, std::string &function, const SDL_Rect * idle, UiItem * const parent, const SDL_Rect * click, const SDL_Rect * hover) : UiItem(position, function, parent)
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

	this->guiType = GUI_TYPES::BUTTON;
	hitBox.w = idle->w;
	hitBox.h = idle->h;
}


void UiItem_Button::AddFuntion(std::string & string)
{

}

void UiItem_Button::DoLogicClicked(std::string &functionName)
{
	if (functionName == "FadeToScene")
		App->gui->FadeToScene();
	if (functionName == "ExitGame")
		App->gui->ExitGame();
	if (functionName == "SettingsScreen")
		App->gui->SettingsScreen();
	if (functionName == "GoBackToMenu")
		App->gui->GoBackToMenu();
	if (functionName == "GoBackToGame")
		App->gui->GoBackToGame();
	if (functionName == "GoBackToStartMenu")
		App->gui->GoBackToStartMenu();
}

void UiItem_Button::Draw(const float &dt)
{
	App->render->BlitGui(App->gui->GetAtlas(), hitBox.x, hitBox.y, &frames[state], 0.0F);
}
