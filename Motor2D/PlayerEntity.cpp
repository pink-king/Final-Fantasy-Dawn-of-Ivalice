#include "PlayerEntity.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1EntityFactory.h"
#include "j1Window.h"
#include <math.h>

PlayerEntity::PlayerEntity(int posX, int posY) : j1Entity(PLAYER, posX , posY, "PlayerParent")
{
	//SetPivot(16, 40);
}

PlayerEntity::~PlayerEntity()
{
}

bool PlayerEntity::Start()
{
	return true;
}

bool PlayerEntity::PreUpdate()
{
	return true;
}

bool PlayerEntity::Update(float dt)
{
	//InputMovement(dt);
	return true;
}

bool PlayerEntity::PostUpdate()
{

	return true;
}

bool PlayerEntity::CleanUp()
{
	return true;
}



bool PlayerEntity::InputMovement(float dt)
{
	bool isMoving = false;

	previousPos = position;

	Sint16 xAxis = App->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTX);
	Sint16 yAxis = App->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTY);

	if (xAxis > 0 || xAxis < 0)
	{
		position.x = position.x + (xAxis * 0.003 * characterBaseSpeed.x) * dt; // TODO: GET speed from buff manager
		isMoving = true;
		//LOG("xAxis %i", xAxis); //32767 -32768
	}
	if (yAxis > 0 || yAxis < 0)
	{
		position.y = position.y + (yAxis * 0.003 * characterBaseSpeed.y) * dt; // TODO: GET speed from buff manager
		isMoving = true;
	}

	// keyboard input, only for debug or test purposes at the moment, main gameplay are based on GamePad
	if ((App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) || (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) ||
		(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) || (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT))
	{
		// condition
		isMoving = true;
		// x "axis" input
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			xAxis = SHRT_MIN;
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			xAxis = SHRT_MAX;
		// y "axis" input
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			yAxis = SHRT_MIN;
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			yAxis = SHRT_MAX;

		if (xAxis == yAxis || yAxis > 0 && xAxis < 0 || yAxis < 0 && xAxis > 0) // diagonal cases || slow down
		{
			//xAxis = xAxis * 0.5f;
			yAxis = yAxis * 0.5f;
		}
		position.x = position.x + (xAxis * 0.003 * characterBaseSpeed.x) * dt; // TODO: GET speed from buff manager
		position.y = position.y + (yAxis * 0.003 * characterBaseSpeed.y) * dt; // TODO: GET speed from buff manager
	}

	// check individually collisions for each axis movement

	// draw iso col
	int colSize = 16;

	iPoint pivot_pos = (iPoint)GetPivotPos() - iPoint(0, colSize * 0.5f);

	SDL_Rect collider = { pivot_pos.x, pivot_pos.y, colSize, colSize};

	// Red isometric quad
	App->render->DrawIsoQuad(collider);

	App->render->DrawIsoQuad({ 32,16, 128,96 });

	// Check collisions and do behaviour ------------------------------------------
	std::vector<SDL_Rect> resultant_intersections = Collision2D(collider);
	if (!resultant_intersections.empty())
	{
		LOG("COLLISIONS: %i", resultant_intersections.size());

		position = GetCollisionsBehaviourNewPos(resultant_intersections, xAxis, yAxis);
	}
	// -----------------------------------------------------------------------------

	if (isMoving)// if we get any input, any direction
	{
		// store actual
		float current_cycle_frame = currentAnimation->GetCurrentFloatFrame();
		lastAxisMovAngle = atan2f(yAxis, xAxis);
		currentAnimation = &run[(int)GetPointingDir(lastAxisMovAngle)];
		currentAnimation->SetCurrentFrame(current_cycle_frame);
	}
	else
	{
		currentAnimation = &idle[pointingDir];
	}

	// checks render flip
	CheckRenderFlip();

	//LOG("animation Angle: %f", animationAngle);

	return true;
}

