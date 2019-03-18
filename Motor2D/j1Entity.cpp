#include "j1App.h"
#include "j1Entity.h"
#include "j1Render.h"

#include "p2Log.h"
#include "Brofiler\Brofiler.h"

j1Entity::j1Entity(ENTITY_TYPE type, float positionX, float positionY,std::string name) : type(type), position(positionX,positionY), name(name)
{}

j1Entity::~j1Entity()
{
	//if (collider.collider != nullptr)
	//	collider.collider->to_delete = true;
}



void j1Entity::Draw(SDL_Texture * texture)
{
	if (currentAnimation != nullptr)
		App->render->Blit(texture, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, flip);
	else
		App->render->Blit(texture, position.x, position.y);
}

fPoint j1Entity::GetPosition()
{
	return position;
}

void j1Entity::SetPivot(const float & x, const float & y)
{
	pivot.create(x, y);
}


