#include "UiItem_Image.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "LootEntity.h"



UiItem_Image::UiItem_Image(iPoint position, const SDL_Rect * section, std::string& name, UiItem * const parent, bool swapPosition, bool isTabbable) : UiItem(position, name, parent)
{
	this->section = *section;
	this->guiType = GUI_TYPES::IMAGE;

	this->swapPosition = swapPosition;
	this->hitBox.w = section->w;
	this->hitBox.h = section->h;
	section_item = *section;

	if (isTabbable == 1)
	{
		this->isTabbable = true;
	}

	// the parent
	this->parent = parent;



	this->hitBox.x = position.x;
	this->hitBox.y = position.y;
}

UiItem_Image::UiItem_Image(iPoint position, const SDL_Rect* section, UiItem* const parent, SDL_Texture* newTex, UiItem_Description* myDescr) : UiItem(position, parent)
{
	this->section = *section;
	this->guiType = GUI_TYPES::IMAGE;


	this->hitBox.w = section->w;
	this->hitBox.h = section->h;


	// the parent
	this->parent = parent;



	this->hitBox.x = position.x;
	this->hitBox.y = position.y;

	// new texture for loot image
	this->newTex = newTex;




	if (this->parent->guiType == GUI_TYPES::INVENTORY)   // image in inventory
	{
		this->myDescr = myDescr;
		this->tabbable = true;
	}

}

UiItem_Image::~UiItem_Image()
{
	
}

void UiItem_Image::Draw(const float& dt)
{


	if (!hide)
	{
		// TODO: don't blit the icon in the loot item description using the GUI atlas, but instead the Loot atlas 

		if (!printFromLoot)
		{
			float speed = 0.0f;

			if (!useCamera)
				speed = 1.0f;

			App->render->BlitGui(App->gui->GetAtlas(), hitBox.x, hitBox.y, &this->section, speed, 1.0f, 0.0f, resizedRect);
		}
		else
		{
			if (this->parent->guiType == GUI_TYPES::DESCRIPTION)  // DESCRIPTION image
			{
				float speed = 0.0f;

				if (!App->scene->inventory->enable)   // when in game, icon image speed is 1.0f
					speed = 1.0f;

				App->render->BlitGui(newTex, hitBox.x, hitBox.y, &this->section, speed, scaleFactor);   // 4.0f
			}
			else if (this->parent->guiType == GUI_TYPES::INVENTORY)  // INVENTORY image
			{
				App->render->BlitGui(newTex, hitBox.x, hitBox.y, &this->section, 0.0f, 1.4f);
			}
		}

	}
}

void UiItem_Image::CleanUp()
{
	/*if(myDescr != nullptr)
		myDescr->to_delete = true;*/


	if (newTex != nullptr)
	{
		App->tex->UnLoad(newTex);
		newTex = nullptr;
	}
}
