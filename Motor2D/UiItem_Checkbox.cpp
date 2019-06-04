#include "UiItem_Checkbox.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "UiItem.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "j1Fonts.h"
#include "UiItem_Image.h"
#include "j1Window.h"
#include "Brofiler/Brofiler.h"
#include "p2Point.h"
#include "p2log.h"
#include <assert.h> 
#include "j1Audio.h"

UiItem_Checkbox::UiItem_Checkbox(iPoint position, std::string& function, std::string& name, const SDL_Rect* panel_section, const SDL_Rect* box_section, const SDL_Rect* tick_section, labelInfo* labelInfo, UiItem* const parent) :UiItem(position, function, name, parent)
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
	panel = App->gui->AddImage(position, panel_section, name, this);

	// box
	box = App->gui->AddImage(boxPos, box_section, name, this);

	// tick
	tick = App->gui->AddImage(tickPos, tick_section, name, this);
	tick->hide = true;


	// label    
	uint offsetFromBox = 28;
	iPoint labelPos(boxPos.x + offsetFromBox, boxPos.y);

	App->gui->AddLabel(labelInfo->labelText, labelInfo->labelColor, App->font->fonts.at(labelInfo->labelIndex), labelPos, parent);
	// - - - - - - - - - - - - - - - - - - - -



	// the parent
	this->parent = parent;

}

UiItem_Checkbox::~UiItem_Checkbox()
{

}

void UiItem_Checkbox::Draw(const float & dt)
{

	//App->render->Blit(App->gui->GetAtlas(), hitBox.x, hitBox.y, &this->section, 0.0F, SDL_FLIP_NONE);
}


void UiItem_Checkbox::DoLogicClicked(std::string &functionName) {

	tick->hide = !tick->hide;
	if (functionName == "FpsCap")
		App->gui->FpsCap();
	App->audio->PlayFx(App->scene->acceptUI, 0);
}



void UiItem_Checkbox::DoLogicHovered() {

	panel->section.x = 284;
	panel->section.y = 722;
	panel->section.w = 220;
	panel->section.h = 48;

}

void UiItem_Checkbox::DoLogicAbandoned() {


	panel->section = this->captureIdleSection;
}

void UiItem_Checkbox::CleanUp()
{
	panel->to_delete = true;
	box->to_delete = true;
	label->to_delete = true;
	tick->to_delete = true;
}


