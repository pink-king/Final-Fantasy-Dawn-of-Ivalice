#include <algorithm>
#include "PlayerEntityManager.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1BuffManager.h"
#include "j1EntityFactory.h"
//buff test
#include "j1Window.h"
#include "j1Scene.h"
#include "UiItem.h"
#include "UiItem_Label.h"

PlayerEntityManager::PlayerEntityManager(iPoint position) : j1Entity(PLAYER, position.x,position.y, "PEM")
{
	marche = new Marche(position.x,position.y);
	ritz = new Ritz(position.x, position.y);
	shara = new Shara(position.x, position.y);
	// crosshair - one for all the characters, its should be improved so if we need
	crossHair = new Crosshair();

	characters.push_back(marche);
	characters.push_back(ritz);
	characters.push_back(shara);

	selectedCharacterEntity = marche;

	// debug normal tile tex
	debugTileTex = App->tex->Load("maps/tile_64x64_2.png");
	debugSubtileTex = App->tex->Load("maps/tile_32x32.png");

	debug = false;
	
}

PlayerEntityManager::~PlayerEntityManager()
{
	
	delete crossHair;

	// TODO: free characters vector
}

//bool PlayerEntityManager::Awake(pugi::xml_node & node)
//{
//	return true;
//}

bool PlayerEntityManager::Start()
{
	// loads the characters data
	std::vector<PlayerEntity*>::iterator item = characters.begin();

	for (; item != characters.end(); ++item)
		(*item)->Start();

	pivot = selectedCharacterEntity->pivot;


	pickLoot = App->audio->LoadFx("audio/fx/pickLoot.wav");
	pickGold = App->audio->LoadFx("audio/fx/pickGold.wav");
	consumHealPotion = App->audio->LoadFx("audio/fx/consumHealingPotion.wav");
	return true;
}

bool PlayerEntityManager::PreUpdate()
{
	return true;
}

bool PlayerEntityManager::Update(float dt)
{
	bool ret = true;

	SwapInputChecker(); // checks gamepad "shoulders" triggers input

	selectedCharacterEntity->Update(dt);
	// update selected character position to its "manager" position
	position = selectedCharacterEntity->position;
	lastCharHeadingAngle = selectedCharacterEntity->GetLastHeadingAngle();

	if (selectedCharacterEntity->IsAiming())
	{
		crossHair->Update(dt);
	}
	else if (!crossHair->isReseted)
		crossHair->Reset();
		
	//collect loot
	for (std::vector<j1Entity*>::iterator item = App->entityFactory->entities.begin(); item != App->entityFactory->entities.end(); ++item)
		if (App->entityFactory->player->GetSubtilePos() == (*item)->GetSubtilePos() && (*item)->type == ENTITY_TYPE::LOOT)
		{
			if (CollectLoot((LootEntity*)(*item)))
			{
				App->audio->PlayFx(pickLoot, 0);
				App->entityFactory->DeleteEntityFromSubtile(*item);
				item = App->entityFactory->entities.erase(item);
				break;
			}
		}
	
	// WARNING: search other way to do this
	////provisional function to life
	//std::vector<PlayerEntity*>::iterator item = characters.begin();
	//for (; item != characters.end(); ++item)
	//{
	//	if ((*item) != selectedCharacterEntity)
	//		(*item)->life = selectedCharacterEntity->life;
	//}

	static char title[30];
	sprintf_s(title, 30, " | life: %f", life);
	App->win->AddStringToTitle(title);

	/*for (std::vector<LootEntity*>::iterator iter = equipedObjects.begin(); iter != equipedObjects.end(); ++iter)
	{
		static char title[30];
		sprintf_s(title, 30, " | objects: %s", (*iter)->name.data());
		App->win->AddStringToTitle(title);
	}*/
	App->win->ClearTitle();

	return ret;
}

bool PlayerEntityManager::PostUpdate()
{
	selectedCharacterEntity->PostUpdate();

	if (debug)
	{
		// normal sized tile
		iPoint tilePos = GetTilePos();
		tilePos = App->map->MapToWorld(tilePos.x, tilePos.y);
		App->render->Blit(debugTileTex, tilePos.x, tilePos.y, NULL);
		// subtiles
		iPoint subTilePos = GetSubtilePos();
		subTilePos = App->map->SubTileMapToWorld(subTilePos.x, subTilePos.y);
		App->render->Blit(debugSubtileTex, subTilePos.x, subTilePos.y, NULL);
	}
	
	return true;
}

