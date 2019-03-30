#include "UiItem_Checkbox.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "UiItem.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "UiItem_Image.h"
#include "j1Window.h"
#include "Brofiler/Brofiler.h"
#include "p2Point.h"
#include "p2log.h"

UiItem_Checkbox::UiItem_Checkbox(iPoint position, const SDL_Rect * panel_section, const SDL_Rect* box_section, UiItem * const parent) :UiItem(position, parent)
{

	this->panel_section = *panel_section;
	this->box_section = *box_section;
	this->guiType = GUI_TYPES::CHECKBOX;

	this->hitBox.x = position.x;
	this->hitBox.y = position.y;

	
	/*bar = App->gui->AddImage(position, section, parent);
	// thumb
	thumb = App->gui->AddImage(position, thumb_section, parent);
	thumb->slidable = true;
	thumb->parent = bar;
	thumb->iFriend = this;*/

	// box
	

	// label     // (leave these to the end)

	// panel 



	// tick

	// to check mouse 
	this->hitBox.w = section->w;
	this->hitBox.h = section->h;
}

void UiItem_Checkbox::Draw(const float & dt)
{

	//App->render->Blit(App->gui->GetAtlas(), hitBox.x, hitBox.y, &this->section, 0.0F, SDL_FLIP_NONE);
}


void UiItem_Checkbox::DoLogicClicked(bool do_slide) {


}


