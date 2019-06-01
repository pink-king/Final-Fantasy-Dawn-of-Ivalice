#ifndef  _CHARACTER_STATS_H_
#define _CHARACTER_STATS_H_

#include "SDL/include/SDL_stdinc.h"
#include "UiItem.h"

#include <map>

#define START_BLOCK_POSITION_RELATIVE_TO_INVENTORY iPoint(700,100)
#define STAT_BLOCK_SEPARATION_Y 30
#define ELEMENT_INSIDE_BLOCK_OFFSET_Y 5
#define ELEMENT_INSIDE_BLOCK_SEPARATION_X 8 


class UiItem_Image; 
class UiItem_Label; 
class UiItem_Description; 

class CharacterStatBlock : public UiItem
{
private: 
	UiItem_Label* BlockName = nullptr;
	UiItem_Label* baseStat = nullptr;
	UiItem_Label* newStat = nullptr;
	Uint8 baseStatValue;
	Uint8 newStatValue;
	UiItem_Label* changeStatArrows = nullptr;
	/*UiItem_Image* changeStatArrows = nullptr;
	UiItem_Image* changeStatIcon = nullptr;*/

public: 
	CharacterStatBlock(UiItem_Label* Name, UiItem_Label* bStat, UiItem_Label* nStat, Uint8 bValue, Uint8 nValue, UiItem_Label* Arrows, UiItem* const parent) : BlockName(Name), baseStat(bStat),
		newStat(nStat), baseStatValue(bValue), newStatValue(nValue), changeStatArrows(Arrows), UiItem(parent){
	}
	


};

class CharacterStats : public UiItem
{

public: 
	CharacterStats(UiItem* const parent);
	~CharacterStats(); 

public: 
	void generateCharacterStats(); 
	void deGenerateCharacterStats(); 

private: 
	void doMapping(); 

	std::map<int, std::string> positionMap; 



};







#endif // ! _CHARACTER_STATS_H_

