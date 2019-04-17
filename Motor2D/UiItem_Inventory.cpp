#include "UiItem_Inventory.h"
#include "j1Gui.h"
#include "j1Render.h"


UiItem_Inventory::UiItem_Inventory(UiItem * const parent):UiItem(parent)
{
	this->parent = parent; 
}



void UiItem_Inventory::Draw(const float & dt)
{
	/*SDL_Rect draw = { 0 }; 

	if (!drawTest)
	{
		for (int i = 0; i < MAX_Weapons_plus_Equipment; ++i)
		{
			draw.x = initialPositions.weaponsAndEquipment.x + i*boxSize;


			App->render->DrawQuad(); 
		}
	}*/

}


