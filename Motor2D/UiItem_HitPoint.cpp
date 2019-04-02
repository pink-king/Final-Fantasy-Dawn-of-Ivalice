/*#include "UiItem_HitPoint.h"
#include "UiItem.h"

#include "j1App.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "j1Render.h"


UiItem_HitPoint::UiItem_HitPoint(valueInfo valueInfo, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent) :UiItem(position, parent)
{
	// TODO: Initialize timer


	texture = App->font->Print(text.data(), color, font);


	this->guiType = GUI_TYPES::HITPOINT;
	
	
	this->value = value;
	this->color = color;
	this->font = font;



	// the parent
	this->parent = parent;

}

void UiItem_HitPoint::Draw(const float & dt)
{
	App->render->BlitGui(texture, hitBox.x, hitBox.y, NULL, 0.0F);


}


lifeState* UiItem_HitPoint::*/