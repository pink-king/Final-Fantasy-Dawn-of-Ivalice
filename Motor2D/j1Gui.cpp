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
		switch (selected_object->state) 
		{

		case CLICK: selected_object->DoLogicClicked();
			break;
		case HOVER: selected_object->DoLogicHovered();
			break;
		case IDLE: selected_object->DoLogicAbandoned();
			break;

		}
	}

}



void j1Gui::ApplyTabBetweenSimilar(bool setClicked) {


	
	// INPUT - - - - - - - - - - - - - - - - - - - - -
	
 if (selected_object && selected_object->tabbed)
	{
		switch (selected_object->guiType)
		{

		case GUI_TYPES::CHECKBOX:
			if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				std::string function = selected_object->function;
				selected_object->DoLogicClicked();
			}

			break;
		case GUI_TYPES::BUTTON:
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
			{
				selected_object->state = CLICK;
			}
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
			{
				std::string function = selected_object->function;
				selected_object->DoLogicClicked(function);
				selected_object->state = HOVER;
			}
			break;


		}

		
	}
	
	
	
	// MOVEMENT - - - - - - - - - - - - - - - - - - - - -

	std::list<UiItem*>::iterator item = ListItemUI.begin();
	if (!setClicked)
	{
		for (; item != ListItemUI.end(); item++)                   // this should work for all types
		{
			if ((*item)->parent->enable)
			{
				if ((*item)->guiType == BAR)
				{
					selected_object = (*item);     // first set as selected the leftmost bar (first created)  
					selected_object->state = HOVER;
					selected_object->tabbed = true;
					setClicked = true;
					break;
				}
				if ((*item)->guiType == CHECKBOX)
				{

					selected_object = (*item);     // first set as selected the leftmost bar (first created)  
					selected_object->state = HOVER;
					selected_object->tabbed = true;
					setClicked = true;
					break;
				}
				if ((*item)->guiType == BUTTON)
				{

					selected_object = (*item);     // first set as selected the leftmost bar (first created)  
					selected_object->state = HOVER;
					selected_object->tabbed = true;
					setClicked = true;
					break;
				}
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
				if ((*item)->parent == selected_object->parent && (*item)->parent->enable)
				{
					if ((*item)->hitBox.x > selected_object->hitBox.x + selected_object->hitBox.w && (*item)->hitBox.y == selected_object->hitBox.y)
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
				if ((*item)->parent == selected_object->parent && (*item)->parent->enable)
				{
					if ((*item)->hitBox.x + (*item)->hitBox.w < selected_object->hitBox.x && (*item)->hitBox.y == selected_object->hitBox.y)

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

		


		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
		{
			std::list<UiItem*>::iterator item = ListItemUI.begin();
			std::list<UiItem*> candidates;

			for (; item != ListItemUI.end(); item++)
			{
				if ((*item)->parent == selected_object->parent && (*item)->parent->enable)
				{
					if ((*item)->hitBox.y < selected_object->hitBox.y && (*item)->hitBox.x == selected_object->hitBox.x)
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
				if ((*item) != selected_object && (*item)->parent == selected_object->parent && (*item)->parent->enable)
				{
					LOG("Trying to taaaaaab   selected : %i vs next: %i", selected_object->hitBox.y, (*item)->hitBox.y);
					if ((*item)->hitBox.y > selected_object->hitBox.y && (*item)->hitBox.x == selected_object->hitBox.x)
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
				if ((*iter)->state == CLICK)
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
	if (atlas != nullptr)
		App->tex->UnLoad(atlas);  
	// TODO: Remove items from list, not hitlabels (they are on their own list)
	
	ListItemUI.clear();
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

UiItem_Button * j1Gui::AddButton(iPoint position, std::string function, const SDL_Rect * idle, UiItem * const parent, const SDL_Rect * click, const SDL_Rect * hover)
{
	UiItem* newUIItem = nullptr;

	if (parent == NULL)
		newUIItem = new UiItem_Button(position, function, idle, canvas, click, hover);
	else
		newUIItem = new UiItem_Button(position, function, idle, parent, click, hover);

	ListItemUI.push_back(newUIItem);

	UiItem_Button* button = (UiItem_Button*)newUIItem;
	button->AddFuntion(function);

	return (UiItem_Button*)newUIItem;
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

UiItem_Checkbox * j1Gui::AddCheckbox(iPoint position, const SDL_Rect* panel_section, const SDL_Rect* box_section, const SDL_Rect* tick_section, labelInfo* labelInfo, UiItem*const parent)
{
	UiItem* newUIItem = nullptr;

	newUIItem = new UiItem_Checkbox(position, panel_section, box_section, tick_section, labelInfo, parent); 
	ListItemUI.push_back(newUIItem);

	return (UiItem_Checkbox*)newUIItem;
}

UiItem_HitPoint * j1Gui::AddHitPointLabel(valueInfo valueInfo, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent, variant type)
{
	UiItem_HitPoint* newUIItem = nullptr;
	newUIItem = new UiItem_HitPoint(valueInfo, color, font, position, parent, type);
	
	
	
  // Add the hitpoint directly to the hitpoin manager list, and not in the listitemui

	App->HPManager->hitPointLabels.push_back(newUIItem);


	return (UiItem_HitPoint*)newUIItem;

}


UiItem_HitPoint * j1Gui::AddHitPointLabel2(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent, variant type)
{
	UiItem_HitPoint* newUIItem = nullptr;
	newUIItem = new UiItem_HitPoint(text, color, font, position, parent, type);



	// Add the hitpoint directly to the hitpoin manager list, and not in the listitemui

	App->HPManager->hitPointLabels.push_back(newUIItem);


	return (UiItem_HitPoint*)newUIItem;

}



UiItem_HealthBar * j1Gui::AddHealthBar(iPoint position, const SDL_Rect* staticSection, const SDL_Rect* dynamicSection, type variant, UiItem*const parent) // , TypeBar type)
{
	UiItem* newUIItem = nullptr;

	newUIItem = new UiItem_HealthBar(position, staticSection, dynamicSection, variant, parent);

	ListItemUI.push_back(newUIItem);

	return (UiItem_HealthBar*)newUIItem;

}



SDL_Texture * j1Gui::GetAtlas()
{
	return atlas;
}

void j1Gui::FadeToScene()
{
	resetHoverSwapping = false;
	App->scene->state = SceneState::GAME;
}

void j1Gui::ExitGame()
{
	App->scene->exitGame = true;
}

void j1Gui::SettingsScreen()
{
	resetHoverSwapping = false;
	App->scene->startMenu->enable = false;
	App->scene->settingPanel->enable = true;
}

void j1Gui::GoBackToMenu()
{
	resetHoverSwapping = false;
	App->scene->settingPanel->enable = false;
	App->scene->startMenu->enable = true;
}

