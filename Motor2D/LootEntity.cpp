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
	
	//lootTexture = App->tex->Load("textures/loot/loot_items.png");
	level = 0;
	adjacentTileNeighboursToGround[0] = { 0,-3 }; // N
	adjacentTileNeighboursToGround[1] = { 0, 3 }; // S
	adjacentTileNeighboursToGround[2] = { -3,0 }; // W
	adjacentTileNeighboursToGround[3] = { 3,0 }; // E
	adjacentTileNeighboursToGround[4] = { 3,-3 }; // NE
	adjacentTileNeighboursToGround[5] = { -3,-3 }; // NW
	adjacentTileNeighboursToGround[6] = { 3, 3 }; // SE
	adjacentTileNeighboursToGround[7] = { -3,3 }; // SW

	GetGroundTilePoints();
	SelectTileToGround();
	/*int actualposX = position.x;
	int actualposY = position.y;
	iPoint dest = App->map->MapToWorld(groundTileDestination.x, groundTileDestination.y);
	
	App->easing->CreateSpline(&actualposX, actualposX -1000, 1000, TypeSpline::EASE);

	App->easing->CreateSpline(&actualposY, actualposY-1000,2000, TypeSpline::EASE_OUT_CUBIC, std::bind(&LootEntity::SetSplineToFall,this));*/
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
	
	start = true;
	endReached = false;
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
fPoint LootEntity::SetDestinationPos(float x, float y)
{
	
	x = position.x + 128;
	y = position.y;

	return { x,y };
}
iPoint LootEntity::Getoriginpos()
{
	iPoint originPos = App->map->SubTileMapToWorld(lootSubtile.x, lootSubtile.y);
	return originPos;
}
iPoint LootEntity::GetFinalPos()
{
	iPoint finalPos = Getoriginpos();

	return finalPos;

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

float LootEntity::EaseOutBackBADvERSION(float t)
{


	

	return 1 + (--t) * t * (2.70158 * t + 1.70158);


}

fPoint LootEntity::Lerp(fPoint origin, fPoint destination, float t)
{
	fPoint vector = origin * (1 - t) + destination * t;
	float lengthmodule = std::sqrtf(vector.x*vector.x + vector.y*vector.y);
	vector.x = vector.x / lengthmodule;
	vector.y = vector.y / lengthmodule;
	return vector;
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
	//LOG("painting loot");
}

EQUIPABLE_TYPE LootEntity::GetEquipable()
{
	return equipableType;
}

void LootEntity::DecideExplosion()
{
	EXPLOSION_DIRECTION  randVale;
	int randVal = GetRandomValue(0, 6);

	switch (randVal)
	{

	case 0:
		randVale = EXPLOSION_DIRECTION::EAST;

		timeXmid = 200.0f;
		incrementX = 0.4;
		decrementX = 0.2;
		timeYmid = 170.0f;
		incrementY = 2.8;
		decrementY = 3.8;
		break;

	case 1:
		randVale = EXPLOSION_DIRECTION::WEST;

		timeXmid = 200.0f;
		incrementX = -0.4;
		decrementX = -0.2;
		timeYmid = 170.0f;
		incrementY = 2.8;
		decrementY = 3.8;

		break;
	case 2:
		randVale = EXPLOSION_DIRECTION::NORTHEAST;
		timeXmid = 200.0f;
		incrementX = 0.4;
		decrementX = 0.2;
		timeYmid = 200.0f;
		incrementY = 2.8;
		decrementY = 3.0;
		break;

	case 3:
		randVale = EXPLOSION_DIRECTION::NORTHWEST;
		timeXmid = 200.0f;
		incrementX = -0.4;
		decrementX = -0.2;
		timeYmid = 200.0f;
		incrementY = 2.8;
		decrementY = 3.0;
		break;
	case 4:
		randVale = EXPLOSION_DIRECTION::SOUTHEAST;
		timeXmid = 200.0f;
		incrementX = 0.4;
		decrementX = 0.2;
		timeYmid = 160.0f;
		incrementY = 2.5;
		decrementY = 3.8;
		break;

	case 5:
		randVale = EXPLOSION_DIRECTION::SOUTHWEST;

		timeXmid = 200.0f;
		incrementX = -0.4;
		decrementX = -0.2;
		timeYmid = 160.0f;
		incrementY = 2.5;
		decrementY = 3.8;
		break;

	case 6:
		timeXmid = 160.0f;
		incrementX = -0.1f;
		decrementX = -0.1f;
		timeYmid = 80.0f;
		incrementY = 2.0f;
		decrementY = 3.0f;
		break;
	}
	
}

void LootEntity::ExplosionMaker(float dt)
{
	timeTest = displacementTime.ReadMs()*0.001;
	position.x = LerpX(position.x, goalPos.x, 0.0000009f);
	if (displacementTime.ReadMs() <= timeXmid)
	{
		position.x += incrementX * dt;
		LOG("positionX increment %i",position.x);
	}
	else position.x += decrementX * dt;

	position.y = LerpX(position.y, goalPos.y, 0.0000009f);

	if (displacementTime.ReadMs() <= timeYmid)
	{
		position.y = position.y + 0.707*timeTest*timeTest;
		position.y -= incrementY;
	}
	else
	{
		position.y  - 0.070*timeTest*timeTest;
		position.y += decrementY;
	}
	
	
}

std::list<iPoint> LootEntity::GetGroundTilePoints()
{
	
	for (int i = 0; i < NUM_NEIGH_TILE_FALL; ++i)
	{
		App->map->WorldToMap(position.x, position.y) + adjacentTileNeighboursToGround[i];
		//iPoint tilePos = App->entityFactory->player->GetTilePos() + adjacentTileNeighboursToGround[i];

		iPoint tilePos = App->map->WorldToMap(position.x, position.y) + adjacentTileNeighboursToGround[i];

		if (App->pathfinding->IsWalkable(tilePos))
		{
			groundTilePoints.push_back(tilePos);

		}
	}
	return groundTilePoints;
}

void LootEntity::SetSplineToFall()
{
	//App->easing->CreateSpline((&float)position.y,)
	int actualpos = position.y;
	App->easing->CreateSpline(&position.y, App->map->MapToWorld(groundTileDestination.x,groundTileDestination.y).y, 1000, TypeSpline::EASE_OUT_BOUNCE); //here
}

void LootEntity::SelectTileToGround()
{
	
	LOG("groundTile size %i", groundTilePoints.size());
	int randVal = GetRandomValue(0, groundTilePoints.size());
	int m=0;

	for (std::list<iPoint>::iterator iter = groundTilePoints.begin(); iter != groundTilePoints.end(); ++iter)
	{
		if (m == randVal)
		{

			
			groundTileDestination = (*iter);
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
	distanceTotravel = App->map->MapToWorld(groundTileDestination.x, groundTileDestination.y) - actual;
}

int LootEntity::Ease(float time_passed, int initialpos, int distance_to_travel, float time_to_travel)
{
	return distance_to_travel * (time_passed / time_to_travel) + initialpos;
}
int LootEntity::EaseOutQuint(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	return distance_to_travel * ((time_passed = time_passed / time_to_travel - 1) * time_passed * time_passed * time_passed * time_passed + 1) + initial_position;
}

int LootEntity::EaseOutCubic(float time_passed, int initialpos, int distance_to_travel, float time_to_travel)
{
	return distance_to_travel * (time_passed /= time_to_travel) * time_passed * time_passed + initialpos;
}



int LootEntity::EaseInOutBack(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	float s = 1.70158f;
	if ((time_passed /= time_to_travel / 2) < 1) {
		return distance_to_travel / 2 * (time_passed * time_passed * (((s *= (1.525f)) + 1) * time_passed - s)) + initial_position;
	}
	else {
		float postFix = time_passed -= 2;
		return distance_to_travel / 2 * ((postFix)* time_passed * (((s *= (1.525f)) + 1) * time_passed + s) + 2) + initial_position;
	}
}

int LootEntity::EaseInBack(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	float s = 2.70158f;
	float postFix = time_passed /= time_to_travel;
	return distance_to_travel * (postFix)* time_passed* ((s + 1) * time_passed - s) + initial_position;
}

int LootEntity::EaseOutBack(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	float s = 1.10158f;
	return distance_to_travel * ((time_passed = time_passed / time_to_travel - 1) * time_passed * ((s + 1) * time_passed + s) + 1) + initial_position;
}

int LootEntity::EaseInCubic(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	return distance_to_travel * (time_passed /= time_to_travel) * time_passed * time_passed + initial_position;
}

int LootEntity::EaseOutBounce(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	if ((time_passed /= time_to_travel) < (1 / 2.75f)) {
		return distance_to_travel * (7.5625f * time_passed * time_passed) + initial_position;
	}
	else if (time_passed < (2 / 2.75f)) {
		float postFix = time_passed -= (1.5f / 2.75f);
		return distance_to_travel * (7.5625f * (postFix)* time_passed + .75f) + initial_position;
	}
	else if (time_passed < (2.5 / 2.75)) {
		float postFix = time_passed -= (2.25f / 2.75f);
		return distance_to_travel * (7.5625f * (postFix)* time_passed + .9375f) + initial_position;
	}
	else {
		float postFix = time_passed -= (2.625f / 2.75f);
		return distance_to_travel * (7.5625f * (postFix)* time_passed + .984375f) + initial_position;
	}
}

int LootEntity::OwnEaseOutQuartic(float t, int b, int c, float d)
{
	int ts = (t /= d) * t;
	int tc = ts * t;
	return b + c * (1.5 * tc * ts + -1.1 * ts * ts + 0.7 * tc + -0.1 * t);
}

int LootEntity::EaseinBounce(float t, int b, int c, float d)
{
	return c - EaseOutBounce(d - t, 0, c, d) + b;
}

int LootEntity::EaseInOutBounce(float t, int b, int c, float d)
{
	if (t < d / 2) return EaseinBounce(t * 2, 0, c, d) * .5f + b;
	else return EaseInOutBounce(t * 2 - d, 0, c, d) * .5f + c * .5f + b;
}
