#include "CharacterStats.h"
#include "j1Fonts.h"
#include "j1Gui.h"  // TO CHECK: This includes CharacterStats.h again !!
#include "p2Log.h"
#include "j1EntityFactory.h"

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
		
	std::array<int, 5> newStatsMappingPositions; 
	std::fill(std::begin(newStatsMappingPositions), std::end(newStatsMappingPositions), NULL);

	SetBaseStats(newStatsMappingPositions);
}

void CharacterStats::SetBaseStats(std::array<int, 5> newStatsMappingPositions)
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

				int a = 0; 

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
				
						int a = 0; 
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

// TODO: remember to clean the map in the global ui item clean up

