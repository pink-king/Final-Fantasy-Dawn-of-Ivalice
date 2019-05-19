#ifndef __WAVEMANAGER_H__
#define	__WAVEMANAGER_H__

#include "j1Entity.h"
#include <vector>
#include "Enemy.h"
#include <random>
struct Enemy;

enum class WAVE_TYPE
{
	LEVEL_1,
	LEVEL_2,
	MAX
};


struct WaveData
{
	// Number of enemies the wave will have
	uint enemiesNumber = 0;

	// Percentage of spawning - by default, if the type is added, 100% chance to spawn
	uint zombieChances = 10;
	uint golemChances = 10;
	uint bombChances = 10;

	// Vector with the type of enemies the wave can spawn
	std::vector<EnemyType> types;
};

class WaveManager : public j1Entity
{
public:

	WaveManager(const SDL_Rect& zone, uint numWaves, WAVE_TYPE type, j1Entity* associatedTrigger = nullptr);
	~WaveManager();


	bool Start() override;
	bool CleanUp() override;

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	void SpawnCurrentWaveLabel();
	void ChangeStaticLabel();

	WaveData LoadNextWaveData(uint waveNumber); // Loads next wave data depending on the present wave
	void CreateNextWave(WaveData waveData);	// Spawns the next wave depending on the wave data
	bool isWaveOver() const;

	void Finish();

	bool Load(pugi::xml_node&) override;
	bool Save(pugi::xml_node&) const override;

	std::vector<Enemy*> alive; // Vector that stores the enemies of the wave

private:

	uint CreateRandomBetween(uint min, uint max);


private:

	uint maxWaves = 0;
	uint currentWave = 0;
	SDL_Rect spawnZone;
	std::string str_wave;

	j1Timer timer;

	WAVE_TYPE type; 

	bool to_finish = false;
	bool toCreateNextWave = false;

	WaveData nextWaveData;

	// linked trigger
	j1Entity* associatedTrigger = nullptr;

	std::random_device rd;
	std::mt19937 gen;
};



#endif // __WAVEMANAGER_H__