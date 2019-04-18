#include "UiItem_Image.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "LootEntity.h"

UiItem_Image::UiItem_Image(iPoint position, const SDL_Rect* section, UiItem*const parent, bool isPanel) : UiItem(position, parent)
{
	this->section = *section;
	this->guiType = GUI_TYPES::IMAGE;


	this->hitBox.w = section->w;
	this->hitBox.h = section->h;

	if (isPanel == 1)
	{
		this->isPanel = true;
	}

	// the parent
	this->parent = parent;



	this->hitBox.x = position.x;
	this->hitBox.y = position.y;

}



UiItem_Image::UiItem_Image(iPoint position, const SDL_Rect* section, UiItem*const parent, SDL_Texture* newTex, UiItem_Description* myDescr) : UiItem(position, parent)
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

	if (this->parent != App->scene->inGamePanel)
	{
		this->myDescr = myDescr; 
	}

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
			// TODO: why don't I use the BlitGui here?? The placing is wrong 
			//App->render->Blit(newTex, hitBox.x, hitBox.y, &this->section, 1.0F, SDL_FLIP_NONE, 3.0F, INT_MAX, INT_MAX, false);

			if (this->parent == App->scene->inGamePanel)  // ingamepanel
			{
				App->render->BlitGui(newTex, hitBox.x, hitBox.y, &this->section, 1.0f, 4.0f);
			}
			else
			{
				App->render->BlitGui(newTex, hitBox.x, hitBox.y, &this->section, 0.0f, 2.5f);
			}
		}
		
	}
}