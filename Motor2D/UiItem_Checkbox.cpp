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

UiItem_Checkbox::UiItem_Checkbox(iPoint position, std::string& function, std::string& name, const SDL_Rect* idle, UiItem* const parent, const SDL_Rect* click, const SDL_Rect* hover, const SDL_Rect* tick_section) :UiItem(position, function, name, parent)
{

	assert(parent != NULL);
	frames[IDLE] = *idle;

	if (click)
		frames[CLICK] = *click;
	else
		frames[CLICK] = *idle;

	if (hover)
		frames[HOVER] = *hover;
	else
		frames[HOVER] = *idle;

	

	
	this->tick_section = *tick_section;
	this->guiType = GUI_TYPES::CHECKBOX;

	hitBox.x = position.x;
	hitBox.y = position.y;

	

	

	int tick_offset_y = 3;
	int tick_offset_x = 3;

	iPoint tickPos(position.x + tick_offset_x, position.y + tick_offset_y);


	// - - - - - - - - - - - - - - - - - - - -




	

	// tick
	tick = App->gui->AddImage(tickPos, tick_section, name, this);
	tick->hide = true;


	

	



	// the parent
	this->parent = parent;

}

UiItem_Checkbox::~UiItem_Checkbox()
{

}

void UiItem_Checkbox::Draw(const float & dt)
{

	App->render->BlitGui(App->gui->GetAtlas(), hitBox.x, hitBox.y, &frames[state], 0.0F);
}


void UiItem_Checkbox::DoLogicClicked(std::string &functionName) {

	tick->hide = !tick->hide;
	if (functionName == "FpsCap")
		App->gui->FpsCap();
	App->audio->PlayFx(App->scene->acceptUI, 0);
}






void UiItem_Checkbox::CleanUp()
{
	
	tick->to_delete = true;
}


