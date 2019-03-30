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
	thumb = App->gui->AddImage(position, thumb_section, parent);
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


void UiItem_Bar::DoLogicDragged(bool do_slide) {

	                                   // right now only vertical sliders
	if (do_slide) {
		iPoint mousePos;
		int x, y;
		App->input->GetMousePosition(x, y);
		mousePos.x = x; 	mousePos.y = y;
		mousePos.x *= App->win->GetScale(); 
		mousePos.y *= App->win->GetScale();

		if (mousePos.y >= bar->hitBox.y && mousePos.y <= bar->hitBox.y + bar->hitBox.h)
		{
			thumb->SetPos(iPoint(thumb->hitBox.x, mousePos.y));
		}
	}


}



void UiItem_Bar::DoLogicHovered(bool do_slide) {

	bar->section.x = 164; 
	bar->section.y = 615;
	bar->section.w = 56;
	bar->section.h = 191;

}

void UiItem_Bar::DoLogicAbandoned(bool do_slide) {


	bar->section = this->section; 
}


float UiItem_Bar::GetBarValue()
{
	return 0.0f;
}

