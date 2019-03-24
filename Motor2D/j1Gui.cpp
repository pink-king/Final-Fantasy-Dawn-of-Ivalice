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
	DoLogicSelected(); 

	// TESTING TAB, its "faked" for the mom, only bars 

	if (!resetHoverSwapping)
	{
		ApplyTabBetweenSimilar(resetHoverSwapping);
		resetHoverSwapping = true; 
	}
	else {
		ApplyTabBetweenSimilar(resetHoverSwapping);
	}

	return true;
}

void j1Gui::DoLogicSelected() {

	if (selected_object != nullptr)
	{
		bool do_slide = false;
		if (selected_object->slidable)
		{
			do_slide = true;
		}
		switch (selected_object->state) 
		{

		case CLICK: selected_object->DoLogicClicked(do_slide);
			break;
		case HOVER: selected_object->DoLogicHovered(do_slide);
			break;
		case DRAG: selected_object->DoLogicDragged(do_slide);
			break;
		case IDLE: selected_object->DoLogicAbandoned(do_slide);
			break;

		}
	}

}

void j1Gui::SearchandSelectClicked() {

	iPoint mousePos;
	int x, y; 
    App->input->GetMousePosition(x, y);
	mousePos.x = x; 	mousePos.y = y;

	uint mouseButtonDown = App->input->GetCurrentMouseButtonDown();
	std::list<UiItem*>::iterator item = ListItemUI.begin(); 

	for (; item != ListItemUI.end(); item++)
	{
		if (!(*item)->tabbed)               // tabbed items should skip this 
		{
			if (mousePos.x > (*item)->hitBox.x && mousePos.x < (*item)->hitBox.x + (*item)->hitBox.w
				&& mousePos.y >(*item)->hitBox.y && mousePos.y < (*item)->hitBox.y + (*item)->hitBox.h)
			{

				if ((*item)->state != CLICK && mouseButtonDown != 0)
				{
					(*item)->mouseButtonDown = mouseButtonDown;
					// thisItem->data->OnClickDown();               // TODO: function pointers
					(*item)->state = CLICK;
					selected_object = *item;
					//ResolveChildren(selected_object); 
				}

				if (((*item)->state == CLICK || (*item)->state == DRAG) && App->input->GetMouseButtonDown((*item)->mouseButtonDown) == KEY_UP)
				{
					// thisItem->data->OnClickUp();               // TODO: function pointers
					(*item)->state = HOVER;
				}

				if (App->input->GetMouseButtonDown((*item)->mouseButtonDown) == KEY_REPEAT)
				{
					(*item)->state = DRAG;
				}

				

				else if ((*item)->state == IDLE)
					(*item)->state = HOVER;

			

			}
			else  if ((*item)->state != IDLE)
			{                                        // TODO: With sliders, this does not apply (FOCUS)
				(*item)->state = IDLE;
			}
			if (App->input->GetMouseButtonDown((*item)->mouseButtonDown) == KEY_UP || App->input->GetMouseButtonDown((*item)->mouseButtonDown) == KEY_IDLE)
				(*item)->mouseButtonDown = 0;


		}
	}

}

/*
void j1Gui::ResolveChildren(UiItem* parent){

	std::list<UiItem*>::iterator item = ListItemUI.begin();

	for (; item != ListItemUI.end(); item++)
	{
		if ((*item)->parent == parent) {
			selected_object = (*item);                 // this is just a base, no sense now 
		}
	
	}

}*/

