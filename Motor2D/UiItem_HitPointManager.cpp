#include "UiItem_HitPoint.h"
#include "UiItem_HitPointManager.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Gui.h"

#include "j1Map.h"
#include "j1Fonts.h"
#include <string.h>

UiItem_HitPointManager::UiItem_HitPointManager()
{
	name.assign("HPLabelManager");
}

UiItem_HitPointManager::~UiItem_HitPointManager()
{
}

bool UiItem_HitPointManager::Awake(pugi::xml_node &node)
{
	return true; 
}

bool UiItem_HitPointManager::Start()
{
	return true;
}

bool UiItem_HitPointManager::Update(float dt)
{

	if (!hitPointLabels.empty())
	{
		updateHitPointPositions();
		updateHitPointSizes(); 
		updateHitPointOpacities(); 
	}

	return true;
}

bool UiItem_HitPointManager::CleanUp()
{

		for (std::vector<UiItem_HitPoint*>::iterator item = hitPointLabels.begin(); item != hitPointLabels.end(); ++item) 
		{
			if ((*item) != nullptr)
			{


				// first clear the texture 
				if ((*item)->texture != nullptr)
				{
					App->tex->UnLoad((*item)->texture);
					(*item)->texture = nullptr;
				}


				delete *item;
				*item = nullptr; 	
			}
		}
		hitPointLabels.clear(); 

	return true;
}


void UiItem_HitPointManager::DestroyHitPointLabel(UiItem_HitPoint* label)
{

	if (label != nullptr)
	{
		for (std::vector<UiItem_HitPoint*>::iterator item = hitPointLabels.begin(); item != hitPointLabels.end(); ++item) 
		{
			if (*item == label && (*item) != nullptr) 
			{

				// first clear the texture 
				if (label->texture != nullptr)
				{
					App->tex->UnLoad(label->texture); 
					label->texture = nullptr; 
				}

				// IMPORTANT!!: do this only if it is not being deleted already in the destroy element function in gui cpp
				/*delete *item;
				*item = nullptr;*/
			
				// delete it from the hitPoint item list

				hitPointLabels.erase(item); 
				
				//break;
				
			}
    	}
	}

}


void UiItem_HitPointManager::callHPLabelSpawn(j1Entity* enemy, uint damage)
{

	std::string str = std::to_string(damage); 

	valueInfo info = {
	               str,
	              damage,
	};


	// possible: color depending on damage type 
	// TODO: load colors and fonts from XML 

	SDL_Color c = {0, 0, 255, 255};  // label is created totally black 

	//iPoint pos(enemy->position.x, enemy->position.y);                                               // adjust this  
	iPoint pos(70, 70);
	App->gui->AddHitPointLabel(info, c, App->font->openSansBold36, pos, nullptr);    // big font for testing


}


void UiItem_HitPointManager::updateHitPointPositions()
{

	for (std::vector<UiItem_HitPoint*>::iterator item = hitPointLabels.begin(); item != hitPointLabels.end(); ++item)
	{

		if ((*item) != nullptr)
		{
			(*item)->hitBox.y -= 2;
			
			// TODO: update de x: keep in mind the scaleFactor, and move the x to the left
			/*int w, h; 
			SDL_QueryTexture((*item)->texture, NULL, NULL, &w, &h);
			 */ 

		}

	}

}

void UiItem_HitPointManager::updateHitPointSizes()
{

	for (std::vector<UiItem_HitPoint*>::iterator item = hitPointLabels.begin(); item != hitPointLabels.end(); ++item)
	{

		if ((*item) != nullptr)
		{
			if ((*item)->returnLifeState() != dead)
			{
				switch ((*item)->returnLifeState())
				{
				case fadeIn:
					(*item)->scaleFactor *= 1.03f;
					break;
				case Middle:
					(*item)->scaleFactor /= 1.003f;
					break;
				case fadeOut:
					(*item)->scaleFactor /= 1.02f;
					break;
				}
			}
		}
	}



}


void UiItem_HitPointManager::updateHitPointOpacities()
{

	
	for (std::vector<UiItem_HitPoint*>::iterator item = hitPointLabels.begin(); item != hitPointLabels.end(); ++item)
	{

		if ((*item) != nullptr)
		{
			if ((*item)->returnLifeState() != dead)
			{
				switch ((*item)->returnLifeState())
				{
				case fadeIn:
					(*item)->alphaValue *= 3; 
					break;
				case Middle:
					(*item)->alphaValue /= 1.1f; 
					break;
				case fadeOut:
					(*item)->alphaValue /= 1.7f; 
					break;
				}
				SDL_SetTextureAlphaMod((*item)->texture, (*item)->alphaValue);
			}

		}
	}

}