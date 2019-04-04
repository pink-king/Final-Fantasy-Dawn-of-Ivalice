#include "j1LootSystem.h"
#include"j1App.h"
#include "p2Log.h"
#include <iostream>
#include <random>

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
		GoldSystem = false;
	lootchance = GetRandomValue(1, 4);
		LOG("LootChance: %i", lootchance);
		if (lootchance == 1)
		{
			element = GetRandomValue(1, 100);
			LOG("element: %i", element);
			if (element <= 55)
			{
				GoldSystem = true;
				if (element <= 15)
					id = 1; 
				
				else if (element > 15 && element <= 40)
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

int j1LootSystem::GetRandomValue( const int& min, const  int& max)
{
	static std::default_random_engine generator;
	std::uniform_int_distribution<int> range(min, max);
	int ret_value = range(generator);

	LOG("value %i", ret_value);

	return ret_value;
}

void j1LootSystem::MatchGold(int id)
{
	
	
		if (id == gold_id[0])
			goldearned = gold_value[0];

		else if (id == gold_id[1])
			goldearned = gold_value[1];

		else if (id == gold_id[2])
			goldearned = gold_value[2];
	
	
}
