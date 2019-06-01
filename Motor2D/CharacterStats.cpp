#include "CharacterStats.h"
#include "j1Fonts.h"
#include "j1Gui.h"  // TO CHECK: This includes CharacterStats.h again !!

CharacterStats::CharacterStats(UiItem* const parent) :UiItem(parent)
{
	this->parent = parent;
	this->guiType = GUI_TYPES::CHARACTERSTATMANAGER;
    
	doMapping(); 
	generateCharacterStats(); 
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

void CharacterStats::generateCharacterStats()   
{
	

	for (int i = 0; i <= positionMap.size(); ++i)
		{
		iPoint currentBlockPos = START_BLOCK_POSITION_RELATIVE_TO_INVENTORY + iPoint(0, STAT_BLOCK_SEPARATION_Y * i); 

		std::string name = positionMap.at(i); 

		UiItem_Label* statName = App->gui->AddLabel(name, { 255, 255, 255, 255 }, App->font->openSansBold18, currentBlockPos, this);
		statName->hasBeenSpawned = true; 

		Uint8 baseStatValue;
		Uint8 newStatValue;
		baseStatValue = newStatValue = 0;   // TODO: get base buffs from player 

		UiItem_Label* baseStat = App->gui->AddLabel(name, { 255, 255, 255, 255 }, App->font->openSansBold18, currentBlockPos + iPoint(ELEMENT_INSIDE_BLOCK_SEPARATION_X, STAT_BLOCK_SEPARATION_Y), this);
		baseStat->hasBeenSpawned = true;

		// upon creation, there is not comparison stat 
		UiItem_Label* Arrows = App->gui->AddLabel(">>", { 255, 0, 0, 255 }, App->font->openSansBold18, currentBlockPos + iPoint(ELEMENT_INSIDE_BLOCK_SEPARATION_X * 2, STAT_BLOCK_SEPARATION_Y), this); 
		Arrows->hasBeenSpawned = true;
		Arrows->hide = true;

		// upon creation, there is not comparison stat
		UiItem_Label* newStat = App->gui->AddLabel("0", { 255, 255, 255, 255 }, App->font->openSansBold18, currentBlockPos + iPoint(ELEMENT_INSIDE_BLOCK_SEPARATION_X * 3, STAT_BLOCK_SEPARATION_Y), this);  
		newStat->hasBeenSpawned = true;
		newStat->hide = true; 

	    
		App->gui->AddCharacterStatBlock(statName, baseStat, newStat, baseStatValue, newStatValue, Arrows, this);

	    }

}

void CharacterStats::deGenerateCharacterStats()
{
	for (std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin(); iter != App->gui->ListItemUI.end();)
	{
		if ((*iter)->parent->guiType == GUI_TYPES::CHARACTERSTATMANAGER)
		{
			iter = App->gui->ListItemUI.erase(iter);
			delete (*iter);
			(*iter) = nullptr;
		}
		else
		{
			iter++; 
		}

	}
}

// TODO: remember to clean the map in the global ui item clean up

