#include "CharacterStats.h"
#include "j1Fonts.h"
#include "j1Gui.h"  // TO CHECK: This includes CharacterStats.h again !!
#include "p2Log.h"
#include "j1EntityFactory.h"
#include "UiItem_Description.h"

CharacterStats::CharacterStats(UiItem* const parent) :UiItem(parent)
{
	this->parent = parent;
	this->guiType = GUI_TYPES::CHARACTERSTATMANAGER;
    
	doMapping(); 
	
}
CharacterStats::~CharacterStats()
{
}


void CharacterStats::doMapping()
{
	positionMap.insert(std::make_pair(0, "ATTACK")); 
	positionMap.insert(std::make_pair(1, "DEFENSE"));
	positionMap.insert(std::make_pair(2, "COOLDOWN"));
	positionMap.insert(std::make_pair(3, "HP"));
	positionMap.insert(std::make_pair(4, "VELOCITY"));
}

void CharacterStats::generateCharacterStats()   // call it when opening inventory
{
	
	for (int i = 0; i < positionMap.size(); ++i)
	{
		iPoint currentBlockPos = START_BLOCK_POSITION_RELATIVE_TO_INVENTORY + iPoint(0, STAT_BLOCK_SEPARATION_Y * i); 

		std::string name = positionMap.at(i); 

		UiItem_Label* statName = App->gui->AddLabel(name, { 255, 255, 255, 255 }, App->font->openSansBold12, currentBlockPos, nullptr);
		statName->hasBeenSpawned = true; 

		float baseStatValue;
		float newStatValue;
		baseStatValue = newStatValue = 0;   // TODO: get base buffs from player 

		UiItem_Label* baseStat = App->gui->AddLabel("0", { 255, 255, 255, 255 }, App->font->openSansBold12, currentBlockPos + iPoint(0, ELEMENT_INSIDE_BLOCK_OFFSET_Y), nullptr);
		baseStat->hasBeenSpawned = true;

		// upon creation, there is not comparison stat 
		UiItem_Label* Arrows = App->gui->AddLabel(">>", { 255, 0, 0, 255 }, App->font->openSansBold12, currentBlockPos + iPoint(ELEMENT_INSIDE_BLOCK_SEPARATION_X, ELEMENT_INSIDE_BLOCK_OFFSET_Y), nullptr);
		Arrows->hasBeenSpawned = true;
		Arrows->hide = true;

		// upon creation, there is not comparison stat
		UiItem_Label* newStat = App->gui->AddLabel("0", { 255, 255, 255, 255 }, App->font->openSansBold12, currentBlockPos + iPoint(ELEMENT_INSIDE_BLOCK_SEPARATION_X * 1.8f, ELEMENT_INSIDE_BLOCK_OFFSET_Y), nullptr);
		newStat->hasBeenSpawned = true;
		newStat->hide = true;

	    
		CharacterStatBlock* block =  App->gui->AddCharacterStatBlock(statName, baseStat, newStat, baseStatValue, newStatValue, Arrows, this);
		block->mapPosition = i; 

		statName->parent = block; 
		statName->guiType = GUI_TYPES::CHARACTERSTATBLOCKLABEL;      // to prevent crashes when deleting the block parent, directly delete them
		baseStat->parent = block;
		baseStat->guiType = GUI_TYPES::CHARACTERSTATBLOCKLABEL;
		newStat->parent = block;
		newStat->guiType = GUI_TYPES::CHARACTERSTATBLOCKLABEL;
		Arrows->parent = block;
		Arrows->guiType = GUI_TYPES::CHARACTERSTATBLOCKLABEL;

    }
	 

	InitializeStats(); 

}


void CharacterStats::InitializeStats()
{

	ResetDefaultStats();    // useful when swapping characters

	SetBaseStats();

	if (!App->entityFactory->player->equipedObjects.empty())
	{
		for (auto& item : App->entityFactory->player->equipedObjects)
		{

			if (App->entityFactory->player->selectedCharacterEntity == item->character)

			{
				getItemBuffsAndCallStatComparison(item);   // summate each items buffs to base stats
				SetNewStats();
			}
		}
			
		
	}

	
}


