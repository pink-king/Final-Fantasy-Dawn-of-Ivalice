#include "UiItem_Label.h"
#include "UiItem.h"

#include "j1App.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "j1Render.h"


UiItem_Label::UiItem_Label(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent) :UiItem(position, parent)
{
	
	texture = App->font->Print(text.data(), color, font);
	this->guiType = GUI_TYPES::LABEL;
	this->text = text;
	this->color = color;
	this->font = font;
	


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
	App->render->BlitGui(texture, hitBox.x, hitBox.y, NULL, 0.0F);



}
