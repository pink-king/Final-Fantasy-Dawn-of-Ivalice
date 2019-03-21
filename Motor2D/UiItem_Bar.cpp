#include "UiItem_Bar.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "UiItem.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "UiItem_Image.h"
#include "Brofiler/Brofiler.h"
#include "p2Point.h"


UiItem_Bar::UiItem_Bar(iPoint position, const SDL_Rect * section, const SDL_Rect* thumb_section, UiItem * const parent, TypeBar type):UiItem(position, parent)
{

//	this->section = *section;
	
	// bar 
	bar = App->gui->AddImage(position, section, parent);
	
	// thumb
	thumb = App->gui->AddImage(position, thumb_section, parent);


}

void UiItem_Bar::Draw(const float & dt)
{
	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		Do_Logic_Clicked();
	}

	//App->render->Blit(App->gui->GetAtlas(), hitBox.x, hitBox.y, &this->section, 0.0F, SDL_FLIP_NONE);
}


void UiItem_Bar::Do_Logic_Clicked() {

	thumb->SetPos(iPoint(200, 100)); 

}

float UiItem_Bar::GetBarValue()
{
	return 0.0f;
}

