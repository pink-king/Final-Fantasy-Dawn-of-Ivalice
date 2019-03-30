#ifndef __J1ENTITY_H__
#define __J1ENTITY_H__

#include "p2Animation.h"
#include "p2Point.h"
#include "j1Timer.h"

#include "j1Module.h"
#include "j1Textures.h"

#include "SDL_image/include/SDL_image.h"

enum ENTITY_TYPE
	{
		NO_TYPE,
		ENTITY_STATIC,
		ENTITY_DYNAMIC,
		ENEMY_TEST
	};

enum class STAT_TYPE
{
	NORMAL,
	BURNED_STAT,
	PARALIZE_STAT
};

class entityStat
{
public:
	entityStat(STAT_TYPE stat, float maxDamage):type(stat), maxDamage(maxDamage){}
	entityStat() {};
public:
	STAT_TYPE		type;
	float			maxDamage;
	j1Timer			count;
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
	~j1Entity();

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

	fPoint GetPosition();
	void SetPivot(const float & x, const float & y);
	fPoint GetPivotPos() const;
	virtual void LoadEntitydata(pugi::xml_node&);

public:
	std::string				name;
	fPoint					position;
	iPoint					size;
	fPoint					pivot;
	ENTITY_TYPE				type;
	std::list<entityStat*>	stat;
	SDL_RendererFlip		flip = SDL_FLIP_NONE;
	//Collider* collider = nullptr;
	float					life = 100.f;
	float					defence = 5.f;
	pugi::xml_document		file;
	pugi::xml_parse_result	result;

	bool					isInRange = false;
	bool					isParalize = false;
	bool					isBurned = false;
	//Animation			idle;
	Animation*				currentAnimation = nullptr;
	SDL_Texture*			entityTex = nullptr;
	
};

#endif
