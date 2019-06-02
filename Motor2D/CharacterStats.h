#ifndef  _CHARACTER_STATS_H_
#define _CHARACTER_STATS_H_

#include "UiItem.h"

#include <map>
#include <array>

#define START_BLOCK_POSITION_RELATIVE_TO_INVENTORY iPoint(650,150)
#define STAT_BLOCK_SEPARATION_Y 40
#define ELEMENT_INSIDE_BLOCK_OFFSET_Y 17
#define ELEMENT_INSIDE_BLOCK_SEPARATION_X 15


class UiItem_Image; 
class UiItem_Label; 
class UiItem_Description; 

class CharacterStatBlock : public UiItem
{
private: 
	UiItem_Label* BlockName = nullptr;
	UiItem_Label* baseStat = nullptr;
	UiItem_Label* newStat = nullptr;
	uint baseStatValue;                      // some should be 1 or 2 decimal floats
	uint newStatValue;
	uint lastNewStatValue; 
	UiItem_Label* changeStatArrows = nullptr;
	/*UiItem_Image* changeStatArrows = nullptr;
	UiItem_Image* changeStatIcon = nullptr;*/
	uint mapPosition; 

public: 
	CharacterStatBlock(UiItem_Label* Name, UiItem_Label* bStat, UiItem_Label* nStat, Uint8 bValue, Uint8 nValue, UiItem_Label* Arrows, UiItem* const parent) : BlockName(Name), baseStat(bStat),
		newStat(nStat), baseStatValue(bValue), newStatValue(nValue), changeStatArrows(Arrows), UiItem(parent)
	{
		this->guiType = GUI_TYPES::CHARACTERSTATBLOCK; 
	}
	

	friend class CharacterStats; 
};

class CharacterStats : public UiItem
{

public:
	CharacterStats(UiItem* const parent);
	~CharacterStats();

private:
	void doMapping();

	std::map<int, std::string> positionMap;


public:
	void generateCharacterStats();
	void deGenerateCharacterStats();
	
	void SetBaseStats(std::array<int, 5> newStatsMappingPositions);   // if Compare recieves a 1 in the mapping pos, change it, with the value at that pos
	void CompareStats(std::array<int, 5> newStatsMappingPositions, std::array<int, 5> values = {});  
	void SetNewStats();




};







#endif // ! _CHARACTER_STATS_H_