bool PlayerEntityManager::CleanUp()
{
	std::vector<PlayerEntity*>::iterator charIter = characters.begin();
	for (; charIter != characters.end(); ++charIter)
	{
		(*charIter)->CleanUp();
		delete (*charIter);
		(*charIter) = nullptr;

	}
	characters.clear();

	std::vector<LootEntity*>::iterator iter = bagObjects.begin();
	for (; iter != bagObjects.end(); ++iter)
	{
		delete *iter;
		*iter = nullptr;
	}
	bagObjects.clear();

	std::vector<LootEntity*>::iterator iter2 = equipedObjects.begin();
	for (; iter2 != equipedObjects.end(); ++iter2)
	{
		delete *iter2;
		*iter2 = nullptr;
	}
	equipedObjects.clear();

	std::vector<LootEntity*>::iterator iter3 = consumables.begin();
	for (; iter3 != consumables.end(); ++iter3)
	{
		delete *iter3;
		*iter3 = nullptr;
	}
	consumables.clear();
	return true;
}


bool PlayerEntityManager::SwapInputChecker()
{
	bool ret = true;

	// checks gamepad and swaps character
	if (selectedCharacterEntity->inputReady)
	{
		if (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_DOWN || App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_DOWN)
		{
			SetPreviousCharacter();
		}

		if (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_DOWN || App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_DOWN)
		{
			SetNextCharacter();
		}
	}

	return ret;
}

void PlayerEntityManager::SetPreviousCharacter()
{
	float current_frame = 0;
	fPoint tempPosition;
	int pointingDirectionTemp = 0;

	std::vector<PlayerEntity*>::reverse_iterator leftItem = characters.rbegin();

	for (; leftItem != characters.rend(); ++leftItem)
	{
		if ((*leftItem) == selectedCharacterEntity)
		{
			bool tempAim = selectedCharacterEntity->aiming;
			selectedCharacterEntity->aiming = false;
			// stores needed data to swap
			current_frame = selectedCharacterEntity->currentAnimation->GetCurrentFloatFrame();
			tempPosition = selectedCharacterEntity->position;
			pointingDirectionTemp = selectedCharacterEntity->pointingDir;

			++leftItem;
			if (leftItem == characters.rend())
				leftItem = characters.rbegin();

			// swaps to
			selectedCharacterEntity = (*leftItem);
			// restores needed data
			selectedCharacterEntity->currentAnimation->SetCurrentFrame(current_frame);
			selectedCharacterEntity->position = tempPosition;
			selectedCharacterEntity->pointingDir = pointingDirectionTemp;
			selectedCharacterEntity->lastAxisMovAngle = lastCharHeadingAngle;
			selectedCharacterEntity->aiming = tempAim;
			selectedCharacterName = selectedCharacterEntity->character;
			// sets current animation
			SetCurrentAnimation();
			// updates pivot
			UpdatePivot();
			App->audio->PlayFx(App->entityFactory->swapChar, 0);
			break;
		}
	}

	LOG("Selected Character: %c", selectedCharacterEntity->name); // TODO: log crashes with %s, std::string relative
}

void PlayerEntityManager::SetNextCharacter()
{
	float current_frame = 0;
	fPoint tempPosition;
	int pointingDirectionTemp = 0;

	std::vector<PlayerEntity*>::iterator nextItem = characters.begin();

	for (; nextItem != characters.end(); ++nextItem)
	{
		if ((*nextItem) == selectedCharacterEntity)
		{
			bool tempAim = selectedCharacterEntity->aiming;
			selectedCharacterEntity->aiming = false;
			current_frame = selectedCharacterEntity->currentAnimation->GetCurrentFloatFrame();
			tempPosition = selectedCharacterEntity->position;
			pointingDirectionTemp = selectedCharacterEntity->pointingDir;

			++nextItem;
			if (nextItem == characters.end())
				nextItem = characters.begin();
	
			selectedCharacterEntity = (*nextItem);
			selectedCharacterEntity->currentAnimation->SetCurrentFrame(current_frame);
			selectedCharacterEntity->position = tempPosition;
			selectedCharacterEntity->pointingDir = pointingDirectionTemp;
			selectedCharacterEntity->lastAxisMovAngle = lastCharHeadingAngle;
			selectedCharacterEntity->aiming = tempAim;
			selectedCharacterName = selectedCharacterEntity->character;
			// sets current animation
			SetCurrentAnimation();
			// updates pivot
			UpdatePivot(); 
			App->audio->PlayFx(App->entityFactory->swapChar, 0);
			break;
		}
	}

	LOG("Selected Character: %c", selectedCharacterEntity->name); // TODO: log crashes with %s, std::string relative
}

