#include "UiItem_HitPoint.h"
#include "UiItem_HitPointManager.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"

UiItem_HitPoint::UiItem_HitPoint(valueInfo valueInfo, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent, variant type, j1Entity* receiver) :UiItem(position, parent)
{
	// TODO: Initialize timer
	lifeSpan.Start(); 

	texture = App->font->Print(valueInfo.string.data(), color, font);
	
	this->numerOrText = type;

	this->guiType = GUI_TYPES::HITPOINT;
	
	this->valueInformation.string = valueInfo.string;
	this->valueInformation.number = valueInfo.number;   // necessary for HitPoint manager

	this->color = color;
	this->font = font;

	// the parent
	this->parent = parent;

	if (receiver)
	attachedEntity = receiver; 

}


UiItem_HitPoint::UiItem_HitPoint(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent, variant type) :UiItem(position, parent)
{
	// TODO: Initialize timer
	lifeSpan.Start();

	texture = App->font->Print(text.data(), color, font);

	// distinguish both types
	this->numerOrText = type; 

	this->guiType = GUI_TYPES::HITPOINT;

	this->valueInformation.string = text;

	this->color = color;
	this->font = font;

	// the parent
	this->parent = parent;

}


void UiItem_HitPoint::Draw(const float & dt)
{
	
	// TODO: blit hitPoints with an extra value: the scaling

	returnLifeState(); 

	if (this->numerOrText == variant::number)
	{
		App->render->BlitGui(texture, hitBox.x, hitBox.y, NULL, 0.0F, scaleFactor, 0.0f);
	}
	else if(this->valueInformation.string == "FIERCE")
	{
		App->render->BlitGui(texture, hitBox.x, hitBox.y, NULL, 0.0F, scaleFactor, -10.0f);  // rotate hitlabels
	}
	else if (this->valueInformation.string == "BRUTAL")
	{
		App->render->BlitGui(texture, hitBox.x, hitBox.y, NULL, 0.0F, scaleFactor, 10.0f);  // rotate hitlabels
	}


}


lifeState UiItem_HitPoint::returnLifeState() {

	lifeState ret = fadeIn; 
	uint lifeMoment = lifeSpan.ReadMs(); 


	// first considerate the type
	uint maxLife = 0; 
	if (this->numerOrText == variant::number)
	{
		maxLife = NUMBER_LIFE; 
	}
	else
	{
		maxLife = TEXT_LIFE;

	}

		if (lifeMoment < 300)
		{
			ret = fadeIn;
		}
		else if (lifeMoment >= 300 && lifeMoment <= 1000)
		{
			ret = Middle;
		}
		else if (lifeMoment > 1000 && lifeMoment <= maxLife)
		{
			ret = fadeOut;
		}
		else
		{
			ret = dead;
			//CleanUp(); 
			to_delete = true;
		}


	return ret; 
}


void UiItem_HitPoint::CleanUp()
{

	if (this->numerOrText == variant::text)
	{
		App->HPManager->labelsSpawned.totalLabels--;
	}


	if (texture != nullptr)
		App->tex->UnLoad(texture);
}

void UiItem_HitPoint::updateHitPointPositions()
{
	if (this->numerOrText == variant::number)
	{
		hitBox.y -= 2;


		/*if (attachedEntity->life > 0)    // TODO: DO THIS FROM THE ENEMY
		{
			//hitBox.x += App->render->WorldToScreen(attachedEntity->GetPosition().x - App->render->ScreenToWorld(this->hitBox.x, 0).x, 0).x;
			

			int w, h; 
			SDL_QueryTexture(this->texture, NULL, NULL, &w, &h); 
			
			hitBox.x = App->render->WorldToScreen(attachedEntity->position.x, 0).x - w/2;


		}*/
	}

			// TODO: update de x: keep in mind the scaleFactor, and move the x to the left
			/*int w, h;
			SDL_QueryTexture((*item)->texture, NULL, NULL, &w, &h);
			 */

}

void UiItem_HitPoint::updateHitPointSizes()
{

	if (returnLifeState() != dead)
	{

		switch (returnLifeState())
		{
			case fadeIn:
				scaleFactor *= 1.03f;
				break;
			case Middle:

				if (this->numerOrText == variant::number)
				{
					scaleFactor /= 1.003f;
				}

				break;
			case fadeOut:

				if (this->numerOrText == variant::number)
				{
					scaleFactor /= 1.02f;
				}
				else
				{
					scaleFactor /= 1.005f;
				}
				break;
		}
	}
		
}


void UiItem_HitPoint::updateHitPointOpacities()
{
	if (returnLifeState() != dead)
		{
			switch (returnLifeState())
			{
			case fadeIn:
				alphaValue *= 3;
				break;
			case Middle:
				if (this->numerOrText == variant::number)
				{
					alphaValue /= 1.1f;
				}
				else
				{
					alphaValue /= 1.01f;
				}

				break;
			case fadeOut:
				if (this->numerOrText == variant::number)
				{
					alphaValue /= 1.7f;
				}
				else
				{
					alphaValue /= 1.1f;
				}
				break;
			}
			SDL_SetTextureAlphaMod(texture,alphaValue);
			}

}