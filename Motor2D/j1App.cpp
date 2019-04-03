#include <iostream> 
#include <sstream> 
#include <iterator>

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1EntityFactory.h"
#include "j1PathFinding.h"
#include "j1Fonts.h"
#include "j1BuffManager.h"

#include "Brofiler/Brofiler.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();
	map = new j1Map();
	gui = new j1Gui();
	entityFactory = new j1EntityFactory();
	pathfinding = new j1PathFinding(); 
	font = new j1Fonts();
	buff = new j1BuffManager();
	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	AddModule(scene);
	AddModule(entityFactory);
	AddModule(buff);
	AddModule(pathfinding);
	AddModule(gui);
	AddModule(font);

	// render last to swap buffer
	AddModule(render);

	PERF_PEEK(ptimer);
}

// Destructor
j1App::~j1App()
{
	// release modules
	std::list<j1Module*>::reverse_iterator item;
	item = modules.rbegin();


	while(item != modules.rend())
	{
		RELEASE(*item);
		--item;
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool j1App::Awake()
{
	PERF_START(ptimer);

	bool ret = false;
		
	config = LoadConfig(config_file);

	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.assign(app_config.child("title").child_value());
		organization.assign(app_config.child("organization").child_value());
		capFrames = app_config.attribute("cap_frames").as_bool();
		framerateCap = app_config.attribute("framerate_cap").as_float();
		capTime = app_config.attribute("framerate_cap").as_int();
		if(capTime != 0)
			capTime = 1000 / capTime;
	}

	if(ret == true)
	{
		std::list<j1Module*>:: iterator item;
		item = modules.begin();

		while(item != modules.end() && ret == true)
		{
			ret = (*item)->Awake(config.child((*item)->name.data()));
			++item;
		}
	}

	PERF_PEEK(ptimer);

	return ret;

}

// Called before the first frame
bool j1App::Start()
{
	PERF_START(ptimer);

	bool ret = true;
	std::list<j1Module*>::iterator item;
	item = modules.begin();

	while(item != modules.end() && ret == true)
	{
		ret = (*item)->Start();
		++item;
	}

	startup_time.Start();

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	BROFILER_CATEGORY("App updates", Profiler::Color::LawnGreen);

	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if(result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

void j1App::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;
	if (pause) 
		dt = 0.0f;
	else 
		dt = 1.0f / framerateCap;


	frame_time.Start();
}

void j1App::FinishUpdate()
{
	BROFILER_CATEGORY("App FinishUpdate", Profiler::Color::Cyan);
	if(want_to_save == true)
		SavegameNow();

	if(want_to_load == true)
		LoadGameNow();

	//Framerate
	//- Calculations
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}
	seconds_since_startup = startup_time.ReadSec();
	float avg_fps = float(frame_count) / seconds_since_startup;
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	std::string capFramesString;

	sprintf_s(title, 256, "%s" , App->GetTitle());

	//- Cap the framerate

	uint32 delay = MAX(0, (int)capTime - (int)last_frame_ms);
	//LOG("Should wait: %i", delay);
	//j1PerfTimer delayTimer;
	SDL_Delay(delay);
	//LOG("Has waited:  %f", delayTimer.ReadMs());
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	BROFILER_CATEGORY("App PreUpdate", Profiler::Color::Turquoise);

	bool ret = true;
	std::list<j1Module*>::iterator item;
	item = modules.begin();
	j1Module* pModule = NULL;

	for(; item != modules.end() && ret == true; ++item)
	{
		pModule = *item;

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	BROFILER_CATEGORY("App Updates", Profiler::Color::LightGreen);

	bool ret = true;
	std::list<j1Module*>::iterator item;
	item = modules.begin();
	j1Module* pModule = NULL;

	for(; item != modules.end() && ret == true; ++item)
	{
		pModule = *item;

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	BROFILER_CATEGORY("App PostUpdates", Profiler::Color::MediumOrchid);

	bool ret = true;
	std::list<j1Module*>::iterator item;
	item = modules.begin();
	j1Module* pModule = NULL;

	for(; item != modules.end() && ret == true; ++item)
	{
		pModule = *item;

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	PERF_START(ptimer);

	bool ret = true;
	std::list<j1Module*>::reverse_iterator item;
	item = modules.rbegin();

	while(item != modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		++item;
	}
	
	modules.size();
	modules.clear();
	PERF_PEEK(ptimer);

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.data();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.data();
}

// Load / Save
void j1App::LoadGame(const char* file)
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	load_game.assign(file);
	want_to_load = true;
	//load_game.create("%s%s", fs->GetSaveDirectory(), file);
}

// ---------------------------------------
void j1App::SaveGame(const char* file) const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?
	save_game.assign(file);
	want_to_save = true;
	//save_game.create(file);
}

// ---------------------------------------
void j1App::GetSaveGames(std::list<std::string>& list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

bool j1App::LoadGameNow()
{
	BROFILER_CATEGORY("App LoadGame", Profiler::Color::Chocolate);

	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.data());

	if(result != NULL)
	{
		LOG("Loading new Game State from %s...", load_game.data());

		root = data.child("game_state");

		std::list<j1Module*>::iterator item;
		item = modules.begin();
		ret = true;

		while(item != modules.end() && ret == true)
		{
			ret = (*item)->Load(root.child((*item)->name.data()));
			++item;
		}

		data.reset();
		if(ret == true)
			LOG("...finished loading");
		else
			LOG("...loading process interrupted with error on module %s", (*item != NULL) ? (*item)->name.data() : "unknown");
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", load_game.data(), result.description());


	want_to_load = false;
	return ret;
}

bool j1App::SavegameNow() const
{
	BROFILER_CATEGORY("App FinishUpdate", Profiler::Color::Coral);

	bool ret = true;

	LOG("Saving Game State to %s...", save_game.data());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;
	
	root = data.append_child("game_state");

	std::list<j1Module*>::const_iterator item = modules.begin();
	while(item != modules.end() && ret == true)
	{
		ret = (*item)->Save(root.append_child((*item)->name.data()));
		++item;
	}

	if(ret == true)
	{
		data.save_file(save_game.data());
	
		LOG("... finished saving", save_game.data());
	}
	else
		LOG("Save process halted from an error in module %s", (*item != NULL) ? (*item)->name.data() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}

float j1App::GetDt()
{
	return dt;
}

//float j1App::GetGameTime()
//{
//	return seconds_since_startup;
//}
