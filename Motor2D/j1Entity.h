#ifndef __J1ENTITY_H__
#define __J1ENTITY_H__

#include "p2Animation.h"
#include "p2Point.h"

#include "j1Module.h"
#include "j1Textures.h"

#include "SDL_image/include/SDL_image.h"

enum ENTITY_TYPE
	{
		NO_TYPE,
		ENTITY_STATIC,
		ENTITY_DINAMIC
	};

struct EntityInfo
{
	iPoint position;
	//Collider* collider;
};

class j1Entity
{
public:
	j1Entity(ENTITY_TYPE type, float positionX, float positionY, std::string name);
	~j1Entity();

	virtual bool Start()=0;
	virtual bool Move(float dt)=0;
	void Draw(SDL_Texture* texture);
	//virtual void OnCollision(Collider* collider);

	fPoint GetPosition();
	void SetPivot(const float & x, const float & y);
	virtual void LoadEntitydata(pugi::xml_node)=0;

public:
	std::string			name;
	fPoint				position;
	iPoint				size;
	fPoint				pivot;
	ENTITY_TYPE			type;
	SDL_RendererFlip	flip = SDL_FLIP_NONE;
	//Collider* collider;

	pugi::xml_document	file;
	pugi::xml_parse_result result;

	bool				isInRange;

	Animation			idle;
	Animation*			currentAnimation;
	
};

#endif
