#include <algorithm>
#include "PlayerEntityManager.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1BuffManager.h"
#include "j1EntityFactory.h"
#include "j1PathFinding.h"
//buff test
#include "j1Window.h"
#include "j1Scene.h"
#include "UiItem.h"
#include "UiItem_Label.h"
#include "j1ParticlesClassic.h"
#include "LootEntity.h"
#include "Brofiler/Brofiler.h"

PlayerEntityManager::PlayerEntityManager(iPoint position) : j1Entity(PLAYER, position.x,position.y, "PEM")
{
	marche = DBG_NEW Marche(position.x,position.y);
	ritz = DBG_NEW Ritz(position.x, position.y);
	shara = DBG_NEW Shara(position.x, position.y);
	// crosshair - one for all the characters, its should be improved so if we need
	crossHair = DBG_NEW Crosshair();

	// vendor
	vendor = DBG_NEW Vendor();


	characters.push_back(marche);
	characters.push_back(ritz);
	characters.push_back(shara);

	selectedCharacterEntity = marche;

	// debug normal tile tex
	debugTileTex = App->tex->Load("maps/tile_64x64_2.png");
	debugSubtileTex = App->tex->Load("maps/tile_32x32.png");
	player_shadowTex = App->tex->Load("textures/characters/shadow_tile.png");
	to_delete = false;
	debug = false;
	
	level = 1;
	exp = 0;
	maxExpInLevel = 7000;

	App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, marche, " ", 12);
	App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, ritz, " ", 12);
	App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, shara, " ", 12);

	Start();
}

PlayerEntityManager::~PlayerEntityManager()
{
	
	delete crossHair;

	// delete vendor
	delete vendor; 

	// TODO: free characters vector
	App->tex->UnLoad(debugTileTex);
	debugTileTex = nullptr;
	App->tex->UnLoad(debugSubtileTex);
	debugSubtileTex = nullptr;
	App->tex->UnLoad(texture);
	texture = nullptr;
	App->tex->UnLoad(player_shadowTex);
	player_shadowTex = nullptr;
	
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

	//vendor->generateVendorItems();  // at the start the vendor has a certain amout of items

	return true;
}

bool PlayerEntityManager::PreUpdate()
{
	pivot = selectedCharacterEntity->pivot;

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		debug = !debug;

	selectedCharacterEntity->PreUpdate();
	//App->scene->AcceptUISFX_logic = false;

	return true;
}

