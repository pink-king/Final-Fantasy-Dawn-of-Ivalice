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

UiItem_Checkbox::UiItem_Checkbox(iPoint position, const SDL_Rect * panel_section, const SDL_Rect* box_section, const SDL_Rect* tick_section, UiItem * const parent) :UiItem(position, parent)
{

	this->panel_section = *panel_section;
	this->box_section = *box_section;
	this->tick_section = *tick_section;
	this->guiType = GUI_TYPES::CHECKBOX;

	this->hitBox.x = position.x;
	this->hitBox.y = position.y;

	// do this in XML ? 

	int box_offset_y = 7; 
	int box_offset_x = 10;
	// - - - - - - - - - - - - - - - - - - - -

	

	// tick
	tick = App->gui->AddImage(position, tick_section, tick);
	tick->hide = true; 

	// panel 
	panel = App->gui->AddImage(position, panel_section, this);

	// box
	box = App->gui->AddImage(iPoint(position.x + box_offset_x, position.y + box_offset_y), box_section, panel);

	// label     // (leave these to the end)



	// the parent
	this->parent = parent;
}

void UiItem_Checkbox::Draw(const float & dt)
{

	//App->render->Blit(App->gui->GetAtlas(), hitBox.x, hitBox.y, &this->section, 0.0F, SDL_FLIP_NONE);
}


void UiItem_Checkbox::DoLogicClicked(bool do_slide) {

	tick->hide = !tick->hide; 
}