void CharacterStats::ResetDefaultStats()
{
	std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin();

	for (; iter != App->gui->ListItemUI.end(); ++iter)
	{
		if ((*iter)->guiType == GUI_TYPES::CHARACTERSTATBLOCK)
		{
			// values to 0 
			dynamic_cast<CharacterStatBlock*>(*iter)->baseStatValue = 0; 
			dynamic_cast<CharacterStatBlock*>(*iter)->newStatValue = 0;
			dynamic_cast<CharacterStatBlock*>(*iter)->lastNewStatValue = 0;
			dynamic_cast<CharacterStatBlock*>(*iter)->candidateToNewStat = 0;

			// idle textures
			SDL_Color c = { 255, 255, 255, 255 }; 
			dynamic_cast<CharacterStatBlock*>(*iter)->baseStat->ChangeTextureIdle(std::to_string(dynamic_cast<CharacterStatBlock*>(*iter)->baseStatValue), &c, NULL);
			dynamic_cast<CharacterStatBlock*>(*iter)->newStat->ChangeTextureIdle(std::to_string(dynamic_cast<CharacterStatBlock*>(*iter)->newStatValue), &c, NULL);
			dynamic_cast<CharacterStatBlock*>(*iter)->changeStatArrows->ChangeTextureIdle(">>", &c, NULL);


			// hide 
			dynamic_cast<CharacterStatBlock*>(*iter)->newStat->hide = true; 
			dynamic_cast<CharacterStatBlock*>(*iter)->changeStatArrows->hide = true; 


		}
	}
}


void CharacterStats::SetBaseStats()
{
	std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin();

	for (; iter != App->gui->ListItemUI.end(); ++iter)
	{
		if ((*iter)->guiType == GUI_TYPES::CHARACTERSTATBLOCK)
		{ 
			     bool uintTrue = false; 

				if (dynamic_cast<CharacterStatBlock*>(*iter)->BlockName->text == "HP")
				{
					dynamic_cast<CharacterStatBlock*>(*iter)->baseStatValue = App->entityFactory->player->maxLife;
					uintTrue = true;
					
				}
				else if (dynamic_cast<CharacterStatBlock*>(*iter)->BlockName->text == "VELOCITY")
				{
					dynamic_cast<CharacterStatBlock*>(*iter)->baseStatValue = App->entityFactory->player->selectedCharacterEntity->mySpeedModular; 
				}
	             




				if (uintTrue)
				{
					dynamic_cast<CharacterStatBlock*>(*iter)->baseStat->ChangeTextureIdle(std::to_string((uint)(int)dynamic_cast<CharacterStatBlock*>(*iter)->baseStatValue), NULL, NULL);
				}
				else
				{
					dynamic_cast<CharacterStatBlock*>(*iter)->baseStat->ChangeTextureIdle(std::to_string(dynamic_cast<CharacterStatBlock*>(*iter)->baseStatValue), NULL, NULL);
				}
			       

		
		}
	}

}

void CharacterStats::CompareStats(std::array<int, 5> newStatsMappingPositions, std::array<int, 5> values)
{
	std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin();

	for (; iter != App->gui->ListItemUI.end(); ++iter)                             
	{
		if ((*iter)->parent->guiType == GUI_TYPES::CHARACTERSTATMANAGER)
		{
	

				for (uint i = 0; i < newStatsMappingPositions.size(); ++i)
				{
					if (dynamic_cast<CharacterStatBlock*>(*iter)->mapPosition == i)
					{
						if (newStatsMappingPositions.at(i) == 1)                       // TODO: GREEN AND RED ACCORDING TO UPGRADE OR DOWNGRADE
						{	//dynamic_cast<CharacterStatBlock*>(*iter)->newStatValue = values.at(i);  // if buff is summative
							dynamic_cast<CharacterStatBlock*>(*iter)->candidateToNewStat = values.at(i);                  // capture this for later
							dynamic_cast<CharacterStatBlock*>(*iter)->newStat->hide = false;

							uint valueIfPickingObject = dynamic_cast<CharacterStatBlock*>(*iter)->baseStatValue - dynamic_cast<CharacterStatBlock*>(*iter)->lastNewStatValue
								+ dynamic_cast<CharacterStatBlock*>(*iter)->candidateToNewStat;

							SDL_Color c = { 0, 0, 0, 0 };
							if (valueIfPickingObject > dynamic_cast<CharacterStatBlock*>(*iter)->baseStatValue)
							{
								c = { 0, 255, 0, 255 };
							}
							else if (valueIfPickingObject <= dynamic_cast<CharacterStatBlock*>(*iter)->baseStatValue)
							{
								c = { 255, 0, 0, 255 };
							}


							/*if (valueIfPickingObject != 0)
							{*/
							dynamic_cast<CharacterStatBlock*>(*iter)->newStat->ChangeTextureIdle(std::to_string(valueIfPickingObject), &c, NULL);

							dynamic_cast<CharacterStatBlock*>(*iter)->changeStatArrows->hide = false;
							dynamic_cast<CharacterStatBlock*>(*iter)->changeStatArrows->ChangeTextureIdle(">>", &c, NULL);
							//	}

						
						}
		
					}
				}
		
			
		}
	}
     
}