void PlayerEntityManager::SetCurrentAnimation()
{
	currentAnimation = selectedCharacterEntity->currentAnimation;
}

void PlayerEntityManager::Draw()
{
	selectedCharacterEntity->Draw();
}

void PlayerEntityManager::UpdatePivot()
{
	pivot = selectedCharacterEntity->pivot;
}

const j1Entity* PlayerEntityManager::GetSelectedCharacterEntity() const
{
	return selectedCharacterEntity;
}

//bool PlayerEntityManager::Draw()
//{
//	switch (selectedCharacter)
//	{
//	case characterName::MARCHE:
//
//		break;
//	case characterName::RITZ:
//		break;
//	case characterName::SHARA:
//		break;
//	default:
//		break;
//	}
//}

iPoint PlayerEntityManager::GetCrossHairSubtile()
{
	iPoint ret = { 0,0 };
	if (crossHair != nullptr)
	{
		ret = crossHair->GetSubtilePoint();
	}
	return ret;
}

iPoint PlayerEntityManager::GetCrossHairPivotPos()
{
	iPoint ret = { 0,0 };
	if (crossHair != nullptr)
	{
		ret = crossHair->GetPivotPos();
	}
	return ret;
}

const float PlayerEntityManager::GetLastPlayerHeadingAngle() const
{
	return lastCharHeadingAngle;
}

bool PlayerEntityManager::CollectLoot(LootEntity * entityLoot)
{
	if (entityLoot->GetType() == LOOT_TYPE::EQUIPABLE)
	{
		if (equipedObjects.size() == 0)
		{
			equipedObjects.push_back(entityLoot);
			App->buff->AddItemStats(entityLoot);
		}
		else
		{
			bool existSimilar = false;
			std::vector<LootEntity*>::iterator item = equipedObjects.begin();
			for (; item != equipedObjects.end(); ++item)
			{

				if (entityLoot->GetObjectType() == (*item)->GetObjectType() && entityLoot->character == (*item)->character)
					existSimilar = true;
			}

			if (!existSimilar && App->entityFactory->player != nullptr)
			{
				equipedObjects.push_back(entityLoot);
				App->buff->AddItemStats(entityLoot);
			}
			else if (existSimilar && App->entityFactory->player != nullptr)
			{
				bagObjects.push_back(entityLoot);
			}
		}
	}
	else if (entityLoot->GetType() == LOOT_TYPE::CONSUMABLE)
	{
		if (entityLoot->GetObjectType() == OBJECT_TYPE::POTIONS)
			consumables.push_back(entityLoot);

		else if (entityLoot->GetObjectType() == OBJECT_TYPE::GOLD)
		{
			App->audio->PlayFx(pickGold, 0);
			gold += entityLoot->price;
			entityLoot->to_delete = true;
			str_coin = "x  " + std::to_string(gold);
			App->scene->coins_label->ChangeTextureIdle(App->entityFactory->player->str_coin, NULL, NULL);
			return false;
		}

	}
	return true;
}

void PlayerEntityManager::EquipItem(LootEntity * entityLoot)
{
	for (std::vector<LootEntity*>::iterator item = bagObjects.begin(); item != bagObjects.end(); ++item)
	{
		if (entityLoot == *item)
		{
			bagObjects.erase(item);
			break;
		}
	}
	if (equipedObjects.size() == 0)
	{
		equipedObjects.push_back(entityLoot);
		App->buff->AddItemStats(entityLoot);
	}
	else
	{
		for (std::vector<LootEntity*>::iterator item = equipedObjects.begin(); item != equipedObjects.end(); ++item)
		{

			if (entityLoot->GetObjectType() == (*item)->GetObjectType() && entityLoot->character == (*item)->character)
			{
				App->buff->RemoveItemStat(*item);
				bagObjects.push_back(*item);
				equipedObjects.erase(item);
				break;
			}
		}
		equipedObjects.push_back(entityLoot);
		App->buff->AddItemStats(entityLoot);
	}
}

void PlayerEntityManager::DesequipItem(LootEntity * entityLoot)
{
	for (std::vector<LootEntity*>::iterator item = equipedObjects.begin(); item != equipedObjects.end(); ++item)
	{
		if (entityLoot == *item)
		{
			App->buff->RemoveItemStat(*item);
			equipedObjects.erase(item);
			bagObjects.push_back(entityLoot);
			break;
		}
	}
}

