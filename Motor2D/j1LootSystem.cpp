#include "j1LootSystem.h"
#include"j1App.h"
#include "p2Log.h"
#include <iostream>
j1LootSystem::j1LootSystem() 
{
	
	name.assign("loot");
}

j1LootSystem::~j1LootSystem() {}


bool j1LootSystem::Awake(pugi::xml_node &config)
{
	bool ret = true;
	int i = 0;

	for (auto node : config.child("Money").children("gold"))
	{
		gold_id[i] = node.attribute("id").as_int();
		gold_value[i] = node.attribute("value").as_int();

		LOG("ARRAY VALUE: %i", gold_value[i]);
		LOG("ARRAY ID: %i", gold_id[i]);

		++i;
		
	}

	return ret;
}


bool j1LootSystem::PostUpdate()
{
	
	if (trigger)
	{

		lootchance = rand() % 4 + 1;
		if (lootchance == 1)
		{
			element = rand() % 100 + 1;

			if (element >= 55)
			{
				if (element >= 15)
					id = 1; 
				
				else if (element > 15 && element >= 40)
					id = 2;

				else if (element > 40 && element <= 55)
					id = 3;

				

				MatchGold(id);
				LOG("GOLD EARNED: %i", goldearned);
			}
			
			else id = 0;
			

			
		}
		
		trigger = false;
	}
	
	return true;
}

void j1LootSystem::MatchGold(int id)
{
	
	switch (id)
	{
	case 1:
		goldearned = gold_value[0];
		break;
	case 2:
		goldearned = gold_value[1];
		break;
	case 3:
		goldearned = gold_value[2];
	}
		/*if (id == gold_id[0])
			goldearned = gold_value[0];

		if (id == gold_id[1])
			goldearned = gold_value[1];

		if (id == gold_id[2])
			goldearned = gold_value[2];
	
		id = 0;*/
}
