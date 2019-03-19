#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Map.h"


j1Gui::j1Gui() : j1Module() 
{
}


j1Gui::~j1Gui()
{
}

bool j1Gui::Awake(pugi::xml_node& conf)
{
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");
	return true;
}

bool j1Gui::Start()
{
	atlas = App->tex->Load("gui/atlas.png");
	return true;
}

bool j1Gui::Update(float dt)
{
	return true;
}

bool j1Gui::PostUpdate()
{
	canvas->DrawUi(dt);
	return true;
}

bool j1Gui::CleanUp()
{
	return true;
}

UiItem_Label * j1Gui::AddLabel(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position)
{
	UiItem* newUIItem = nullptr;
	newUIItem = new UiItem_Label(text, color, font, position);
	ListItemUI.push_back(newUIItem);
	return (UiItem_Label*)newUIItem;
	
}

UiItem_Image * j1Gui::AddImage(iPoint position, SDL_Rect section)
{
	UiItem* newUIItem = nullptr;
	
	newUIItem = new UiItem_Image(position, section);

	ListItemUI.push_back(newUIItem);

	return (UiItem_Image*)newUIItem;
}

const SDL_Texture * j1Gui::GetAtlas() const
{
	return atlas;
}