bool PlayerEntityManager::Update(float dt)
{
	BROFILER_CATEGORY("Update PEM", Profiler::Color::LawnGreen);
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		playerUpdateReady = !playerUpdateReady;

		if (!playerUpdateReady)
			LockPlayerInput();
		else
			UnlockPlayerInput();
	}

	// ALWAYS check ritz last projectile if ritz its not selected
	// check if the player is not Ritz and she has a linked projectile protection
	if (selectedCharacterEntity->character != characterName::RITZ)
	{
		ritz->CheckMyLastProjectile();
	}

	SwapInputChecker(); // checks gamepad "shoulders" triggers input

	if (playerUpdateReady)
	{
		// update character
		selectedCharacterEntity->Update(dt);
	}
	// update selected character position to its "manager" position
	position = selectedCharacterEntity->position;
	lastCharHeadingAngle = selectedCharacterEntity->GetLastHeadingAngle();

	if (selectedCharacterEntity->IsAiming() && selectedCharacterEntity->character != characterName::MARCHE)
	{
		crossHair->Update(dt);
	}
	else if (!crossHair->isReseted)
		crossHair->Reset();

	if (App->input->GetKey(SDL_SCANCODE_KP_3) == KEY_DOWN && level <= 20)
	{
		++level;
		

		if (level < 20)
		{

		/*	iPoint targetLabelPos = App->render->WorldToScreen(App->entityFactory->player->selectedCharacterEntity->GetPosition().x - 75,
				App->entityFactory->player->selectedCharacterEntity->GetPosition().y - 135, true);*/

			uint width, height; 
			width = height = 0; 

			App->win->GetWindowSize(width, height); 

			iPoint targetLabelPos = iPoint((width / 2) - 150, 50);
			App->HPManager->callLevelUpLabelSpawn(targetLabelPos, level);

			GetVendor()->generateVendorItems(true);
			App->gui->healthBar->RecalculateSection();

			App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, App->entityFactory->player, " ", 5);
			App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::HEALTH, App->entityFactory->player, " ", 5);

			std::string dest = "LVL" + std::to_string(level);
			App->scene->exp_label->ChangeTextureIdle(dest, NULL, NULL);

			for (std::list<UiItem*>::iterator item = App->gui->ListItemUI.begin(); item != App->gui->ListItemUI.end(); item++)
			{

				if (level == 2 && (*item)->name == "chain1")
				{
					(*item)->hide = true;
					App->scene->canExecuteChainAnim = true;
					App->particles->AddParticle(App->particles->lvlUpFx, 191, 661, { 0,0 }, 0u, SDL_FLIP_NONE, 0.0, 32, 32, 0.5F, 0.0F, false, true);
				}
				else if (level == 3 && (*item)->name == "chain2")
				{
					(*item)->hide = true;
					App->scene->canExecuteChainAnim = true;
					App->particles->AddParticle(App->particles->lvlUpFx, 253, 661, { 0,0 }, 0u, SDL_FLIP_NONE, 0.0, 32, 32, 0.5F, 0.0F, false, true);
				}
				else if (level == 4 && (*item)->name == "chain3")
				{
					(*item)->hide = true;
					App->scene->canExecuteChainAnim = true;
					App->particles->AddParticle(App->particles->lvlUpFx, 316, 661, { 0,0 }, 0u, SDL_FLIP_NONE, 0.0, 32, 32, 0.5F, 0.0F, false, true);
				}
			}
		}

		else
		{
			std::string dest = "MAXLVL";
			App->scene->exp_label->ChangeTextureIdle(dest, NULL, NULL);
		}
	}

	if (life > maxLife)
		life = maxLife;

	// TODO: crash, rework this
	//collect loot

	if (LastHoveredLootFlag)       // TO PREVENT: when you don't move, an item comes to you, and descr appears, then you move and it stays there
	{
		if (App->entityFactory->isThisSubtileLootFree(GetSubtilePos()) == nullptr)
		{

			if (dynamic_cast<LootEntity*>(lastHoveredLootItem)->spawnedDescription)
			{
				if (lastHoveredLootItem != nullptr)
				{
					PlayerOnTopOfLootToSpawnDescription(false, lastHoveredLootItem);


					lastHoveredLootItem = nullptr;

					if (LastHoveredLootFlag)
						LastHoveredLootFlag = false;

				}
			}


		}

	}


	if (App->entityFactory->isThisSubtileLootFree(GetSubtilePos()) != nullptr)
	{
		if (LastHoveredLootFlag)
		{
			if ((playerHasMovedLastFrame) && dynamic_cast<LootEntity*>(lastHoveredLootItem)->spawnedDescription)
			{
				if (lastHoveredLootItem != nullptr)
				{
					PlayerOnTopOfLootToSpawnDescription(false, lastHoveredLootItem);


					lastHoveredLootItem = nullptr; 

					if(LastHoveredLootFlag)
						LastHoveredLootFlag = false;

				}
			}
		}
	
	}


	//check triggers
	if (App->entityFactory->BoolisThisSubtileTriggerFree(GetSubtilePos()))
	{
		dynamic_cast<Trigger*>(App->entityFactory->isThisSubtileTriggerFree(GetSubtilePos()))->DoTriggerAction();

	}

	//check loot
	if (App->entityFactory->isThisSubtileLootFree(GetSubtilePos()) != nullptr)
	{
			/*lastHoveredLootItem = dynamic_cast<LootEntity*>(App->entityFactory->isThisSubtileLootFree(GetSubtilePos()));
			LastHoveredLootFlag = true;*/

		std::list<LootEntity*> consumableList = App->entityFactory->GetConsumableListFromSubtile(GetSubtilePos());
		if (!consumableList.empty())
		{

			for (std::list<LootEntity*>::iterator itr = consumableList.begin(); itr != consumableList.end(); ++itr)
			{
				if (CollectLoot((*itr)))
				{
					App->entityFactory->DeleteEntityFromSubtile((*itr));

					(*itr)->picked = true;

					App->entityFactory->entities.erase(
						std::remove(App->entityFactory->entities.begin(), App->entityFactory->entities.end(), (*itr)), App->entityFactory->entities.end());
				}
			}

			consumableList.clear();
		}

		/*LootEntity* checkLoot = dynamic_cast<LootEntity*>(App->entityFactory->isThisSubtileLootFree(GetSubtilePos()));

		if (checkLoot->objectType  == OBJECT_TYPE::GOLD || checkLoot->objectType == OBJECT_TYPE::POTIONS || checkLoot->objectType == OBJECT_TYPE::PHOENIX_TAIL)
		{
			if (CollectLoot(checkLoot))
			{
				App->entityFactory->DeleteEntityFromSubtile(checkLoot);

				App->entityFactory->entities.erase(
					std::remove(App->entityFactory->entities.begin(), App->entityFactory->entities.end(), checkLoot), App->entityFactory->entities.end());
			}

		}*/
		else
		{

			if (!playerHasMovedLastFrame)
			{
				lastHoveredLootItem = dynamic_cast<LootEntity*>(App->entityFactory->isThisSubtileLootFree(GetSubtilePos()));
				LastHoveredLootFlag = true;

				if (!lastHoveredLootItem->manualCollectable)
				{

					//TODO: description         dynamic_cast<LootEntity*>(equipable)

					if (lastHoveredLootItem->type == ENTITY_TYPE::LOOT)
					{
						if (!lastHoveredLootItem->spawnedDescription)
							PlayerOnTopOfLootToSpawnDescription(true, lastHoveredLootItem);
					}

					if (App->input->GetControllerGeneralPress(App->input->gamepadScheme.sharedInput.interact) == KEY_DOWN)
					{
						if (!App->entityFactory->player->usedButtonAToPickLootWithCrosshairLastFrame)
						{
							if (lastHoveredLootItem && !dynamic_cast<LootEntity*>(lastHoveredLootItem)->clampedByCrosshair
								&& App->entityFactory->player->GetCrosshair()->GetClampedEntity() == nullptr)
							{
								// at this current stage of dev, we have on this test around 780 entities | 1 day before vertical slice assignment (22/04/19)


								if (App->entityFactory->player->CollectLoot(lastHoveredLootItem, false))  // first collect
								{
									PlayerOnTopOfLootToSpawnDescription(false, lastHoveredLootItem);    // THEN despwn descr and put last hovered to null`tr

									// then delete loot from subtile and factory 
									App->entityFactory->DeleteEntityFromSubtile(lastHoveredLootItem);
									//LOG("entities size: %i", App->entityFactory->entities.size());
									// erase - remove idiom.
									App->entityFactory->entities.erase(
										std::remove(App->entityFactory->entities.begin(), App->entityFactory->entities.end(), lastHoveredLootItem), App->entityFactory->entities.end());

									//last detach clamped entity
									lastHoveredLootItem = nullptr;
									//LOG("entities size: %i", App->entityFactory->entities.size());


									LastHoveredLootFlag = false;
								}
							}
						}
						else
						{
							App->entityFactory->player->usedButtonAToPickLootWithCrosshairLastFrame = false;
						}
					}
				}
			}
		}
		
	}


	if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_DOWN && App->scene->inGamePanel->enable && !App->scene->inventory->enable)
	{
		for (std::vector<LootEntity*>::iterator item = App->entityFactory->player->consumables.begin(); item != consumables.end(); item++)
		{
			if ((*item)->objectType == OBJECT_TYPE::POTIONS)
			{
				App->entityFactory->player->ConsumConsumable(*item, this);
				break;
			}
		}
	}

	if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_DOWN && App->scene->inGamePanel->enable && !App->scene->inventory->enable)
	{
		for (std::vector<LootEntity*>::iterator item = App->entityFactory->player->consumables.begin(); item != consumables.end(); item++)
		{
			if ((*item)->objectType == OBJECT_TYPE::PHOENIX_TAIL)
			{
				App->entityFactory->player->ConsumConsumable(*item, this);
				break;
			}
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
	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		deleteObj = !deleteObj;

	if (deleteObj)
	{
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
	}

	if (marche->stat.size() != 0 && !App->pause)
	{
		if (App->buff->DamageInTime(marche))
		{
			App->buff->entitiesTimeDamage.remove(marche);
		}
	}
	if (ritz->stat.size() != 0 && !App->pause)
	{
		if (App->buff->DamageInTime(ritz))
		{
			App->buff->entitiesTimeDamage.remove(ritz);
		}
	}
	if (shara->stat.size() != 0 && !App->pause )
	{
		if (App->buff->DamageInTime(shara))
		{
			App->buff->entitiesTimeDamage.remove(shara);
		}
	}
	if (stat.size() != 0 && !App->pause)
	{
		if (App->buff->DamageInTime(this))
		{
			App->buff->entitiesTimeDamage.remove(this);
		}
	}


	if (changedTile)
		App->entityFactory->ReleaseAllReservedSubtiles();
	return ret;
}

bool PlayerEntityManager::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate PEM", Profiler::Color::MediumOrchid);
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
//	SetHudAlphaValue();
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

	/*delete crossHair;
	crossHair = nullptr;*/

	App->tex->UnLoad(debugTileTex);
	debugTileTex = nullptr;
	App->tex->UnLoad(debugSubtileTex);
	debugSubtileTex = nullptr;
	App->tex->UnLoad(texture);
	texture = nullptr;

	// vendor objects
	vendor->cleanUp(); 


	return true;
}

