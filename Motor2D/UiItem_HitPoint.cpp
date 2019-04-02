#include "UiItem_HitPoint.h"
#include "UiItem_HitPointManager.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "j1Render.h"


UiItem_HitPoint::UiItem_HitPoint(valueInfo valueInfo, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent) :UiItem(position, parent)
{
	// TODO: Initialize timer
	lifeSpan.Start(); 

	texture = App->font->Print(valueInfo.string.data(), color, font);


	this->guiType = GUI_TYPES::HITPOINT;
	
	this->valueInformation.string = valueInfo.string;
	this->valueInformation.number = valueInfo.number;   // necessary for HitPoint manager

	this->color = color;
	this->font = font;

	// the parent
	this->parent = parent;

}

void UiItem_HitPoint::Draw(const float & dt)
{

	// TODO: blit hitPoints with an extra value: the scaling

	returnLifeState(); 
	App->render->BlitGui(texture, hitBox.x, hitBox.y, NULL, 0.0F);

}


lifeState UiItem_HitPoint::returnLifeState() {

	lifeState ret = fadeIn; 
	uint lifeMoment = lifeSpan.ReadMs(); 


	// These are semi random numbers, needs adjustment

	if (lifeMoment < 100)
	{
		ret = fadeIn; 
	}
	else if (lifeMoment >= 100 && lifeMoment <= 900)
	{
		ret = Middle;
	}
	else if(lifeMoment > 900 && lifeMoment <= 1000)
	{
		ret = fadeOut;
	}
	else
	{
		CleanUp(); 
	}


	return ret; 
}


void UiItem_HitPoint::CleanUp()
{
	App->HPManager->DestroyHitPointLabel(this);

}