bool PlayerEntity::InputCombat()
{
	combat_state = combatState::IDLE;
	
	if (App->input->GetControllerAxisPulsation(SDL_CONTROLLER_AXIS_TRIGGERLEFT) == KEY_DOWN || 
		App->input->GetControllerAxisPulsation(SDL_CONTROLLER_AXIS_TRIGGERLEFT) ==  KEY_REPEAT)
	{
		if(App->input->GetControllerAxisPulsation(SDL_CONTROLLER_AXIS_TRIGGERLEFT) == KEY_DOWN)
			aiming = true;

		// check ultimate trigger
		if (App->input->GetControllerAxisPulsation(SDL_CONTROLLER_AXIS_TRIGGERRIGHT) == KEY_DOWN)
		{
			combat_state = combatState::ULTIMATE;
			//LOG("ULTIMATE");
		}



		// check basic attack
		if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_X) == KEY_DOWN)
		{
			combat_state = combatState::BASIC;
			LOG("BASIC");
		}
		// check dodge
		if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			combat_state = combatState::DODGE;
			LOG("DODGE");
		}
		// etc
		// code under construction ...
		//LOG("");
	}


	// - - - - - - - -  - - - - - - faked ability 1 & 2 buttons
		// check ability 1 trigger
	if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_X) == KEY_DOWN)
	{
		combat_state = combatState::SPECIAL1;
		//LOG("ULTIMATE");
	}

	// check ability 2 trigger
	if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN)
	{
		combat_state = combatState::SPECIAL2;
		//LOG("ULTIMATE");
	}




	if (App->input->GetControllerAxisPulsation(SDL_CONTROLLER_AXIS_TRIGGERLEFT) == KEY_UP)
	{
		aiming = false;
		return false;
	}

	return true;
}

//combatState PlayerEntity::GetInstantCombatState()

void PlayerEntity::CheckRenderFlip()
{
	if (pointingDir == int(facingDirection::SW) || pointingDir == 4 || pointingDir == 7)
	{
		flip = SDL_FLIP_HORIZONTAL;
	}
	else
		flip = SDL_FLIP_NONE;
}

void PlayerEntity::Draw()
{
	if (spritesheet != nullptr)
	{
		if (currentAnimation != nullptr)
			App->render->Blit(spritesheet, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, flip);
		else
			App->render->Blit(spritesheet, position.x, position.y);
	}
}

int PlayerEntity::GetPointingDir(float angle)
{

	int numAnims = 8;
	//LOG("angle: %f", angle);
								     // divide the semicircle in 4 portions
	float animDistribution = PI / (numAnims * 0.5f); // each increment between portions //two semicircles

	int i = 0;
	if (angle >= 0) // is going right or on bottom semicircle range to left
	{
		// iterate between portions to find a match
		for (float portion = animDistribution * 0.5f; portion <= PI; portion += animDistribution) // increment on portion units
		{
			if (portion >= angle) // if the portion is on angle range
			{
				// return the increment position matching with enumerator direction animation
				// TODO: not the best workaround, instead do with std::map
				/*LOG("bottom semicircle");
				LOG("portion: %i", i);*/
				break;
			}
			++i;
		}
	}
	else if (angle <= 0) // animations relatives to upper semicircle
	{
		i = 0; // the next 4 on the enum direction
		
		for (float portion = -animDistribution * 0.5f; portion >= -PI; portion -= animDistribution)
		{
			if (i == 1) i = numAnims * 0.5f + 1;
			if (portion <= angle)
			{
				/*LOG("upper semicircle");
				LOG("portion: %i", i);*/
				break;
			}
			++i;
		}
	}

	pointingDir = i;
	if (pointingDir == numAnims) // if max direction
		pointingDir = numAnims - 1; // set to prev

	//LOG("portion: %i", pointingDir);

	return pointingDir;
}

float PlayerEntity::GetLastHeadingAngle() const
{
	return lastAxisMovAngle;
}