void PlayerEntityManager::LockPlayerInput()
{
	// updates all characters to idle anim with pointing dir with the last selected
	for (std::vector<PlayerEntity*>::iterator charsIter = characters.begin(); charsIter != characters.end(); ++charsIter)
	{
		(*charsIter)->currentAnimation = &(*charsIter)->idle[selectedCharacterEntity->pointingDir];
	}

	playerUpdateReady = false;
}

void PlayerEntityManager::UnlockPlayerInput()
{
	playerUpdateReady = true;

	// checks conflict key for back and dash while returns to updates the player on same frame
	if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
	{
		App->input->SetControllerButtonState(SDL_CONTROLLER_BUTTON_B, KEY_UP); // trick
	}
}

bool PlayerEntityManager::Load(pugi::xml_node &node)
{
	
	level = node.child("Experience").attribute("level").as_uint();
	exp = node.child("Experience").attribute("exp").as_uint();

	maxLife = 100;
	life = maxLife - node.child("life").attribute("actualLife").as_float();
	if (life <= 0)
		life = 100;

	App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, this, " ", (level - 1) * 5);
	App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::HEALTH, this, " ", (level - 1) * 5);

	App->gui->healthBar->RecalculateSection(); 
	
	gold = node.child("gold").attribute("value").as_uint();

	std::string dest = "LVL" + std::to_string(App->entityFactory->player->level);
	App->scene->exp_label->ChangeTextureIdle(dest, NULL, NULL);

	std::string str_coin = std::to_string(App->entityFactory->player->gold) + " x";
	App->scene->coins_label->ChangeTextureIdle(str_coin, NULL, NULL);
	App->scene->coins_label_inventory->ChangeTextureIdle(str_coin, NULL, NULL);

	for (pugi::xml_node nodebagObjects = node.child("bagObjects"); nodebagObjects; nodebagObjects = nodebagObjects.next_sibling("bagObjects"))
	{
		LootEntity* bagObj = DBG_NEW LootEntity();
		bagObj->Load(nodebagObjects, bagObj);
		if (bagObj != nullptr)
			bagObjects.push_back(bagObj);
	}

	for (pugi::xml_node nodeeqipedObjects = node.child("equipedObjects"); nodeeqipedObjects; nodeeqipedObjects = nodeeqipedObjects.next_sibling("equipedObjects"))
	{
		LootEntity* equipedObj = DBG_NEW LootEntity();
		equipedObj->Load(nodeeqipedObjects, equipedObj);
		if (equipedObj != nullptr)
		{
			equipedObjects.push_back(equipedObj);
			App->buff->AddItemStats(equipedObj);
		}
	}

	for (pugi::xml_node nodecons = node.child("consumableObjects"); nodecons; nodecons = nodecons.next_sibling("consumableObjects"))
	{
		LootEntity* cons = DBG_NEW LootEntity();
		cons->Load(nodecons, cons);
		if (cons != nullptr)
			consumables.push_back(cons);
	}
	std::string PlayerAux = node.attribute("player").as_string();

	if ( PlayerAux.compare("marche") == 0)
		selectedCharacterEntity = marche;
	else if (PlayerAux.compare("ritz") == 0)
		selectedCharacterEntity = ritz;
	else if (PlayerAux.compare("shara") == 0)
		selectedCharacterEntity = shara;

	return true;
}

