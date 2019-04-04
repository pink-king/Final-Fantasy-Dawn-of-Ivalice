#include "UiItem_HealthBar.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Scene.h"

UiItem_HealthBar::UiItem_HealthBar(iPoint position, const SDL_Rect* staticSection, const SDL_Rect* dynamicSection, UiItem*const parent) : UiItem(position, parent)
{
	this->guiType = GUI_TYPES::HEALTHBAR;


	iPoint staticPos = position; 
	staticImage = App->gui->AddImage(staticPos, staticSection, this);

	iPoint newPos(staticPos.x + (staticSection->w - dynamicSection->w) / 2, staticPos.y + (staticSection->h - dynamicSection->h) / 2);

	dynamicImage = App->gui->AddImage(newPos, dynamicSection, this);
}

void UiItem_HealthBar::Draw(const float& dt)
{

	// we will use the draw call to calculate, but the two images are drawn in image cpp



}
