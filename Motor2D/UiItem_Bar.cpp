#include "UiItem_Bar.h"
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




UiItem_Bar::UiItem_Bar(iPoint position, std::string name, const SDL_Rect* section, const SDL_Rect* thumb_section, const SDL_Rect* image_idle, const SDL_Rect* image_hover, UiItem*const parent) : UiItem(position, parent)

{
	this->section = *section;
	this->guiType = GUI_TYPES::BAR;
	this->image_idle = *image_idle;
	this->image_hover = *image_hover;
	this->hitBox.x = position.x;
	this->hitBox.y = position.y;
	this->name = name;
	// bar 
	bar = App->gui->AddImage(position, section, this);   // TODO: this should have as pareNT "this"


														 // thumb
	iPoint thumbPos(position.x + (section->w*0.5) - (thumb_section->w*0.5), position.y + (section->h*0.07));

	thumb = App->gui->AddImage(thumbPos, thumb_section, this);    // TODO: this should have as pareNT "this"

	image_bar = App->gui->AddImage({ position.x - 100, position.y }, &this->image_idle, this);
	thumb->slidable = true;
	//thumb->parent = bar; 
	thumb->iFriend = this;

	// to check mouse 
	this->hitBox.w = section->w;
	this->hitBox.h = section->h;
	thumb->hitBox.w = thumb_section->w;
	thumb->hitBox.h = thumb_section->h;


	// the parent
	this->parent = parent;
}

void UiItem_Bar::Draw(const float & dt)
{

	//App->render->Blit(App->gui->GetAtlas(), hitBox.x, hitBox.y, &this->section, 0.0F, SDL_FLIP_NONE);
}





void UiItem_Bar::DoLogicHovered() {

	image_bar->section = image_hover;

	if (!thumbReposition)
	{
		thumbReposition = !thumbReposition;
	}

	Sint16 xAxis = App->input->GetControllerAxis(SDL_CONTROLLER_AXIS_RIGHTX);

	uint nexPosX = 0;
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || xAxis>0 || App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_REPEAT)
	{
		nexPosX = thumb->hitBox.x + 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || xAxis < 0 || App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_REPEAT)
	{
		nexPosX = thumb->hitBox.x - 2;
	}

	if (nexPosX >= bar->hitBox.x && nexPosX <= (bar->hitBox.x + bar->hitBox.w - thumb->hitBox.w))
	{
		thumb->SetPos(iPoint(nexPosX, thumb->hitBox.y));
	}
}

void UiItem_Bar::DoLogicAbandoned() {

	if (thumbReposition)
	{
		thumbReposition = !thumbReposition;
	}

	bar->section = this->section;
	image_bar->section = this->image_idle;
}


float UiItem_Bar::GetBarValue()
{
	float ipos_bar = thumb->hitBox.x + (thumb->section.w / 2);
	float fixed_pos = bar->hitBox.x + (thumb->section.w / 2);
	float fpos_bar = bar->hitBox.x + bar->section.w - (thumb->section.w / 2);
	float final_pos = (ipos_bar - fixed_pos) / (fpos_bar - fixed_pos);
	return final_pos;
}