void j1Gui::ApplyTabBetweenSimilar(bool setClicked) {

	// first check that, while one object is tabbed, the user can hover another one (CAUTION HERE): 
	
	
	
	if (selected_object != nullptr && selected_object->guiType == BAR)
	{
		std::list<UiItem*>::iterator theNewOne = ListItemUI.begin();

		for (; theNewOne != ListItemUI.end(); theNewOne++)
		{
			if ((*theNewOne)->guiType == BAR && selected_object != (*theNewOne))
			{
				if ( (*theNewOne)->state == CLICK)
				{
					selected_object->tabbed = false;
					selected_object->state = IDLE;               // deselect current object
					selected_object->DoLogicAbandoned();

					selected_object = (*theNewOne);
					(*theNewOne)->state = HOVER;
					(*theNewOne)->tabbed = true;
				}
			}
		}

	}



	std::list<UiItem*>::iterator item = ListItemUI.begin();
	if (!setClicked)
	{
		for (; item != ListItemUI.end(); item++)                   // this should work for all types
		{
			if ((*item)->guiType == BAR)
			{

				selected_object = (*item);     // first set as selected the leftmost bar (first created)  
				selected_object->state = HOVER;
				selected_object->tabbed = true;
				setClicked = true;
				break; 
			}
		}
	}
	else                                                       // this is done in loops
	{
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{
			std::list<UiItem*>::iterator item = ListItemUI.begin();
			std::list<UiItem*> candidates; 
			
			for (; item != ListItemUI.end(); item++)                
			{
				if ((*item)->guiType == BAR)
				{
					if ((*item)->hitBox.x > selected_object->hitBox.x + selected_object->hitBox.w)
					{
						selected_object->tabbed = false;
						selected_object->state = IDLE;               // deselect current object
						selected_object->DoLogicAbandoned();           

						candidates.push_back(*item);       // add objects on the right to a list
						
					}
				}
			}

			if (!candidates.empty()) {              // IF there ARE items on the right, select the closest one

				int distanceToBeat = 10000;
				int currentDistance = 0;

				item = candidates.begin(); 

				for (; item != candidates.end(); item++)        // distance between objects:
				{
					currentDistance = (*item)->hitBox.x - (selected_object->hitBox.x + selected_object->hitBox.w);

					if (currentDistance < distanceToBeat)
					{                   
						distanceToBeat = currentDistance;
						selected_object = (*item);                     // make the closest item be the current one 
					          
	 
					}
				}

				selected_object->state = HOVER;
				selected_object->tabbed = true;
			}
			
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		{
			std::list<UiItem*>::iterator item = ListItemUI.begin();
			std::list<UiItem*> candidates;

			for (; item != ListItemUI.end(); item++)
			{
				if ((*item)->guiType == BAR)
				{
					if ((*item)->hitBox.x + (*item)->hitBox.w < selected_object->hitBox.x)

					{
						selected_object->tabbed = false;
						selected_object->state = IDLE;               // deselect current object
						selected_object->DoLogicAbandoned();

						candidates.push_back(*item);       // add objects on the left to a list

					}
				}
			}

			if (!candidates.empty()) {              // IF there ARE items on the left, select the closest one

				int distanceToBeat = 10000;
				int currentDistance = 0;

				item = candidates.begin();

				for (; item != candidates.end(); item++)        // distance between objects:
				{
					currentDistance = selected_object->hitBox.x - ((*item)->hitBox.x + (*item)->hitBox.w); 

					if (currentDistance < distanceToBeat)
					{
						distanceToBeat = currentDistance;  
						selected_object = (*item);           // make the closest item be the current one 
					}
    
				}

				
				selected_object->state = HOVER;            
				selected_object->tabbed = true;
				
			}

		}



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
		//(*iter)->Draw(dt);
		canvas->DrawUi(dt);

		if (debug_)
		{
			SDL_Rect r;
			r.x = (*iter)->hitBox.x; 
			r.y = (*iter)->hitBox.y;
			r.w = (*iter)->hitBox.w;
			r.h = (*iter)->hitBox.h; 
			if((*iter)->state == CLICK || (*iter)->state == DRAG)
			App->render->DrawQuad(r, 100, 50, 200, 200, true, false);
			else if((*iter)->state == HOVER)
				App->render->DrawQuad(r, 110, 125, 240, 200, true, false);
			else
				App->render->DrawQuad(r, 100, 50, 200, 100, true, false);
			
		}

	}
	return true;
}

bool j1Gui::CleanUp()
{
	App->tex->UnLoad(atlas); 
	return true;
}

UiItem_Label * j1Gui::AddLabel(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent)
{
	UiItem* newUIItem = nullptr;
	newUIItem = new UiItem_Label(text, color, font, position, parent);
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


