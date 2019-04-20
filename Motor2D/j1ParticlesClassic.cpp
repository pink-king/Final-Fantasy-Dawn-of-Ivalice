#include <math.h>
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1ParticlesClassic.h"
#include "j1Audio.h"
#include "j1EntityFactory.h"
#include "Brofiler/Brofiler.h"
//#include "SDL/include/SDL_timer.h"

#define MARGIN 20

j1ParticlesClassic::j1ParticlesClassic()
{
	name.assign("ParticlesClassic");
}

j1ParticlesClassic::~j1ParticlesClassic()
{}

bool j1ParticlesClassic::Awake(pugi::xml_node& node)
{
	LOG("AWAKING PARTICLES");
	if (node.empty())
		LOG("empty particles node");

	particleNode = node;

	return true;
}

// Load assets
bool j1ParticlesClassic::Start()
{
	LOG("Loading particles");
	// ---------------------------------------------------
	//// exemple animation from xml
	//LoadAnimation(particleNode.child("animation_teleport01"), teleport01.anim, true);
	//// teleport 02 animation
	//LoadAnimation(particleNode.child("animation_teleport01"), teleport02.anim, true);
	// ---------------------------------------------------
	////load textures and links pointers to --------------
	//teleport01_tex = App->tex->LoadTexture(particleNode.child("teleport_texture01").text().as_string());
	//teleport02_tex = App->tex->LoadTexture(particleNode.child("teleport_texture02").text().as_string());
	//// -------------------------------------------------
	////load and links textures for particles ------------
	//teleport01.texture = teleport01_tex;
	//teleport02.texture = teleport02_tex;


	// TODO: LOAD FROM XML ----------------------

	particleAtlas = App->tex->Load("textures/particles/particle_spritesheet.png");
	particleAtlas2 = App->tex->Load("textures/particles/BuffParticles.png");
	explosion01.anim.PushBack({0,0,32,32});
	explosion01.anim.PushBack({ 0,32,32,32 });
	explosion01.anim.PushBack({ 0,64,32,32 });
	explosion01.anim.PushBack({ 32,0,32,32 });
	explosion01.anim.PushBack({ 32,32,32,32 });
	explosion01.anim.PushBack({ 32,64,32,32 });
	explosion01.anim.PushBack({ 32,96,32,32 });
	explosion01.anim.speed = 10.f;
	explosion01.anim.loop = false;
	explosion01.texture = particleAtlas;
	//explosion01.life = 2000; // if we want a particle that loops, must be specified a life for destroy it


	fire01.anim.PushBack({ 0, 192, 16, 48});
	fire01.anim.PushBack({ 16, 192, 16, 48 });
	fire01.anim.PushBack({ 32, 192, 16, 48 });
	fire01.anim.PushBack({ 48, 192, 16, 48 });
	fire01.anim.PushBack({ 64, 192, 16, 48 });
	fire01.anim.PushBack({ 80, 192, 16, 48 });
	fire01.anim.PushBack({ 96, 192, 16, 48 });
	fire01.anim.PushBack({ 112, 192, 16, 48 });
	fire01.anim.loop = false;
	fire01.anim.speed = 13.f;
	fire01.texture = particleAtlas2; 

	healing.anim.PushBack({ 0, 0, 16, 48 });
	healing.anim.PushBack({ 16, 0, 16, 48 });
	healing.anim.PushBack({ 32, 0, 16, 48 });
	healing.anim.PushBack({ 48, 0, 16, 48 });
	healing.anim.PushBack({ 64, 0, 16, 48 });
	healing.anim.PushBack({ 80, 0, 16, 48 });
	healing.anim.PushBack({ 96, 0, 16, 48 });
	healing.anim.PushBack({ 112, 0, 16, 48 });
	healing.anim.loop = false;
	healing.anim.speed = 10.F; 
	healing.texture = particleAtlas2; 

	ice01.anim.PushBack({ 0, 48, 16, 48 });
	ice01.anim.PushBack({ 16, 48, 16, 48 });
	ice01.anim.PushBack({ 32, 48, 16, 48 });
	ice01.anim.PushBack({ 48, 48, 16, 48 });
	ice01.anim.PushBack({ 64, 48, 16, 48 });
	ice01.anim.PushBack({ 80, 48, 16, 48 });
	ice01.anim.PushBack({ 96, 48, 16, 48 });
	ice01.anim.PushBack({ 112, 48, 16, 48 });
	ice01.anim.loop = false;
	ice01.anim.speed = 10.F;
	ice01.texture = particleAtlas2;

	ice02.anim.PushBack({ 0, 96, 16, 48 });
	ice02.anim.PushBack({ 16, 96, 16, 48 });
	ice02.anim.PushBack({ 32, 96, 16, 48 });
	ice02.anim.PushBack({ 48, 96, 16, 48 });
	ice02.anim.PushBack({ 64, 96, 16, 48 });
	ice02.anim.PushBack({ 80, 96, 16, 48 });
	ice02.anim.PushBack({ 96, 96, 16, 48 });
	ice02.anim.PushBack({ 112, 96, 16, 48 });
	ice02.anim.loop = false;
	ice02.anim.speed = 10.F;
	ice02.texture = particleAtlas2;

	ice03.anim.PushBack({ 0, 144, 16, 48 });
	ice03.anim.PushBack({ 16, 144, 16, 48 });
	ice03.anim.PushBack({ 32, 144, 16, 48 });
	ice03.anim.PushBack({ 48, 144, 16, 48 });
	ice03.anim.PushBack({ 64, 144, 16, 48 });
	ice03.anim.PushBack({ 80, 144, 16, 48 });
	ice03.anim.PushBack({ 96, 144, 16, 48 });
	ice03.anim.PushBack({ 112, 144, 16, 48 });
	ice03.anim.loop = false;
	ice03.anim.speed = 10.F;
	ice03.texture = particleAtlas2;

	poison01.anim.PushBack({ 0, 253, 16, 16 });
	poison01.anim.PushBack({ 16, 253, 16, 16 });
	poison01.anim.PushBack({ 32, 253, 16, 16 });
	poison01.anim.PushBack({ 48, 253, 16, 16 });
	poison01.anim.loop = false;
	poison01.anim.speed = 35.F;
	poison01.texture = particleAtlas2;

	poison02.anim.PushBack({ 0, 269, 16, 16 });
	poison02.anim.PushBack({ 16, 269, 16, 16 });
	poison02.anim.PushBack({ 32, 269, 16, 16 });
	poison02.anim.PushBack({ 48, 269, 16, 16 });
	poison02.anim.loop = false;
	poison02.anim.speed = 35.F;
	poison02.texture = particleAtlas2;
	//load specific Wavs effects for particles -----------
	//App->audio->LoadFx("path");
	// ------------------------------------------------


	return true;
}

