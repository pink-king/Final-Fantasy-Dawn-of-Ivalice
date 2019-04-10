#include "UiItem_HealthBar.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "j1EntityFactory.h"
#include "PlayerEntityManager.h"

UiItem_HealthBar::UiItem_HealthBar(iPoint position, const SDL_Rect* staticSection, const SDL_Rect* dynamicSection, const SDL_Rect* damageSection, type variant, UiItem*const parent) : UiItem(position, parent)
{
	this->guiType = GUI_TYPES::HEALTHBAR;
	this->variantType = variant;

	iPoint staticPos = position;
	staticImage = App->gui->AddImage(staticPos, staticSection, this);

	iPoint newPos(staticPos.x + (staticSection->w - dynamicSection->w) / 2, staticPos.y + (staticSection->h - dynamicSection->h) / 2);

	dynamicImage = App->gui->AddImage(newPos, dynamicSection, this);

	damageImage = App->gui->AddImage(newPos + iPoint(8, 0), damageSection, this);  // this will appear when player gets hurt  // TODO: print it perfectly
	damageImage->hide = true;

	maxSection = dynamicImage->section.w;

}


UiItem_HealthBar::UiItem_HealthBar(const SDL_Rect* dynamicSection, type variant, UiItem*const parent, j1Entity* deliever) : UiItem(parent)
{
	this->guiType = GUI_TYPES::HEALTHBAR;
	this->variantType = variant;
	this->deliever = deliever; 

	
               // rigth now only the dynamic is needed
	offsetFromEnemy = iPoint(dynamicSection->w / 4 - deliever->size.x / 2, dynamicSection->h / 2);

	iPoint newPos(deliever->position.x - offsetFromEnemy.x, deliever->position.y - offsetFromEnemy.y);
	dynamicImage = App->gui->AddImage(newPos, dynamicSection, this);

	maxSection = dynamicImage->section.w;


	
	// capture the enemy's max life and hide image

	enemyMaxLife = deliever->life; 
	dynamicImage->hide = true; 


}



void UiItem_HealthBar::Draw(const float& dt)
{

	

	// we will use the draw call to calculate, but the two images are drawn in image cpp

	
	if (this->variantType == type::player)
	{

			if (conversionFactor == 0.0f)
			{
				conversionFactor = maxSection / App->entityFactory->player->selectedCharacterEntity->life;
			}

			lastSection = dynamicImage->section.w;
			dynamicImage->section.w = conversionFactor * App->entityFactory->player->selectedCharacterEntity->life;


			if (damageInform.doDamage)
			{
				damageBarTimer.Start();
				DamageLogic();
			}
			else if (damageBarTimer.ReadMs() > 400) // if time's over
			{
				DamageQuadReset();
			}
			else if (lastSection < dynamicImage->section.w) // if life being recuperated
			{
				DamageQuadReset();
			}
		}
	else
	{

		// start showing bar only after first attack to the enemy

		if (!startShowing && deliever->life < enemyMaxLife)
		{
			dynamicImage->hide = false; 
			startShowing = true; 
		}



		if (deliever->life > 0)          // TODO: which condition should be here ???? 
		{
			UpdatePos();

			if (conversionFactor == 0.0f)
			{
				conversionFactor = maxSection / deliever->life;
			}

			lastSection = dynamicImage->section.w;
			dynamicImage->section.w = conversionFactor * deliever->life;

		}
		else
		{
			CleanUp(); 
		}
		
	}

}


void UiItem_HealthBar::UpdatePos()
{
	iPoint pos = App->render->WorldToScreen(deliever->position.x - offsetFromEnemy.x, deliever->position.y - offsetFromEnemy.y); 

	dynamicImage->hitBox.x = pos.x;
	dynamicImage->hitBox.y = pos.y;
	

}



void UiItem_HealthBar::DamageLogic()
{

	int destinationRectWidth = lastSection - dynamicImage->section.w;   // the diff betwween max section and current bar health; 

	iPoint destinationRectPos = iPoint(dynamicImage->hitBox.x + dynamicImage->section.w, dynamicImage->hitBox.y);

	damageImage->hide = false;

	if (destinationRectPos.x >= damageImage->hitBox.x)                              // check that it does not go beyond left limit
	{
		damageImage->resizedRect = { destinationRectPos.x , destinationRectPos.y, destinationRectWidth, damageImage->hitBox.h };
	}
	else
	{
		damageImage->hide = true;
	}

	damageInform.doDamage = false;
}


void UiItem_HealthBar::DamageQuadReset()
{


	damageImage->hide = true;
	damageImage->resizedRect = { 0, 0, 0, 0 };


}


void UiItem_HealthBar::CleanUp()
{


	delete this; 
	

}
