#include "j1App.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "j1EntityFactory.h"
#include "p2Log.h"
#include "Brofiler\Brofiler.h"

j1Entity::j1Entity(ENTITY_TYPE type, float positionX, float positionY,std::string name) : type(type), position(positionX,positionY), name(name)
{}

j1Entity::~j1Entity()
{
	//if (collider.collider != nullptr)
	//	collider.collider->to_delete = true;
}

bool j1Entity::PreUpdate()
{
	return true;
}

bool j1Entity::Update(float dt)
{
	return true;
}
bool j1Entity::PostUpdate()
{
	Draw(App->entityFactory->texture); // general purposes big entityfactory spritesheet (enemies, or etc)
	return true;
}

void j1Entity::Draw(SDL_Texture * texture)
{
	if (texture != nullptr)
	{
		if (currentAnimation != nullptr)
			App->render->Blit(texture, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, flip);
		else
			App->render->Blit(texture, position.x, position.y);

	}
	else
		LOG("entity texture not found");

}

fPoint j1Entity::GetPosition()
{
	return position;
}

void j1Entity::SetPivot(const float & x, const float & y)
{
	pivot.create(x, y);
}

bool j1Entity::Move(float dt)
{
	return true;
}

void j1Entity::LoadEntitydata(pugi::xml_node& node)
{}

