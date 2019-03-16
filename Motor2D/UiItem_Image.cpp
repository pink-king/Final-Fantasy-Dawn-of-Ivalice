#include "UiItem_Image.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Scene.h"

UiItem_Image::UiItem_Image(SDL_Rect hitBox, const SDL_Rect * section, UiItem * const parent, p2Point<int> pivot) : UiItem(hitBox, parent, pivot)
{

}

void UiItem_Image::Draw(const float& dt)
{
}
