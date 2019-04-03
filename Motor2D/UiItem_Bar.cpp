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

UiItem_Bar::UiItem_Bar(iPoint position, const SDL_Rect * section, const SDL_Rect* thumb_section, UiItem * const parent/*, TypeBar type*/):UiItem(position, parent)
{

	this->section = *section;
	this->guiType = GUI_TYPES::BAR;

	this->hitBox.x = position.x;
	this->hitBox.y = position.y;

	// bar 
	bar = App->gui->AddImage(position, section, parent);


	// thumb
	iPoint thumbPos(position.x + 5, position.y + 9);

	thumb = App->gui->AddImage(thumbPos, thumb_section, parent);
	thumb->slidable = true; 
	thumb->parent = bar; 
	thumb->iFriend = this; 

	// to check mouse 
	this->hitBox.w = section->w;
	this->hitBox.h = section->h;


	// the parent
	this->parent = parent; 
}

void UiItem_Bar::Draw(const float & dt)
{

	//App->render->Blit(App->gui->GetAtlas(), hitBox.x, hitBox.y, &this->section, 0.0F, SDL_FLIP_NONE);
}





void UiItem_Bar::DoLogicHovered() {
 
	if (!thumbReposition)
	{
		thumb->hitBox.y += 4;
		thumbReposition = !thumbReposition;
	}

	bar->section.x = 1088; 
	bar->section.y = 476;
	bar->section.w = 191;
	bar->section.h = 58;

		uint nexPosX = 0;
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			nexPosX = thumb->hitBox.x + 2;
		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			nexPosX = thumb->hitBox.x - 2;
		}

		if (nexPosX >= bar->hitBox.x && nexPosX <= bar->hitBox.x + bar->hitBox.w)
		{
			thumb->SetPos(iPoint(nexPosX, thumb->hitBox.y));
		}
	

}

void UiItem_Bar::DoLogicAbandoned() {

	if (thumbReposition)
	{
	thumb->hitBox.y -= 4;
	thumbReposition = !thumbReposition;
	}

	bar->section = this->section; 
}


float UiItem_Bar::GetBarValue()
{
	return 0.0f;
}

