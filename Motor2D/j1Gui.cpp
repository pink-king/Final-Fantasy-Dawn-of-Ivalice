#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1TransitionManager.h"
#include "p2Log.h"
#include "Brofiler/Brofiler.h"

j1Gui::j1Gui() : j1Module()
{
	name.assign("gui");
	canvas = DBG_NEW UiItem({ 0,0 }, NULL);
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
	lootTexture = App->tex->Load("textures/loot/loot_items.png");

	if (hurt_hud_tex == nullptr)
		hurt_hud_tex = App->tex->Load("textures/hud dmg/playerhurt.png");

	if (hurt_hud_tex2 == nullptr)
		hurt_hud_tex2 = App->tex->Load("textures/hud dmg/hurt2.png");

	if (hurt_hud_tex3 == nullptr)
		hurt_hud_tex3 = App->tex->Load("textures/hud dmg/hurt3.png");
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

	if (selected_object && selected_object->tabbed && selected_object->parent->enable)
	{
		switch (selected_object->guiType)
		{

		case GUI_TYPES::CHECKBOX:
			if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
			{
				std::string function = selected_object->function;
				selected_object->DoLogicClicked(function);
			}

			break;
		case GUI_TYPES::BUTTON:
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT || App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_A) == KEY_REPEAT)
			{
				selected_object->state = CLICK;
			}
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP || App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_A) == KEY_UP)
			{
				std::string function = selected_object->function;
				selected_object->DoLogicClicked(function);
				
				if (function == "SocialMedia")
					selected_object->state = HOVER;
				else
					selected_object->state = IDLE;

				selected_object->tabbed = true;
				
			}
			break;


		}
		

	}




	// MOVEMENT - - - - - - - - - - - - - - - - - - - - -

	std::list<UiItem*>::iterator item = ListItemUI.begin();
	if (!setClicked)
	{
		iPoint item_pos = { 0,0 };
		bool first = false;
		for (; item != ListItemUI.end(); ++item)
		{
			if (((*item)->guiType == BAR || (*item)->guiType == CHECKBOX || (*item)->guiType == BUTTON || (*item)->tabbable) && (*item)->parent->enable && !(*item)->hide)
			{
				if (!first)
				{
					
					item_pos.x = (*item)->hitBox.x;
					item_pos.y = (*item)->hitBox.y;
					first = true;
					selected_object = (*item);

					selected_object->state = HOVER;
					selected_object->tabbed = true;
					setClicked = true;
				}
				else if (first && (*item)->hitBox.x <= item_pos.x && (*item)->hitBox.y <= item_pos.y)
				{
					item_pos.x = (*item)->hitBox.x;
					item_pos.y = (*item)->hitBox.y;
					selected_object->state = IDLE;
					selected_object->tabbed = false;                          // solve merge
					selected_object = (*item);


					selected_object->state = HOVER;
					selected_object->tabbed = true;
					setClicked = true;
				}
			}
		}



		/*for (; item != ListItemUI.end(); item++)                   // this should work for all types
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
		}*/
	}
	else                                                       // this is done in loops
	{
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || 
			App->input->GetJoystickPulsation(JOY_STICK_LEFT, JOYSTICK_DIR_RIGHT) == KEY_DOWN ||
			App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_DOWN)
		{
			std::list<UiItem*>::iterator item = ListItemUI.begin();
			std::list<UiItem*> candidates;
			for (; item != ListItemUI.end(); item++)
			{
				if (selected_object && (*item)->parent == selected_object->parent && (*item)->parent->enable && !(*item)->hide)
				{
					if ((*item)->hitBox.x > selected_object->hitBox.x + selected_object->hitBox.w && (*item)->hitBox.y == selected_object->hitBox.y)
					{
						selected_object->tabbed = false;
						selected_object->state = IDLE;               // deselect current object
						selected_object->DoLogicAbandoned();
						App->audio->PlayFx(App->scene->selectUI, 0);

						candidates.push_back(*item);       // add objects on the right to a list

					}
				}
			}

			if (!candidates.empty()) {              // IF there ARE items on the right, select the closest one

				int distanceToBeat = 10000;
				int currentDistance = 0;
				UiItem* first_selected = selected_object;
				item = candidates.begin();

				for (; item != candidates.end(); item++)        // distance between objects:
				{
					currentDistance = (*item)->hitBox.x - (first_selected->hitBox.x + first_selected->hitBox.w);

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

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || 
			App->input->GetJoystickPulsation(JOY_STICK_LEFT, JOYSTICK_DIR_LEFT) == KEY_DOWN ||
			App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_DOWN)
		{
			std::list<UiItem*>::iterator item = ListItemUI.begin();
			std::list<UiItem*> candidates;

			for (; item != ListItemUI.end(); item++)
			{
				if (selected_object && (*item)->parent == selected_object->parent && (*item)->parent->enable && !(*item)->hide)
				{
					if ((*item)->hitBox.x + (*item)->hitBox.w < selected_object->hitBox.x && (*item)->hitBox.y == selected_object->hitBox.y)

					{
						selected_object->tabbed = false;
						selected_object->state = IDLE;               // deselect current object
						selected_object->DoLogicAbandoned();
						App->audio->PlayFx(App->scene->selectUI, 0);

						candidates.push_back(*item);       // add objects on the left to a list

					}
				}
			}

			if (!candidates.empty()) {              // IF there ARE items on the left, select the closest one

				int distanceToBeat = 10000;
				int currentDistance = 0;
				UiItem* first_selected = selected_object;
				item = candidates.begin();

				for (; item != candidates.end(); item++)        // distance between objects:
				{
					currentDistance = first_selected->hitBox.x - ((*item)->hitBox.x + (*item)->hitBox.w);

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




		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || 
			App->input->GetJoystickPulsation(JOY_STICK_LEFT, JOYSTICK_DIR_UP) == KEY_DOWN ||
			App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN)
		{
			std::list<UiItem*>::iterator item = ListItemUI.begin();
			std::list<UiItem*> candidates;

			for (; item != ListItemUI.end(); item++)
			{
				if (selected_object && (*item)->parent == selected_object->parent && (*item)->parent->enable && !(*item)->hide && !(*item)->isNPCLabel)
				{
					if ((*item)->hitBox.y < selected_object->hitBox.y && (*item)->hitBox.x == selected_object->hitBox.x)
					{
						
							selected_object->tabbed = false;
							selected_object->state = IDLE;               // deselect current object
							selected_object->DoLogicAbandoned();
							App->audio->PlayFx(App->scene->selectUI, 0);

							candidates.push_back(*item);       // add objects on the right to a list

						
					
					}
				}
			}

			if (!candidates.empty()) {              // IF there ARE items on the right, select the closest one

				int distanceToBeat = 10000;
				int currentDistance = 0;
				UiItem* first_selected = selected_object;
				item = candidates.begin();

				for (; item != candidates.end(); item++)        // distance between objects:
				{
					currentDistance = first_selected->hitBox.y - (*item)->hitBox.y;

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


		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || 
			App->input->GetJoystickPulsation(JOY_STICK_LEFT, JOYSTICK_DIR_DOWN) == KEY_DOWN ||
			App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN)
		{
			std::list<UiItem*>::iterator item = ListItemUI.begin();
			std::list<UiItem*> candidates;

			for (; item != ListItemUI.end(); item++)
			{
				if (selected_object && (*item) != selected_object && (*item)->parent == selected_object->parent && (*item)->parent->enable && !(*item)->hide)
				{
					LOG("Trying to taaaaaab   selected : %i vs next: %i", selected_object->hitBox.y, (*item)->hitBox.y);
					if ((*item)->hitBox.y > selected_object->hitBox.y && (*item)->hitBox.x == selected_object->hitBox.x)
					{
						selected_object->tabbed = false;
						selected_object->state = IDLE;               // deselect current object
						selected_object->DoLogicAbandoned();
						App->audio->PlayFx(App->scene->selectUI, 0);

						candidates.push_back(*item);       // add objects on the right to a list

					}
				}
			}

			if (!candidates.empty()) {              // IF there ARE items on the right, select the closest one

				int distanceToBeat = 10000;
				int currentDistance = 0;
				UiItem* first_selected = selected_object;
				item = candidates.begin();

				for (; item != candidates.end(); item++)        // distance between objects:
				{
					currentDistance = (*item)->hitBox.y - first_selected->hitBox.y;

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

	BROFILER_CATEGORY("UI PostUpdates", Profiler::Color::Yellow);

	App->render->Blit(hurt_hud_tex3, 0, 0, 0, 0.0f);
	App->render->Blit(hurt_hud_tex2, 0, 0, 0, 0.0f);
	App->render->Blit(hurt_hud_tex, 0, 0, 0, 0.0f);

	// temporal debug 
	
	
	//if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN) {
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {

		debug_ = !debug_;
	}

	canvas->DrawUi(dt);
	for (std::list<UiItem*>::iterator iter = ListItemUI.begin(); iter != ListItemUI.end(); ++iter)
	{
		//(*iter)->Draw(dt);
		
		if (debug_)
		{

			if ((*iter)->hitBox.x > 0 && (*iter))
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
	{
		App->tex->UnLoad(atlas);
		atlas = nullptr;
	}
	if (lootTexture != nullptr)
	{
		App->tex->UnLoad(lootTexture);
		lootTexture = nullptr;
	}
	if (hurt_hud_tex != nullptr)
	{
		App->tex->UnLoad(hurt_hud_tex);
		hurt_hud_tex = nullptr;
	}

	if (hurt_hud_tex2 != nullptr)
	{
		App->tex->UnLoad(hurt_hud_tex2);
		hurt_hud_tex = nullptr;
	}

	if (hurt_hud_tex3 != nullptr)
	{
		App->tex->UnLoad(hurt_hud_tex3);
		hurt_hud_tex3 = nullptr;
	}
	/*delete canvas;
	canvas = nullptr;*/

	// TODO: Remove items from list, not hitlabels (they are on their own list)
	for (std::list<UiItem*>::iterator item = ListItemUI.begin(); item != ListItemUI.end(); ++item)
	{
		if ((*item) != nullptr)
		{
			(*item)->CleanUp();
			ListItemUI.remove(*item);
			delete *item;
			*item = nullptr;
		}
	}
	ListItemUI.clear();


	return true;
}

void j1Gui::destroyElement(UiItem * elem)
{

	for (std::list<UiItem*>::iterator item = ListItemUI.begin(); item != ListItemUI.end(); ++item)
	{
		if (elem != nullptr && (*item) == elem)
		{
			delete (*item);
			*item = nullptr;

			item = ListItemUI.erase(item);

		}

	}
}

void j1Gui::deleteCurrentDialogs()
{
	for (std::list<UiItem*>::iterator item = ListItemUI.begin(); item != ListItemUI.end();)
	{
		if ((*item) && !(*item)->to_delete)
		{
			if ((*item)->isDialog)
			{
				delete (*item);
				item = ListItemUI.erase(item);
			}
			else
			{
				++item; 
			}
		}
	}

}


UiItem_Label * j1Gui::AddLabel(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem * const parent, bool type_writer)
{
	UiItem* newUIItem = nullptr;
	newUIItem = DBG_NEW UiItem_Label(text, color, font, position, parent, type_writer);
	ListItemUI.push_back(newUIItem);
	return (UiItem_Label*)newUIItem;
}

UiItem_Image* j1Gui::AddImage(iPoint position, const SDL_Rect* section, std::string& name, UiItem* const parent, bool swapPosition, bool isPanel)
{
	UiItem* newUIItem = nullptr;

	if (parent == NULL)
		newUIItem = DBG_NEW UiItem_Image(position, section, name, canvas, swapPosition, isPanel);
	else
		newUIItem = DBG_NEW UiItem_Image(position, section, name, parent, swapPosition, isPanel);

	ListItemUI.push_back(newUIItem);

	return (UiItem_Image*)newUIItem;
}



UiItem_Image* j1Gui::AddSpecialImage(iPoint position, const SDL_Rect * section, UiItem * const parent, SDL_Texture * newTex, UiItem_Description * myDescr)
{
	UiItem* newUIItem = nullptr;


	newUIItem = new UiItem_Image(position, section, parent, newTex, myDescr);

	ListItemUI.push_back(newUIItem);

	return (UiItem_Image*)newUIItem;
}

UiItem_Inventory* j1Gui::AddInventory(UiItem * const parent)
{
	UiItem* newUIItem = nullptr;
	newUIItem = DBG_NEW UiItem_Inventory(parent);
	ListItemUI.push_back(newUIItem);
	return (UiItem_Inventory*)newUIItem;
}

CharacterStats * j1Gui::AddCharacterStatsItem(UiItem * const parent)
{
	UiItem* newUIItem = nullptr;
	newUIItem = DBG_NEW CharacterStats(parent);
	ListItemUI.push_back(newUIItem);
	return (CharacterStats*)newUIItem; 
}

CharacterStatBlock * j1Gui::AddCharacterStatBlock( UiItem_Label * Name, UiItem_Label * bStat, UiItem_Label * nStat, Uint8 bValue, Uint8 nValue, UiItem_Label * Arrows, UiItem_Image* Icons, UiItem* const parent)
{
	UiItem* newUIItem = nullptr;
	newUIItem = DBG_NEW CharacterStatBlock(Name, bStat, nStat, bValue, nValue, Arrows, Icons, parent);
	ListItemUI.push_back(newUIItem);
	return (CharacterStatBlock*)newUIItem;
}

// TODO: add character stat manager


UiItem_Bar* j1Gui::AddBar(iPoint position, std::string name, const SDL_Rect * section, const SDL_Rect * thumb_section, const SDL_Rect * image_idle, const SDL_Rect * image_hover, UiItem * const parent)
{
	UiItem* newUIItem = nullptr;

	newUIItem = DBG_NEW UiItem_Bar(position, name, section, thumb_section, image_idle, image_hover, parent);

	ListItemUI.push_back(newUIItem);

	return (UiItem_Bar*)newUIItem;
}



UiItem_Button* j1Gui::AddButton(iPoint position, std::string function, std::string name, const SDL_Rect * idle, UiItem * const parent, const SDL_Rect * click, const SDL_Rect * hover)
{
	UiItem* newUIItem = nullptr;

	if (parent == NULL)
		newUIItem = DBG_NEW UiItem_Button(position, function, name, idle, canvas, click, hover);
	else
		newUIItem = DBG_NEW UiItem_Button(position, function, name,  idle, parent, click, hover);

	ListItemUI.push_back(newUIItem);

	UiItem_Button * button = (UiItem_Button*)newUIItem;
	button->AddFuntion(function);

	return (UiItem_Button*)newUIItem;
}



UiItem* j1Gui::AddEmptyElement(iPoint pos, UiItem * const parent)
{
	UiItem* newUIItem = nullptr;
	if (parent == NULL)
		newUIItem = DBG_NEW UiItem(pos, canvas);
	else
		newUIItem = DBG_NEW UiItem(pos, parent);

	ListItemUI.push_back(newUIItem);
	return newUIItem;
}

UiItem_Checkbox* j1Gui::AddCheckbox(iPoint position, std::string & function, std::string name, const SDL_Rect * panel_section, const SDL_Rect * box_section, const SDL_Rect * tick_section, labelInfo * labelInfo, UiItem * const parent)
{
	UiItem* newUIItem = nullptr;

	newUIItem = DBG_NEW UiItem_Checkbox(position, function,name, panel_section, box_section, tick_section, labelInfo, parent);
	ListItemUI.push_back(newUIItem);

	return (UiItem_Checkbox*)newUIItem;
}

UiItem_HitPoint* j1Gui::AddHitPointLabel(valueInfo valueInfo, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem * const parent, variant type)
{
	UiItem_HitPoint* newUIItem = nullptr;
	newUIItem = DBG_NEW UiItem_HitPoint(valueInfo, color, font, position, parent, type);



	// Add the hitpoint directly to the hitpoin manager list, and not in the listitemui

	App->HPManager->hitPointLabels.push_back(newUIItem);


	return (UiItem_HitPoint*)newUIItem;

}


UiItem_HitPoint* j1Gui::AddHitPointLabel2(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem * const parent, variant type)
{
	UiItem_HitPoint* newUIItem = nullptr;
	newUIItem = DBG_NEW UiItem_HitPoint(text, color, font, position, parent, type);



	// Add the hitpoint directly to the hitpoin manager list, and not in the listitemui

	App->HPManager->hitPointLabels.push_back(newUIItem);


	return (UiItem_HitPoint*)newUIItem;

}



UiItem_HealthBar* j1Gui::AddHealthBar(iPoint position, const SDL_Rect * dynamicSection, const SDL_Rect * damageSection, type variant, UiItem * const parent) // , TypeBar type)
{
	UiItem* newUIItem = nullptr;

	newUIItem = DBG_NEW UiItem_HealthBar(position, dynamicSection, damageSection, variant, parent);

	ListItemUI.push_back(newUIItem);

	return (UiItem_HealthBar*)newUIItem;

}



UiItem_HealthBar* j1Gui::AddHealthBarToEnemy(const SDL_Rect * dynamicSection, type variant, j1Entity * deliever, UiItem * const parent) // , TypeBar type)
{
	UiItem* newUIItem = nullptr;

	newUIItem = DBG_NEW UiItem_HealthBar(dynamicSection, variant, parent, deliever);

	ListItemUI.push_back(newUIItem);

	return (UiItem_HealthBar*)newUIItem;

}

UiItem_HealthBar* j1Gui::AddHealthBarToBoss(iPoint position, const SDL_Rect* dynamicSection, const SDL_Rect* staticSection, const SDL_Rect* divSection, type variant, uint maxLife, j1Entity* deliever, UiItem* const parent)
{
	UiItem* newUIItem = nullptr;

	newUIItem = DBG_NEW UiItem_HealthBar(position, dynamicSection, staticSection, divSection, variant, maxLife, deliever, parent);

	ListItemUI.push_back(newUIItem);

	return (UiItem_HealthBar*)newUIItem;

}

UiItem_CooldownClock* j1Gui::AddClock(iPoint position, SDL_Rect * section, std::string type, std::string charName, UiItem * const parent)
{
	UiItem* newUIItem = nullptr;

	newUIItem = DBG_NEW UiItem_CooldownClock(position, section, type, charName, parent);

	ListItemUI.push_back(newUIItem);


	return (UiItem_CooldownClock*)newUIItem;

}



UiItem_Description* j1Gui::AddDescriptionToEquipment(iPoint position, std::string itemName, const SDL_Rect * panelRect, const SDL_Rect * iconRect, float Value, EquipmentStatType variableType, uint level, LootEntity * callback, UiItem * const parent, uint price)
{

	UiItem* newUIItem = nullptr;

	newUIItem = DBG_NEW UiItem_Description(position, itemName, panelRect, iconRect, Value, variableType, level, callback, parent, price);

	ListItemUI.push_back(newUIItem);


	return (UiItem_Description*)newUIItem;

}

UiItem_Description* j1Gui::AddDescriptionToWeapon(iPoint position, std::string itemName, const SDL_Rect * panelRect, const SDL_Rect * iconRect, float Attack, float resistance, float cooldown, uint level, LootEntity * callback, UiItem * const parent, uint price) {

	UiItem* newUIItem = nullptr;

	newUIItem = DBG_NEW UiItem_Description(position, itemName, panelRect, iconRect, Attack, resistance, cooldown, level, callback, parent, price);

	ListItemUI.push_back(newUIItem);


	return (UiItem_Description*)newUIItem;

}

UiItem_Description* j1Gui::AddDescriptionToPotion(iPoint position, std::string itemName, const SDL_Rect * panelRect, const SDL_Rect * iconRect, std::string effect, iPoint HPandTime, LootEntity * callback, UiItem * const parent, uint price)
{
	UiItem* newUIItem = nullptr;

	newUIItem = DBG_NEW UiItem_Description(position, itemName, panelRect, iconRect, effect, HPandTime, callback, parent, price);

	ListItemUI.push_back(newUIItem);
	

	return (UiItem_Description*)newUIItem;
}





SDL_Texture* j1Gui::GetAtlas()
{
	return atlas;
}



void j1Gui::FadeToScene()
{
	resetHoverSwapping = false;
	App->transitionManager->CreateFadeTransition(1.F, true, SceneState::LOBBY);
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
	App->scene->creditsPanel->enable = false;
	App->scene->startMenu->enable = true;
}

void j1Gui::FpsCap()
{


}

void j1Gui::GoBackToGame()
{
	App->pause = false;
	App->scene->pausePanel->enable = false;
}

void j1Gui::Credits()
{
	resetHoverSwapping = false;
	App->scene->startMenu->enable = false;
	App->scene->creditsPanel->enable = true;
}


void j1Gui::SocialMedia(std::string &name)
{
	if (name == "instagram")
	{
		ShellExecuteA(NULL, "open", "https://www.instagram.com/pinkking_games/?hl=es", NULL, NULL, SW_SHOWNORMAL);
	}
	if (name == "youtube")
	{
		ShellExecuteA(NULL, "open", "https://www.youtube.com/channel/UCIPDvRkdBphrM7Lle0wrIcA", NULL, NULL, SW_SHOWNORMAL);
	}
	if (name == "twitter")
	{
		ShellExecuteA(NULL, "open", "https://twitter.com/PinkKingGames", NULL, NULL, SW_SHOWNORMAL);
	}
	if (name == "github")
	{
		ShellExecuteA(NULL, "open", "https://github.com/pink-king", NULL, NULL, SW_SHOWNORMAL);
	}
	if (name == "Sebastia")
	{
		ShellExecuteA(NULL, "open", "https://github.com/Sebi-Lopez", NULL, NULL, SW_SHOWNORMAL);
	}
	if (name == "Jose")
	{
		ShellExecuteA(NULL, "open", "https://github.com/peterMcP", NULL, NULL, SW_SHOWNORMAL);
	}
	if (name == "Carlos")
	{
		ShellExecuteA(NULL, "open", "https://github.com/CarlosUPC", NULL, NULL, SW_SHOWNORMAL);
	}
	if (name == "Doctor")
	{
		ShellExecuteA(NULL, "open", "https://github.com/thedoctormarc", NULL, NULL, SW_SHOWNORMAL);
	}
	if (name == "Didac")
	{
		ShellExecuteA(NULL, "open", "https://github.com/didaclis", NULL, NULL, SW_SHOWNORMAL);
	}
	if (name == "Gerard")
	{
		ShellExecuteA(NULL, "open", "https://github.com/GerardClotet", NULL, NULL, SW_SHOWNORMAL);
	}
	if (name == "Pol")
	{
		ShellExecuteA(NULL, "open", "https://github.com/PolRecasensSarra", NULL, NULL, SW_SHOWNORMAL);
	}
}

void j1Gui::GoBackToStartMenu()
{
	resetHoverSwapping = false;
	App->pause = false;
	App->transitionManager->CreateFadeTransition(1.F, true, SceneState::STARTMENU);
}

void j1Gui::GoBackToStartMenuFromDeathWin()
{
	resetHoverSwapping = false;
	App->transitionManager->CreateFadeTransition(1.F, true, SceneState::STARTMENU);
}


