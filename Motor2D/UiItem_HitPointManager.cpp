#include "UiItem_HitPoint.h"
#include "UiItem_HitPointManager.h"
#include "PlayerEntityManager.h"
#include "j1EntityFactory.h"
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

	CheckComboTime(); 

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


void UiItem_HitPointManager::callHPLabelSpawn(j1Entity* enemy, uint damage, damageType type)
{
	// player streak

	if (!resetCountdown)
	{
		streakCountdown.Start();
		resetCountdown = true;
		LOG("....................................................................Timer ms = %f", streakCountdown.ReadMs()); 
	}
	else if (streakCountdown.ReadMs() > 100 && streakCountdown.ReadMs() < 2000)    //  if player hits another enemy before 2 seconds have passed 
	{
		damageStreak += damage; 
		resetCountdown = false; 
		LOG("....................................................................Timer 2nd time ms = %f", streakCountdown.ReadMs());
	}

	std::string str = std::to_string(damage); 

	valueInfo info = {
	               str,
	              damage,
	};


	// TODO: load colors and fonts from XML 

	SDL_Color c = {}; 
	switch (type)
	{
	case BURN: 
		c = { 255, 0, 0, 255 };
		break; 
	default: 
		c = { 0, 0, 0, 255 };
		break; 
	}
	

	iPoint pos(App->render->WorldToScreen(enemy->position.x, enemy->position.y));                                               // adjust this  


	App->gui->AddHitPointLabel(info, c, App->font->openSansBold36, pos, nullptr);    // big font for testing


}



void UiItem_HitPointManager::CheckComboTime()
{

	LOG("....................................................................  - - - - - - the current TIME - - - - - = %f", streakCountdown.ReadMs());


	if (damageStreak >= 20)
	{
		SDL_Color c = { 255, 0, 0, 255 };                         // red for the moment
		int pos_X = App->entityFactory->player->GetPosition().x;
		int pos_Y = App->entityFactory->player->GetPosition().y;
		LOG("....................................................................  The Combo Score is = %i", damageStreak);
		App->gui->AddLabel("BRUTAL", c, App->font->openSansBold36, iPoint(pos_X, pos_Y + 50), nullptr);
	}


	if (streakCountdown.ReadMs() > 2000)      // if player remains 2 secs without damaging enemy, the combo is lost
	{

		damageStreak = 0; 
		resetCountdown = false; 
		LOG("....................................................................Timer RESET time ms = %f", streakCountdown.ReadMs());
	}


	

	
}