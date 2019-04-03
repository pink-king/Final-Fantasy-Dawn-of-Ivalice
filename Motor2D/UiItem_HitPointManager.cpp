#include "UiItem_HitPoint.h"
#include "UiItem_HitPointManager.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Render.h"

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

	std::vector<UiItem_HitPoint*>::iterator hitPointIterator = hitPointLabels.begin();

	for (; hitPointIterator != hitPointLabels.end();)
	{
		if (!(*hitPointIterator)->to_delete)
		{
			(*hitPointIterator)->updateHitPointPositions();
			(*hitPointIterator)->updateHitPointSizes();
			(*hitPointIterator)->updateHitPointOpacities();

			++hitPointIterator;
		}
		else
		{
			// cleanup
			(*hitPointIterator)->CleanUp();
			delete (*hitPointIterator);
			hitPointIterator = hitPointLabels.erase(hitPointIterator);
		}
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

	iPoint pos(App->render->WorldToScreen(enemy->position.x, enemy->position.y));                                               // adjust this  


	App->gui->AddHitPointLabel(info, c, App->font->openSansBold36, pos, nullptr);    // big font for testing


}