void CharacterStats::SetNewStats()
{

	std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin();


	for (; iter != App->gui->ListItemUI.end(); ++iter)                        
	{
		if ((*iter)->parent->guiType == GUI_TYPES::CHARACTERSTATMANAGER)
		{
		
			

				// NEW BASE STAT = base - last + new 
				dynamic_cast<CharacterStatBlock*>(*iter)->newStatValue = dynamic_cast<CharacterStatBlock*>(*iter)->candidateToNewStat;

				dynamic_cast<CharacterStatBlock*>(*iter)->baseStatValue = dynamic_cast<CharacterStatBlock*>(*iter)->baseStatValue - dynamic_cast<CharacterStatBlock*>(*iter)->lastNewStatValue
					+ dynamic_cast<CharacterStatBlock*>(*iter)->newStatValue;


				dynamic_cast<CharacterStatBlock*>(*iter)->baseStat->ChangeTextureIdle(std::to_string(dynamic_cast<CharacterStatBlock*>(*iter)->baseStatValue), NULL, NULL);


				dynamic_cast<CharacterStatBlock*>(*iter)->newStat->hide = true;
				dynamic_cast<CharacterStatBlock*>(*iter)->changeStatArrows->hide = true;


				if (!dynamic_cast<CharacterStatBlock*>(*iter)->AddedNewBuff)
					dynamic_cast<CharacterStatBlock*>(*iter)->AddedNewBuff = true;


				//	if (dynamic_cast<CharacterStatBlock*>(*iter)->AddedNewBuff)
				dynamic_cast<CharacterStatBlock*>(*iter)->lastNewStatValue = dynamic_cast<CharacterStatBlock*>(*iter)->newStatValue;  // we will need to substract this later

				

		}
	}

}


void CharacterStats::HideAllComparisonStats()
{
	std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin();

	for (; iter != App->gui->ListItemUI.end(); ++iter)
	{
		if ((*iter)->parent->guiType == GUI_TYPES::CHARACTERSTATMANAGER)
		{
		
				dynamic_cast<CharacterStatBlock*>(*iter)->newStat->hide = true;
				dynamic_cast<CharacterStatBlock*>(*iter)->changeStatArrows->hide = true;
		
		}
	}

}



void CharacterStats::ShowAllComparisonStats()
{
	std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin();

	for (; iter != App->gui->ListItemUI.end(); ++iter)
	{
		if ((*iter)->parent->guiType == GUI_TYPES::CHARACTERSTATMANAGER)
		{
		
				dynamic_cast<CharacterStatBlock*>(*iter)->newStat->hide = false;
				dynamic_cast<CharacterStatBlock*>(*iter)->changeStatArrows->hide = false;
		
		}
	}

}


