#ifndef __WAVEMANAGER_H__
#define	__WAVEMANAGER_H__

#include "j1Entity.h"
#include <vector>
#include "Enemy.h"
#include <random>
struct Enemy;

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

	WaveManager(const SDL_Rect& zone, uint numWaves);
	~WaveManager();


	bool Start() override;
	bool CleanUp() override;

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;

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
	uint actualWave = 0;
	SDL_Rect spawnZone;

	j1Timer timer;


	bool to_finish = false;
	bool toCreateNextWave = false;

	WaveData nextWaveData;

	std::random_device rd;
	std::mt19937 gen;
};



#endif // __WAVEMANAGER_H__