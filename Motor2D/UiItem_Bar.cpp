#include "UiItem_Bar.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "UiItem.h"
#include "j1Scene.h"
#include "UiItem_Image.h"
#include "Brofiler/Brofiler.h"
#include "p2Point.h"



UiItem_Bar::UiItem_Bar(iPoint position, const SDL_Rect * section, UiItem * const parent, TypeBar type):UiItem(position)
{

	//App->gui->AddImage(position, &section);


}

void UiItem_Bar::Draw(const float & dt)
{
	//App->render->Blit(App->gui->GetAtlas(), hitBox.x, hitBox.y, &this->section, 0.0F, SDL_FLIP_NONE);
}

float UiItem_Bar::GetBarValue()
{
	return 0.0f;
}

