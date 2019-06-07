#include "UiItem_HitPoint.h"
#include "UiItem_HitPointManager.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Fonts.h"
#include "j1EntityFactory.h"
#include "PlayerEntityManager.h"
#include "Brofiler/Brofiler.h"

#include <string.h>

UiItem_HitPointManager::UiItem_HitPointManager()
{
	name.assign("HPLabelManager");
}

UiItem_HitPointManager::~UiItem_HitPointManager()
{
}

bool UiItem_HitPointManager::Awake(pugi::xml_node &node)
{
	return true;
}

bool UiItem_HitPointManager::Start()
{
	return true;
}

bool UiItem_HitPointManager::Update(float dt)
{
	BROFILER_CATEGORY("Update HitPoint", Profiler::Color::PeachPuff);


	std::vector<UiItem_HitPoint*>::iterator hitPointIterator = hitPointLabels.begin();

	for (; hitPointIterator != hitPointLabels.end();)
	{
		if (!(*hitPointIterator)->to_delete)
		{
			(*hitPointIterator)->updateHitPointPositions();
			(*hitPointIterator)->updateHitPointSizes();
			(*hitPointIterator)->updateHitPointOpacities();

			++hitPointIterator;
		}
		else
		{
			if ((*hitPointIterator)->fromEnemy)
			{
				enemyLabels--;
			}
			else
			{
				if ((*hitPointIterator)->numerOrText != variant::gold && (*hitPointIterator)->numerOrText != variant::wave)                    // don't consider gold labels
					labelScoreAccum -= (*hitPointIterator)->valueInformation.number;
			}


			// cleanup
			(*hitPointIterator)->CleanUp();
			delete (*hitPointIterator);
			hitPointIterator = hitPointLabels.erase(hitPointIterator);

		}
	}



	calculatePlayerCombo();    // TODO: ideate condition so that this is not executed every frame, BUT it must not break the label spawning looping 


	return true;
}

bool UiItem_HitPointManager::CleanUp()
{

	for (std::vector<UiItem_HitPoint*>::iterator item = hitPointLabels.begin(); item != hitPointLabels.end(); ++item)
	{
		if ((*item) != nullptr)
		{


			// first clear the texture 
			if ((*item)->texture != nullptr)
			{
				App->tex->UnLoad((*item)->texture);
				(*item)->texture = nullptr;
			}


			delete *item;
			*item = nullptr;
		}
	}
	hitPointLabels.clear();

	return true;
}


UiItem_HitPoint* UiItem_HitPointManager::callHPLabelSpawn(iPoint pos, uint damage, ELEMENTAL_TYPE type, bool healing, bool playerIsAttacker)
{
	UiItem_HitPoint* ret = nullptr;

	std::string str = std::to_string(damage);

	valueInfo info = {
				   str,
				  damage,
	};


	// TODO: load colors and fonts from XML 

	SDL_Color c = {};
	switch (type)
	{
	case ELEMENTAL_TYPE::FIRE_ELEMENT:
		c = { 255, 0, 0, 255 };
		break;

	case ELEMENTAL_TYPE::POISON_ELEMENT:
		c = { 90, 60, 255, 255 };
		break;

	case ELEMENTAL_TYPE::ICE_ELEMENT:
		c = { 153, 175, 255, 255 };
		break;

	case ELEMENTAL_TYPE::NO_ELEMENT:
		c = { 233, 200, 200, 255 };
		break;

	default:
		c = { 233, 200, 200, 255 };
		break;
	}

	if (healing)
		c = { 0, 255, 0, 255 };


	if (playerIsAttacker)
		labelScoreAccum += damage;

	else
		++enemyLabels;







	iPoint newPos(App->render->WorldToScreen(pos.x + 3, pos.y - 10, true));  // todo: pas the attacked entity and capture the width                                               // adjust this  


	ret = App->gui->AddHitPointLabel(info, c, App->font->openSansBold36, newPos, nullptr, variant::number);    // big font for testing

	if (!playerIsAttacker)
	{
		ret->fromEnemy = true;
	}
	else
	{
		ret->fromEnemy = false;
	}

	return ret;

}

UiItem_HitPoint * UiItem_HitPointManager::callGoldLabelSpawn(iPoint pos, uint value)
{
	valueInfo info = {
		"GOLD",
		value,
	};

	App->gui->AddHitPointLabel(info, {255, 255, 194, 255}, App->font->openSansBold36, pos, nullptr, variant::gold);

	return nullptr;
}

UiItem_HitPoint* UiItem_HitPointManager::callWaveLabelSpawn(iPoint pos, uint value)
{
	valueInfo info = {
		"WAVE",
		value,
	};

	App->gui->AddHitPointLabel(info, { 255, 0, 54, 255 }, App->font->piecesofEight48, pos, nullptr, variant::wave);

	return nullptr;
}

UiItem_HitPoint * UiItem_HitPointManager::callSaveLabelSpawn(iPoint pos)
{
	valueInfo info = {
		"SAVE",
	};

	App->gui->AddHitPointLabel2("SAVE", { 0, 255, 0, 255 }, App->font->openSansBold18, pos, nullptr, variant::text);

	return nullptr;
}


void UiItem_HitPointManager::calculatePlayerCombo()
{

	// streak = number of score labels and summation of their scores 
	if (!hitPointLabels.empty())
	{
		playerStreak = (hitPointLabels.size() - labelsSpawned.totalLabels - enemyLabels) * labelScoreAccum;    // text labels must not be considerated
	}

	//LOG("............................................  Player  streak %i, number of labels %i, summation of label scores %i ", playerStreak, (hitPointLabels.size() - labelsSpawned.totalLabels), labelScoreAccum);

	if (playerStreak > 200 && !labelsSpawned.fierce)                         // fierce 
	{
		iPoint pos(App->render->WorldToScreen(App->entityFactory->player->selectedCharacterEntity->GetPosition().x, App->entityFactory->player->selectedCharacterEntity->GetPosition().y));

		pos.x -= 500;
		App->gui->AddHitPointLabel2("FIERCE", { 255, 165, 0,255 }, App->font->shatterBoxx36, pos, nullptr, variant::text);

		labelsSpawned.fierce = true;
		labelsSpawned.totalLabels++;
	}

	if (playerStreak > 2000 && !labelsSpawned.brutal)                         // brutal   
	{
		iPoint pos(App->render->WorldToScreen(App->entityFactory->player->selectedCharacterEntity->GetPosition().x, App->entityFactory->player->selectedCharacterEntity->GetPosition().y));

		pos.x += 300; // to sepparate both labels
		App->gui->AddHitPointLabel2("BRUTAL", { 255, 0, 0,255 }, App->font->shatterBoxx48, pos, nullptr, variant::text);

		labelsSpawned.brutal = true;
		labelsSpawned.totalLabels++;
	}


	if (labelScoreAccum == 0)     // reset all labels when player loses streak
	{
		playerStreak = 0;
		labelsSpawned.fierce = false;
		labelsSpawned.brutal = false;

	}
}