std::vector<SDL_Rect> PlayerEntity::Collision2D(SDL_Rect& collider)
{
	bool debug = true;
	std::vector<SDL_Rect> ret; // stores all the no walkable intersection as resultant rects

	int tile_size = 32;

	iPoint tempcolpos = App->map->IsoTo2D(collider.x, collider.y);
	collider.x = tempcolpos.x;
	collider.y = tempcolpos.y;

	// check only the neighbours adjacents to player current TILE
	iPoint currentTile = App->entityFactory->player->GetTilePos();
	currentTile.x += 1; // TODO: MAP DISPLACEMENT...
	//currentTile.y += 1;
	iPoint tileNeighbours[8]
	{
		{ currentTile.x,	 currentTile.y - 1	}, // N
		{ currentTile.x + 1, currentTile.y - 1	}, // NE
		{ currentTile.x + 1, currentTile.y		}, // E
		{ currentTile.x + 1, currentTile.y + 1	}, // SE
		{ currentTile.x,	 currentTile.y + 1	}, // S
		{ currentTile.x - 1, currentTile.y + 1	}, // SW
		{ currentTile.x - 1, currentTile.y		}, // W
		{ currentTile.x - 1, currentTile.y - 1	}  // NW
	};

	int scale =  (int)App->win->GetScale();

	iPoint offset = App->map->MapToWorld(tileNeighbours[7].x, tileNeighbours[7].y);
	offset = App->map->IsoTo2D(offset.x, offset.y);
	offset += (App->camera2D->GetCamPos() / scale).RoundPoint();
	//Place it on the right corner of the screen
	uint w, h;
	App->win->GetWindowSize(w, h);
	offset.x -= round((int)w / scale) - tile_size * 3;
	
	// colors for debug visuals
	SDL_Color color_pink = { 255,162,240,255 };
	SDL_Color color_red = { 255,0,0,255 };

	for (int i = 0; i < 8; ++i)
	{
		bool walkableTile = true;
		SDL_Color color = color_pink;
		// check first if this tile is walkable or not
		if (!App->pathfinding->IsWalkable({ tileNeighbours[i].x - 1, tileNeighbours[i].y }))
		{
			color = color_red;
			walkableTile = false;
		}

		//Isometric
		tileNeighbours[i] = App->map->MapToWorld(tileNeighbours[i].x, tileNeighbours[i].y);
		if(debug) // TODO: temporal DRAWs, must be moved to postupdate
			App->render->DrawIsoQuad({ tileNeighbours[i].x, tileNeighbours[i].y, tile_size,tile_size }, color);

		//Orthogonal
		tileNeighbours[i] = App->map->IsoTo2D(tileNeighbours[i].x, tileNeighbours[i].y);
		SDL_Rect tileWorldRect = { tileNeighbours[i].x , tileNeighbours[i].y, tile_size,tile_size }; // size of tile data
		if(debug) // DRAW
			App->render->DrawQuad(
			{ tileWorldRect.x - offset.x, tileWorldRect.y - offset.y, tileWorldRect.w, tileWorldRect.h },
			color.r, color.g, color.b, color.a);

		if (SDL_HasIntersection(&collider, &tileWorldRect))
		{
			if (!walkableTile)
			{
				//LOG("COLLISION");
				SDL_Rect intersectionRect;
				SDL_IntersectRect(&collider, &tileWorldRect, &intersectionRect);

				ret.push_back(intersectionRect);

			}
		}
	}

	// DEBUG draw -----------
	//Draw yellow quad
	App->render->DrawQuad(
		{collider.x - offset.x, collider.y - offset.y, collider.w, collider.h},
		255, 255, 0, 255);

	if (!ret.empty())
	{
		std::vector<SDL_Rect>::iterator iter = ret.begin();
		uint i = 3; // color multiplier
		for (; iter != ret.end(); ++iter)
		{
			App->render->DrawQuad(
				{ (*iter).x - offset.x, (*iter).y - offset.y, (*iter).w, (*iter).h },
				255, 30 * i, 16, 255);
			
			++i;
		}
	}
	// ----------------------

	return ret;
}

