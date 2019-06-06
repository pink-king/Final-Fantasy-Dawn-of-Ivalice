#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "LootEntity.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1EntityFactory.h"
#include "j1PathFinding.h"
#include "j1EasingSplines.h"
LootEntity::LootEntity(LOOT_TYPE type, int posX, int posY) : j1Entity(LOOT, posX, posY, "LootParent"), loot_type(type)
{
	
	//ChooseEntity();
	lootSubtile.x = posX;
	lootSubtile.y = posY;
	engine.seed(rd());
	
	level = 0;
	adjacentTileNeighboursToGround[0] = { 0,-1 }; // N
	adjacentTileNeighboursToGround[1] = { 0, 1 }; // S
	adjacentTileNeighboursToGround[2] = { -1,0 }; // W
	adjacentTileNeighboursToGround[3] = { 1,0 }; // E
	adjacentTileNeighboursToGround[4] = { 1,-1 }; // NE
	adjacentTileNeighboursToGround[5] = { -1,-1 }; // NW
	adjacentTileNeighboursToGround[6] = { 1, 1 }; // SE
	adjacentTileNeighboursToGround[7] = { -1,1 }; // SW
	adjacentTileNeighboursToGround[8] = { 0,-2 }; // N
	adjacentTileNeighboursToGround[9] = { 0, 2 }; // S
	adjacentTileNeighboursToGround[10] = { -2,0 }; // W
	adjacentTileNeighboursToGround[11] = { 2,0 }; // E
	adjacentTileNeighboursToGround[12] = { 2,-2 }; // NE
	adjacentTileNeighboursToGround[13] = { -2,-2 }; // NW
	adjacentTileNeighboursToGround[14] = { 2, 2 }; // SE
	adjacentTileNeighboursToGround[15] = { -2,2 }; // SW
	adjacentTileNeighboursToGround[16] = { 0,-3 }; // N
	adjacentTileNeighboursToGround[17] = { 0, 3 }; // S
	adjacentTileNeighboursToGround[18] = { -3,0 }; // W
	adjacentTileNeighboursToGround[19] = { 3,0 }; // E
	adjacentTileNeighboursToGround[20] = { 3,-3 }; // NE
	adjacentTileNeighboursToGround[21] = { -3,-3 }; // NW
	adjacentTileNeighboursToGround[22] = { 3, 3 }; // SE
	adjacentTileNeighboursToGround[23] = { -3,3 }; // SW
	adjacentTileNeighboursToGround[24] = { 0,-4 }; // N
	adjacentTileNeighboursToGround[25] = { 0, 4 }; // S
	adjacentTileNeighboursToGround[26] = { -4,0 }; // W
	adjacentTileNeighboursToGround[27] = { 4,0 }; // E
	adjacentTileNeighboursToGround[28] = { 4,-4 }; // NE
	adjacentTileNeighboursToGround[29] = { -4,-4 }; // NW
	adjacentTileNeighboursToGround[30] = { 4, 4 }; // SE
	adjacentTileNeighboursToGround[31] = { -4,4 }; // SW
	GetGroundTilePoints();
	SelectSubtileToGround();

	shadow_rect = { 0,0,32,32 };
}

LootEntity::~LootEntity()
{
	// TODO: call DeleteEverything() in the associated GUI description
	

}


//bool LootEntity::PreUpdate()
//{
//	return true;
//}