// Unload assets
bool j1ParticlesClassic::CleanUp()
{
	LOG("Unloading particles");

	//unloading graphics
	if (App->tex->UnLoad(particleAtlas))
		particleAtlas = nullptr;

	//removing active particles
	if (!active.empty())
	{
		std::list<Particle*>::iterator particles = active.begin();

		for (; particles != active.end();)
		{
			delete (*particles);
			(*particles) = nullptr;
			particles = active.erase(particles);
		}
	}

	//removing particles FX audio
	//App->audio->UnloadSFX();

	return true;
}

bool j1ParticlesClassic::Update(float dt)
{
	bool ret = true;

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);

		/*AddParticle(fire01, p.x, p.y, { 0,0 },0u);
		AddParticle(healing, p.x, p.y, { 0,0 }, 1000u);
		AddParticle(ice01, p.x, p.y, { 0,0 }, 2000u);
		AddParticle(ice02, p.x, p.y, { 0,0 }, 4000u);
		AddParticle(ice03, p.x, p.y, { 0,0 }, 6000u);*/
		/*AddParticle(poison01, p.x, p.y, { 0,0 }, 0u);
		AddParticle(poison02, p.x, p.y, { 0,0 }, 1000u);*/
	}

	return ret;
}

// Update: draw background
bool j1ParticlesClassic::PostUpdate()//float dt)
{
	BROFILER_CATEGORY("Particles Update", Profiler::Color::BlanchedAlmond);

	for (std::list<Particle*>::iterator p = active.begin(); p != active.end();)
	{

		if ((*p)->Update(App->GetDt()) == false)
		{
			//if (active[i]->deathParticle != nullptr)
			//	AddParticle(*active[i]->deathParticle, active[i]->position.x, //+ active[i]->impactPosition.x,
			//		active[i]->position.y, COLLIDER_ENEMY_SHOT);

			delete (*p);
			(*p) = nullptr;
			p = active.erase(p);
		}
		else if (SDL_GetTicks() >= (*p)->born)
		{
			App->render->Blit((*p)->texture, (*p)->position.x, (*p)->position.y, &(*p)->anim.GetCurrentFrame(), 1.0f, (*p)->renderFlip);
			if ((*p)->fx_played == false && (*p)->fx != 0)
			{
				(*p)->fx_played = true;
				// Play particle fx here
				App->audio->PlayFx((*p)->fx, 0);
			}
			++p;
		}
		else
			++p;
	}

	return true;
}