void PlayerEntityManager::ConsumConsumable(LootEntity * consumable, j1Entity * entity)
{
	for (std::vector<LootEntity*>::iterator item = consumables.begin(); item != consumables.end(); ++item)
	{
		if (consumable == *item)
		{
			if (consumable->objectType == OBJECT_TYPE::POTIONS)
				App->audio->PlayFx(consumHealPotion, 0);
			for (std::vector<Buff*>::iterator iter = consumable->stats.begin(); iter != consumable->stats.end(); ++iter)
			{
				App->buff->CreateHealth((*iter)->GetCharacter(), (*iter)->GetValue(), 8);
			}
			item = consumables.erase(item);
			break;
		}
	}
}

j1Entity * PlayerEntityManager::GetMarche()
{
	return marche;
}

j1Entity * PlayerEntityManager::GetRitz()
{
	return ritz;
}

j1Entity * PlayerEntityManager::GetShara()
{
	return shara;
}

// CROSSHAIR class -------------------------------------------------------------------------------------

Crosshair::Crosshair()
{
	tex = App->tex->Load("textures/spells/crosshair/crosshair_sprites.png");

	startAnim.PushBack({ 23,12,216,63 });
	startAnim.PushBack({ 279,12,216,63 });
	startAnim.PushBack({ 535,12,216,63 });
	startAnim.PushBack({ 791,12,216,63 });

	startAnim.PushBack({ 23,75,216,63 });
	startAnim.PushBack({ 279,75,216,63 });
	startAnim.PushBack({ 535,75,216,63 });
	startAnim.PushBack({ 791,75,216,63 });

	startAnim.PushBack({ 23,139,216,63 });
	startAnim.PushBack({ 279,139,216,63 });
	startAnim.speed = 20.0f;
	startAnim.loop = false;

	loopAnim.PushBack({ 279,139,216,63 });
	loopAnim.PushBack({ 23,139,216,63 });
	loopAnim.PushBack({ 791,75,216,63 });
	loopAnim.PushBack({ 535,75,216,63 });
	
	loopAnim.PushBack({ 279,75,216,63 });

	loopAnim.PushBack({ 535,75,216,63 });
	loopAnim.PushBack({ 791,75,216,63 });
	loopAnim.PushBack({ 23,139,216,63 });
	loopAnim.PushBack({ 279,139,216,63 });
	loopAnim.speed = 10.0f;

	pivotOffset.create(36, 10);

	maxRadiusDistance = 110.0f; // world coords.


}

Crosshair::~Crosshair()
{}

bool Crosshair::Start()
{
	fPoint headingVector = GetHeadingVector(App->entityFactory->player->GetLastPlayerHeadingAngle());
	headingVector = headingVector * maxRadiusDistance;

	position = App->entityFactory->player->GetPivotPos() + headingVector;
	position.x -= pivotOffset.x;
	position.y -= pivotOffset.y;

	return true;
}

bool Crosshair::Update(float dt)
{
	if (isReseted)
	{
		Start();
		isReseted = !isReseted;
	}

	ManageInput(dt);

	// draw animations
	if (!startAnim.Finished())
		App->render->Blit(tex, position.x, position.y, &startAnim.GetCurrentFrame(), 1.0f, SDL_FLIP_NONE, 0.35f);
	else
		App->render->Blit(tex, position.x, position.y, &loopAnim.GetCurrentFrame(), 1.0f, SDL_FLIP_NONE, 0.35f);


	return true;
}

