#ifndef __J1ENTITY_H__
#define __J1ENTITY_H__

#include "p2Animation.h"
#include "p2Point.h"
#include "j1Timer.h"
#include "j1PerfTimer.h"
#include "j1Module.h"
#include "j1Textures.h"


#include "SDL_image/include/SDL_image.h"

#include "UiItem_HealthBar.h"
#include "UiItem_Image.h"

#include <math.h>

class UiItem_HealthBar; 
class UiItem_Image; 

enum ENTITY_TYPE  // todo, pass to class
	{
		NO_TYPE,
		PARTICLE,
		ENTITY_STATIC,
		ENTITY_DYNAMIC,
		//---------------
		PLAYER,
		ENEMY_BOMB,
		ENEMY02,
		//etc
		LOOT,
		ENEMY_TEST,
		// LAST
		MAX_TYPE
	};

enum class STAT_TYPE
{
	NORMAL,
	BURNED_STAT,
	PARALIZE_STAT,
	COOLDOWN,
	POTION_STAT,
	ATTACKSPEED
};

class entityStat
{
public:
	entityStat(STAT_TYPE stat, uint totalTime, float secDamage = 0.f):type(stat), secDamage(secDamage), totalTime(totalTime){}
	entityStat() {};
public:
	STAT_TYPE		type = STAT_TYPE::NORMAL;
	float			secDamage = 0.f;
	j1Timer			count;
	uint			totalTime = 0;
};

struct EntityInfo
{
	iPoint position;
	//Collider* collider = nullptr;
};

class j1Entity
{
public:
	j1Entity(ENTITY_TYPE type, float positionX, float positionY, std::string name);
	// assets constructor ---
	// static sprite environment asset
	j1Entity(iPoint position, SDL_Rect spriteAtlasRect);
	// static sprite with animation
	//j1Entity(//TODO);
	virtual ~j1Entity();

	virtual bool Start();
	// core loops ---------------
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();
	//----------------------------
	virtual bool Move(float dt);
	virtual void Draw();
	//virtual void OnCollision(Collider* collider);

	iPoint GetTilePos() const;
	iPoint GetSubtilePos() const;
	iPoint GetPreviousSubtilePos() const;
	void UpdateTilePositions();
	fPoint GetPosition();
	void SetPivot(const float & x, const float & y);
	fPoint GetPivotPos() const;	
	fPoint GetThrowingPos() const; 
	bool ChangedTile() const; 

	virtual void LoadEntitydata(pugi::xml_node&);
	

public:
	SDL_Rect drawAtlasRect; // for static draw from spritesheet
	bool					to_delete = false;
	std::string				name;
	fPoint					position;
	iPoint					size;
	fPoint					pivot;
	ENTITY_TYPE				type;
	std::list<entityStat*>	stat;
	SDL_RendererFlip		flip = SDL_FLIP_NONE;
	//Collider* collider = nullptr;
	float					life = 100.f;
	float					maxLife = 100.f;
	float					defence = 0.f;
	pugi::xml_document		file;
	pugi::xml_parse_result	result;

	bool					isInRange = false;
	bool					isParalize = false;
	bool					isBurned = false;
	bool					isPotionActive = false;
	
	bool					changedTile = false; 
	//Animation			idle;
	Animation*				currentAnimation = nullptr;
	SDL_Texture*			entityTex = nullptr;

	// FOR GUI ELEMENTS
	UiItem_HealthBar* lifeBar = nullptr; 
	

	int value;
protected:
	iPoint imOnTile;
	iPoint imOnSubtile;
	iPoint previousSubtilePos;
	iPoint previousTilePos; 
};

#endif
