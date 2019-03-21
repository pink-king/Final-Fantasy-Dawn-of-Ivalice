#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "p2Log.h"

j1Gui::j1Gui() : j1Module() 
{
	name.assign("gui");
	canvas = new UiItem({ 0,0 }, NULL);
}


j1Gui::~j1Gui()
{
}

bool j1Gui::Awake(pugi::xml_node& conf)
{
	bool ret = true;
	atlas_file_name = conf.child("atlas").attribute("file").as_string();
	return true;
}

bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.data());
	return true;
}

bool j1Gui::Update(float dt)
{
	SearchandSelectClicked(); // TODO: don't do this always 
	return true;
}

void j1Gui::SearchandSelectClicked() {

	iPoint mousePos;
	int x, y; 
    App->input->GetMousePosition(x, y);
	mousePos.x = x; 	mousePos.y = y;

	LOG("- - - - -  - --  - -- - -  - --     mouse pos is %i %i", mousePos.x, mousePos.y);

	uint mouseButtonDown = App->input->GetCurrentMouseButtonDown();
	std::list<UiItem*>::iterator item = ListItemUI.begin(); 

	for (; item != ListItemUI.end(); item++)
	{
		LOG("- - - - -  - --  - -- - -  - --     item pos is %i %i", (*item)->hitBox.x, (*item)->hitBox.y);

		if (mousePos.x > (*item)->hitBox.x && mousePos.x < (*item)->hitBox.x + (*item)->hitBox.w
			&& mousePos.y >(*item)->hitBox.y && mousePos.y < (*item)->hitBox.y + (*item)->hitBox.h)
		{
			if ((*item)->state != CLICK && mouseButtonDown != 0)
			{
				/*/thisItem->data->mouseButtonDown = mouseButtonDown;   // TODO: function pointers
				thisItem->data->OnClickDown();*/ 
				(*item)->state = CLICK;
				LOG("- - - - -  - --  - -- - -  - --     cliked object"); 
			}

			if ((*item)->state == CLICK && App->input->GetMouseButtonDown((*item)->mouseButtonDown) == KEY_UP)
			{
				// thisItem->data->OnClickUp();  // TODO: function pointers
				(*item)->state = HOVER;
				LOG("- - - - -  - --  - -- - -  - --     hovered object");
			}

			else if ((*item)->state == IDLE)
				(*item)->state = HOVER; LOG("- - - - -  - --  - -- - -  - --     hovered object");


		}
		else  if ((*item)->state != IDLE)
		{                                        // TODO: With sliders, this does not apply (FOCUS)
			(*item)->state = IDLE;
			LOG("- - - - -  - --  - -- - -  - --     OUTSIDE object");
		}
		if (App->input->GetMouseButtonDown((*item)->mouseButtonDown) == KEY_UP || App->input->GetMouseButtonDown((*item)->mouseButtonDown) == KEY_IDLE)
			(*item)->mouseButtonDown = 0;


	}


}


bool j1Gui::PostUpdate()
{

	// temporal debug 

	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN) {  
		debug_ = !debug_;
	}

	//canvas->DrawUi(dt);
	for (std::list<UiItem*>::iterator iter = ListItemUI.begin(); iter != ListItemUI.end(); ++iter)
	{
		(*iter)->Draw(dt);

		if (debug_) {
			SDL_Rect r;
			r.x = (*iter)->hitBox.x; 
			r.y = (*iter)->hitBox.y;
			r.w = (*iter)->hitBox.w;
			r.h = (*iter)->hitBox.h; 
			int akbar = (*iter)->hitBox.x;
			App->render->DrawQuad(r, 100, 50, 200, 200, true, false);

			LOG("The debug quad is %i %i %i %i", r.x, r.y, r.w, r.h); 
		}

	}
	return true;
}

bool j1Gui::CleanUp()
{
	App->tex->UnLoad(atlas); 
	return true;
}

UiItem_Label * j1Gui::AddLabel(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position)
{
	UiItem* newUIItem = nullptr;
	newUIItem = new UiItem_Label(text, color, font, position);
	ListItemUI.push_back(newUIItem);
	return (UiItem_Label*)newUIItem;
	
}

UiItem_Image * j1Gui::AddImage(iPoint position, const SDL_Rect* section, UiItem *const parent)
{
	UiItem* newUIItem = nullptr;

	if (parent == NULL)
		newUIItem = new UiItem_Image(position, section, canvas);
	else
		newUIItem = new UiItem_Image(position, section, parent);

	ListItemUI.push_back(newUIItem);

	return (UiItem_Image*)newUIItem;
}

UiItem_Bar * j1Gui::AddBar(iPoint position, const SDL_Rect* section, const SDL_Rect* thumb_section, UiItem*const parent, TypeBar type)
{
	UiItem* newUIItem = nullptr;

	newUIItem = new UiItem_Bar(position, section, thumb_section, parent, type);

	ListItemUI.push_back(newUIItem);

	return (UiItem_Bar*)newUIItem;

}

UiItem * j1Gui::AddEmptyElement(iPoint pos, UiItem * const parent)
{
	UiItem* newUIItem = nullptr;
	if (parent == NULL)
		newUIItem = new UiItem(pos, canvas);
	else
		newUIItem = new UiItem(pos, parent);

	ListItemUI.push_back(newUIItem);
	return newUIItem;
}

SDL_Texture * j1Gui::GetAtlas()
{
	return atlas;
}


