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
#include <assert.h> 

UiItem_Checkbox::UiItem_Checkbox(iPoint position, const SDL_Rect * panel_section, const SDL_Rect* box_section, const SDL_Rect* tick_section, UiItem * const parent) :UiItem(position, parent)
{

	assert(parent != NULL); 

	this->panel_section = *panel_section;
	this->captureIdleSection = *panel_section;

	this->box_section = *box_section;
	this->tick_section = *tick_section;
	this->guiType = GUI_TYPES::CHECKBOX;

	this->hitBox.x = position.x;
	this->hitBox.y = position.y;

	// do this in XML ? 

	int box_offset_y = 9; 
	int box_offset_x = 16;

	iPoint boxPos(position.x + box_offset_x, position.y + box_offset_y);

	int tick_offset_y = 3;
	int tick_offset_x = 3;

	iPoint tickPos(boxPos.x + tick_offset_x, boxPos.y + tick_offset_y);


	// - - - - - - - - - - - - - - - - - - - -

	


	// panel 
	panel = App->gui->AddImage(position, panel_section, this);

	// box
	box = App->gui->AddImage(boxPos, box_section, panel);

	// tick
	tick = App->gui->AddImage(tickPos, tick_section, tick);
	tick->hide = true;


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



void UiItem_Checkbox::DoLogicHovered(bool do_slide) {

	panel->section.x = 284;
	panel->section.y = 722;
	panel->section.w = 220;
	panel->section.h = 48;

}

void UiItem_Checkbox::DoLogicAbandoned(bool do_slide) {


	panel->section = this->captureIdleSection;
}


