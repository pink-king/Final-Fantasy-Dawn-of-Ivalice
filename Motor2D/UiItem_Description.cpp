#include "UiItem_Description.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1Window.h"
#include "UiItem_Inventory.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "PlayerEntityManager.h"
#include "j1EntityFactory.h"
#include "LootEntity.h"
#include "j1DialogSystem.h"

UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Value, EquipmentStatType variableType, uint level, LootEntity* callback, UiItem* const parent, uint price) : UiItem(position, parent)
{
	this->callback = callback;

	this->resistance = Value;
	this->HP = variableType.HP;
	this->velocity = variableType.velocity;

	this->descrType = descriptionType::EQUIPMENT;
	this->parent = parent;

	this->guiType = GUI_TYPES::DESCRIPTION;

	// common 
	panelWithButton = App->gui->AddImage(iPoint(0, 0), panelRect, this);
	panelWithButton->useCamera = false;

	if (itemName.empty())
	{
		itemName = "Item has no name";
	}

	name = App->gui->AddLabel(itemName, { 255, 69, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	name->useCamera = false;


	std::string lvlString("LVL ");
	lvlString.append(std::to_string((int)level));
	this->level = App->gui->AddLabel(lvlString, { 255, 255, 255, 255 }, App->font->openSansBold18, iPoint(0, 0), this);

	this->level->useCamera = false;

	// - - - - - - - - - - - - - - - - - - 


	std::string resString("DEF: ");
	resString.append(std::to_string((int)Value));
	resistanceLabel = App->gui->AddLabel(resString, { 255, 255, 255, 255 }, App->font->openSansBold18, iPoint(0, 0), this);

	resistanceLabel->useCamera = false;


	// hp, velocity or both depending on type; 

	this->equipmentLootInfo = variableType;

	std::string HPString("HP: ");
	HPString.append(std::to_string((int)variableType.HP));


	HPLabel = App->gui->AddLabel(HPString, { 255, 255, 255, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	HPLabel->useCamera = false;


	std::string VelocityString("VEL: ");
	VelocityString.append(std::to_string((int)variableType.velocity));

	VelocityLabel = App->gui->AddLabel(VelocityString, { 255, 255, 255, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	VelocityLabel->useCamera = false;





	// the icon image is created after creating description in loot spawning



	if (callback->equipableType == EQUIPABLE_TYPE::ARMOR)
	{
		attachedCharacterString = "Marche";

		resistanceComparisonLabel.character = "Marche";
		resistanceComparisonLabel.type = "armor";

		HPComparisonLabel.character = "Marche";
		HPComparisonLabel.type = "armor";

		velocityComparisonLabel.character = "Marche";
		velocityComparisonLabel.type = "armor";

	}
	else if (callback->equipableType == EQUIPABLE_TYPE::MANTLE)
	{
		attachedCharacterString = "Ritz";

		resistanceComparisonLabel.character = "Ritz";
		resistanceComparisonLabel.type = "mantle";

		HPComparisonLabel.character = "Ritz";
		HPComparisonLabel.type = "mantle";

		velocityComparisonLabel.character = "Ritz";
		velocityComparisonLabel.type = "mantle";
	}
	else if (callback->equipableType == EQUIPABLE_TYPE::VEST)
	{
		attachedCharacterString = "Shara";

		resistanceComparisonLabel.character = "Shara";
		resistanceComparisonLabel.type = "vest";

		HPComparisonLabel.character = "Shara";
		HPComparisonLabel.type = "vest";

		velocityComparisonLabel.character = "Shara";
		velocityComparisonLabel.type = "vest";
	}

	this->attachedCharacter = App->gui->AddLabel(attachedCharacterString, { 200, 200, 200, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	attachedCharacter->useCamera = false;


	// the label with the "+6" etc

	resistanceComparisonLabel.text = " ";
	resistanceComparisonLabel.label = App->gui->AddLabel(" ", { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	resistanceComparisonLabel.label->useCamera = false;


	HPComparisonLabel.text = " ";
	HPComparisonLabel.label = App->gui->AddLabel(" ", { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	HPComparisonLabel.label->useCamera = false;


	velocityComparisonLabel.text = " ";
	velocityComparisonLabel.label = App->gui->AddLabel(" ", { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	velocityComparisonLabel.label->useCamera = false;





	// price

	if (price > 0)
	{
		std::string PriceString("Price: ");
		PriceString.append(std::to_string((int)price));

		this->price = App->gui->AddLabel(PriceString, { 255, 222, 54, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
		this->price->hide = true;
	}
}


UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Attack, float resistance, float cooldown, uint level, LootEntity* callback, UiItem* const parent, uint price) : UiItem(position, parent)
{
	this->callback = callback;

	this->attack = Attack;
	this->resistance = resistance;   // needed for comparison labels
	this->cooldown = cooldown;

	this->descrType = descriptionType::WEAPON;
	this->parent = parent;
	this->guiType = GUI_TYPES::DESCRIPTION;

	// common 
	panelWithButton = App->gui->AddImage(iPoint(0, 0), panelRect, this);
	panelWithButton->useCamera = false;

	if (itemName.empty())
	{
		itemName = "Item has no name";
	}

	name = App->gui->AddLabel(itemName, { 255, 69, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	name->useCamera = false;


	std::string lvlString("LVL ");
	lvlString.append(std::to_string((int)level));
	this->level = App->gui->AddLabel(lvlString, { 255, 255, 255, 255 }, App->font->openSansBold18, iPoint(0, 0), this);

	this->level->useCamera = false;

	// - - - - - - - - - - - - - - - - - - 
	std::string dmgString("DMG: ");
	dmgString.append(std::to_string((int)Attack));
	damageLabel = App->gui->AddLabel(dmgString, { 255, 255, 255, 255 }, App->font->openSansBold18, iPoint(0, 0), this);

	damageLabel->useCamera = false;


	std::string resString("RES: ");
	resString.append(std::to_string((int)resistance));
	resistanceLabel = App->gui->AddLabel(resString, { 255, 255, 255, 255 }, App->font->openSansBold18, iPoint(0, 0), this);

	resistanceLabel->useCamera = false;


	std::string coolString("COOL: ");
	coolString.append(std::to_string((int)cooldown));
	cooldownLabel = App->gui->AddLabel(coolString, { 255, 255, 255, 255 }, App->font->openSansBold18, iPoint(0, 0), this);

	cooldownLabel->useCamera = false;

	// the icon image is created after creating description in loot spawning



	if (callback->equipableType == EQUIPABLE_TYPE::SWORD)
	{
		attachedCharacterString = "Marche";

		damageComparisonLabel.character = "Marche";
		damageComparisonLabel.type = "sword";

		resistanceComparisonLabel.character = "Marche";
		resistanceComparisonLabel.type = "sword";

		cooldownComparisonLabel.character = "Marche";
		cooldownComparisonLabel.type = "sword";
	}
	else if (callback->equipableType == EQUIPABLE_TYPE::ROD)
	{
		attachedCharacterString = "Ritz";

		damageComparisonLabel.character = "Ritz";
		damageComparisonLabel.type = "rod";

		resistanceComparisonLabel.character = "Ritz";
		resistanceComparisonLabel.type = "rod";

		cooldownComparisonLabel.character = "Ritz";
		cooldownComparisonLabel.type = "rod";
	}
	else if (callback->equipableType == EQUIPABLE_TYPE::BOW)
	{
		attachedCharacterString = "Shara";

		damageComparisonLabel.character = "Shara";
		damageComparisonLabel.type = "bow";

		resistanceComparisonLabel.character = "Shara";
		resistanceComparisonLabel.type = "bow";

		cooldownComparisonLabel.character = "Shara";
		cooldownComparisonLabel.type = "bow";
	}

	this->attachedCharacter = App->gui->AddLabel(attachedCharacterString, { 200, 200, 200, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	attachedCharacter->useCamera = false;


	// the label with the "+6" etc

	damageComparisonLabel.text = " ";
	damageComparisonLabel.label = App->gui->AddLabel(" ", { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	damageComparisonLabel.label->useCamera = false;


	resistanceComparisonLabel.text = " ";
	resistanceComparisonLabel.label = App->gui->AddLabel(" ", { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	resistanceComparisonLabel.label->useCamera = false;


	cooldownComparisonLabel.text = " ";
	cooldownComparisonLabel.label = App->gui->AddLabel(" ", { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	cooldownComparisonLabel.label->useCamera = false;



	if (price > 0)
	{
		std::string PriceString("Price: ");
		PriceString.append(std::to_string((int)price));

		this->price = App->gui->AddLabel(PriceString, { 255, 222, 54, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
		this->price->hide = true;
	}

}


UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, std::string effect, iPoint HPandTime, LootEntity* callback, UiItem* const parent, uint price) : UiItem(position, parent)
{
	this->callback = callback;
	this->descrType = descriptionType::POTION;
	this->parent = parent;
	this->guiType = GUI_TYPES::DESCRIPTION;




	if (callback->objectType == OBJECT_TYPE::POTIONS)
	{
		this->name_object = "potion_1";
	}

	//Descomment when fenix tail is created
	if (callback->objectType == OBJECT_TYPE::PHOENIX_TAIL)
	{
		this->name_object = "fenix_tail";
	}


	// common 
	panelWithButton = App->gui->AddImage(iPoint(0, 0), panelRect, this);
	panelWithButton->useCamera = false;

	if (itemName.empty())
	{
		itemName = "Item has no name";
	}

	name = App->gui->AddLabel(itemName, { 255, 69, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	name->useCamera = false;

	// TODO: effect: 


	if (callback->objectType != OBJECT_TYPE::PHOENIX_TAIL)
	{
		std::string hpString("Cures ");
		hpString.append(std::to_string((int)HPandTime.x));
		hpString.append(" HP in ");
		hpString.append(std::to_string((int)HPandTime.y));
		hpString.append(" sec");

		effectLabel = App->gui->AddLabel(hpString, { 255, 255, 255, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
		effectLabel->useCamera = false;

	}
	else if (callback->objectType == OBJECT_TYPE::PHOENIX_TAIL)
	{
		std::string hpString("Go to hall / level");
		effectLabel = App->gui->AddLabel(hpString, { 255, 255, 255, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
		effectLabel->useCamera = false;

	}



	// the icon image is created after creating description in loot spawning


	if (price > 0)
	{
		std::string PriceString("Price: ");
		PriceString.append(std::to_string((int)price));

		this->price = App->gui->AddLabel(PriceString, { 255, 222, 54, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
		this->price->hide = true;
	}
}

UiItem_Description::~UiItem_Description()
{

}



void UiItem_Description::Draw(const float& dt)
{
	// generate description if the wasn't one 

	if (!App->scene->inventory->enable && spawnedInventoryImage)
	{
		// if inventory is no longer enabled, delete description
		App->scene->inventoryItem->De_______GenerateDescription(this->callback, false);
		showedPrice = false;
	}

	if (spawnedInventoryImage)
	{

		if (!switchedCameraUsage && App->scene->inventory->enable)
		{
			SwitchCameraUsage();           // when ingame, descr blit speed is 1.0f, but in invetory it should be 0.0f

			switchedCameraUsage = true;
		}

		if (App->scene->inventory->enable)             // shpw description
		{
			if (!showedPrice)
			{
				if (App->scene->inventoryItem->isVendorInventory)
				{
					std::string PriceString("Price: ");
					PriceString.append(std::to_string((int)callback->vendorPrice));
					this->price->ChangeTextureIdle(PriceString, NULL, NULL);
				}
				else
				{
					std::string PriceString("Price: ");
					PriceString.append(std::to_string((int)callback->price));
					this->price->ChangeTextureIdle(PriceString, NULL, NULL);
				}
				showedPrice = true;
			}

			if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN)
			{
				if (!tabOnConsumable)
				{
					bool isAnyItemRemaining = false;

					std::list<UiItem*>::iterator item = App->gui->ListItemUI.begin();
					for (; item != App->gui->ListItemUI.end(); ++item)
					{
						if ((*item)->tabbable && (*item)->parent->enable && !(*item)->hide && (*item)->hitBox.x == 863 && (*item)->hitBox.y == 381)
						{
							App->gui->selected_object = (*item);
							foundPoti = true;
							isAnyItemRemaining = true;
							break;
						}
						else
							foundPoti = false;


					}
					if (!foundPoti)
					{

						for (std::list<UiItem*>::iterator item = App->gui->ListItemUI.begin(); item != App->gui->ListItemUI.end(); ++item)
						{
							if ((*item)->tabbable && (*item)->parent->enable && !(*item)->hide && (*item)->hitBox.x == 863 && (*item)->hitBox.y == 457)
							{
								App->gui->selected_object = (*item);
								isAnyItemRemaining = true;
							}
						}
					}
					if (isAnyItemRemaining)
					{
						App->scene->tab_inventory->hitBox.x = App->gui->selected_object->hitBox.x - tabOffsetPotion.x;
						App->scene->tab_inventory->hitBox.y = App->gui->selected_object->hitBox.y - tabOffsetPotion.y;
						tabOnConsumable = true;
					}

				}
				else
				{
					App->gui->resetHoverSwapping = false;
					tabOnConsumable = false;
				}



			}
			if (App->gui->selected_object == iconImageInventory)
			{



				if (this->name_object == "potion_1")
				{
					App->scene->tab_inventory->hitBox.x = App->gui->selected_object->hitBox.x - tabOffsetPotion.x;
					App->scene->tab_inventory->hitBox.y = App->gui->selected_object->hitBox.y - tabOffsetPotion.y;
				}

				else if (this->name_object == "fenix_tail")
				{
					App->scene->tab_inventory->hitBox.x = App->gui->selected_object->hitBox.x - tabOffsetPotion.x;
					App->scene->tab_inventory->hitBox.y = App->gui->selected_object->hitBox.y - tabOffsetPotion.y;
				}

				else
				{
					App->scene->tab_inventory->hitBox.x = App->gui->selected_object->hitBox.x - tabOffset.x;
					App->scene->tab_inventory->hitBox.y = App->gui->selected_object->hitBox.y - tabOffset.y;
				}


				HideAllElements(false);

				RepositionAllElements(iPoint(staringPosition.x + 545, staringPosition.y + 20));

				if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN) // || App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
				{
					if (myLootItemIsEquipped.state == INACTIVE)                                  // only call (de)equip if the item is not already active
					{
						// get stats before equipping, so that they can be added 

					
						App->scene->inventoryItem->De_______Equip(this->callback);

					
					}

				}
				/*if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_DOWN || App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_DOWN)
				{
					App->gui->resetHoverSwapping = false;
					App->scene->inventoryItem->LoadElements();    // ALERT ALERT: already done in player manager when switching chars
				}*/

				if (!hasToCompare)
				{
					/*if (myLootItemIsEquipped.state == ACTIVE)
					{
						HideAllComparisonLabels();   // hide equipped objects comparison
					}*/
				}
				else
				{
					if (myLootItemIsEquipped.state == INACTIVE)
					{
						ChangeComparisonLabels();    // "+3 dmg", "+4def ect
					}

				}

				/*if (hasToCompare)
				{
					ChangeComparisonLabels();    // "+3 dmg", "+4def ect
				}
				else
				{
					if (myLootItemIsEquipped.state == ACTIVE)
					{
						HideAllComparisonLabels();   // hide equipped objects comparison
					}

				}*/



				// MORE PROTECTION TO NOT COMPARE WHEN THE ITEM DOES NOT BELONG TO CURRENT CHARACTER

				if (App->entityFactory->player->selectedCharacterEntity != this->callback->character
					|| myLootItemIsEquipped.state == ACTIVE)
				{
					HideAllComparisonLabels();
				}


			}
			else
			{
				HideAllElements(true);

				hasToCompare = true;  // reset comparison label
			}


		}



	}



}

void UiItem_Description::HideAllComparisonLabels()
{


	switch (this->callback->objectType)
	{
	case OBJECT_TYPE::WEAPON_OBJECT:

		this->damageComparisonLabel.label->hide = true;

		// TODO: cooldown and defense
		this->cooldownComparisonLabel.label->hide = true;

		this->resistanceComparisonLabel.label->hide = true;

		break;

	case OBJECT_TYPE::ARMOR_OBJECT:

		this->resistanceComparisonLabel.label->hide = true;

		// TODO: hp and velocity comparisons
		this->HPComparisonLabel.label->hide = true;

		this->velocityComparisonLabel.label->hide = true;


		break;
	}

}

bool UiItem_Description::ChangeComparisonLabels()
{

	LOG(" --------------------------------------------    Compared items");

	bool ret = false;

	std::array<int, 5> characterStatsMapping = {};
	std::array<int, 5> characterStatsValues = {};


	if (!App->entityFactory->player->equipedObjects.empty())
	{
		std::vector<LootEntity*>::iterator lootItem = App->entityFactory->player->equipedObjects.begin();

		for (; lootItem != App->entityFactory->player->equipedObjects.end(); ++lootItem)
		{
			if ((*lootItem) != this->callback)  // do not compare with itself
			{

				float attack = 0.0f;
				float resistance = 0.0f;
				float cooldown = 0.0f;

				float HP = 0.0f;
				float velocity = 0.0f;


				SDL_Color destColor = { 0, 0, 0, 255 };

				if (((*lootItem)->GetObjectType() == OBJECT_TYPE::WEAPON_OBJECT && this->descrType == descriptionType::WEAPON)
					||
					((*lootItem)->GetObjectType() == OBJECT_TYPE::HEAD_OBJECT && this->descrType == descriptionType::EQUIPMENT)
					||
					((*lootItem)->GetObjectType() == OBJECT_TYPE::ARMOR_OBJECT && this->descrType == descriptionType::EQUIPMENT)
					)
				{

					if ((App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetMarche()   // right now, only resistance comparion label is checked
						&& this->attachedCharacterString == "Marche"
						&& ((*lootItem)->character == App->entityFactory->player->GetMarche()))


						|| (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetRitz()
							&& this->attachedCharacterString == "Ritz"
							&& ((*lootItem)->character == App->entityFactory->player->GetRitz()))

						|| (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetShara()
							&& this->attachedCharacterString == "Shara"
							&& ((*lootItem)->character == App->entityFactory->player->GetShara())))

					{

						std::vector<Buff*>::iterator iter = (*lootItem)->stats.begin();

						if (this->descrType == descriptionType::WEAPON)
						{
							for (; iter != (*lootItem)->stats.end(); ++iter)    // capture att and def 
							{
								if ((*iter)->GetRol() == ROL::ATTACK_ROL)
								{
									attack = (*iter)->GetValue();

									characterStatsMapping.at(0) = 1; 
									characterStatsValues.at(0) = attack; 
								}
								else if ((*iter)->GetRol() == ROL::DEFENCE_ROL)
								{
									resistance = (*iter)->GetValue();

									characterStatsMapping.at(1) = 1;
									characterStatsValues.at(1) = resistance;
								}
								else if ((*iter)->GetRol() == ROL::COOLDOWN)
								{
									cooldown = (*iter)->GetValue();

									characterStatsMapping.at(2) = 1;
									characterStatsValues.at(2) = cooldown;
								}

							}

							// - - - - - - - - - - - - - -  DAMAGE

							this->damageComparisonLabel.value = (int)(this->attack - attack);

							std::string dmgString = "";


							if (this->damageComparisonLabel.value > 0)
							{
								dmgString.append("+");
								destColor = { 0, 255, 0, 255 };
							}
							else if (this->damageComparisonLabel.value <= 0)
							{
								if (this->damageComparisonLabel.value == 0)
								{
									dmgString.append("+");
								}
								destColor = { 255, 0, 0, 255 };
							}

							dmgString.append(std::to_string((int)this->damageComparisonLabel.value));

							this->damageComparisonLabel.label->ChangeTextureIdle(dmgString, &destColor, App->font->openSansBold18);


							// - - - - - - - - - - - - - -  COOLDOWN

							this->cooldownComparisonLabel.value = (int)(this->cooldown - cooldown);

							std::string coolString = "";


							if (this->cooldownComparisonLabel.value > 0)
							{
								coolString.append("+");
								destColor = { 0, 255, 0, 255 };
							}
							else if (this->cooldownComparisonLabel.value <= 0)
							{
								if (this->cooldownComparisonLabel.value == 0)
								{
									coolString.append("+");
								}
								destColor = { 255, 0, 0, 255 };
							}

							coolString.append(std::to_string((int)this->cooldownComparisonLabel.value));

							this->cooldownComparisonLabel.label->ChangeTextureIdle(coolString, &destColor, App->font->openSansBold18);






						}
						else
						{
							for (; iter != (*lootItem)->stats.end(); ++iter)   // capture def and other 2 possible rols
							{
								if ((*iter)->GetRol() == ROL::DEFENCE_ROL)
								{
									resistance = (*iter)->GetValue();

									characterStatsMapping.at(1) = 1;
									characterStatsValues.at(1) = resistance;
								}
								else if ((*iter)->GetRol() == ROL::HEALTH)
								{
									HP = (*iter)->GetValue();

									characterStatsMapping.at(3) = 1;
									characterStatsValues.at(3) = HP;
								}
								else if ((*iter)->GetRol() == ROL::VELOCITY)
								{
									velocity = (*iter)->GetValue();


									characterStatsMapping.at(4) = 1;
									characterStatsValues.at(4) = velocity;
								}

							}

							// TODO: compare health and velocity


							// - - - - - - - - - - - - - -  HP

							this->HPComparisonLabel.value = (int)(this->HP - HP);

							std::string hpString = "";


							if (this->HPComparisonLabel.value > 0)
							{
								hpString.append("+");
								destColor = { 0, 255, 0, 255 };
							}
							else if (this->HPComparisonLabel.value <= 0)
							{
								if (this->HPComparisonLabel.value == 0)
								{
									hpString.append("+");
								}
								destColor = { 255, 0, 0, 255 };
							}

							hpString.append(std::to_string((int)this->HPComparisonLabel.value));

							this->HPComparisonLabel.label->ChangeTextureIdle(hpString, &destColor, App->font->openSansBold18);


							// - - - - - - - - - - - - - -  VELOCITY

							this->velocityComparisonLabel.value = (int)(this->velocity - velocity);

							std::string velString = "";


							if (this->velocityComparisonLabel.value > 0)
							{
								velString.append("+");
								destColor = { 0, 255, 0, 255 };
							}
							else if (this->velocityComparisonLabel.value <= 0)
							{
								if (this->velocityComparisonLabel.value == 0)
								{
									velString.append("+");
								}
								destColor = { 255, 0, 0, 255 };
							}

							velString.append(std::to_string((int)this->velocityComparisonLabel.value));

							this->velocityComparisonLabel.label->ChangeTextureIdle(velString, &destColor, App->font->openSansBold18);



						}

						// they both have resistance 

						this->resistanceComparisonLabel.value = (int)(this->resistance - resistance);    // diff between new item and current

						std::string resString = "";


						destColor = { 0, 0, 0, 255 };
						if (this->resistanceComparisonLabel.value > 0)
						{
							resString.append("+");
							destColor = { 0, 255, 0, 255 };
						}
						else if (this->resistanceComparisonLabel.value <= 0)
						{
							if (this->resistanceComparisonLabel.value == 0)
							{
								resString.append("+");
							}
							destColor = { 255, 0, 0, 255 };
						}

						resString.append((std::to_string((int)this->resistanceComparisonLabel.value)));

						this->resistanceComparisonLabel.label->ChangeTextureIdle(resString, &destColor, App->font->openSansBold18);


						ret = true;



						/*if (App->scene->inventoryItem->enable)
							App->scene->characterStatsItem->CompareStats(characterStatsMapping, characterStatsValues);*/

					}
					else
					{

				/*	if (App->scene->inventoryItem->enable)
						App->scene->characterStatsItem->HideAllComparisonStats(); */
                    }

				}



			}

		}

	}
	else     // when no items are equipped but stats change !!!!
	{
	
		
    
    }



	if (App->gui->selected_object == this->iconImageInventory)
	{

		if ((App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetMarche()   // right now, only resistance comparion label is checked
			&& this->attachedCharacterString == "Marche")


			|| (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetRitz()
				&& this->attachedCharacterString == "Ritz")

			|| (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetShara()
				&& this->attachedCharacterString == "Shara"))

		{
		//	App->scene->characterStatsItem->HideAllComparisonStats();
			getItemBuffsAndCallStatComparison(this->callback);                 // CAUTIOn: if item is selected again, prevent calculating again (xd)

		}
		else
		{
			App->scene->characterStatsItem->HideAllComparisonStats();
		}
	}

	

	hasToCompare = false;


	return ret;

}



void UiItem_Description::getItemBuffsAndCallStatComparison(LootEntity* ent)
{
	std::array<int, 5> characterStatsMapping = {};
	std::array<int, 5> characterStatsValues = {};

	float attack = 0.0f;
	float resistance = 0.0f;
	float cooldown = 0.0f;

	float HP = 0.0f;
	float velocity = 0.0f;

	std::vector<Buff*>::iterator iter = ent->stats.begin();

	if (this->descrType == descriptionType::WEAPON)
	{
		for (; iter != ent->stats.end(); ++iter)    // capture att and def 
		{
			if ((*iter)->GetRol() == ROL::ATTACK_ROL)
			{
				attack = (*iter)->GetValue();

				characterStatsMapping.at(0) = 1;
				characterStatsValues.at(0) = attack;
			}
			else if ((*iter)->GetRol() == ROL::DEFENCE_ROL)
			{
				resistance = (*iter)->GetValue();

				characterStatsMapping.at(1) = 1;
				characterStatsValues.at(1) = resistance;
			}
			else if ((*iter)->GetRol() == ROL::COOLDOWN)
			{
				cooldown = (*iter)->GetValue();

				characterStatsMapping.at(2) = 1;
				characterStatsValues.at(2) = cooldown;
			}

		}

	}
	else
	{
		for (; iter != ent->stats.end(); ++iter)   // capture def and other 2 possible rols
		{
			if ((*iter)->GetRol() == ROL::DEFENCE_ROL)
			{
				resistance = (*iter)->GetValue();

				characterStatsMapping.at(1) = 1;
				characterStatsValues.at(1) = resistance;
			}
			else if ((*iter)->GetRol() == ROL::HEALTH)
			{
				HP = (*iter)->GetValue();

				characterStatsMapping.at(3) = 1;
				characterStatsValues.at(3) = HP;
			}
			else if ((*iter)->GetRol() == ROL::VELOCITY)
			{
				velocity = (*iter)->GetValue();


				characterStatsMapping.at(4) = 1;
				characterStatsValues.at(4) = velocity;
			}

		}

	}

	//App->scene->characterStatsItem->GetNewStatsWithoutComparing(characterStatsMapping, characterStatsValues);
	App->scene->characterStatsItem->CompareStats(characterStatsMapping, characterStatsValues);

}




void UiItem_Description::SwitchCameraUsage()
{

	// common 

	this->iconImage->useCamera = true;
	this->panelWithButton->useCamera = true;
	this->name->useCamera = true;
	this->useCamera = true;

	this->price->useCamera = true;

	if (spawnedInventoryImage)                 // only when the inventory is closed, the duplicated icon image should be hiden 
	{
		iconImageInventory->useCamera = true;
	}
	// - - - - - - - - - - - - - 

	if (this->descrType == descriptionType::WEAPON)
	{
		this->level->useCamera = true;
		this->damageLabel->useCamera = true;
		this->resistanceLabel->useCamera = true;
		this->cooldownLabel->useCamera = true;
		this->attachedCharacter->useCamera = true;

		this->damageComparisonLabel.label->useCamera = true;
		this->resistanceComparisonLabel.label->useCamera = true;
		this->cooldownComparisonLabel.label->useCamera = true;
	}
	else if (this->descrType == descriptionType::EQUIPMENT)
	{
		this->level->useCamera = true;
		this->attachedCharacter->useCamera = true;
		this->resistanceLabel->useCamera = true;
		this->HPLabel->useCamera = true;
		this->VelocityLabel->useCamera = true;

		this->resistanceComparisonLabel.label->useCamera = true;   // ADD HP LABEL AND VELOCITY LABEL
		this->HPComparisonLabel.label->useCamera = true;   // ADD HP LABEL AND VELOCITY LABEL
		this->velocityComparisonLabel.label->useCamera = true;   // ADD HP LABEL AND VELOCITY LABEL


	}
	else if (this->descrType == descriptionType::POTION)
	{
		this->effectLabel->useCamera = true;
	}

}

void UiItem_Description::HideAllElements(bool hide, bool closeInventory, bool buyingOrSelling)
{
	// common 

	this->iconImage->hide = hide;
	this->panelWithButton->hide = hide;
	this->name->hide = hide;
	this->hide = hide;

	this->price->hide = hide;

	if (spawnedInventoryImage)
	{

		if (closeInventory || buyingOrSelling)
		{
			iconImageInventory->hide = true;
		}

	}


	// - - - - - - - - - - - - - 

	if (this->descrType == descriptionType::WEAPON)
	{
		this->level->hide = hide;
		this->damageLabel->hide = hide;
		this->resistanceLabel->hide = hide;
		this->cooldownLabel->hide = hide;
		this->attachedCharacter->hide = hide;

		this->damageComparisonLabel.label->hide = hide;
		this->resistanceComparisonLabel.label->hide = hide;
		this->cooldownComparisonLabel.label->hide = hide;
	}
	else if (this->descrType == descriptionType::EQUIPMENT)
	{
		this->level->hide = hide;
		this->attachedCharacter->hide = hide;
		this->resistanceLabel->hide = hide;
		this->HPLabel->hide = hide;
		this->VelocityLabel->hide = hide;

		this->resistanceComparisonLabel.label->hide = hide;   // ADD HP LABEL AND VELOCITY LABEL
		this->HPComparisonLabel.label->hide = hide;   // ADD HP LABEL AND VELOCITY LABEL
		this->velocityComparisonLabel.label->hide = hide;   // ADD HP LABEL AND VELOCITY LABEL



	}
	else if (this->descrType == descriptionType::POTION)
	{
		this->effectLabel->hide = hide;
	}




}

void UiItem_Description::RepositionAllElements(iPoint referencePanelPosition)
{

	this->panelWithButton->hitBox.x = referencePanelPosition.x;
	this->panelWithButton->hitBox.y = referencePanelPosition.y;



	int destPricePosX = referencePanelPosition.x + this->panelWithButton->section.w / 2 - this->price->textureDimensions.x / 2;

	this->price->hitBox.x = destPricePosX;
	this->price->hitBox.y = referencePanelPosition.y + 190;

	if (this->descrType != descriptionType::POTION)
	{
		this->iconImage->hitBox.x = referencePanelPosition.x + 20;   // The icon image in potions should be repositioned
		this->iconImage->hitBox.y = referencePanelPosition.y + 80;
	}



	int destNamePosX = referencePanelPosition.x + this->panelWithButton->section.w / 2 - this->name->textureDimensions.x / 2;
	int offset = 10;

	this->name->hitBox.x = destNamePosX - offset;
	this->name->hitBox.y = referencePanelPosition.y + 40;

	if (this->descrType == descriptionType::WEAPON || this->descrType == descriptionType::EQUIPMENT)
	{
		this->level->hitBox.x = referencePanelPosition.x + 150;
		this->level->hitBox.y = referencePanelPosition.y + 160;

		this->attachedCharacter->hitBox.x = referencePanelPosition.x + 30;
		this->attachedCharacter->hitBox.y = referencePanelPosition.y + 160;
	}

	if (this->descrType == descriptionType::WEAPON)
	{

		this->damageLabel->hitBox.x = referencePanelPosition.x + 90;
		this->damageLabel->hitBox.y = referencePanelPosition.y + 70;

		this->resistanceLabel->hitBox.x = referencePanelPosition.x + 90;
		this->resistanceLabel->hitBox.y = referencePanelPosition.y + 100;

		this->cooldownLabel->hitBox.x = referencePanelPosition.x + 90;
		this->cooldownLabel->hitBox.y = referencePanelPosition.y + 130;



		if (App->scene->inventory->enable)
		{
			this->damageComparisonLabel.label->hitBox.x = referencePanelPosition.x + 164;
			this->damageComparisonLabel.label->hitBox.y = referencePanelPosition.y + 70;
			this->resistanceComparisonLabel.label->hitBox.x = referencePanelPosition.x + 164;
			this->resistanceComparisonLabel.label->hitBox.y = referencePanelPosition.y + 100;
			this->cooldownComparisonLabel.label->hitBox.x = referencePanelPosition.x + 164;
			this->cooldownComparisonLabel.label->hitBox.y = referencePanelPosition.y + 130;
		}
		else
		{
			this->damageComparisonLabel.label->hitBox.x = referencePanelPosition.x + 164;
			this->damageComparisonLabel.label->hitBox.y = referencePanelPosition.y + 70;
			this->resistanceComparisonLabel.label->hitBox.x = referencePanelPosition.x + 164;
			this->resistanceComparisonLabel.label->hitBox.y = referencePanelPosition.y + 100;
			this->cooldownComparisonLabel.label->hitBox.x = referencePanelPosition.x + 164;
			this->cooldownComparisonLabel.label->hitBox.y = referencePanelPosition.y + 130;
		}


	}
	else if (this->descrType == descriptionType::EQUIPMENT)
	{
		this->resistanceLabel->hitBox.x = referencePanelPosition.x + 90;
		this->resistanceLabel->hitBox.y = referencePanelPosition.y + 70;


		HPLabel->hitBox.x = referencePanelPosition.x + 90;
		HPLabel->hitBox.y = referencePanelPosition.y + 100;

		VelocityLabel->hitBox.x = referencePanelPosition.x + 90;
		VelocityLabel->hitBox.y = referencePanelPosition.y + 130;


		if (App->scene->inventory->enable)
		{
			this->resistanceComparisonLabel.label->hitBox.x = referencePanelPosition.x + 164;
			this->resistanceComparisonLabel.label->hitBox.y = referencePanelPosition.y + 70;
			this->HPComparisonLabel.label->hitBox.x = referencePanelPosition.x + 164;
			this->HPComparisonLabel.label->hitBox.y = referencePanelPosition.y + 100;
			this->velocityComparisonLabel.label->hitBox.x = referencePanelPosition.x + 164;
			this->velocityComparisonLabel.label->hitBox.y = referencePanelPosition.y + 130;
		}
		else
		{

			this->resistanceComparisonLabel.label->hitBox.x = referencePanelPosition.x + 164;
			this->resistanceComparisonLabel.label->hitBox.y = referencePanelPosition.y + 70;
			this->HPComparisonLabel.label->hitBox.x = referencePanelPosition.x + 164;
			this->HPComparisonLabel.label->hitBox.y = referencePanelPosition.y + 100;
			this->velocityComparisonLabel.label->hitBox.x = referencePanelPosition.x + 164;
			this->velocityComparisonLabel.label->hitBox.y = referencePanelPosition.y + 130;
		}

	}
	else if (this->descrType == descriptionType::POTION)
	{
		// TODO: tweak this 

		this->effectLabel->hitBox.x = referencePanelPosition.x + 30;
		this->effectLabel->hitBox.y = referencePanelPosition.y + 165;


		int offset = 10;
		int destIconPosX = referencePanelPosition.x + this->panelWithButton->section.w / 2 - this->iconImage->section.w * this->iconImage->scaleFactor / 2;
		this->iconImage->hitBox.x = destIconPosX - offset;
		this->iconImage->hitBox.y = referencePanelPosition.y + 80;
	}




	// switch description type and reposition everything

}



void UiItem_Description::DeleteEverything()
{

	// TODO: put the lootentity pointer to nullptr ??? careful; 

	App->gui->destroyElement(this->iconImage);
	App->gui->destroyElement(this->panelWithButton);
	App->gui->destroyElement(this->name);

	App->gui->destroyElement(this->price);

	// TODO: delete the icon image in the inventory only if it exists
	//this->iconImageInventory->to_delete = true;

	if (spawnedInventoryImage)
	{
		App->gui->destroyElement(this->iconImageInventory);
	}

	if (this->descrType == descriptionType::WEAPON)
	{

		App->gui->destroyElement(this->level);
		App->gui->destroyElement(this->damageLabel);
		App->gui->destroyElement(this->resistanceLabel);
		App->gui->destroyElement(this->cooldownLabel);
		App->gui->destroyElement(this->attachedCharacter);

		App->gui->destroyElement(this->damageComparisonLabel.label);
		App->gui->destroyElement(this->resistanceComparisonLabel.label);
		App->gui->destroyElement(this->cooldownComparisonLabel.label);
	}
	else if (this->descrType == descriptionType::EQUIPMENT)
	{
		App->gui->destroyElement(this->level);
		App->gui->destroyElement(this->attachedCharacter);
		App->gui->destroyElement(this->resistanceLabel);
		App->gui->destroyElement(this->HPLabel);
		App->gui->destroyElement(this->VelocityLabel);


		App->gui->destroyElement(this->resistanceComparisonLabel.label);  // ADD HP AND VELOCITY LABELS
		App->gui->destroyElement(this->HPComparisonLabel.label);  // ADD HP AND VELOCITY LABELS
		App->gui->destroyElement(this->velocityComparisonLabel.label);  // ADD HP AND VELOCITY LABELS

	}
	else if (this->descrType == descriptionType::POTION)
	{
		App->gui->destroyElement(this->effectLabel);
	}

	App->gui->destroyElement(this);

}