bool LootEntity::Start()
{
	
	
	//adjacentTileNeighboursToGround[0] = { 0,-1 }; // N
	//adjacentTileNeighboursToGround[1] = { 0, 1 }; // S
	//adjacentTileNeighboursToGround[2] = { -1,0 }; // W
	//adjacentTileNeighboursToGround[3] = { 1,0 }; // E
	//adjacentTileNeighboursToGround[4] = { 1,-1 }; // NE
	//adjacentTileNeighboursToGround[5] = { -1,-1 }; // NW
	//adjacentTileNeighboursToGround[6] = { 1, 1 }; // SE
	//adjacentTileNeighboursToGround[7] = { -1,1 }; // SW

	
	return true;
}
bool LootEntity::CleanUp()
{
	std::vector<Buff*>::reverse_iterator Itemstat = stats.rbegin();

	while (Itemstat != stats.rend())
	{
		if (*Itemstat != nullptr)
		{
			RELEASE(*Itemstat);
			*Itemstat = nullptr;
		}
		++Itemstat;
	}
	stats.clear();

	/*delete MyDescription;
	MyDescription = nullptr;*/
	return true;
}
bool LootEntity::Load(pugi::xml_node &node, LootEntity* loot)
{
	pugi::xml_node nodeData = node.child("data");

	entityTex = App->entityFactory->lootItemsTex;

	level = nodeData.attribute("level").as_int();

	pugi::xml_node nodeRect = nodeData.child("rect");
	loot_rect.x = nodeRect.attribute("x").as_int();
	loot_rect.y = nodeRect.attribute("y").as_int();
	loot_rect.w = nodeRect.attribute("w").as_int();
	loot_rect.h = nodeRect.attribute("h").as_int();

	std::string nameObj = nodeData.attribute("name").as_string();
	loot->name.assign(nameObj);
	loot->lootname.assign(loot->name.data());

	std::string charName = nodeData.attribute("characterObject").as_string();
	if (charName.compare("Marche") == 0)
		loot->character = App->entityFactory->player->GetMarche();
	else if (charName.compare("Ritz") == 0)
		loot->character = App->entityFactory->player->GetRitz();
	else if (charName.compare("Shara") == 0)
		loot->character = App->entityFactory->player->GetShara();

	std::string charEquipable = nodeData.attribute("equipableType").as_string();
	if (charEquipable.compare("sword") == 0)
		loot->equipableType = EQUIPABLE_TYPE::SWORD;
	else if (charEquipable.compare("bow") == 0)
		loot->equipableType = EQUIPABLE_TYPE::BOW;
	else if (charEquipable.compare("rod") == 0)
		loot->equipableType = EQUIPABLE_TYPE::ROD;
	else if (charEquipable.compare("armor") == 0)
		loot->equipableType = EQUIPABLE_TYPE::ARMOR;
	else if (charEquipable.compare("vest") == 0)
		loot->equipableType = EQUIPABLE_TYPE::VEST;
	else if (charEquipable.compare("mantle") == 0)
		loot->equipableType = EQUIPABLE_TYPE::MANTLE;

	std::string charElement = nodeData.attribute("elementalType").as_string();
	if (charElement.compare("noElement") == 0)
		loot->elemetalType = ELEMENTAL_TYPE::NO_ELEMENT;
	else if (charElement.compare("fireElement") == 0)
		loot->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
	else if (charElement.compare("iceElement") == 0)
		loot->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;
	else if (charElement.compare("poisonElement") == 0)
		loot->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
	else if (charElement.compare("stoneElement") == 0)
		loot->elemetalType = ELEMENTAL_TYPE::STONE_ELEMENT;
	else if (charElement.compare("allElement") == 0)
		loot->elemetalType = ELEMENTAL_TYPE::ALL_ELEMENTS;
	else if (charElement.compare("dust") == 0)
		loot->elemetalType = ELEMENTAL_TYPE::DUST;

	std::string charObject = nodeData.attribute("objectType").as_string();
	if (charObject.compare("weapon") == 0)
		loot->objectType = OBJECT_TYPE::WEAPON_OBJECT;
	else if (charObject.compare("armor") == 0)
		loot->objectType = OBJECT_TYPE::ARMOR_OBJECT;
	else if (charObject.compare("head") == 0)
		loot->objectType = OBJECT_TYPE::HEAD_OBJECT;
	else if (charObject.compare("potion") == 0)
	{
		loot->objectType = OBJECT_TYPE::POTIONS;
	}
	else if (charObject.compare("gold") == 0)
		loot->objectType = OBJECT_TYPE::GOLD;
	else if (charObject.compare("phoenixTail") == 0)
		loot->objectType = OBJECT_TYPE::PHOENIX_TAIL;

	std::string charType = nodeData.attribute("lootType").as_string();
	if (charType.compare("consumable") == 0)
	{
		loot->loot_type = LOOT_TYPE::CONSUMABLE;
	}
	else if (charType.compare("equipable") == 0)
	{
		loot->loot_type = LOOT_TYPE::EQUIPABLE;
	}

	for (pugi::xml_node nodebuffs = node.child("buffs"); nodebuffs; nodebuffs = nodebuffs.next_sibling("buffs"))
	{
		Buff* buf = DBG_NEW Buff();
		buf = buf->Load(nodebuffs);
		if (buf != nullptr)
		{
			buf->SetCharacter(loot->character);
			buf->SetItemObject(loot);
			stats.push_back(buf);
		}
	}

	return true;
}
bool LootEntity::Save(pugi::xml_node &node) const
{
	pugi::xml_node nodeData = node.append_child("data");

	nodeData.append_attribute("name") = lootname.data();
	nodeData.append_attribute("level") = level;

	pugi::xml_node nodeRect = nodeData.append_child("rect");
	nodeRect.append_attribute("x") = loot_rect.x;
	nodeRect.append_attribute("y") = loot_rect.y;
	nodeRect.append_attribute("w") = loot_rect.w;
	nodeRect.append_attribute("h") = loot_rect.h;

	if(character != nullptr)
		nodeData.append_attribute("characterObject") = character->name.data();

	switch (equipableType)
	{
	case EQUIPABLE_TYPE::SWORD:
		nodeData.append_attribute("equipableType") = "sword";
		break;
	case EQUIPABLE_TYPE::BOW:
		nodeData.append_attribute("equipableType") = "bow";
		break;
	case EQUIPABLE_TYPE::ROD:
		nodeData.append_attribute("equipableType") = "rod";
		break;
	case EQUIPABLE_TYPE::ARMOR:
		nodeData.append_attribute("equipableType") = "armor";
		break;
	case EQUIPABLE_TYPE::VEST:
		nodeData.append_attribute("equipableType") = "vest";
		break;
	case EQUIPABLE_TYPE::MANTLE:
		nodeData.append_attribute("equipableType") = "mantle";
		break;
	case EQUIPABLE_TYPE::NO_EQUIPABLE:
		break;
	default:
		break;
	}

	switch (elemetalType)
	{
	case ELEMENTAL_TYPE::NO_ELEMENT:
		nodeData.append_attribute("elementalType") = "noElement";
		break;
	case ELEMENTAL_TYPE::FIRE_ELEMENT:
		nodeData.append_attribute("elementalType") = "fireElement";
		break;
	case ELEMENTAL_TYPE::ICE_ELEMENT:
		nodeData.append_attribute("elementalType") = "iceElement";
		break;
	case ELEMENTAL_TYPE::POISON_ELEMENT:
		nodeData.append_attribute("elementalType") = "poisonElement";
		break;
	case ELEMENTAL_TYPE::STONE_ELEMENT:
		nodeData.append_attribute("elementalType") = "stoneElement";
		break;
	case ELEMENTAL_TYPE::ALL_ELEMENTS:
		nodeData.append_attribute("elementalType") = "allElement";
		break;
	case ELEMENTAL_TYPE::DUST:
		nodeData.append_attribute("elementalType") = "dust";
		break;
	case ELEMENTAL_TYPE::MAX:
		break;
	default:
		break;
	}

	switch (objectType)
	{
	case OBJECT_TYPE::WEAPON_OBJECT:
		nodeData.append_attribute("objectType") = "weapon";
		break;
	case OBJECT_TYPE::ARMOR_OBJECT:
		nodeData.append_attribute("objectType") = "armor";
		break;
	case OBJECT_TYPE::HEAD_OBJECT:
		nodeData.append_attribute("objectType") = "head";
		break;
	case OBJECT_TYPE::POTIONS:
		nodeData.append_attribute("objectType") = "potion";
		break;
	case OBJECT_TYPE::GOLD:
		nodeData.append_attribute("objectType") = "gold";
		break;
	case OBJECT_TYPE::PHOENIX_TAIL:
		nodeData.append_attribute("objectType") = "phoenixTail";
		break;
	case OBJECT_TYPE::NO_OBJECT:
		break;
	default:
		break;
	}

	switch (loot_type)
	{
	case LOOT_TYPE::CONSUMABLE:
		nodeData.append_attribute("lootType") = "consumable";
		break;
	case LOOT_TYPE::EQUIPABLE:		
		nodeData.append_attribute("lootType") = "equipable";
		break;
	case LOOT_TYPE::NO_LOOT:
		break;
	default:
		break;
	}

	for (std::vector<Buff*>::const_iterator item = stats.begin(); item != stats.end(); ++item)
	{
		pugi::xml_node nodeBuffs = node.append_child("buffs");
		(*item)->Save(nodeBuffs);
	}
	return true;
}
bool LootEntity::Update(float dt)
{
	
	return true;
}
float LootEntity::LerpX(float origin, float destination, float t )
{
	return origin * (1 - t) + destination * t;
}

