#include "j1App.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "j1EntityFactory.h"
#include "j1Window.h"
#include "j1BuffManager.h"
#include "p2Log.h"
#include "Brofiler\Brofiler.h"

j1Entity::j1Entity(ENTITY_TYPE type, float positionX, float positionY,std::string name) : type(type), position(positionX,positionY), name(name)
{}

j1Entity::~j1Entity()
{
	//if (collider.collider != nullptr)
	//	collider.collider->to_delete = true;
}

bool j1Entity::Start()
{
	return true;
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

	return true;
}

bool j1Entity::CleanUp()
{
	std::list<entityStat*>::iterator item = stat.begin();
	for (; item != stat.end(); ++item)
	{
		stat.remove(*item);
	}
	stat.clear();
	return true;
}

void j1Entity::Draw()
{
	//App->render->DrawCircle((position.x + pivot.x), (position.y + pivot.y), 3, 255, 0, 0, 255, false);
}

fPoint j1Entity::GetPosition()
{
	return position;
}

void j1Entity::SetPivot(const float & x, const float & y)
{
	pivot.create(x,y );
}

fPoint j1Entity::GetPivotPos() const
{
	return position + pivot;
}

bool j1Entity::Move(float dt)
{
	return true;
}

void j1Entity::LoadEntitydata(pugi::xml_node& node)
{}

