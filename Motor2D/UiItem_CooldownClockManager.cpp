#include "UiItem_CooldownClock.h"
#include "UiItem_CooldownClockManager.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1EntityFactory.h"
#include "PlayerEntityManager.h"


UiItem_CooldownClockManager::UiItem_CooldownClockManager()
{
	name.assign("ClockManager");
}

UiItem_CooldownClockManager::~UiItem_CooldownClockManager()
{
}

bool UiItem_CooldownClockManager::Awake(pugi::xml_node &node)
{
	return true;
}

bool UiItem_CooldownClockManager::Start()
{


	return true;
}

bool UiItem_CooldownClockManager::Update(float dt)
{

	std::vector<UiItem_CooldownClock*>::iterator currentClock = clocks.begin();

	for (; currentClock != clocks.end();)
	{
		if (!(*currentClock)->to_delete)
		{
			++currentClock;
		}
		else
		{
			// cleanup
			(*currentClock)->CleanUp();
			delete (*currentClock);
			currentClock = clocks.erase(currentClock);

		}
	}



	return true;
}

bool UiItem_CooldownClockManager::CleanUp()
{

	for (std::vector<UiItem_CooldownClock*>::iterator item = clocks.begin(); item != clocks.end(); ++item)
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
	clocks.clear();

	return true;
}
