#include "UiItem_HealthBar.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "j1EntityFactory.h"
#include "PlayerEntityManager.h"
#include "j1Fonts.h"
#include "Brofiler/Brofiler.h"

UiItem_HealthBar::UiItem_HealthBar(iPoint position, const SDL_Rect* dynamicSection, const SDL_Rect* damageSection, type variant, UiItem* const parent) : UiItem(position, parent)
{
	this->guiType = GUI_TYPES::HEALTHBAR;
	this->variantType = variant;




	dynamicImage = App->gui->AddImage(position, dynamicSection,name, this);

	damageImage = App->gui->AddImage(position + playerBarOffset, damageSection,name, this);  // this will appear when player gets hurt  // TODO: print it perfectly
	damageImage->hide = true;

	maxSection = dynamicImage->section.w;

}

UiItem_HealthBar::UiItem_HealthBar(iPoint position, const SDL_Rect* dynamicSection, const SDL_Rect* staticSection, const SDL_Rect* divSection, type variant, uint maxLife, j1Entity* deliever, UiItem* const parent) : UiItem(position, parent)
{
	this->guiType = GUI_TYPES::HEALTHBAR;
	this->variantType = variant;

	staticImage = App->gui->AddImage(position, staticSection,name, this);  // this will appear when player gets hurt  // TODO: print it perfectly

	iPoint offset((staticSection->w - dynamicSection->w) / 2, (staticSection->h - dynamicSection->h) / 2);

	dynamicImage = App->gui->AddImage(position + offset, dynamicSection, name, this);

	maxSection = dynamicImage->section.w;

	enemyMaxLife = deliever->life;  // boss life

	conversionFactor = maxSection / deliever->life;

	this->deliever = deliever;

	bossSeparationWidth = maxSection / bossBarSeparations;  // divide bar in 4 sections


	this->divisionImage = App->gui->AddImage(position + iPoint(bossSeparationWidth, offset.y), divSection, name, this);

	this->nameOnTop = App->gui->AddLabel(deliever->name, { 230, 240, 200, 255 }, App->font->piecesofEight36, iPoint(0, staticImage->hitBox.y - 40), this, false);

	int stringw, stringh = 0;
	SDL_QueryTexture(nameOnTop->texture, NULL, NULL, &stringw, &stringh);
	iPoint destPos = { (int)((float)this->staticImage->hitBox.x + (float)this->staticImage->section.w * .5f) - (int)((float)stringw * .5f), 0 };
	this->nameOnTop->hitBox.x = destPos.x;

	this->staticImage->hide = true;
	this->dynamicImage->hide = true;
	this->divisionImage->hide = true;
	this->nameOnTop->hide = true;


	// todo: skull 
	skull = App->gui->AddImage(iPoint(dynamicImage->hitBox.x - 30, dynamicImage->hitBox.y ), &App->gui->enemySkullInfo.BossEnemyRect, name, this);

	skullOffset.y = -(skull->section.h / 2 - dynamicImage->section.h / 2);
	skull->hitBox.y += skullOffset.y;
	skull->hide = true; 


}


