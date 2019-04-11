#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include <list>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	// Set music volume
	void SetVolume(float volume);

	// Set FX volume
	void SetFxVolume(float volume);

private:

	_Mix_Music*			music = NULL;
	std::list<Mix_Chunk*>	fx;
	float final_volume = MIX_MAX_VOLUME;
	float final_fx_volume = MIX_MAX_VOLUME;
	float last_volume = 0.0f;
};

#endif // __j1AUDIO_H__