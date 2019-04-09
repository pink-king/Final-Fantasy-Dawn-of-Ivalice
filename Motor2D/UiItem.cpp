#include "UiItem.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "j1Window.h"
#include "UiItem_HitPointManager.h"


UiItem::UiItem(const iPoint & pos, UiItem * const parent)
{
	hitBox.x = pos.x;
	hitBox.y = pos.y;
	if (parent != nullptr)
		this->parent = parent;

	this->function = function;

	SDL_ShowCursor(SDL_DISABLE);
}




UiItem::UiItem(const iPoint & pos, std::string &function, UiItem *const parent) : parent(parent)
{
	hitBox.x = pos.x;
	hitBox.y = pos.y;
	if (parent != nullptr)
		this->parent = parent;

	this->function = function;

	SDL_ShowCursor(SDL_DISABLE);
}

//UiItem::UiItem(SDL_Rect hitBox, UiItem * const parent, p2Point<int> pivot) :pivot(pivot), parent(parent)
//{
//
//}

UiItem::~UiItem()
{
	LOG("destroyed UI item somewhere");
}

void UiItem::DrawUi(float dt)
{

	for (std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin(); iter != App->gui->ListItemUI.end(); ++iter)
	{
		if ((*iter)->parent!=App->gui->canvas)
		{
			if ((*iter)->parent != NULL && (*iter)->parent->enable && (*iter)->parent->parent == App->gui->canvas)
				(*iter)->Draw(dt);
			else if ((*iter)->parent->parent != App->gui->canvas)
			{
				if ((*iter)->parent->parent->enable)
					(*iter)->Draw(dt);
			}
		}
	}


	for (std::vector<UiItem_HitPoint*>::iterator iter = App->HPManager->hitPointLabels.begin(); iter != App->HPManager->hitPointLabels.end(); ++iter)
	{
		if ((*iter) != nullptr)
			(*iter)->Draw(dt);
	}





	Draw_Cursor(dt);
}



void UiItem::Draw_Cursor(float dt) {
	int x, y;
	x = y = 0;
	App->input->GetMousePosition(x, y);
	x *= App->win->GetScale();
	y *= App->win->GetScale();


	SDL_Rect section = { 252,638,25,32 };       // do this in XML 
	App->render->BlitGui(App->gui->GetAtlas(), x, y, &section, 0.0F);



}

