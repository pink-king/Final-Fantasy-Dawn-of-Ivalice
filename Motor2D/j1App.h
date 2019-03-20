#ifndef __j1APP_H__
#define __j1APP_H__

#include <list>

#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"

#include "PugiXml\src\pugixml.hpp"
#include "SDL/include/SDL_stdinc.h"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class j1Gui;
class j1EntityFactory;
class j1PathFinding;
class j1Fonts;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame(const char* file);
	void SaveGame(const char* file) const;
	void GetSaveGames(std::list<std::string>& list_to_fill) const;


	float GetDt();
	//float GetGameTime();
private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

private:

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

public:

	// Modules
	j1Window*			win = nullptr;
	j1Input*			input = nullptr;
	j1Render*			render = nullptr;
	j1Textures*			tex = nullptr;
	j1Audio*			audio = nullptr;
	j1Scene*			scene = nullptr;
	j1Map*				map = nullptr;
	j1Gui*              gui = nullptr;
	j1EntityFactory*	entityFactory = nullptr;
	j1PathFinding*		pathfinding = nullptr; 
	j1Fonts*			font = nullptr;

	bool				transition = false;
	bool				vsync = false;
	bool				pause = false;

	std::string			title;
private:

	std::list<j1Module*>modules;
	uint				frames;
	int					argc;
	char**				args;

	std::string			organization;

	mutable bool		want_to_save;
	bool				want_to_load;
	std::string			load_game;
	mutable std::string	save_game;

	j1PerfTimer			ptimer;
	uint64				frame_count = 0;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	uint32				capTime = 0;
	float				framerateCap = 0.0f;
	float				dt = 0.0f;
	bool				capFrames = false;
	float				seconds_since_startup = 0.0f;
};

extern j1App* App; // No student is asking me about that ... odd :-S

#endif