/*
void CharacterStats::GetNewStatsWithoutComparing(std::array<int, 5> newStatsMappingPositions, std::array<int, 5> values)
{


	std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin();

	for (; iter != App->gui->ListItemUI.end(); ++iter)
	{
		if ((*iter)->parent->guiType == GUI_TYPES::CHARACTERSTATMANAGER)
		{
			for (uint i = 0; i < newStatsMappingPositions.size(); ++i)
			{
				if (dynamic_cast<CharacterStatBlock*>(*iter)->mapPosition == i)
				{
					if (newStatsMappingPositions.at(i) == 1)                       // TODO: GREEN AND RED ACCORDING TO UPGRADE OR DOWNGRADE
					{
						dynamic_cast<CharacterStatBlock*>(*iter)->lastNewStatValue = dynamic_cast<CharacterStatBlock*>(*iter)->newStatValue;  // we will need to substract this later
						dynamic_cast<CharacterStatBlock*>(*iter)->newStatValue = dynamic_cast<CharacterStatBlock*>(*iter)->baseStatValue + values.at(i);  // if buff is summative
				
					}
				}
			}

		}
	}

}*/

void CharacterStats::deGenerateCharacterStats()  // call it when closing inventory
{
	std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin(); 


	/*for (; iter != App->gui->ListItemUI.end();)
	{
		if ((*iter)->guiType == GUI_TYPES::CHARACTERSTATBLOCK || (*iter)->guiType == GUI_TYPES::CHARACTERSTATBLOCKLABEL)
		{
			iter = App->gui->ListItemUI.erase(iter);
			delete (*iter);
			(*iter) = nullptr;
		
		}
		else
		{
			iter++; 
		}

	}*/

	for (; iter != App->gui->ListItemUI.end(); ++iter)
	{
		if ((*iter)->guiType == GUI_TYPES::CHARACTERSTATBLOCK || (*iter)->guiType == GUI_TYPES::CHARACTERSTATBLOCKLABEL)
		{
			(*iter)->to_delete = true; 

		}

	}

}

void CharacterStats::getItemBuffsAndCallStatComparison(LootEntity* ent)
{

	if (App->entityFactory->player->selectedCharacterEntity == ent->character) 
	  
	{

		std::array<int, 5> characterStatsMapping = {};
		std::array<int, 5> characterStatsValues = {};

		int attack = 0;
		int resistance = 0;
		int cooldown = 0;

		int HP = 0;
		int velocity = 0;

		std::vector<Buff*>::iterator iter = ent->stats.begin();

		if (ent->GetObjectType() == OBJECT_TYPE::WEAPON_OBJECT)
		{
			for (; iter != ent->stats.end(); ++iter)    // capture att and def 
			{
				if ((*iter)->GetRol() == ROL::ATTACK_ROL)
				{
					attack = (int)(*iter)->GetValue();

					characterStatsMapping.at(0) = 1;
					characterStatsValues.at(0) = attack;
				}
				else if ((*iter)->GetRol() == ROL::DEFENCE_ROL)
				{
					resistance = (int)(*iter)->GetValue();

					characterStatsMapping.at(1) = 1;
					characterStatsValues.at(1) = resistance;
				}
				else if ((*iter)->GetRol() == ROL::COOLDOWN)
				{
					cooldown = (int)(*iter)->GetValue();

					characterStatsMapping.at(2) = 1;
					characterStatsValues.at(2) = cooldown;
				}

			}

		}
		else if (ent->GetObjectType() == OBJECT_TYPE::ARMOR_OBJECT)
		{
			for (; iter != ent->stats.end(); ++iter)   // capture def and other 2 possible rols
			{
				if ((*iter)->GetRol() == ROL::DEFENCE_ROL)
				{
					resistance = (int)(*iter)->GetValue();

					characterStatsMapping.at(1) = 1;
					characterStatsValues.at(1) = resistance;
				}
				else if ((*iter)->GetRol() == ROL::HEALTH)
				{
					HP = (*iter)->GetValue();

					characterStatsMapping.at(3) = 1;
					characterStatsValues.at(3) = HP;
				}
				else if ((*iter)->GetRol() == ROL::VELOCITY)
				{
					velocity = (int)(*iter)->GetValue();


					characterStatsMapping.at(4) = 1;
					characterStatsValues.at(4) = velocity;
				}

			}

		}

		//App->scene->characterStatsItem->GetNewStatsWithoutComparing(characterStatsMapping, characterStatsValues);
		CompareStats(characterStatsMapping, characterStatsValues);
	}
	else
	{
		HideAllComparisonStats();
	}
}


// TODO: remember to clean the map in the global ui item clean up