iPoint LootEntity::Getoriginpos()
{
	iPoint originPos = App->map->SubTileMapToWorld(lootSubtile.x, lootSubtile.y);
	return originPos;
}

iPoint LootEntity::GetPosition()
{
	return (iPoint(position));
}

std::string LootEntity::GetName()
{
	return name;
}  

LOOT_TYPE LootEntity::GetType()
{
	return 	loot_type;

}

OBJECT_TYPE LootEntity::GetObjectType()
{
	return objectType;
}

//Quan arriba aqui per primera vegada ya ha fet 10 randoms WTF
int LootEntity::GetRandomValue(int min, int max)
{
	
	
	std::uniform_int_distribution<int> range(min, max);
	
	int ret = range(rd);

	LOG("RANDOM VALUE %i", ret);
	//LOG("value %i", ret_value);

	return ret;
}

void LootEntity::CreateBuff(BUFF_TYPE type, j1Entity* character, std::string stat, ELEMENTAL_TYPE elementType, ROL rol, float value, LootEntity* item)
{
	stats.push_back(DBG_NEW Buff(type, character, stat, elementType, rol, value, item));

}

void LootEntity::Draw()
{
	//TO FIX only blits pivot
	if (entityTex != nullptr)
		App->render->Blit(entityTex, position.x, position.y, &loot_rect,1.0f);

	if (objShadow != nullptr)
		App->render->Blit(objShadow, position.x, App->map->SubTileMapToWorld(groundSubtileDestination.x + 1, groundSubtileDestination.y).y - pivot.y*.5f, &shadow_rect, 1.0f);
	
	//LOG("painting loot");
}

