#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "Brofiler/Brofiler.h"

#include "SDL\include\SDL.h"
#include "SDL_TTF\include\SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

j1Fonts::j1Fonts() : j1Module()
{
	name.assign("fonts");
}

// Destructor
j1Fonts::~j1Fonts()
{}

// Called before render is available
bool j1Fonts::Awake(pugi::xml_node& conf)
{
	LOG("Init True Type Font library");
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		const char* path = conf.child("default_font").attribute("file").as_string(DEFAULT_FONT);
		int size = conf.child("default_font").attribute("size").as_int(DEFAULT_FONT_SIZE);
		default = Load(path, size);
	}

	openSansBold12 = Load("fonts/open_sans/OpenSans-Bold.ttf", 12);
	openSansBold18 = Load("fonts/open_sans/OpenSans-Bold.ttf", 18);
	openSansBold36 = Load("fonts/open_sans/OpenSans-Bold.ttf", 36);
	openSansSemiBold24 = Load("fonts/open_sans/OpenSans-Semibold.ttf", 24);
	openSansSemiBold36 = Load("fonts/open_sans/OpenSans-Semibold.ttf", 36);

	shatterBoxx36 = Load("fonts/shatterboxx/shatterboxx.regular.ttf", 36);
	shatterBoxx48 = Load("fonts/shatterboxx/shatterboxx.regular.ttf", 48);

	piecesofEight36 = Load("fonts/Pieces_of_Eight/Pieces of Eight.ttf", 36);
	piecesofEight48 = Load("fonts/Pieces_of_Eight/Pieces of Eight.ttf", 48);

	piecesofEight24= Load("fonts/Pieces_of_Eight/Pieces of Eight.ttf", 24);
	piecesofEight18 = Load("fonts/Pieces_of_Eight/Pieces of Eight.ttf", 18);


	knightsQuest18 = Load("fonts/knights-quest.regular/knights-quest.regular.ttf", 18);
	knightsQuest24 = Load("fonts/knights-quest.regular/knights-quest.regular.ttf", 48);


	return ret;
}

// Called before quitting
bool j1Fonts::CleanUp()
{
	LOG("Freeing True Type fonts and library");
	std::vector<TTF_Font*>::iterator item = fonts.begin();

	for (; item != fonts.end(); ++item)
	{
		TTF_CloseFont(*item);
	}

	fonts.clear();
	TTF_Quit();
	return true;
}

// Load new texture from file path
TTF_Font* const j1Fonts::Load(const char* path, int size)
{
	TTF_Font* font = TTF_OpenFont(path, size);

	if (font == NULL)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
	}
	else
	{
		LOG("Successfully loaded font %s size %d", path, size);
		fonts.push_back(font);
	}

	return font;
}

// Print text using font

SDL_Texture * j1Fonts::Print(const char * text, SDL_Color color, _TTF_Font * font)
{
	BROFILER_CATEGORY("FONTS Print", Profiler::Color::Gold);

	SDL_Texture* ret = NULL;
	SDL_Surface* surface = TTF_RenderText_Blended((font) ? font : default, text, color);

	if (surface == NULL)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = App->tex->LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return ret;
}

// calculate size of a text
bool j1Fonts::CalcSize(const char* text, int& width, int& height, _TTF_Font* font) const
{
	BROFILER_CATEGORY("FONTS Calc Size", Profiler::Color::Gold);

	bool ret = false;

	if (TTF_SizeText((font) ? font : default, text, &width, &height) != 0)
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
		ret = true;

	return ret;
}