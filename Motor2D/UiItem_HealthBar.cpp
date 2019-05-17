#include "UiItem_HealthBar.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "j1EntityFactory.h"
#include "PlayerEntityManager.h"
#include "Brofiler/Brofiler.h"

UiItem_HealthBar::UiItem_HealthBar(iPoint position, const SDL_Rect* dynamicSection, const SDL_Rect* damageSection, type variant, UiItem* const parent) : UiItem(position, parent)
{
	this->guiType = GUI_TYPES::HEALTHBAR;
	this->variantType = variant;




	dynamicImage = App->gui->AddImage(position, dynamicSection, this);

	damageImage = App->gui->AddImage(position + playerBarOffset, damageSection, this);  // this will appear when player gets hurt  // TODO: print it perfectly
	damageImage->hide = true;

	maxSection = dynamicImage->section.w;

}


UiItem_HealthBar::UiItem_HealthBar(const SDL_Rect* dynamicSection, type variant, UiItem* const parent, j1Entity* deliever) : UiItem(parent)
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

UiItem_HealthBar::~UiItem_HealthBar()
{

}



void UiItem_HealthBar::Draw(const float& dt)
{

	BROFILER_CATEGORY("Healthbar Draw", Profiler::Color::MidnightBlue);


	// we will use the draw call to calculate, but the two images are drawn in image cpp


	if (this->variantType == type::player)
	{

		if (conversionFactor == 0.0f)
		{
			conversionFactor = maxSection / App->entityFactory->player->life;
		}

		lastSection = dynamicImage->section.w;
		
		uint value = conversionFactor * App->entityFactory->player->life + playerBarOffset.x;

		if (App->entityFactory->player->life <= 0)
		{
			dynamicImage->section.w = playerBarOffset.x; 
		}
		else
		{
			dynamicImage->section.w = value;
		}
		

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



		if (!to_delete && deliever)          // TODO: DO THIS FROM THE ENEMY 
		{

			// start showing bar only after first attack to the enemy

			if (!startShowing && deliever->life < enemyMaxLife)
			{
				dynamicImage->hide = false;
				startShowing = true;
			}

			if (startShowing) {

				if (!App->scene->inventory->enable)
				{
					dynamicImage->hide = false;
				}
				else
				{
					dynamicImage->hide = true;
				}

			}


			UpdatePos();

			if (conversionFactor == 0.0f)
			{
				conversionFactor = maxSection / deliever->life;
			}

			lastSection = dynamicImage->section.w;
			dynamicImage->section.w = conversionFactor * deliever->life;

		}



	}

}


void UiItem_HealthBar::UpdatePos()
{
	BROFILER_CATEGORY("Healthbar Pos", Profiler::Color::MidnightBlue);

	iPoint pos = App->render->WorldToScreen(deliever->position.x - offsetFromEnemy.x, deliever->position.y - offsetFromEnemy.y);

	dynamicImage->hitBox.x = pos.x;
	dynamicImage->hitBox.y = pos.y;


}

void UiItem_HealthBar::CleanUp()
{
	/*if(dynamicImage != nullptr)
		dynamicImage->to_delete = true;*/
	/*if(damageImage != nullptr)
		damageImage->to_delete = true;*/
}



void UiItem_HealthBar::DamageLogic()
{
	BROFILER_CATEGORY("Healthbar Logic", Profiler::Color::MidnightBlue);


	int destinationRectWidth = lastSection - dynamicImage->section.w;   // the diff betwween max section and current bar health; 

	iPoint destinationRectPos = iPoint(dynamicImage->hitBox.x + dynamicImage->section.w, dynamicImage->hitBox.y + playerBarOffset.y);

	damageImage->hide = false;

	if (destinationRectPos.x >= damageImage->hitBox.x + playerBarOffset.x)                              // check that it does not go beyond left limit
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