//void ModuleParticles::AddParticle(const Particle& particle, Animation& sourceAnim, int x, int y, Uint32 delay, iPoint speed, Uint32 life, char* name)
void j1ParticlesClassic::AddParticle(const Particle& particle, int x, int y, iPoint speed, Uint32 delay, SDL_RendererFlip rFlip)
{
	Particle* p = new Particle(particle);
	p->born = SDL_GetTicks() + delay;
	p->position.x = x;
	p->position.y = y;
	if (speed.x != 0 || speed.y != 0) //if we send specific speed, defines it
	{
		p->speed = speed;
	}
	p->renderFlip = rFlip;

	active.push_back(p);	
}

// -------------------------------------------------------------
//void j1ParticlesCLassic::OnCollisionSubtile()//Collider* c1, Collider* c2)
//{
//	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
//	{
//		// Always destroy particles that collide
//		if (active[i] != nullptr && active[i]->collider == c1)
//		{
//			//AddParticle(explosion, active[i]->position.x, active[i]->position.y);
//			// checks if we are collisioning on walls, or anything else
//
//			if (c2->type == COLLIDER_WALL) // if collider wall...
//			{
//				if (active[i]->onCollisionWallParticle != nullptr)
//				{
//					if (active[i]->impactPosition.x != NULL || active[i]->impactPosition.y != NULL)
//					{
//						AddParticle(*active[i]->onCollisionWallParticle, active[i]->position.x + active[i]->impactPosition.x,
//							active[i]->position.y + active[i]->impactPosition.y, COLLIDER_NONE, { 0,0 }, 0);
//					}
//					else
//						AddParticle(*active[i]->onCollisionWallParticle, active[i]->position.x, active[i]->position.y, COLLIDER_NONE, { 0,0 }, 0);
//				}
//			}
//			//if (c2->type != COLLIDER_WALL) // anything collider
//			//{
//			if (active[i]->onCollisionGeneralParticle != nullptr)
//				AddParticle(*active[i]->onCollisionGeneralParticle, active[i]->position.x + active[i]->impactPosition.x,
//					active[i]->position.y + active[i]->impactPosition.y, COLLIDER_NONE, { 0,0 }, 0);
//			//}
//
//			//active[i]->texture = nullptr;
//			delete active[i];
//			active[i] = nullptr;
//			break;
//		}
//	}
//}

// -------------------------------------------------------------

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
	anim(p.anim), position(p.position), speed(p.speed), fx(p.fx), born(p.born), life(p.life), texture(p.texture), renderFlip(p.renderFlip)/*,
	damage(p.damage), onCollisionGeneralParticle(p.onCollisionGeneralParticle), onCollisionWallParticle(p.onCollisionWallParticle),
	impactPosition(p.impactPosition), deathParticle(p.deathParticle)*/
{}

Particle::~Particle()
{
	/*if (collider != nullptr)
		App->collision->DeleteCollider(collider);*/
	//App->entityFactory->DeleteEntityFromSubtile(this);
}

bool Particle::Update(float dt)
{
	bool ret = true;

	if (life > 0)
	{
		if ((SDL_GetTicks() - born) > life)
			ret = false;
	}

	//destroy particles respect camera position margins // TODO PARTICLES
	/*if (position.x > (abs(App->render->camera.x) / SCREEN_SIZE) + SCREEN_WIDTH - MARGIN)
		ret = false;
	else if (position.x < (abs(App->render->camera.x) / SCREEN_SIZE) - MARGIN - 150)
		ret = false;*/

		// destroy particle when animation is finished
	if (!anim.loop && anim.Finished())
		ret = false;

	position.x += speed.x * dt;
	position.y += speed.y * dt;

	/*if (collider != nullptr)
		collider->SetPos(position.x, position.y);*/

	return ret;
}

//bool j1ParticlesClassic::LoadAnimation(pugi::xml_node &node, Animation &anim, bool sequential)
//{
//	if (!sequential)
//	{
//		anim.speed = node.attribute("speed").as_float();
//		for (node = node.child("sprite"); node; node = node.next_sibling("sprite")) {
//			SDL_Rect frame;
//			frame.x = node.attribute("x").as_int();
//			frame.y = node.attribute("y").as_int();
//			frame.w = node.attribute("w").as_int();
//			frame.h = node.attribute("h").as_int();
//			anim.PushBack(frame);
//		}
//	}
//	else
//	{
//		int tile_width = node.attribute("tile_width").as_int();//48;
//		int tile_height = node.attribute("tile_height").as_int();
//		int x = node.child("size").attribute("x").as_int();
//		int y = node.child("size").attribute("y").as_int();
//
//		for (int j = 0; j < y; ++j)
//		{
//			for (int i = 0; i < x; ++i)
//			{
//				anim.PushBack({ i * tile_width, j * tile_height, tile_width, tile_height });
//			}
//
//		}
//		anim.speed = node.attribute("speed").as_float();
//		anim.loop = node.attribute("loop").as_bool(true);
//	}
//
//	return true;
//}