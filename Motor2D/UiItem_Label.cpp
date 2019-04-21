#include "UiItem_Label.h"
#include "UiItem.h"

#include "j1App.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "j1Render.h"
#include <assert.h>


UiItem_Label::UiItem_Label(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent) :UiItem(position, parent)
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

//bool UiItem_Label::ChangeTextureHover(const std::string * textHover, const SDL_Color * color, const TTF_Font * font)
//{
//	return true;
//}
//
//bool UiItem_Label::ChangeTextureIdle(const std::string * textHover, const SDL_Color * color, const TTF_Font * font)
//{
//	return true;
//}

void UiItem_Label::Draw(const float & dt)
{
	if (!hide)
	{
		prevTextDimension.x = textureDimensions.x;
		prevTextDimension.y = textureDimensions.y;
		float speed = 0.0f;

		if (!useCamera)
			speed = 1.0f;

		SDL_QueryTexture(texture, NULL, NULL, &textureDimensions.x, &textureDimensions.y);
		if (textureDimensions.x > prevTextDimension.x)
		{
			if (!first_hitbox)
			{
				hitBox.x -= 28;
				first_hitbox = true;
			}
			else
				hitBox.x -= 14;
		}

		App->render->BlitGui(texture, hitBox.x, hitBox.y, NULL, speed);
	}
}

bool UiItem_Label::ChangeTextureIdle(std::string textIdle, const SDL_Color* color, const TTF_Font* font)
{
	bool ret = false;

	SDL_Color col = (color != NULL) ? *color : this->color;
	TTF_Font * f = (font != NULL) ? (TTF_Font *)font : this->font;

	SDL_Texture* aux = App->font->Print(textIdle.data(), col, f);
	assert(aux != nullptr);

	if (this->texture != nullptr)
	{
		App->tex->UnLoad(this->texture);
		this->texture = nullptr;
	}

	texture = aux;

	return ret;


}