bool PlayerEntityManager::Save(pugi::xml_node &node) const
{
	pugi::xml_node nodeexperience = node.append_child("Experience");
	nodeexperience.append_attribute("level") = level;
	nodeexperience.append_attribute("exp") = exp;

	pugi::xml_node nodelife = node.append_child("life");

	nodelife.append_attribute("actualLife") = maxLife - life;

	pugi::xml_node nodegold = node.append_child("gold");
	nodegold.append_attribute("value") = gold;

	std::vector<LootEntity*>::const_iterator iter = bagObjects.begin();
	for (; iter != bagObjects.end(); ++iter)
	{
		pugi::xml_node nodebagObjects = node.append_child("bagObjects");
		(*iter)->Save(nodebagObjects);
	}

	std::vector<LootEntity*>::const_iterator iter2 = equipedObjects.begin();
	for (; iter2 != equipedObjects.end(); ++iter2)
	{
		pugi::xml_node nodeequipedObjects = node.append_child("equipedObjects");
		(*iter2)->Save(nodeequipedObjects);
	}

	
	std::vector<LootEntity*>::const_iterator iter3 = consumables.begin();
	for (; iter3 != consumables.end(); ++iter3)
	{
		pugi::xml_node nodeconsumableObjects = node.append_child("consumableObjects");
		(*iter3)->Save(nodeconsumableObjects);
	}

	if (selectedCharacterEntity == marche)
		node.append_attribute("player") = "marche";
	else if (selectedCharacterEntity == shara)
		node.append_attribute("player") = "shara";
	else if (selectedCharacterEntity == ritz)
		node.append_attribute("player") = "ritz";
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

			if (App->scene->inventory->enable)
			{
				/*if (!App->scene->inventoryItem->isVendorInventory)
				{*/
				App->scene->inventoryItem->callDeleteWhenSwitchingCharacters();  // delete equipped items in ivnentory



				if (App->scene->inventoryItem->isVendorInventory)
				{

					App->scene->inventoryItem->SwapVendorBag(false); 
				}
			//}
			}

			SetPreviousCharacter();

			if (App->scene->inventory->enable)
			{
				if (!App->scene->inventoryItem->isVendorInventory)
				{
					App->scene->inventoryItem->LoadElements(true);   // generate the new ones
					//App->scene->characterStatsItem->InitializeStats();
				}
				else
				{
					App->scene->inventoryItem->LoadElements(true, true);   // generate the new ones


					App->scene->inventoryItem->SwapVendorBag(true);
				}


				App->scene->characterStatsItem->InitializeStats();
			}

		}

		if (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_DOWN || App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_DOWN)
		{
			if (App->scene->inventory->enable)
			{
				/*if (!App->scene->inventoryItem->isVendorInventory)
				{*/
				App->scene->inventoryItem->callDeleteWhenSwitchingCharacters();   // delete equipped items in ivnentory
			//}

				if (App->scene->inventoryItem->isVendorInventory)
				{

					App->scene->inventoryItem->SwapVendorBag(false);
				}
			}

			SetNextCharacter();

			if (App->scene->inventory->enable)
			{
				if (!App->scene->inventoryItem->isVendorInventory)
				{
					App->scene->inventoryItem->LoadElements(true);   // generate the new ones
					//App->scene->characterStatsItem->InitializeStats();
				}
				else
				{
					App->scene->inventoryItem->LoadElements(true, true);   // generate the new ones

					App->scene->inventoryItem->SwapVendorBag(true);
				}

				App->scene->characterStatsItem->InitializeStats();

			}

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
			App->audio->PlayFx(App->scene->swapCharSFX, 0);
			// updates internal state
			//selectedCharacterEntity->Update(App->GetDt());
			break;
		}
	}

	LOG("Selected Character: %c", selectedCharacterEntity->name); // TODO: log crashes with %s, std::string relative
}