EQUIPABLE_TYPE LootEntity::GetEquipable()
{
	return equipableType;
}

void LootEntity::ReRECTlootToLegRect(EQUIPABLE_TYPE equipable)
{
	switch (equipable)
	{
	case EQUIPABLE_TYPE::SWORD:
		loot_rect = { 353, 96, 32, 32 };
		SetPivot(16, 28);
		size.create(32, 32);
		//353, 96
		break;

	case EQUIPABLE_TYPE::BOW:
		loot_rect = { 34, 834, 32, 32 };
		SetPivot(16, 16);
		size.create(32, 32);
		//34,834
		break;

	case EQUIPABLE_TYPE::ROD:
		loot_rect = { 33, 738, 32, 32 };
		SetPivot(16, 28);
		size.create(32, 32);
		//33 , 738
		break;

	case EQUIPABLE_TYPE::ARMOR:
		loot_rect = { 128, 964, 32, 32 };
		SetPivot(16, 28);
		size.create(32, 32);
		//128 , 964
		break;

	case EQUIPABLE_TYPE::VEST:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;
		loot_rect = { 95, 1409, 32, 32 };
		SetPivot(16, 28);
		size.create(32, 32);
		//95,1409
		break;


	case EQUIPABLE_TYPE::MANTLE:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;
		loot_rect = { 446, 1408, 32, 32 };
		SetPivot(16, 28);
		size.create(32, 32);
		//leg mantle x446,y1408

		break;

	}
}



