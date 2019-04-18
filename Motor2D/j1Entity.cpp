#include "j1App.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "j1EntityFactory.h"
#include "j1Window.h"
#include "j1BuffManager.h"
#include "p2Log.h"
#include "Brofiler\Brofiler.h"
#include "j1PathFinding.h"
#include "j1Map.h"

j1Entity::j1Entity(ENTITY_TYPE type, float positionX, float positionY,std::string name) : type(type), position(positionX,positionY), name(name)
{}

j1Entity::j1Entity(iPoint worldPos, SDL_Rect spriteAtlasRect) : type(NO_TYPE), drawAtlasRect(spriteAtlasRect)
{
	position.x = worldPos.x;
	position.y = worldPos.y;

	pivot.create(spriteAtlasRect.w * 0.5f, spriteAtlasRect.h * 0.75f); // places pivot always on sprite "tile midpoint"
}

j1Entity::~j1Entity()
{
	App->entityFactory->DeleteEntityFromSubtile(this); 



	if (!App->cleaningUp)    // When closing the App, Gui cpp already deletes the healthbar before this. Prevent invalid accesses
	{

		if (lifeBar != nullptr)
		{
			lifeBar->deliever = nullptr;
			lifeBar->dynamicImage->to_delete = true;          // deleted in uitemcpp draw
			lifeBar->to_delete = true;
		}


	}



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
	return true;
}

void j1Entity::Draw()
{
	//App->render->DrawCircle((position.x + pivot.x), (position.y + pivot.y), 3, 255, 0, 0, 255, false);
	
	// Default draw (if the entity itself has nothing specific)
	if (entityTex != nullptr) // if we have any specific linked texture
	{
		App->render->Blit(entityTex, position.x, position.y, &drawAtlasRect);
	}
	else // if not, use atlas
	{
		App->render->Blit(App->entityFactory->assetsAtlasTex, position.x, position.y, &drawAtlasRect);
	}

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

bool j1Entity::ChangedTile() const
{
	return changedTile;
}

iPoint j1Entity::GetTilePos() const
{
	return imOnTile;
}

iPoint j1Entity::GetSubtilePos() const
{
	return imOnSubtile;
}

iPoint j1Entity::GetPreviousSubtilePos() const
{
	return previousSubtilePos;
}

void j1Entity::UpdateTilePositions()
{
	changedTile = false; 
	fPoint pivotPos = GetPivotPos();

	// extra protection TODO: rework the player/entities invalid walkability return positions
	iPoint onSubtilePosTemp = App->map->WorldToSubtileMap(pivotPos.x, pivotPos.y);
	if (App->entityFactory->CheckSubtileMapBoundaries(onSubtilePosTemp))
	{
		imOnTile = App->map->WorldToMap(pivotPos.x, pivotPos.y);
		imOnSubtile = onSubtilePosTemp;//App->map->WorldToSubtileMap(pivotPos.x, pivotPos.y);

		if (previousSubtilePos != imOnSubtile)
		{
			//LOG("subtile pos changed");
			// assign this entity to a tile vector
			App->entityFactory->AssignEntityToSubtile(this);
			App->entityFactory->DeleteEntityFromSubtile(this);
			// updates previousPosition to new position
			previousSubtilePos = imOnSubtile;
		}

		if (previousTilePos != imOnTile && App->pathfinding->IsWalkable(imOnTile)) // TODO: not only player change tile (just in case)
		{
			changedTile = true; 
			previousTilePos = imOnTile; 
		}

	}
	else
	{
		//LOG("invalid updateTilePositions, ignoring");
	}	
}