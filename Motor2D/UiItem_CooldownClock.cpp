#include "UiItem_CooldownClock.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"

UiItem_CooldownClock::UiItem_CooldownClock(std::string theType, PlayerEntity* callback, UiItem*const parent) :UiItem(parent)
{

	// first capture sections according to the ability type. They are defined in gui cpp 

	if (theType == "ability1")
	{
		position = App->gui->allclocksData.ability1.position;
		section = App->gui->allclocksData.ability1.section;
	}

	else if (theType == "ability2")
	{
		position = App->gui->allclocksData.ability2.position;
		section = App->gui->allclocksData.ability2.section;
	}

	else if (theType == "ulti")
	{
		position = App->gui->allclocksData.ulti.position;
		section = App->gui->allclocksData.ulti.section;
	}

	else if (theType == "potion")
	{
		position = App->gui->allclocksData.potion.position;
		section = App->gui->allclocksData.potion.section;
	}

	hitBox.x = position.x; 
	hitBox.y = position.y;

	// then load the ability type 

	keepAnEye.ability = theType; 


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