std::list<iPoint> LootEntity::GetGroundTilePoints()
{
	
	for (int i = 0; i < NUM_NEIGH_SUBTILETILE_FALL; ++i)
	{

		iPoint subtilePos = App->map->WorldToSubtileMap(position.x, position.y) + adjacentTileNeighboursToGround[i];
		iPoint temp;
		temp = App->map->SubTileMapToWorld(subtilePos.x, subtilePos.y);
		iPoint tilePos = App->map->WorldToMap(temp.x, temp.y);
		if (App->pathfinding->IsWalkable(tilePos))
		{
			groundSubtilePoints.push_back(subtilePos);
			
		}
	}
	
	return groundSubtilePoints;
}

void LootEntity::SetSplineToFall()
{
	//App->easing->CreateSpline((&float)position.y,)
	int actualpos = position.y;
	App->easing->CreateSpline(&position.y, App->map->SubTileMapToWorld(groundSubtileDestination.x+1, groundSubtileDestination.y).y - pivot.y, 1000, TypeSpline::EASE_OUT_BOUNCE); //here
}

void LootEntity::SelectSubtileToGround()
{
	LOG("lootEjection");
	LOG("groundTile size %i", groundSubtilePoints.size());
	int m;
	if (groundSubtilePoints.size() == 0)
		m = groundSubtilePoints.size();
	else
		m = groundSubtilePoints.size()-1;

	int randVal = GetRandomValue(0, m);
	m=0;

	for (std::list<iPoint>::iterator iter = groundSubtilePoints.begin(); iter != groundSubtilePoints.end(); ++iter)
	{
		if (m == randVal)
		{
			// if randVal = 8 tiledestinatio gets fucked!!
			
			groundSubtileDestination = (*iter);
			LOG("tw8");
			break;

		}
		++m;
	}
}



// TODO: why loot entity asks on every frame ?
// its not worth, what happen when we have a bunch of loots?
// solution: only asks the player itself, or the crosshair instead

/*
void LootEntity::CheckClampedCrossHairToSpawnDescription()  // TODO: Change this with player proximity instead of crosshair
{
	// if the crosshair focuses the item and description is hiden 

	if (App->entityFactory->player->GetCrosshair()->GetClampedEntity() == this && !spawnedDescription)
	{


		clampedByCrosshair = true;

		// create a new one
		App->entityFactory->GenerateDescriptionForLootItem(this);
		iPoint offset(-80, -this->MyDescription->panelWithButton->section.y - 180);
		this->MyDescription->RepositionAllElements(App->render->WorldToScreen(this->GetPosition().x, this->GetPosition().y, true) + offset);
		this->MyDescription->HideAllElements(false);

		spawnedDescription = true;
	}

	// if description is showing, but crosshair stops focusing item 

	// this is preventing the player from picking loot by staying on top of it himself

	if (spawnedDescription && App->entityFactory->player->GetCrosshair()->GetClampedEntity() && !this->MyDescription->hide)
	{

		// delete last descr
		this->MyDescription->DeleteEverything();
		this->MyDescription = nullptr;


		//this->MyDescription->HideAllElements(true);

		spawnedDescription = false;
	}



}*/

void LootEntity::GetDistanceTotravel()
{
	iPoint actual{ (int)position.x,(int)position.y };
	distanceTotravel = App->map->MapToWorld(groundSubtileDestination.x, groundSubtileDestination.y) - actual;
}


