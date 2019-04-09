#include "UiItem_CooldownClock.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"

UiItem_CooldownClock::UiItem_CooldownClock(iPoint position, const SDL_Rect* section, UiItem*const parent) :UiItem(position, parent)
{
	this->section = *section;
	this->hitBox.x = position.x; 
	this->hitBox.y = position.y;

	
}

void UiItem_CooldownClock::Draw(const float & dt)
{

	
	DoLogic(); 
	App->render->BlitGui(App->gui->GetAtlas(), hitBox.x, hitBox.y, &this->section, 0.0F);

}


void UiItem_CooldownClock::CheckState() {


	// TODO 

}


void UiItem_CooldownClock::CleanUp()
{

	if (texture != nullptr)
		App->tex->UnLoad(texture);
}


void UiItem_CooldownClock::DoLogic()
{



}