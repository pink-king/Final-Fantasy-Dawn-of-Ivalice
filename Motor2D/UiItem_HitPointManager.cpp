#include "UiItem_HitPoint.h"
#include "UiItem_HitPointManager.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Gui.h"
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

	return true;
}

bool UiItem_HitPointManager::CleanUp()
{

		for (std::vector<UiItem_HitPoint*>::iterator item = hitPointLabels.begin(); item != hitPointLabels.end(); ++item) {
			if ((*item) != nullptr) {
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
			if (*item == label) 
			{

				// first clear the texture 
				if (label->texture != nullptr)
				{
					App->tex->UnLoad(label->texture); 
					label->texture = nullptr; 
				}

				delete *item;
				*item = nullptr;
	           
				// IMPORTANT TODO: delete the element from the list
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


	/*info->number = damage; 
	info->string = ("%i", damage);    // does this REALLY WORK ??? hahah*/




	// possible: color depending on damage type 
	// TODO: load colors and fonts from XML 

	SDL_Color c = {0, 0, 0, 255};  // label is created totally black 

	iPoint pos(enemy->position.x, enemy->position.y - enemy->size.y);          // adjust this  

	App->gui->AddHitPointLabel(info, c, App->font->openSansBold18, pos, nullptr); 


}