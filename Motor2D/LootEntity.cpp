#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "LootEntity.h"
#include "j1LootManager.h"
#include "p2Log.h"
#include "j1Map.h"

LootEntity::LootEntity(LOOT_TYPE type, int posX, int posY) : j1Entity(LOOT, posX, posY, "LootParent"), loot_type(type)
{
	entityTex = App->tex->Load("textures/loot/loot_items.png");
	//ChooseEntity();
	lootSubtile.x = posX;
	lootSubtile.y = posY;
	engine.seed(rd());
	

	
}

LootEntity::~LootEntity()
{}


//bool LootEntity::PreUpdate()
//{
//	return true;
//}

bool LootEntity::Start()
{
	
	start = true;
	endReached = false;
	
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
	return App->map->SubTileMapToWorld(App->lootManager->GetEnemySubtile(this).x, App->lootManager->GetEnemySubtile(this).y);
}
/* ARc

posY = sin(angle) * currenttime - gravity*t^2/2
y = xtan(angle) - gravity*x^2 / (2*cos^2(angle))
*/
//Basic trigonometry example
//
//cosine for x axis
//
//sine for y axis
//
//speed is whatever you would like the movement each time to increment by... Obviously the higher the number the more pixels it will move at any given time so the quicker it will be.
//
//dx = (double)(Math.cos(angle) * speed);
//dy = (double)(Math.sin(angle) * speed);
//
//A.x += dx;
//A.y += dy;
//bool LootEntity::PostUpdate()
//{
//	return true;
//}
//
//bool LootEntity::CleanUp()
//{
//	return true;
//}
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

float LootEntity::EaseOutBack(float t)
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


void LootEntity::CreateBuff(BUFF_TYPE type, j1Entity* character, std::string stat, ELEMENTAL_TYPE elementType, OBJECT_ROL rol, float value, LootEntity* item)
{
	stats.push_back(new Buff(type, character, stat, elementType, rol, value, item));

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
		position.y - 0.070*timeTest*timeTest;
		position.y += decrementY;
	}
	
	
}