UiItem_HealthBar::UiItem_HealthBar(const SDL_Rect* dynamicSection, type variant, UiItem* const parent, j1Entity* deliever) : UiItem(parent)
{
	this->guiType = GUI_TYPES::HEALTHBAR;
	this->variantType = variant;
	this->deliever = deliever;


	// rigth now only the dynamic is needed
	offsetFromEnemy = iPoint(dynamicSection->w / 4 - deliever->size.x / 2, 10);

	iPoint newPos(deliever->position.x - offsetFromEnemy.x, deliever->position.y - offsetFromEnemy.y);
	dynamicImage = App->gui->AddImage(newPos, dynamicSection, name, this);

	maxSection = dynamicImage->section.w;



	// capture the enemy's max life and hide image

	enemyMaxLife = deliever->life;
	dynamicImage->hide = true;

	// skull

	iPoint dynImPos = iPoint(dynamicImage->hitBox.x, dynamicImage->hitBox.y); 

	if (deliever->type == ENTITY_TYPE::ENEMY_BOMB || deliever->type == ENTITY_TYPE::ENEMY_TEST)
	{
		skull = App->gui->AddImage(dynImPos, &App->gui->enemySkullInfo.baseEnemyRect, name, this);

		skullOffset.y = -(skull->section.h / 2 - dynamicImage->section.h / 2);

		spawnedSkull = true; 
	}
	else if(deliever->type == ENTITY_TYPE::ENEMY_ARCHER)
	{
		skull = App->gui->AddImage(dynImPos, &App->gui->enemySkullInfo.strongEnemyRect, name, this);
		
		skullOffset.y = -(skull->section.h / 2 - dynamicImage->section.h / 2);

		spawnedSkull = true;
	}

	//skull->hitBox.x += skullOffset.x;

	// at the start, put the skull on top of the enemy: 

	if (spawnedSkull)
	{
		skull->hitBox.x += dynamicImage->section.w / 2 - skull->section.w / 2;

		skull->hitBox.y += skullOffset.y;

		skull->hide = false;
	}
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
	else if (this->variantType == type::boss)
	{

		/*lastSection = dynamicImage->section.w;
		dynamicImage->section.w = conversionFactor * deliever->life;*/  // shouldn't do anything here

	}
	else if (this->variantType == type::enemy)
	{



		if (!to_delete && deliever)          // TODO: DO THIS FROM THE ENEMY 
		{

			// start showing bar only after first attack to the enemy

			if (!startShowing && deliever->life < enemyMaxLife)
			{
				dynamicImage->hide = false;
				skull->hide = false;
				startShowing = true;
			}

			if (startShowing) {

				if (!App->scene->inventory->enable)
				{
					dynamicImage->hide = false;             // CAUTION: dummies do not have skull
					skull->hide = false;
				}
				else
				{
					dynamicImage->hide = true;
					skull->hide = true; 
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

void UiItem_HealthBar::ShowBossBarWhenDialogIsOver()
{

	this->staticImage->hide = false;
	this->dynamicImage->hide = false;
	this->divisionImage->hide = false;
	this->nameOnTop->hide = false;

	// todo: skull
	if(spawnedSkull)
	this->skull->hide = false; 

}
void UiItem_HealthBar::UpdatePos()
{
	BROFILER_CATEGORY("Healthbar Pos", Profiler::Color::MidnightBlue);

	iPoint pos = App->render->WorldToScreen(deliever->position.x + deliever->size.x / 2 - dynamicImage->section.w / 4, deliever->position.y - offsetFromEnemy.y);

	dynamicImage->hitBox.x = pos.x;
	dynamicImage->hitBox.y = pos.y;

	if (spawnedSkull)
	{
		if (deliever->type != ENTITY_TYPE::FLOWERBOSS)
		{
			iPoint skullpos = App->render->WorldToScreen(deliever->position.x + deliever->size.x / 2 - skull->section.w / 4, deliever->position.y - offsetFromEnemy.y);

			if (startShowing)
			{
				skull->hitBox.x = pos.x + skullOffset.x;
				skull->hitBox.y = pos.y + skullOffset.y;

			}
			else
			{
				skull->hitBox.x = skullpos.x; // +dynamicImage->section.w / 2 - skull->section.w / 2;
				skull->hitBox.y = skullpos.y + skullOffset.y;
			}

		}

	}





}

void UiItem_HealthBar::CleanUp()
{
	/*if(dynamicImage != nullptr)
		dynamicImage->to_delete = true;*/
	/*if(damageImage != nullptr)
		damageImage->to_delete = true;*/

	if (this->variantType == type::player)
	{
		if (damageImage != nullptr)
			damageImage->to_delete = true;
	}

}

void UiItem_HealthBar::doDamageToBoss(uint lifeToSubstract)
{
	uint SeparationJumps = 0;

	uint lifeEquivalentInSection = lifeToSubstract * conversionFactor;
	uint actualDynImageWidth = dynamicImage->section.w;

	for (int i = 1; i <= remainingBossSeparations; ++i)
	{
		int c1 = (int)actualDynImageWidth - (int)lifeEquivalentInSection;
		int c2 = bossSeparationWidth * (remainingBossSeparations - i);

		if (c1 < c2)
		{
			SeparationJumps++;

		}
	}

	if (SeparationJumps > 0)
	{
		remainingBossSeparations -= SeparationJumps;
		dynamicImage->section.w = bossSeparationWidth * remainingBossSeparations;
		LOG("There has been a boss helathbar substaction!!!");

	}

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