bool Crosshair::ManageInput(float dt)
{
	bool debug = true;

	uint32 maxClampThreshold = 20000;

	Sint16 RJoystickX = App->input->GetControllerAxis(SDL_CONTROLLER_AXIS_RIGHTX);
	Sint16 RJoystickY = App->input->GetControllerAxis(SDL_CONTROLLER_AXIS_RIGHTY);

	int RX = abs(RJoystickX);
	int RY = abs(RJoystickY);

	if (!clamped) // if the crosshair it doesnt clamped, search for possible targets
	{
		//LOG("med: %i", med);
		//LOG("x: %i, y: %i", RX, RY);

		if (RX < maxClampThreshold && RY < maxClampThreshold) // if the player movement exceeds a threshold, not search
		{
			//// relative position to player
			//fPoint relativePos = position  - App->entityFactory->player->GetPivotPos();
			//relativePos.x += pivotOffset.x;
			//relativePos.y += pivotOffset.y;
			//LOG("relapos %f, %f", relativePos.x, relativePos.y);
			// search for enemy on this subtile
			clampedEntity = SearchForTargetOnThisSubtile(GetSubtilePoint());

			if (clampedEntity != nullptr)
				clamped = true;
		}
		else
		{
			//LOG("");
		}
			
			//LOG("DONT SEARCH");
	}
	else
	{
		if (clampedEntity != nullptr && !clampedEntity->to_delete) // for if the entity is killed protection
		{
			position = clampedEntity->GetPivotPos();
			position.x -= pivotOffset.x;
			position.y -= pivotOffset.y;

		}
		else
		{
			clamped = false; // protection
			clampedEntity = nullptr;
		}
	}


	if (RJoystickX > 0 || RJoystickX < 0 || RJoystickY > 0 || RJoystickY < 0)
	{
		if (clamped && (RX > maxClampThreshold || RY > maxClampThreshold)) // if we exceed a threshold, unclamp
		{
			//LOG("unclamp");
			clamped = false;
			clampedEntity = nullptr;
		}

		if (!clamped) // free movement of crosshair
		{
			position.x = position.x + (RJoystickX * 0.003f * sensitivitySpeed.x) * dt;
			position.y = position.y + (RJoystickY * 0.003f * sensitivitySpeed.y) * dt;

			
		}

	}

	// Clamp position to limited radius
	// get distance from player to this point
	iPoint crossPivotPos = GetPivotPos();
	fPoint pppt = App->entityFactory->player->GetPivotPos();
	iPoint playerPivotPos = { (int)pppt.x, (int)pppt.y };
	float distance = crossPivotPos.DistanceTo(playerPivotPos);

	// if we exceed max radius distance, normalize the vector and multiplicate for max radius

	if (distance >= maxRadiusDistance)
	{
		fPoint headingVector;
		headingVector.x = crossPivotPos.x - playerPivotPos.x;
		headingVector.y = crossPivotPos.y - playerPivotPos.y;
		headingVector.Normalize();
		
		position.x = playerPivotPos.x - pivotOffset.x;
		position.y = playerPivotPos.y - pivotOffset.y;

		position.x += headingVector.x * maxRadiusDistance;
		position.y += headingVector.y * maxRadiusDistance;
	}
	

	//LOG("crosshair distance to player: %f", distance);

	// draw pivot position
	if (debug)
	{
		App->render->DrawQuad({ pivotOffset.x + (int)position.x, pivotOffset.y + (int)position.y, 2,2 }, 0, 255, 0, 255, true, true);
	}

	return true;
}

fPoint Crosshair::GetHeadingVector(float angle) // radians
{
	fPoint retVec;
	retVec.create(cos(angle), sin(angle));
	
	LOG("heading vector %f,%f", retVec.x, retVec.y);
	return retVec;
}

iPoint Crosshair::GetSubtilePoint()
{
	// subtile calc
	iPoint subtilePoint = GetPivotPos();
	subtilePoint = App->map->WorldToSubtileMap(subtilePoint.x, subtilePoint.y);

	return subtilePoint;
}

j1Entity* Crosshair::SearchForTargetOnThisSubtile(const iPoint subtile) const
{
	j1Entity* ret = nullptr;
	
	std::vector<j1Entity*>* subtileVec = App->entityFactory->GetSubtileEntityVectorAt(subtile);

	if (subtileVec != NULL)
	{
		std::vector<j1Entity*>::iterator subIter = subtileVec->begin();

		for (; subIter != subtileVec->end(); ++subIter)
		{
			if ((*subIter)->type != ENTITY_TYPE::PLAYER)
			{
				//LOG("enemy found");
				ret = (*subIter);
				break;
			}
		}
	}

	return ret;
}

iPoint Crosshair::GetPivotPos()
{
	iPoint ret;

	ret.create(position.x, position.y);
	ret += pivotOffset;

	return ret;
}

bool Crosshair::PostUpdate()
{
	return true;
}

bool Crosshair::Reset()
{
	startAnim.Reset();
	loopAnim.Reset();
	clamped = false;
	clampedEntity = nullptr;
	isReseted = true;
	return true;
}

bool Crosshair::CleanUp()
{
	if(tex != nullptr)
		App->tex->UnLoad(tex);
	
	return true;
}

//float Crosshair::Clamp(float n, float lower, float upper)
//{
//	return MAX(lower, MIN(n, upper));
//}