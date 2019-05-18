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
		ENEMY_TEST,
		ENEMY_BOMB,
		WAVE_MANAGER,
		FLOWERBOSS,
		ENEMY_ARCHER,
		//etc
		LOOT,
		PROJECTILE,

		TRIGGER,
		// LAST
		MAX_TYPE
	};

enum class STAT_TYPE
{
	NORMAL,
	BURNED_STAT,
	ICE_STAT,
	POISON_STAT,

	POTION_STAT,

	ATTACK_BUFF,
	DEFENCE_BUFF,
	HEALTH_BUFF,
	SPEED_BUFF

};
class Buff; 

class entityStat
{
public:
	entityStat(STAT_TYPE stat, uint totalTime, float secDamage = 0.f, Buff* temporalBuff = nullptr, bool to_paralitze = false)
		:type(stat), secDamage(secDamage), totalTime(totalTime), temporalBuff(temporalBuff), to_paralitze(to_paralitze) {}
public:
	STAT_TYPE		type = STAT_TYPE::NORMAL;
	float			secDamage = 0.f;
	j1Timer			count;
	uint			totalTime = 0;
	Buff*			temporalBuff = nullptr;
	bool			to_paralitze = false;

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
	j1Entity() {};
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

	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&) const;
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

	void DoPushback();
	friend class PlayerEntity;
	

public:
	SDL_Rect drawAtlasRect; // for static draw from spritesheet
	bool					to_delete = false;
	bool					to_die = false; 
	std::string				name = "\0";
	fPoint					position;
	iPoint					size;
	fPoint					pivot;
	ENTITY_TYPE				type;
	std::list<entityStat*>	stat;
	SDL_RendererFlip		flip = SDL_FLIP_NONE;
	//Collider* collider = nullptr;
	float					life = 100.f;
	float					maxLife = 100.f;
	float					defence = 1.f;
	pugi::xml_document		file;
	pugi::xml_parse_result	result;

	bool					isInRange = false;
	bool					isFrozen = false;
	bool					isBurned = false;
	bool					isPosioned = false;
	bool					isPotionActive = false;
	bool					isParalize = false;

	bool					changedTile = false; 
	bool					changedSubtile = false; 
	//Animation			idle;
	Animation*				currentAnimation = nullptr;
	SDL_Texture*			entityTex = nullptr;

	// FOR GUI ELEMENTS
	UiItem_HealthBar* lifeBar = nullptr; 
	
	bool					manualCollectable;
	int						value;



	fPoint dmgDestinationPos;
	j1Timer					alphaTimer;
protected:
	iPoint imOnTile;
	iPoint imOnSubtile;
	iPoint previousSubtilePos;
	iPoint previousTilePos; 
};

#endif
