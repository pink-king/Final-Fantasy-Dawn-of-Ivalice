#include "UiItem_CooldownClock.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1EntityFactory.h"

UiItem_CooldownClock::UiItem_CooldownClock(iPoint position, const SDL_Rect* section, std::string type, std::string charName, UiItem*const parent) :UiItem(position, parent)
{

	this->guiType = GUI_TYPES::CLOCK; 

	// first capture sections according to the ability type. They are defined in gui cpp 


	this->position = position; 
	this->section = *section;
	this->type = type;

	// then load the ability type 

	keepAnEye.ability = type; 


	// lastly, define the player to keep an eye at

	if (charName == "Marche")
	{
		keepAnEye.character = "Marche"; 
	}
	else if (charName == "Ritz")
	{
		keepAnEye.character = "Ritz";
	}
	else if (charName == "Shara")
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


void UiItem_CooldownClock::CheckState() 
{


	// ability 1 
	if (keepAnEye.ability == "special1")
	{
		if (App->entityFactory->player->selectedCharacterEntity->coolDownData.special1.timer.Read()
			>= App->entityFactory->player->selectedCharacterEntity->coolDownData.special1.cooldownTime)
		{

		}
	}


	// ability 2
	if (keepAnEye.ability == "special2")
	{
		if (App->entityFactory->player->selectedCharacterEntity->coolDownData.special2.timer.Read()
			>= App->entityFactory->player->selectedCharacterEntity->coolDownData.special2.cooldownTime)
		{

		}
	}


	// ulti 
	if (keepAnEye.ability == "ulti")
	{
		if (App->entityFactory->player->selectedCharacterEntity->coolDownData.ultimate.timer.Read()
			>= App->entityFactory->player->selectedCharacterEntity->coolDownData.ultimate.cooldownTime)
		{

		}
	}


	// potion    // TODO (when potion cooldonwn available 
	



}



void UiItem_CooldownClock::Restart()
{

}


void UiItem_CooldownClock::DoLogic()
{





}