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


	this->hitBox.x = position.x;
	this->hitBox.y = position.y;
	this->section = *section;
	maxHeight = section->h;      // capture the section height, it will be modified
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

	
	
	//SDL_SetTextureAlphaMod(App->gui->GetAtlas(), alphaValue);
	
	DoLogic(); 

	if(!hide)
	App->render->BlitGui(App->gui->GetAtlas(), hitBox.x, hitBox.y, &this->section, 0.0f);

}


void UiItem_CooldownClock::CheckState() 
{


	


	// potion    // TODO (when potion cooldonwn available 
	



}



void UiItem_CooldownClock::DoLogic()
{



	if ((App->entityFactory->player->selectedCharacterEntity->character == characterName::MARCHE && keepAnEye.character == "Marche")
		|| (App->entityFactory->player->selectedCharacterEntity->character == characterName::RITZ && keepAnEye.character == "Ritz")
		|| (App->entityFactory->player->selectedCharacterEntity->character == characterName::SHARA && keepAnEye.character == "Shara"))
	{

		hide = false; 

		float proportion = 0.0f; 

		// ability 1 
		if (keepAnEye.ability == "special1")
		{
			if (App->entityFactory->player->selectedCharacterEntity->coolDownData.special1.timer.Read()
				< App->entityFactory->player->selectedCharacterEntity->coolDownData.special1.cooldownTime)
			{
				proportion = App->entityFactory->player->selectedCharacterEntity->coolDownData.special1.cooldownTime / maxHeight; 

				this->section.h = maxHeight - App->entityFactory->player->selectedCharacterEntity->coolDownData.special1.timer.Read() / proportion;
			}
			else
			{
				// Restart();    dont't call it if it is already called from the player
			}
		}


		// ability 2
		if (keepAnEye.ability == "special2")
		{
			if (App->entityFactory->player->selectedCharacterEntity->coolDownData.special2.timer.Read()
				< App->entityFactory->player->selectedCharacterEntity->coolDownData.special2.cooldownTime)
			{
				proportion = App->entityFactory->player->selectedCharacterEntity->coolDownData.special2.cooldownTime / maxHeight;

				this->section.h = maxHeight - App->entityFactory->player->selectedCharacterEntity->coolDownData.special2.timer.Read() / proportion;

			}
			else
			{
				// Restart();    dont't call it if it is already called from the player
			}
		}


		// ulti 
		if (keepAnEye.ability == "ulti")
		{
			if (App->entityFactory->player->selectedCharacterEntity->coolDownData.ultimate.timer.Read()
				< App->entityFactory->player->selectedCharacterEntity->coolDownData.ultimate.cooldownTime)
			{
				proportion = App->entityFactory->player->selectedCharacterEntity->coolDownData.ultimate.cooldownTime / this->section.h;

			}
			else
			{
				// Restart();    dont't call it if it is already called from the player
			}
		}

		

	}
	else
	{
		hide = true; 


		// TODO: Prevent h section from diminishing when clock is paused 
	}


}

void UiItem_CooldownClock::Restart()
{
	hide = true;

	// TODO: Reset clock


}