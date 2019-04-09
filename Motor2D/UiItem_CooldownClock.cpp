#include "UiItem_CooldownClock.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"

UiItem_CooldownClock::UiItem_CooldownClock(iPoint position, const SDL_Rect* section, std::string type, PlayerEntity* callback, UiItem*const parent) :UiItem(position, parent)
{

	// first capture sections according to the ability type. They are defined in gui cpp 


	this->position = position; 
	this->section = *section;
	this->type = type;

	// then load the ability type 

	keepAnEye.ability = type; 


	// lastly, define the player to keep an eye at

	if (callback->character == characterName::MARCHE)
	{
		keepAnEye.character = "Marche"; 
	}
	else if (callback->character == characterName::RITZ)
	{
		keepAnEye.character = "Ritz";
	}
	else if (callback->character == characterName::SHARA)
	{
		keepAnEye.character = "Shara";
	}

	
	this->parent = parent; 
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