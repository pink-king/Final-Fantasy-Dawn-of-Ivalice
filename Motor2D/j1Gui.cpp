#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Window.h"
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



	/*std::list<UiItem*>::iterator item = ListItemUI.begin();

	for (; item != ListItemUI.end(); item++)
	{
		if ((*item)->tabbed)               // tabbed items should skip this 
		{
			if ((*item)->state != CLICK && mouseButtonDown != 0)
			{

				if (selected_object->tabbed && (*item)->guiType == selected_object->guiType)    // condition so that if there was a tabbed object, it must not be selected now
				{
					selected_object->tabbed = false;
					selected_object->state = IDLE;               // deselect current object
					selected_object->DoLogicAbandoned();


					selected_object = (*item);                     // the selected object is now tabbed
					selected_object->state = HOVER;
					selected_object->tabbed = true;

				}
				else
				{
				(*item)->mouseButtonDown = mouseButtonDown;
				(*item)->state = CLICK;
				selected_object = *item;
				}


			}

			if (((*item)->state == CLICK) && App->input->GetMouseButtonDown((*item)->mouseButtonDown) == KEY_UP)
			{
				(*item)->state = HOVER;
			}

			if (App->input->GetMouseButtonDown((*item)->mouseButtonDown) == KEY_REPEAT)
			{
				(*item)->state = DRAG;
			}



			else if ((*item)->state == IDLE)
			{
				(*item)->state = HOVER;
				(*item)->DoLogicHovered(false);   // check this call
			}



		}                    // REWORK ALL THIS WITHOUT MOUSE 

		/*else  if ((*item)->state != IDLE)       // When an item is outside
		{


			if (!(*item)->slidable)        // If it is not slider, then it switches to IDLE
			{
				(*item)->state = IDLE;
				(*item)->DoLogicAbandoned(false);
			}
			else if ((*item)->state == DRAG)        // sliders can be dragged outside the bar
			{

				if (App->input->GetMouseButtonDown((*item)->mouseButtonDown) == KEY_UP)
				{
					(*item)->iFriend->state = IDLE;
					(*item)->state = IDLE;
				}
				else
				{
					(*item)->iFriend->DoLogicDragged(true); // slider thumbs are still hovered if mouse leaves the bar
				}
				}
			}*/


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


	
	// INPUT - - - - - - - - - - - - - - - - - - - - -
	
	
	if (selected_object && selected_object->tabbed && App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		selected_object->DoLogicClicked(false); 
	}

	
	
	
	// MOVEMENT - - - - - - - - - - - - - - - - - - - - -

	std::list<UiItem*>::iterator item = ListItemUI.begin();
	if (!setClicked)
	{
		for (; item != ListItemUI.end(); item++)                   // this should work for all types
		{
			if ((*item)->guiType == CHECKBOX)
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
		/*if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{
			std::list<UiItem*>::iterator item = ListItemUI.begin();
			std::list<UiItem*> candidates; 
			
			for (; item != ListItemUI.end(); item++)                
			{
				if ((*item)->parent == selected_object->parent)
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
				if ((*item)->parent == selected_object->parent)
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

		*/


		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
		{
			std::list<UiItem*>::iterator item = ListItemUI.begin();
			std::list<UiItem*> candidates;

			for (; item != ListItemUI.end(); item++)
			{
				if ((*item)->parent == selected_object->parent)
				{
					if ((*item)->hitBox.y < selected_object->hitBox.y)
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
					currentDistance =  selected_object->hitBox.y - (*item)->hitBox.y;

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


		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		{
			std::list<UiItem*>::iterator item = ListItemUI.begin();
			std::list<UiItem*> candidates;

			for (; item != ListItemUI.end(); item++)
			{
				if ((*item) != selected_object && (*item)->parent == selected_object->parent)
				{
					LOG("Trying to taaaaaab   selected : %i vs next: %i", selected_object->hitBox.y, (*item)->hitBox.y);
					if ((*item)->hitBox.y > selected_object->hitBox.y)
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
					currentDistance = (*item)->hitBox.y - selected_object->hitBox.y;

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

			if ((*iter)->hitBox.x > 0)
			{
				SDL_Rect r;
				r.x = (*iter)->hitBox.x;
				r.y = (*iter)->hitBox.y;
				r.w = (*iter)->hitBox.w;
				r.h = (*iter)->hitBox.h;
				if ((*iter)->state == CLICK || (*iter)->state == DRAG)
					App->render->DrawQuad(r, 100, 50, 200, 200, true, false);
				else if ((*iter)->state == HOVER)
					App->render->DrawQuad(r, 110, 125, 240, 200, true, false);
				else
					App->render->DrawQuad(r, 100, 50, 200, 100, true, false);

			}

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

UiItem_Image * j1Gui::AddImage(iPoint position, const SDL_Rect* section, UiItem *const parent, bool isPanel)
{
	UiItem* newUIItem = nullptr;

	if (parent == NULL)
		newUIItem = new UiItem_Image(position, section, canvas, isPanel);
	else
		newUIItem = new UiItem_Image(position, section, parent, isPanel);

	ListItemUI.push_back(newUIItem);

	return (UiItem_Image*)newUIItem;
}

UiItem_Bar * j1Gui::AddBar(iPoint position, const SDL_Rect* section, const SDL_Rect* thumb_section, UiItem*const parent) // , TypeBar type)
{
	UiItem* newUIItem = nullptr;

	newUIItem = new UiItem_Bar(position, section, thumb_section, parent); // , type);

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

UiItem_Checkbox * j1Gui::AddCheckbox(iPoint position, const SDL_Rect* panel_section, const SDL_Rect* box_section, const SDL_Rect* tick_section, UiItem*const parent)
{
	UiItem* newUIItem = nullptr;

	newUIItem = new UiItem_Checkbox(position, panel_section, box_section, tick_section); 
	ListItemUI.push_back(newUIItem);

	return (UiItem_Checkbox*)newUIItem;
}

SDL_Texture * j1Gui::GetAtlas()
{
	return atlas;
}


