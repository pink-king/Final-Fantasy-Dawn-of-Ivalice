#include "UiItem_Label.h"
#include "UiItem.h"

#include "j1App.h"
#include "j1Fonts.h"
#include "j1Scene.h"
#include "j1Textures.h"
#include "j1Render.h"
#include <assert.h>


#include "j1EntityFactory.h"



UiItem_Label::UiItem_Label(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent, bool typewriter) :UiItem(position, parent)
{
	if (!typewriter)
	{
		texture = App->font->Print(text.data(), color, font);
		this->guiType = GUI_TYPES::LABEL;
		this->text = text;
		this->color = color;
		this->font = font;


		if (texture)
			SDL_QueryTexture(texture, NULL, NULL, &textureDimensions.x, &textureDimensions.y);

		// the parent
		this->parent = parent;
	}

	else
	{
		this->guiType = GUI_TYPES::LABEL;
		typewriter_text = text;
		this->text = "";
		this->color = color;
		this->font = font;
		texture = App->font->Print(this->text.data(), color, font);

		if (texture)
			SDL_QueryTexture(texture, NULL, NULL, &textureDimensions.x, &textureDimensions.y);

		// the parent
		this->parent = parent;
		spawn_typewriter = true;
		right_text = true;
	}

}

UiItem_Label::~UiItem_Label()
{
	
}

//bool UiItem_Label::ChangeTextureHover(const std::string * textHover, const SDL_Color * color, const TTF_Font * font)
//{
//	this->guiType = GUI_TYPES::LABEL;
//	typewriter_text = text;
//	this->text = "";
//	this->color = color;
//	this->font = font;
//	texture = App->font->Print(this->text.data(), color, font);
//
//	if (texture)
//		SDL_QueryTexture(texture, NULL, NULL, &textureDimensions.x, &textureDimensions.y);
//
//	// the parent
//	this->parent = parent;
//	spawn_typewriter = true;
//	right_text = true;
//	
//}


void UiItem_Label::Draw(const float & dt)
{
	if (!hide)
	{
		if (!this->right_text)
		{
			prevTextDimension.x = textureDimensions.x;
			prevTextDimension.y = textureDimensions.y;
			float speed = 0.0f;

			if (!useCamera)
				speed = 1.0f;

			SDL_QueryTexture(texture, NULL, NULL, &textureDimensions.x, &textureDimensions.y);
			if (textureDimensions.x != prevTextDimension.x)
			{
				if (textureDimensions.x == 33)
				{
					hitBox.x = 1080;
					App->scene->coins_label_inventory->hitBox.x = 1080;
				}
				if (textureDimensions.x == 47)
				{
					hitBox.x = 1066;
					App->scene->coins_label_inventory->hitBox.x = 1066;
				}
				
				if (textureDimensions.x == 61)
				{
					hitBox.x = 1052;
					App->scene->coins_label_inventory->hitBox.x = 1052;
				}
				if (textureDimensions.x == 75)
				{
					hitBox.x = 1038;
					App->scene->coins_label_inventory->hitBox.x = 1038;
				}
				if (textureDimensions.x == 89)
				{
					hitBox.x = 1024;
					App->scene->coins_label_inventory->hitBox.x = 1024;
				}
				if (textureDimensions.x == 103)
				{
					hitBox.x = 1010;
					App->scene->coins_label_inventory->hitBox.x = 1010;
				}
					
					
				
			}

			App->render->BlitGui(texture, hitBox.x, hitBox.y, NULL, speed);
		}

		else
		{
		
			float speed = 0.0f;

			if (!useCamera)
				speed = 1.0f;

			
			App->render->BlitGui(texture, hitBox.x, hitBox.y, NULL, speed);
		}
	}



	

	if (!timer_typewriter && spawn_typewriter)
	{
		TypeWriter();
	}


}

void UiItem_Label::CleanUp()
{
	if (texture != nullptr)
	{
		App->tex->UnLoad(texture);
		texture = nullptr;
	}
}



bool UiItem_Label::TypeWriter()
{
	int i = 0;
	
		
	for (i; i < 1; i++)
	{
		if (counter_typewriter < typewriter_text.length())
		{
			this->text = this->text + typewriter_text.at(counter_typewriter);
			ChangeTextureIdle(this->text, NULL, NULL);
		}
	}
	
	if (counter_typewriter == typewriter_text.length())
	{
		timer_typewriter = true;
		spawn_typewriter = false;

		finishedWriting = true;
	}
	else
		counter_typewriter++;
		
		
	
	return true;
}

bool UiItem_Label::ChangeTextureIdle(std::string textIdle, const SDL_Color* color, const TTF_Font* font, uint numberOfDecimals)
{
	bool ret = false;

	if (numberOfDecimals != NULL && numberOfDecimals > 0)
	{
		std::string newString = {}; 
		uint decimals = 0; 
		bool count = false; 

		for (int i = 0; i < textIdle.size() - 1; ++i)
		{
			if (textIdle.at(i) == '.')
			{
				count = true; 
			}

			if (count)
			{
				if (decimals <= numberOfDecimals)
				{
				    
				newString += textIdle.at(i);
				decimals++;
					
				
				}
				else
				{
					break; 
				}
				
			}
			else
			{
				newString += textIdle.at(i);
			}
		}

		textIdle.clear(); 
		textIdle.append(newString); 
	}
	
	SDL_Color col = (color != NULL) ? *color : this->color;
	TTF_Font * f = (font != NULL) ? (TTF_Font *)font : this->font;

	SDL_Texture* aux = App->font->Print(textIdle.data(), col, f);
	if(aux != nullptr)
		assert(aux != nullptr);

	if (this->texture != nullptr)
	{
		App->tex->UnLoad(this->texture);
		this->texture = nullptr;
	}

	texture = aux;

	return ret;


}

void UiItem_Label::DoLogicHovered()
{
	if (this->isDialog)
	{
		const SDL_Color c = { 180, 200, 22, 200 };
		ChangeTextureIdle(this->text, &c, App->font->openSansBold18);
	}
}



void UiItem_Label::DoLogicAbandoned()
{
	if (this->isDialog)
	{
		const SDL_Color c = { 34, 200, 43, 255 };
		ChangeTextureIdle(this->text, &c, App->font->openSansBold18);
	}
}