void PlayerEntityManager::SetNextCharacter()
{
	float current_frame = 0.f;
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
			App->audio->PlayFx(App->scene->swapCharSFX, 0);
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

bool PlayerEntityManager::CollectLoot(LootEntity * entityLoot, bool fromCrosshair)
{
	bool ret = true;
	if (entityLoot->GetType() == LOOT_TYPE::EQUIPABLE)
	{
		App->audio->PlayFx(App->scene->pickLoot, 0);
		// when a loot item is collected, the description should be hiden
		App->scene->consumableinfo.push_back(App->scene->GetConsumableInfo(entityLoot));
		LOG("consumable size PEM %i", App->scene->consumableinfo.size());

		
		// entityLoot->MyDescription->HideAllElements(true);    // now it is deleted instead

	/*	if (entityLoot->spawnedDescription)                 // only destroy description if it has been spawned( when collecting with the crosshair)
		{
			entityLoot->MyDescription->DeleteEverything();
			entityLoot->MyDescription = nullptr;

			entityLoot->spawnedDescription = false;

		}*/
		
		// - - - - - - - - - - - - - - - - - - - - - - 

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
	if (entityLoot->GetType() == LOOT_TYPE::CONSUMABLE)
	{
		if (!fromCrosshair)
		{
			if (entityLoot->GetObjectType() == OBJECT_TYPE::POTIONS)
			{
				App->audio->PlayFx(App->scene->pickPotion, 0);
				App->scene->consumableinfo.push_back(App->scene->GetConsumableInfo(entityLoot));
				LOG("consumable size PEM %i", App->scene->consumableinfo.size());

				consumables.push_back(entityLoot);
			}

			else if (entityLoot->GetObjectType() == OBJECT_TYPE::PHOENIX_TAIL)
			{
				App->audio->PlayFx(App->scene->pickPotion, 0);
				App->scene->consumableinfo.push_back(App->scene->GetConsumableInfo(entityLoot));
				LOG("consumable size PEM %i", App->scene->consumableinfo.size());

				consumables.push_back(entityLoot);
			}

			else if (entityLoot->GetObjectType() == OBJECT_TYPE::GOLD)
			{
				App->audio->PlayFx(App->scene->pickGold, 0);
				gold += entityLoot->price;
				entityLoot->to_delete = true;
				str_coin = std::to_string(gold) + " x";
				App->scene->coins_label->ChangeTextureIdle(App->entityFactory->player->str_coin, NULL, NULL);
				App->scene->coins_label_inventory->ChangeTextureIdle(App->entityFactory->player->str_coin, NULL, NULL);

				// gold label that pops up

				App->HPManager->callGoldLabelSpawn(App->render->WorldToScreen(this->GetPosition().x, this->GetPosition().y, true), entityLoot->price);

				//App->tex->UnLoad(entityLoot->entityTex);
				return false;
			}
		}
		else ret = false;
	}
	return ret;
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

void PlayerEntityManager::AddItemToTheBag(LootEntity * entityLoot)
{

	if (equipedObjects.size() == 0)
	{
		equipedObjects.push_back(entityLoot);
	}
	else
	{
		for (std::vector<LootEntity*>::iterator item = equipedObjects.begin(); item != equipedObjects.end(); ++item)
		{

			if (entityLoot->GetObjectType() == (*item)->GetObjectType() && entityLoot->character == (*item)->character)
			{
				bagObjects.push_back(*item);
				equipedObjects.erase(item);
				break;
			}
		}
		equipedObjects.push_back(entityLoot);

	}


}

void PlayerEntityManager::RemoveItemFromBag(LootEntity * entityLoot)
{
	for (std::vector<LootEntity*>::iterator item = bagObjects.begin(); item != bagObjects.end(); ++item)
	{
		if (entityLoot == *item)
		{
			//App->buff->RemoveItemStat(*item);      // don't remove stats, because it goes to vendor  (it needs differentiation)
			bagObjects.erase(item);
			break;
		}
	}

}

void PlayerEntityManager::AddItemToConsumables(LootEntity * entityLoot)
{
	consumables.push_back(entityLoot); 

}

void PlayerEntityManager::RemoveItemFromConsumables(LootEntity * entityLoot)
{
	for (std::vector<LootEntity*>::iterator item = consumables.begin(); item != consumables.end(); ++item)
	{
		if (entityLoot == *item)
		{
			//App->buff->RemoveItemStat(*item);      // don't remove stats, because it goes to vendor  (it needs differentiation)
			consumables.erase(item);
			break;
		}
	}
}

void PlayerEntityManager::ConsumConsumable(LootEntity * consumable, j1Entity * entity)
{
	
		for (std::vector<LootEntity*>::iterator item = consumables.begin(); item != consumables.end(); ++item)
		{
			if (consumable == *item && consumable->objectType == OBJECT_TYPE::POTIONS && App->entityFactory->player->life < App->entityFactory->player->maxLife && !App->entityFactory->player->isPotionActive)
			{
				App->audio->PlayFx(App->scene->consumHealPotion, 0);
				for (std::vector<Buff*>::iterator iter = consumable->stats.begin(); iter != consumable->stats.end(); ++iter)
				{
					App->buff->CreateHealth(App->entityFactory->player, (*iter)->GetValue(), 8);

				}
				item = consumables.erase(item);
				break;
			}

			if (consumable == *item && consumable->objectType == OBJECT_TYPE::PHOENIX_TAIL && (App->scene->state == SceneState::LEVEL1 || App->scene->state == SceneState::LEVEL2) && App->entityFactory->waveManager == nullptr)
			{
				//App->audio->PlayFx(consumHealPotion, 0);
				fPoint destination = { cosf(selectedCharacterEntity->lastAxisMovAngle), sinf(selectedCharacterEntity->lastAxisMovAngle) };
				destination.Normalize();

			

				destination = { position.x + destination.x,position.y + destination.y };
				if (!App->pathfinding->IsWalkable(App->map->WorldToMap(destination.x, destination.y)))
					break;

				App->entityFactory->CreateTrigger(TRIGGER_TYPE::PORTAL, destination.x, destination.y, SceneState::LOBBY, White);
				delete * item;
				*item = nullptr;
				consumables.erase(item);
				break;

			}

			//TODO for the coders, make whatever phoenix tail does inside this else if below xD
			/*else if (consumable == *item && consumable->objectType == OBJECT_TYPE::PHOENIX_TAIL)
			{

			}*/
		}
		
	
}

void PlayerEntityManager::SetHudAlphaValue()
{
	float percentlife = 100* App->entityFactory->player->life / maxLife;
	float alphavalue = (255 * ((percentlife - 100) * 0.01));
	alphavalue = sqrt(alphavalue * alphavalue);
	if (percentlife <= 35)
	{
		
		App->render->SetTextureAlpha(App->gui->hurt_hud_tex, alphavalue);
	}
}

void PlayerEntityManager::SetPosition(fPoint pos)
{
	position = pos;

}

void PlayerEntityManager::PlayerOnTopOfLootToSpawnDescription(bool onTop, LootEntity* entity)
{
	if (onTop && !entity->spawnedDescription && !entity->clampedByCrosshair)   // player hover loot item
	{

		// create a new one
		App->entityFactory->GenerateDescriptionForLootItem(entity);
		iPoint offset(-80, -entity->MyDescription->panelWithButton->section.y - 180);
		entity->MyDescription->RepositionAllElements(App->render->WorldToScreen(this->GetPosition().x, this->GetPosition().y, true) + offset);
		entity->MyDescription->HideAllElements(false);

		entity->spawnedDescription = true;
		lastHoveredLootItem = entity;

		lastHoveredLootItem->clampedByPlayerOnTop = true;


		App->entityFactory->DoDescriptionComparison((LootEntity*)(entity));

	}

	// if description is showing, but player goes away

	if (!onTop && entity == lastHoveredLootItem && lastHoveredLootItem->spawnedDescription && lastHoveredLootItem->clampedByPlayerOnTop)
	{
		lastHoveredLootItem->clampedByPlayerOnTop = false;

		if (!lastHoveredLootItem->clampedByCrosshair)
		{

			// delete last descr
			entity->MyDescription->DeleteEverything();
			entity->MyDescription = nullptr;
			entity->spawnedDescription = false;


		}
	
		
		
		
		// lastHoveredLootItem = nullptr;  // CAUTION, do not put here, it will be put to nullpt after it is DELETED form SUBTILE

		//lastHoveredLootItem->clampedByPlayerOnTop = false;
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

	// allowed clamped entities matrix
	allowedClampedTypes[ENTITY_TYPE::BREAKABLE_ASSET] = true;
	allowedClampedTypes[ENTITY_TYPE::ENEMY_ARCHER] = true;
	allowedClampedTypes[ENTITY_TYPE::ENEMY_BOMB] = true;
	allowedClampedTypes[ENTITY_TYPE::ENEMY_ARCHER] = true;
	allowedClampedTypes[ENTITY_TYPE::ENEMY_DUMMY] = true;
	allowedClampedTypes[ENTITY_TYPE::ENEMY_TEST] = true;
	allowedClampedTypes[ENTITY_TYPE::FLOWERBOSS] = true;
	allowedClampedTypes[ENTITY_TYPE::LOOT] = true;

	pivotOffset.create(36, 10);

	subtileCheckRadius[0] = { 0,-1 }; // N
	subtileCheckRadius[1] = { 1,-1 }; // NE
	subtileCheckRadius[2] = { 1,0 }; // E
	subtileCheckRadius[3] = { 1, 1 }; // SE
	subtileCheckRadius[4] = { 0,1 }; // S
	subtileCheckRadius[5] = { -1,1 }; // SW
	subtileCheckRadius[6] = { -1,0 }; // W
	subtileCheckRadius[7] = { -1,-1 }; // NW
	subtileCheckRadius[8] = { 0,0 }; // C

	maxRadiusDistance = 160.f;//110.0f; // world coords.


}

Crosshair::~Crosshair()
{
	if (tex != nullptr)
	{
		App->tex->UnLoad(tex);
		tex = nullptr;
	}
}

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
//	App->scene->AcceptUISFX_logic = false;
	uint32 maxClampThreshold = 24000;

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


			if (clamped && clampedEntity->type == ENTITY_TYPE::LOOT)
			{
				if (!dynamic_cast<LootEntity*>(clampedEntity)->spawnedDescription && !dynamic_cast<LootEntity*>(clampedEntity)->manualCollectable)
				{



					// create a new one
					App->entityFactory->GenerateDescriptionForLootItem((LootEntity*)(clampedEntity));
					iPoint offset(-80, -dynamic_cast<LootEntity*>(clampedEntity)->MyDescription->panelWithButton->section.y - 180);
					dynamic_cast<LootEntity*>(clampedEntity)->MyDescription->RepositionAllElements(App->render->WorldToScreen(dynamic_cast<LootEntity*>(clampedEntity)->GetPosition().x, dynamic_cast<LootEntity*>(clampedEntity)->GetPosition().y, true) + offset);
					dynamic_cast<LootEntity*>(clampedEntity)->MyDescription->HideAllElements(false);

					dynamic_cast<LootEntity*>(clampedEntity)->spawnedDescription = true;
					dynamic_cast<LootEntity*>(clampedEntity)->clampedByCrosshair = true;
				}

			

			}
		}
		else
		{
			//LOG("");
		}

			//LOG("DONT SEARCH");
	}
	else

	{

    //TODO: UNCLAMP when player pull the crosshair out a target
		if (clampedEntity != nullptr && !clampedEntity->to_delete) // for if the entity is killed protection
		{
			position = clampedEntity->GetPivotPos();
			position.x -= pivotOffset.x;
			position.y -= pivotOffset.y;


			// if clamped type is loot, it can be picked 

			if (clampedEntity->type == ENTITY_TYPE::LOOT)  // TODO:  add condition so that potions do not enter this 
			{
				
				App->entityFactory->DoDescriptionComparison((LootEntity*)(clampedEntity));  // compare item with the current one
				if (App->input->GetControllerGeneralPress(App->input->gamepadScheme.sharedInput.interact) == KEY_DOWN)
				{
					// at this current stage of dev, we have on this test around 780 entities | 1 day before vertical slice assignment (22/04/19)
					
					if (dynamic_cast<LootEntity*>(clampedEntity)->clampedByCrosshair)
					{
						if (App->entityFactory->player->CollectLoot((LootEntity*)(clampedEntity), true))
						{


							if (dynamic_cast<LootEntity*>(clampedEntity)->spawnedDescription)
							{
								dynamic_cast<LootEntity*>(clampedEntity)->clampedByCrosshair = false;

								if (!dynamic_cast<LootEntity*>(clampedEntity)->clampedByPlayerOnTop)
								{
									// delete last descr

									dynamic_cast<LootEntity*>(clampedEntity)->MyDescription->DeleteEverything();
									dynamic_cast<LootEntity*>(clampedEntity)->MyDescription = nullptr;

									dynamic_cast<LootEntity*>(clampedEntity)->spawnedDescription = false;
								}
							
							
							}


							// then delete loot from subtile and factory 
							App->entityFactory->DeleteEntityFromSubtile((j1Entity*)clampedEntity);
							//LOG("entities size: %i", App->entityFactory->entities.size());
							// erase - remove idiom.
							App->entityFactory->entities.erase(
								std::remove(App->entityFactory->entities.begin(), App->entityFactory->entities.end(), clampedEntity), App->entityFactory->entities.end());

							//last detach clamped entity
							clampedEntity = nullptr;
							clamped = false;
							//LOG("entities size: %i", App->entityFactory->entities.size());

							App->entityFactory->player->usedButtonAToPickLootWithCrosshairLastFrame = true; 
						}
					}
			
					
				}
			}
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



			if (clampedEntity->type == ENTITY_TYPE::LOOT && dynamic_cast<LootEntity*>(clampedEntity)->clampedByCrosshair)
			{
				if (dynamic_cast<LootEntity*>(clampedEntity)->spawnedDescription && !dynamic_cast<LootEntity*>(clampedEntity)->clampedByPlayerOnTop)
				{

					// delete last descr
					dynamic_cast<LootEntity*>(clampedEntity)->MyDescription->DeleteEverything();
					dynamic_cast<LootEntity*>(clampedEntity)->MyDescription = nullptr;

					dynamic_cast<LootEntity*>(clampedEntity)->spawnedDescription = false;
					dynamic_cast<LootEntity*>(clampedEntity)->clampedByCrosshair = false;
				}
			}



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

		// and if we have any clamped entity, unclamp
		if (clamped) 
		{

			if (clampedEntity->type == ENTITY_TYPE::LOOT && dynamic_cast<LootEntity*>(clampedEntity)->clampedByCrosshair)
			{
				if (dynamic_cast<LootEntity*>(clampedEntity)->spawnedDescription && !dynamic_cast<LootEntity*>(clampedEntity)->clampedByPlayerOnTop)
				{

					// delete last descr
					dynamic_cast<LootEntity*>(clampedEntity)->MyDescription->DeleteEverything();
					dynamic_cast<LootEntity*>(clampedEntity)->MyDescription = nullptr;

					dynamic_cast<LootEntity*>(clampedEntity)->spawnedDescription = false;
					dynamic_cast<LootEntity*>(clampedEntity)->clampedByCrosshair = false;
				}
			}



			clamped = false;
			clampedEntity = nullptr;
		}
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

	for (int i = 0; i < 9; ++i)
	{
		std::vector<j1Entity*>* subtileVec = App->entityFactory->GetSubtileEntityVectorAt(subtile + subtileCheckRadius[i]);

		if (subtileVec != NULL)
		{
			std::vector<j1Entity*>::iterator subIter = subtileVec->begin();

			for (; subIter != subtileVec->end(); ++subIter)
			{
				if (allowedClampedTypes[(*subIter)->type])
				{
					if ((*subIter)->type == ENTITY_TYPE::LOOT)
					{
						if (dynamic_cast<LootEntity*>(*subIter)->objectType == OBJECT_TYPE::GOLD ||
							dynamic_cast<LootEntity*>(*subIter)->objectType == OBJECT_TYPE::PHOENIX_TAIL ||
							dynamic_cast<LootEntity*>(*subIter)->objectType == OBJECT_TYPE::POTIONS)
						{
							continue;
						}
					}
		
					ret = (*subIter);
					break;
				}
			}
		}

		if (ret != nullptr)
			break;
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
	if (clamped && clampedEntity->type == ENTITY_TYPE::LOOT && dynamic_cast<LootEntity*>(clampedEntity)->clampedByCrosshair)
	{
		if (dynamic_cast<LootEntity*>(clampedEntity)->spawnedDescription && !dynamic_cast<LootEntity*>(clampedEntity)->clampedByPlayerOnTop)
		{

			// delete last descr
			dynamic_cast<LootEntity*>(clampedEntity)->MyDescription->DeleteEverything();
			dynamic_cast<LootEntity*>(clampedEntity)->MyDescription = nullptr;

			dynamic_cast<LootEntity*>(clampedEntity)->spawnedDescription = false;
			dynamic_cast<LootEntity*>(clampedEntity)->clampedByCrosshair = false;
		}
	}


	startAnim.Reset();
	loopAnim.Reset();
	clamped = false;
	clampedEntity = nullptr;
	isReseted = true;
	return true;
}

bool Crosshair::CleanUp()
{
	if (tex != nullptr)
	{
		App->tex->UnLoad(tex);
		tex = nullptr;
	}
	
	return true;
}

//float Crosshair::Clamp(float n, float lower, float upper)
//{
//	return MAX(lower, MIN(n, upper));
//}
j1Entity* Crosshair::GetClampedEntity() const
{
	j1Entity* ret = nullptr;

	if (clampedEntity != nullptr)
	{
		if (!clampedEntity->to_delete)
			ret = clampedEntity;
	}

	return ret;
}