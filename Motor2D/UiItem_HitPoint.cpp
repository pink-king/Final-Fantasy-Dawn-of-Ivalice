#include "UiItem_HitPoint.h"
#include "UiItem_HitPointManager.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Scene.h"
#include "Brofiler/Brofiler.h"

UiItem_HitPoint::UiItem_HitPoint(valueInfo valueInfo, SDL_Color color, TTF_Font* font, p2Point<int> position, UiItem* const parent, variant type) :UiItem(position, parent)
{
	// TODO: Initialize timer
	lifeSpan.Start();

	if (type == variant::gold)
	{
		std::string newString(std::to_string((int)valueInfo.number));
		newString.append(" ");
		newString.append(valueInfo.string);
		texture = App->font->Print(newString.data(), color, font);
	}
	else if (type == variant::wave || type == variant::levelUp)
	{
		std::string newString(valueInfo.string);
		newString.append(" ");
		newString.append(std::to_string((int)valueInfo.number));
		texture = App->font->Print(newString.data(), color, font);
	}

	else if (type == variant::number)
	{
		texture = App->font->Print(valueInfo.string.data(), color, font);
	}



	this->numerOrText = type;

	this->guiType = GUI_TYPES::HITPOINT;

	this->valueInformation.string = valueInfo.string;
	this->valueInformation.number = valueInfo.number;   // necessary for HitPoint manager

	this->color = color;
	this->font = font;

	// the parent
	this->parent = parent;

}


UiItem_HitPoint::UiItem_HitPoint(std::string text, SDL_Color color, TTF_Font* font, p2Point<int> position, UiItem* const parent, variant type) :UiItem(position, parent)
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


void UiItem_HitPoint::Draw(const float& dt)
{
	BROFILER_CATEGORY("Hit Point Draw", Profiler::Color::DarkTurquoise);

	// TODO: blit hitPoints with an extra value: the scaling

	returnLifeState();

	if (!App->scene->inventory->enable)
	{
		if (this->numerOrText == variant::number || this->numerOrText == variant::gold || this->numerOrText == variant::wave || this->numerOrText == variant::levelUp)
		{
			App->render->BlitGui(texture, hitBox.x, hitBox.y, NULL, 1.0F, scaleFactor, 0.0f);
		}
		else if (this->valueInformation.string == "FIERCE")
		{
			App->render->BlitGui(texture, hitBox.x, hitBox.y, NULL, 0.0F, scaleFactor, -10.0f);  // rotate hitlabels
		}
		else if (this->valueInformation.string == "BRUTAL")
		{
			App->render->BlitGui(texture, hitBox.x, hitBox.y, NULL, 0.0F, scaleFactor, 10.0f);  // rotate hitlabels
		}
		else if (this->valueInformation.string == "SAVE")
		{
			App->render->BlitGui(texture, hitBox.x, hitBox.y, NULL, 0.0F, scaleFactor, 0.0f);  // rotate hitlabels
		}

	}




}


lifeState UiItem_HitPoint::returnLifeState() {

	lifeState ret = fadeIn;
	uint lifeMoment = lifeSpan.ReadMs();


	// first considerate the type
	uint maxLife = 0;
	uint middlelife = 0;

	if (this->numerOrText == variant::number || this->numerOrText == variant::gold)
	{
		maxLife = NUMBER_LIFE;
		middlelife = middeLife;
	}
	else if (this->numerOrText == variant::wave || this->numerOrText == variant::levelUp)
	{
		maxLife = WAVE_LIFE;
		middlelife = middlelifewave;
	}
	else
	{
		maxLife = TEXT_LIFE;
		middlelife = middeLife;
	}

	if (lifeMoment < 300)
	{
		ret = fadeIn;
	}
	else if (lifeMoment >= 300 && lifeMoment <= middlelife)
	{
		ret = Middle;
	}
	else if (lifeMoment > middlelife && lifeMoment <= maxLife)
	{

		ret = fadeOut;
	}
	else if (lifeMoment > maxLife)
	{
		ret = dead;
		//CleanUp(); 
		to_delete = true;


		if (this->numerOrText == variant::wave)
		{
			LOG("VAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAWE");
		}
	}


	return ret;
}


void UiItem_HitPoint::CleanUp()
{

	if (this->numerOrText == variant::text || this->numerOrText == variant::gold || this->numerOrText == variant::wave || this->numerOrText == variant::levelUp || this->numerOrText == variant::save)
	{
		App->HPManager->labelsSpawned.totalLabels--;
	}


	if (texture != nullptr)
	{
		App->tex->UnLoad(texture);
		texture = nullptr;
	}
}

void UiItem_HitPoint::updateHitPointPositions()
{
	if (this->numerOrText == variant::number || this->numerOrText == variant::gold)
	{
		hitBox.y -= 2;
	}

}

void UiItem_HitPoint::updateHitPointSizes()
{
	BROFILER_CATEGORY("Hit Point Update Sizes", Profiler::Color::DarkTurquoise);

	if (returnLifeState() != dead)
	{

		switch (returnLifeState())
		{
		case fadeIn:
			if (this->numerOrText != variant::wave || this->numerOrText == variant::levelUp)
				scaleFactor *= 1.03f;
			else
				scaleFactor *= 1.05f;

			break;
		case Middle:

			if (this->numerOrText == variant::number || this->numerOrText == variant::gold)
			{
				scaleFactor /= 1.003f;
			}


			break;
		case fadeOut:

			if (this->numerOrText == variant::number || this->numerOrText == variant::gold || this->numerOrText == variant::wave || this->numerOrText == variant::levelUp)
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
	BROFILER_CATEGORY("Hit Point Update Opacities", Profiler::Color::DarkTurquoise);

	if (returnLifeState() != dead)
	{
		switch (returnLifeState())
		{
		case fadeIn:
			if (this->numerOrText != variant::wave && this->numerOrText != variant::levelUp)
			{
				alphaValue *= 3;
			}
			else
			{
				alphaValue *= 3.3f;
			}

			break;
		case Middle:
			if (this->numerOrText == variant::number || this->numerOrText == variant::gold)
			{
				alphaValue /= 1.1f;
			}
			else if (this->numerOrText != variant::wave && this->numerOrText != variant::levelUp)
			{
				alphaValue /= 1.01f;
			}
			else
			{
			//	alphaValue /= 1.003f;
			}

			break;
		case fadeOut:
			if (this->numerOrText == variant::number || this->numerOrText == variant::gold)
			{
				alphaValue /= 1.7f;
			}
			else if (this->numerOrText != variant::wave && this->numerOrText != variant::levelUp)
			{
				alphaValue /= 1.1f;
			}
			else
			{
			    alphaValue /= 1.02f;
			}
			break;
		}
		SDL_SetTextureAlphaMod(texture, alphaValue);
	}

}