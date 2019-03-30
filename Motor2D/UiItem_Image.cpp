#include "UiItem_Image.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Scene.h"

UiItem_Image::UiItem_Image(iPoint position, const SDL_Rect* section, UiItem*const parent, bool isPanel) : UiItem(position, parent)
{
	this->section = *section;
	this->guiType = GUI_TYPES::IMAGE;


	this->hitBox.w = section->w; 
	this->hitBox.h = section->h;

	if (isPanel == 1)
	{
		this->isPanel = true; 
	}
}

void UiItem_Image::Draw(const float& dt)
{
	if (!hide)
	{
		App->render->BlitGui(App->gui->GetAtlas(), hitBox.x, hitBox.y, &this->section, 0.0F);
	}
}