fPoint PlayerEntity::GetCollisionsBehaviourNewPos(std::vector<SDL_Rect>& resultant_intersections, Sint16 xAxis, Sint16 yAxis)
{
	fPoint ret(0,0);

	//ret.create(position.x, position.y);
	float angle = atan2f(yAxis, xAxis) * 180 / PI;

	if (resultant_intersections.size() < 3) // 3 intersections means a corner between 3 cells
	{
		// get the sum between the all rects (max two)
		std::vector<SDL_Rect>::iterator iterResults = resultant_intersections.begin();
		SDL_Rect collision = { (*iterResults).x, (*iterResults).y,0,0 };
		for (; iterResults != resultant_intersections.end(); ++iterResults)
		{
			// preserve the x,y pos for the top-left resultant
			if(collision.x > (*iterResults).x)
				collision.x = (*iterResults).x;
			if (collision.y > (*iterResults).y)
				collision.y = (*iterResults).y;
			// sum the sizes
			collision.w += (*iterResults).w;
			collision.h += (*iterResults).h;
		}

		// we have two possibilites of resultant collision rect
		fPoint slidingSpeed = { 1.0f,0.5f };
		ret = previousPos;

		if (collision.w > collision.h)
		{
			//LOG("Up or down tile collision");
			// operate dependent of the axis direction
			if (angle <= -45.f) // if the player are going right or up
			{
				LOG("UP");
				ret -= slidingSpeed;
			}
			else if (angle >= -25.f && angle < 45.f)
			{
				LOG("RIGHT");
				ret += slidingSpeed;
			}
			else if (angle > 125.f && angle <= 180.f)
			{
				LOG("LEFT");
				ret -= slidingSpeed;
			}
			else if (angle > 45.f && angle < 125.f)
			{
				LOG("DOWN");
				ret += slidingSpeed;
			}
			else
				ret = previousPos;
				
		}
		else if (collision.w < collision.h) // left- right tile collision
		{
			//LOG("Left or right tile collision");
			if (angle >= -150.f && angle <= -26.f) // if the player are going left or up
			{
				LOG("UP");
				ret.x += slidingSpeed.x;
				ret.y -= slidingSpeed.y;
			}
			else if (angle <= 180.f && angle > 45.f)
			{
				LOG("LEFT");
				ret.x -= slidingSpeed.x;
				ret.y += slidingSpeed.y;
			}
			else if (angle >= 0.f && angle < 26.f)
			{
				LOG("RIGHT");
				ret.x += slidingSpeed.x;
				ret.y -= slidingSpeed.y;
			}
			//else if (angle > 45.f && angle < 125.f)
			//{
			//	LOG("DOWN");
			//	ret = previousPos;
			//	ret += slidingSpeed;
			//}
			//else
			//	ret = previousPos;
			else
				ret = previousPos;
		}
		else if (collision.w == collision.h)
		{
			LOG("leaving corner response");
			ret = position;
			// and adds little push to movement direction
			float piAngle = angle * PI / 180.f;
			
			fPoint newPushVector;
			newPushVector.x = cosf(piAngle);
			newPushVector.y = sinf(piAngle);

			newPushVector.Normalize();

			ret += newPushVector * 4.f;
			LOG("");

			// but we have 4 cases
			// up, down, right, left perfect corners in one axis (in direction of corner)

			
		}

		LOG("xAxis: %i", xAxis);
		LOG("yAxis: %i", yAxis);
		LOG("angle: %f", angle);

		// check player vector direction



		//ret = previousPos;

	}
	else
		ret = previousPos; // if we have a corner intersection, return previous position before this move

	return ret;
}